#include "definepin.h"
#include "lineCal.h" 
#include "PIDver1.h"
#include "func.h"
#include "VarSpeedServo.h"

#define numOfSensor 8
#define myKp  6
#define myKi  0
#define myKd  0
#define mySetp  3.5
#define myOutMax  -60
#define myOutMin  60
#define myTime 0.01

#define INA 6
#define INB 5
#define PWM 4
#define EN 7

#define BRAKE 0
#define CW 1
#define CCW 2
#define MOTOR_ON 0



PID myPID(myKp, myKi, myKd, myTime, myOutMax, myOutMin,1);
unsigned long preTime;
VarSpeedServo myServo;
//Servo myDCMotor;
double servoSpeed = 0;
double Out = 0;
short motorSpeed = 45;
float val[numOfSensor][numOfSensor];
int halfLine = 0;
int fullLine = 0;
int intersectSignal = 0;
int isTurning = 0;  
float res;
double xCoordinate = 0, yCoordinate = 0;  
int pin[8] = {A_IN1, A_IN2, A_IN3, A_IN4, A_IN5, A_IN6, A_IN7, A_IN8};
int maxArr[8];
int minArr[8];
double ledVal[8];

void readMinMax(){
//   for(int i = 0; i < N; i++){
//    maxArr[i] = MAXVALUE;
//    minArr[i] = MINVALUE;
//  } 
  Serial.println("reading min max");
  for(int i = 0; i < N; i++){
    maxArr[i] = MINVALUE;
    minArr[i] = MAXVALUE;
  }
  int preSensor[8];
  for (int i = 30; i <= 150; i = i + 10){
     myServo.write(i, 150, true); 
     delay(50);      
     for(int j = 0; j <=7 ; j++){
       preSensor[j] = analogRead(pin[7 - j]);
       if (preSensor[j] > maxArr[j]) maxArr[j] = preSensor[j];
       if (preSensor[j] < minArr[j]) minArr[j] = preSensor[j];
     }
     delay(50);
  }
  for (int i = 150; i > 30; i = i - 10){
    myServo.write(i, 150, true);
    delay(50);
    for(int j = 0; j <=7 ; j++){
     preSensor[j] = analogRead(pin[7 - j]);
     if (preSensor[j] > maxArr[j]) maxArr[j] = preSensor[j];
     if (preSensor[j] < minArr[j]) minArr[j] = preSensor[j];
    } 
    delay(50);   
  }
}
void print(double arr[]){
      Serial.println("Print:");
      for(int i = 0; i < N; i++){
        Serial.print(arr[i]);
        Serial.print("   ");
      }
      Serial.println("");
}

void print(int arr[]){
      Serial.println("Print:");
      for(int i = 0; i < N; i++){
        Serial.print(arr[i]);
        Serial.print("   ");
      }
      Serial.println("");
}
void setup(){
  pinMode(INA, OUTPUT);
    pinMode(INB, OUTPUT);
    pinMode(PWM, OUTPUT);
    pinMode(EN, OUTPUT);

    digitalWrite(EN, HIGH);
  for(int i = 0; i < N; i++){
    pinMode(pin[i], INPUT);
  }
  myServo.attach(SERVO_PPM); 
  //myDCMotor.attach(MOTOR_PPM);
  //myDCMotor.write(90);
  delay(1000);
  myServo.write(45, 100, true);
  int preAngle = -30;
  Serial.begin(9600);
  readMinMax();
  preTime = millis();
  print(maxArr);
  print(minArr);
  Serial.println("Start loop");
  myServo.write(90, 150, true);
  delay(200);
}

void update(){
  switch (lineType(ledVal, yCoordinate)) {
    case 0: //Line Middle
      if (isTurning) {
        //"Turning Completed. Moving Ahead."      
        isTurning = 0;
        fullLine = 0;
        halfLine = 0;
        intersectSignal = 0;        
      }
      if (fullLine == 2) {
        //"Line Detected. Moving Ahead."
        fullLine = 0;
        halfLine = 0;
      }
      xCoordinate = getLinePos(ledVal);
      break;
    case 1: //Line Full White
      if (fullLine == 0 && halfLine != 1) {
        //"Challenge is about to appear. Moving ahead"
        fullLine = 1;
        //End game not solving.
      }
      if (halfLine == 1) {
        //"Turn left at the T-Intersection."
        xCoordinate = 0;
        isTurning = 1;
      }
      else if (halfLine == 2) {
        //"Turn right at the T-Intersection."
        xCoordinate = 7;
        isTurning = 1;
      }
      break;
    case 2: //Line Full Black   
      if (isTurning) {      
        //"Turning slowly. Avoid Crashing."
      }
      else if (halfLine == 1) {
        if (fullLine == 0 || fullLine == 2) {
          //"Line is disappearing for a while. Moving Left for Searching Line"
        }
      }
      else if (halfLine == 2) {
        if (fullLine == 0 || fullLine == 2) {
          //"Line is disappearing for a while. Moving Right for Searching Line"
        }
        fullLine = 2;
      }
      else {
        //"Line is disappearing for a while. Keep moving ahead."
        //Handle obstacle.
      }
     
      fullLine = 2;
      break;
    case 3: //Half-Line Left      
      if (fullLine == 1) {
        //"Turn left 90 degree."
        xCoordinate = 0;
        isTurning = 1;
      }
      else {
        //"Half-Line Left Signal For Searching Line in Black or Intersection."
        halfLine = 1;
      }
      break;
    case 4: //Half-Line Right   
      if (fullLine == 1) {
        // "Turn right 90 degree."
        xCoordinate = 7;
        isTurning = 1;
      }
      else {
        //"Half-Line Right Signal For Searching Line in Black." << endl;
        halfLine = 2;
      }
      break;
    case 5:
      xCoordinate = getLinePos(ledVal);
      break;   
    }
   
}
/*Motor COntrol*/
void forward() {
  motorControl(0, CW, motorSpeed);
}
void reverse() {
  motorControl(0, CCW, motorSpeed);
}
void stopMotor() {
  motorControl(0, BRAKE, motorSpeed);
}
void motorControl(uint8_t motor, uint8_t direct, uint8_t pwm_t ) {
  if(motor == MOTOR_ON)
  {
    if(direct == CW)
    {
      digitalWrite(INA, LOW); 
      digitalWrite(INB, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(INA, HIGH);
      digitalWrite(INB, LOW);      
    }
    else
    {
      digitalWrite(INA, LOW);
      digitalWrite(INB, LOW);            
    }
    
    analogWrite(PWM, pwm_t); 
  }
}

void loop() 
{   
 
  // TODO: Bat dau chay xe myDCMotor
    //myDCMotor.write(110);
    //myServo.write(60);    
   unsigned long elapsedTime = millis() - preTime;
   if (elapsedTime > 10){
      for(int i = 0; i < N; i++){
        ledVal[i] = analogRead(pin[N - 1 -i]);
      }
      normalize(ledVal, maxArr, minArr, MAXVALUE, MINVALUE);
      update();
      preTime = preTime + elapsedTime;
      //forward();
      servoSpeed = map(xCoordinate - mySetp, 0, 3.5, 0, 255);
      Out = myPID.Calculate(mySetp, xCoordinate);
      myServo.write(-Out + 90, servoSpeed );
//      Serial.print(3.5);
//      Serial.print(",");
//      Serial.print(xCoordinate);
//      Serial.print(",");
//      Serial.println(Out);
   }

}
