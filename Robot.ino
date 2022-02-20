#include <AFMotor.h>

AF_DCMotor leftm(1, MOTOR12_64KHZ); // create leftm #2, 64KHz pwm
AF_DCMotor rightm(2, MOTOR12_64KHZ);

#include <Servo.h> 

int servoPin = 10;
 
Servo servo;  

//setIR Pin 3
//setIR Pin 4

byte trig = 2;                                    //Assign the ultrasonic sensor pins
byte echo = A5;
byte maxDist = 150;                               //Maximum sensing distance (Objects further than this distance are ignored)
byte stopdist = 50;                               //Minimum distance from an object to stop in cm
float timeOut = 2*(maxDist+10)/100/340*1000000;   //Maximum time to wait for a return signal

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor test!");
  
  servo.attach(servoPin); 
  int motor_speed=55;
  int offset_speed=10;
  int turn_speed=50;
  leftm.setSpeed(255);
  rightm.setSpeed(255);
}

void loop() {
  servo.write(90);                            //Set the servo to look straight ahead
  delay(750);
  int distance = getDistance();                   //Check that there are no objects ahead
  if(distance >= stopdist)                        //If there are no objects within the stopping distance, move forward
  {
    go_forward();
  }
  while(distance >= stopdist)                     //Keep checking the object distance until it is within the minimum stopping distance
  {
    distance = getDistance();
    delay(250);
  }
  //stopMove();                                     //Stop the motors
  int turnDir = checkDirection();                 //Check the left and right object distances and get the turning instruction
  Serial.print(turnDir);
}
void go_forward()
{
    leftm.run(FORWARD); 
    rightm.run(FORWARD);
}
void turn_left(int duration)
{
  leftm.setSpeed(50);
  rightm.setSpeed(50);
  leftm.run(BACKWARD); 
  rightm.run(FORWARD);
  delay(duration);
  leftm.run(RELEASE); 
  rightm.run(RELEASE);
}
void turn_right(int duration)
{
   leftm.run(FORWARD); 
   rightm.run(BACKWARD);
   delay(duration);
   leftm.run(RELEASE); 
   rightm.run(RELEASE);
}
void u_turn(int duration)
{
  turn_left(700);
}
int getDistance()                                   //Measure the distance to an object
{
  unsigned long pulseTime;                          //Create a variable to store the pulse travel time
  int distance;                                     //Create a variable to store the calculated distance
  digitalWrite(trig, HIGH);                         //Generate a 10 microsecond pulse
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  pulseTime = pulseIn(echo, HIGH, timeOut);         //Measure the time for the pulse to return
  distance = (float)pulseTime * 340 / 2 / 10000;    //Calculate the object distance based on the pulse time
  return distance;
}

int checkDirection()                                            //Check the left and right directions and decide which way to turn
{
  int distances [2] = {0,0};                                    //Left and right distances
  int turnDir = 1;                                              //Direction to turn, 0 left, 1 reverse, 2 right
  servo.write(180);                                         //Turn servo to look left
  delay(500);
  distances [0] = getDistance();                                //Get the left object distance
  servo.write(0);                                           //Turn servo to look right
  delay(1000);
  distances [1] = getDistance();                                //Get the right object distance
  if (distances[0]>=200 && distances[1]>=200)                   //If both directions are clear, turn left
    turnDir = 0;
  else if (distances[0]<=stopdist && distances[1]<=stopdist)    //If both directions are blocked, turn around
    turnDir = 1;
  else if (distances[0]>=distances[1])                          //If left has more space, turn left
    turnDir = 0;
  else if (distances[0]<distances[1])                           //If right has more space, turn right
    turnDir = 2;
  return turnDir;
}
