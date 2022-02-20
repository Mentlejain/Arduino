#include <AFMotor.h>

AF_DCMotor leftm(1, MOTOR12_64KHZ); // create leftm #2, 64KHz pwm
AF_DCMotor rightm(2, MOTOR12_64KHZ);

#include <Servo.h> 

int servoPin = 9;
 
Servo servo;  
 
int angle = 0;
void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor test!");
  
  servo.attach(servoPin); 
  
  leftm.setSpeed(255);
  rightm.setSpeed(255);// 
}

void loop() {
  
}
void go_forward()
{
    leftm.run(FORWARD); 
    rightm.run(FORWARD);
}
void turn_left()
{
   leftm.run(BACKWARD); 
   rightm.run(FORWARD);
}
