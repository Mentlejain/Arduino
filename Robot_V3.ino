#include <AFMotor.h>      //add Adafruit Motor Shield library
#include <Servo.h>        //add Servo Motor library            
#include <NewPing.h>      //add Ultrasonic sensor library

#define TRIG_PIN 2 // Pin A0 on the Motor Drive Shield soldered to the ultrasonic sensor
#define ECHO_PIN A5 // Pin A1 on the Motor Drive Shield soldered to the ultrasonic sensor
#define MAX_DISTANCE 150 // sets maximum useable sensor measuring distance to 300cm
#define MAX_SPEED 100 // sets speed of DC traction motors to 150/250 or about 70% of full speed - to get power drain down.
#define MAX_SPEED_OFFSET 10 // this sets offset to allow for differences between the two DC traction motors
#define COLL_DIST 20 // sets distance at which robot stops and reverses to 30cm
#define TURN_DIST COLL_DIST+20 // sets distance at which robot veers away from object
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // sets up sensor library to use the correct pins to measure distance.

AF_DCMotor leftm(1, MOTOR12_1KHZ); // create motor #1 using M1 output on Motor Drive Shield, set to 1kHz PWM frequency
AF_DCMotor rightm(2, MOTOR12_1KHZ); // create motor #2, using M2 output, set to 1kHz PWM frequency
Servo myservo;  // create servo object to control a servo 

int leftDistance, rightDistance; //distances on either side
int curDist = 0;
String motorSet = "";
int speedSet = 0;

//-------------------------------------------- SETUP LOOP ----------------------------------------------------------------------------
void setup() {
  myservo.attach(10);  // attaches the servo on pin 10 (SERVO_1 on the Motor Drive Shield to the servo object 
  myservo.write(90); // tells the servo to position at 90-degrees ie. facing forward.
  delay(1000); // delay for one seconds
 }
//------------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------MAIN LOOP ------------------------------------------------------------------------------
void loop() {
  myservo.write(90);
  delay(90);
  curDist = readPing();   // read distance
  if (curDist < COLL_DIST) {changePath();}  // if forward is blocked change direction
  moveForward();  // move forward
  delay(200);
 }
//-------------------------------------------------------------------------------------------------------------------------------------

void changePath() {
  moveStop();   // stop forward movement
  myservo.write(0);  // check distance to the right
    delay(500);
    rightDistance = readPing(); //set right distance
    delay(500);
    myservo.write(180);  // check distace to the left
    delay(700);
    leftDistance = readPing(); //set left distance
    delay(500);
    myservo.write(90); //return to center
    delay(100);
    compareDistance();
  }

  
void compareDistance()   // find the longest distance
{
  if (leftDistance>rightDistance) //if left is less obstructed 
  {
    turnLeft();
  }
  else if (rightDistance>leftDistance) //if right is less obstructed
  {
    turnRight();
  }
   else //if they are equally obstructed
  {
    turnAround();
  }
}


//-------------------------------------------------------------------------------------------------------------------------------------

int readPing() { // read the ultrasonic sensor distance
  delay(70);
  unsigned int uS =0;
  uS=sonar.ping();
  while(uS==0){uS=sonar.ping();}
  int cm = uS/US_ROUNDTRIP_CM;
  return cm;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void moveStop() {leftm.run(RELEASE); rightm.run(RELEASE);}  // stop the motors.
//-------------------------------------------------------------------------------------------------------------------------------------
void moveForward() {
    motorSet = "FORWARD";
    rightm.run(FORWARD);      // turn it on going forward
    leftm.run(FORWARD);      // turn it on going forward
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    rightm.setSpeed(speedSet);      // turn it on going forward
    leftm.setSpeed(speedSet);
    delay(5);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void moveBackward() {
    motorSet = "BACKWARD";
    rightm.run(BACKWARD);     // turn it on going backward
    leftm.run(BACKWARD);     // turn it on going backward
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    rightm.setSpeed(speedSet);
    leftm.setSpeed(speedSet);
    delay(5);
  }
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void turnRight() {
  motorSet = "RIGHT";
  leftm.run(FORWARD);      
  rightm.run(BACKWARD); 
  //rightm.setSpeed(speedSet+MAX_SPEED_OFFSET);      
  delay(300);   
  motorSet = "FORWARD";
  rightm.run(FORWARD);      // turn it on going forward
    leftm.run(FORWARD);      // turn it on going forward      
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void turnLeft() {
  motorSet = "LEFT";
  leftm.run(BACKWARD);      // turn motor 1 backward
  //leftm.setSpeed(speedSet+MAX_SPEED_OFFSET);     
  rightm.run(FORWARD);     // turn motor 4 forward
  delay(300); // run motors this way for 1500  
  motorSet = "FORWARD";
  rightm.run(FORWARD);      // turn it on going forward
    leftm.run(FORWARD);      // turn it on going forward   
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void turnAround() {
  motorSet = "RIGHT";
  leftm.run(FORWARD);      
  rightm.run(BACKWARD); 
 // rightm.setSpeed(speedSet+MAX_SPEED_OFFSET);      
  delay(500); 
  motorSet = "FORWARD";
  rightm.run(FORWARD);      // turn it on going forward
    leftm.run(FORWARD);      
}  
