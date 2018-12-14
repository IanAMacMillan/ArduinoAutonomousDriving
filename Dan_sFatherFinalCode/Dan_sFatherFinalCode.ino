//Includes
#include <NewPing.h>
#include <Servo.h>
Servo servo;

/*Defining the constants which keep track of where the motors and such are connected*/
#define mot1 A0
#define mot2 A1
#define mot3 A2
#define mot4 A3
#define enL 11
#define enR 3
#define servo_pin 8
#define irC A5

int irR = 12;
int irL = 13;

int left=13;
int right=12;
#define center A5

int leftdist =0;
int rightdist =0;
int maxdist = 18;

int distanceFromStart = 0;
float initialTime = 0;
long currentDistance1;
long lastDistance1;
long currentDistance2 = 0;
long lastDistance2;
int duration1;
long currentDistance3;
int duration2;
bool parkingSpotFound = false;
long depth1;
int nowTimeBackingUp = 0;

NewPing sonF(2,10,200);
NewPing sonL(5,6,200);
NewPing sonR(A4,9,200);
NewPing sonB(7,4,200);

void setup() {

  Serial.begin(9600);
  //Initializing the pins for the motors
  pinMode(mot1,OUTPUT);
  pinMode(mot2,OUTPUT);
  pinMode(mot3,OUTPUT);
  pinMode(mot4,OUTPUT);
  pinMode(enL,OUTPUT);
  pinMode(enR,OUTPUT);
 
  //initializes pins as inputs from IRs

  pinMode(left,INPUT);
  pinMode(right,INPUT);
  pinMode(center,INPUT);
  //Initializing the pins for the sensors
  pinMode(irR,INPUT);
  pinMode(irC,INPUT);
  pinMode(irL,INPUT);
  //Initializing the servo pins
  servo.attach(servo_pin);
}




/////LINE FOLLOW AND WALL FOLLOW CODE

void GoForwardFast()
{
  analogWrite(enR, 140);
  analogWrite(enL, 140);
  analogWrite(mot1,255);
  analogWrite(mot2,0);
  analogWrite(mot3,255);
  analogWrite(mot4,0);
}
void GoForwardSlow()
{
  analogWrite(enR, 80);
  analogWrite(enL, 80);
  analogWrite(mot1,255);
  analogWrite(mot2,0);
  analogWrite(mot3,255);
  analogWrite(mot4,0);
  
}

void Stop()
{
  analogWrite(enR,0);
  analogWrite(enL,0);
}

void Left()
{
  analogWrite(enR, 140);
  analogWrite(enL, 0);
  analogWrite(mot3, 255);
  analogWrite(mot4, 0);

  //delay(100);
}
void Right()
{
  analogWrite(enR, 0);
  analogWrite(enL, 140);
  analogWrite(mot1, 255);
  analogWrite(mot2, 0);

  //delay(100);
}
void linefollow()
{
  
  {
    GoForwardSlow();
    
    int leftint = digitalRead(left);
    int rightint = digitalRead(right);
    int centerint = digitalRead(center);
    
    if(leftint && !(rightint))
    {
      Stop();
      delay(100);
      
      leftint = digitalRead(left);
      rightint = digitalRead(right);
      centerint = digitalRead(center);
    
        while(leftint && !(rightint)  )
        {
          Left();
          //Stop();
          //delay(100);
          leftint = digitalRead(left);
          rightint = digitalRead(right);
          centerint = digitalRead(center);
    
          
        }
        while(!(centerint))
        {
          Left();
          leftint = digitalRead(left);
          rightint = digitalRead(right);
          centerint = digitalRead(center);
          
        }
      leftint = digitalRead(left);
      rightint = digitalRead(right);
      centerint = digitalRead(center);
    }
    
    if(rightint && !(leftint))
    {
      Stop();
      delay(100);
      leftint = digitalRead(left);
      rightint = digitalRead(right);
      centerint = digitalRead(center);
        while(rightint && !(leftint))
          {
            Right();
            //Stop();
            //delay(100);
            leftint = digitalRead(left);
            rightint = digitalRead(right);
            centerint = digitalRead(center);
            
          }
         while(!(centerint))
          {
            Right();
            leftint = digitalRead(left);
            rightint = digitalRead(right);
            centerint = digitalRead(center);
          
          }
      leftint = digitalRead(left);
      rightint = digitalRead(right);
      centerint = digitalRead(center);
    }
    
      
       
  }
}
void wallfollow()
{
  
  GoForwardFast();
 
  leftdist = sonL.ping_cm();
  rightdist = sonR.ping_cm();
  //Serial.println(leftdist);
  
  //senses right wall
    if(rightdist < maxdist)
    {
      leftdist = sonL.ping_cm();
      rightdist = sonR.ping_cm();
 
      while(rightdist < maxdist)
      {
         Left();
         delay(10);
         GoForwardFast();
         leftdist = sonL.ping_cm();
         rightdist = sonR.ping_cm();
      }
    }
  leftdist = sonL.ping_cm();
  rightdist = sonR.ping_cm();

  //senses left wall
    if(leftdist < maxdist)
    {
      leftdist = sonL.ping_cm();
      rightdist = sonR.ping_cm();
      
      while(leftdist < maxdist)
      {
        Right();
        delay(10);
        GoForwardFast();
         leftdist = sonL.ping_cm();
         rightdist = sonR.ping_cm();
       
      }
    }
}

///////////OBSTACLE COURSE CODE

void turnInPlace(double theta, bool dir) {
  //Constant that holds the rate at which the cart turns at full speed
  double omega = 232;
  //Turning the motors off
  STOP();
  //If the direction variable denotes left, set the left motors to back
  // and the right motors to forwards
  if (!(dir)) {
    analogWrite(mot1, 0);
    analogWrite(mot2, 250);  
    analogWrite(mot3, 250);
    analogWrite(mot4, 0);
  } else {
    analogWrite(mot1, 250);
    analogWrite(mot2, 0);  
    analogWrite(mot3, 0);
    analogWrite(mot4, 250);
  }
  //Turning the motors on
  analogWrite(enL, 255);
  analogWrite(enR, 255);
  //Determining the time for the turn
  double t = theta/omega;
  //Setting the pwm to maximum, waiting, and then setting to 0
  delay(static_cast<int>(t*1000));
  STOP();
}

//Sets the velocity to high or low
//  an input of true corresponds to full speed
//  an input of false corresponds to backing up
void setVel(bool vel) {
  analogWrite(enL, 90);
  analogWrite(enR, 90);
  if (vel) {
    analogWrite(mot1, 255);
    analogWrite(mot2, 0);  
    analogWrite(mot3, 255);
    analogWrite(mot4, 0);
  } else {
    analogWrite(mot1, 0);
    analogWrite(mot2, 255);  
    analogWrite(mot3, 0);
    analogWrite(mot4, 255);
  }
}

void STOP() {
  analogWrite(mot1, 0);
  analogWrite(mot2, 0);  
  analogWrite(mot3, 0);
  analogWrite(mot4, 0);
  analogWrite(enL, 0);
  analogWrite(enR, 0);
}

int pingL() {
  double result = 0;
  double tmp;
  int count = 0;
  for(int i = 0; i < 5; i++) {
    tmp = sonL.ping_cm();
    if (tmp > 0) {
      result = result + tmp;
      count++;
    }
  }
  if (result == 0) {
    return 0;
  }
  return (static_cast<int>(result/count));
}

int pingR() {
  double result = 0;
  double tmp;
  int count = 0;
  for(int i = 0; i < 10; i++) {
    tmp = sonR.ping_cm();
    if (tmp > 0) {
      result = result + tmp;
      count++;
    }
  }
  if (result == 0) {
    return 0;
  }
  return (static_cast<int>(result/count));
}

void checkLR(int sweeparr[]) {
  sweeparr[0] = sonL.ping_cm();
  sweeparr[1] = sonR.ping_cm();
}

bool sweep() {
  int theta = 0;
  while (theta <= 180) {
    servo.write(theta);
    delay(20);
    int p = sonF.ping_cm();
    if (0 < p && p < 15*sin(theta*(3.1415)/180)+7) {
      servo.write(30);
      delay(40);
      return true;
    }
    theta += 10;
  }
  servo.write(30);
  delay(40);
  return false;
}

int wallFollow(int t_i) {
  setVel(true);
  //Holds whether there is an object
  bool obj;
  int objs = 0;
  int theta = t_i;
  //Holds the distances of objects to the left and right of the cart
  int LR[2];
  //else If theta greater than 60 check the left side and if its open
  //  turn to the left
  if (theta > 80) {
    int dist;
    dist = pingL();
    if (dist > 10 || dist == 0) {
      delay(300);
      STOP();
      delay(1000);
      turnInPlace(90,true);
      theta -= 90;
    }
  }
  //If theta less than -60 check the right side and if its open
  //  turn to the right
  else if (theta < -80) {
    int dist;
    dist = pingR();
    if (dist > 10 || dist == 0) {
      delay(300);
      STOP();
      delay(1000);
      turnInPlace(90,false);
      theta += 90;
    }
  }
  //Check if there's obstacles
  obj = sweep();
  if (obj) {
    //If there's an obstacle stop
    STOP();
    delay(100);
    //Sweep and store the output of the sweep
    checkLR(LR);
    //Back up
    setVel(false);
    delay(500);
    STOP();
    //Alternating which direction one prefers
    if (objs%2 == 0) {
      //If left open and not facing "down" turn left
      if (theta > -80 && ((LR[0] > LR[1] && LR[0] >= 10) || LR[0] == 0)) {
        delay(1000);
        turnInPlace(45,true);
        theta -= 45;
      }
      //Else if right open and not facing "up" turn right
      else if (theta < 80 && ((LR[1]>LR[0] && LR[1] >= 10) || LR[1] == 0)) {
        delay(1000);
        turnInPlace(45,false);
        theta += 45;
      }
      //Else spin 180
      else {
        delay(1000);
        turnInPlace(90,true);
        turnInPlace(90,true);
        theta = theta - 180;
        setVel(true);
        delay(200);
      }
    }
    else {
      //If right open and not facing "up" turn right
      if (theta < 80 && (LR[1] >= 8 || LR[1] == 0)) {
        delay(1000);
        turnInPlace(45,false);
        theta += 45;
      }
      //Else if left open and not facing "down" turn left
      else if (theta > -80 && (LR[0] >= 8 || LR[0] == 0)) {
        delay(1000);
        turnInPlace(45,true);
        theta -= 45;
      }
      //Else spin 180
      else {
        delay(1000);
        turnInPlace(90,true);
        turnInPlace(90,true);
        theta = theta - 180;
        setVel(true);
        delay(200);
      }
    }
    objs++;
  }
  //Keeping the angle between -180 and 180
  if (theta > 180) {
    theta = 360 - theta;
  }
  else if (theta < -180) {
    theta = theta + 360;
  }
  //Starting going again
  setVel(true);
  return theta;
}

int obstacleCourse(int t_i) {
  setVel(true);
  //Holds whether there is an object
  bool obj;
  int objs = 0;
  int theta = t_i;
  //Holds the distances of objects to the left and right of the cart
  int LR[2];
  //If we are going backwards turn around
  if (theta > 130 || theta < -130) {
    STOP();
    delay(1000);
    turnInPlace(90,false);
    turnInPlace(90,false);
    theta += 180;
  }
  //else If theta greater than 60 check the left side and if its open
  //  turn to the left
  if (theta > 80) {
    int dist;
    dist = pingL();
    if (dist > 65 || dist == 0) {
      delay(300);
      STOP();
      delay(1000);
      turnInPlace(90,true);
      theta -= 90;
    }
  }
  //If theta less than -60 check the right side and if its open
  //  turn to the right
  else if (theta < -80) {
    int dist;
    dist = pingR();
    if (dist > 65 || dist == 0) {
      delay(300);
      STOP();
      delay(1000);
      turnInPlace(90,false);
      theta += 90;
    }
  }
  //Check if there's obstacles
  obj = sweep();
  if (obj) {
    //If there's an obstacle stop
    STOP();
    delay(100);
    //Sweep and store the output of the sweep
    checkLR(LR);
    //Back up
    setVel(false);
    delay(500);
    STOP();
    //Alternating which direction one prefers
    if (objs%2 == 0) {
      //If left open and not facing "down" turn left
      if (theta > -80 && (LR[0] >= 60 || LR[0] == 0)) {
        delay(1000);
        turnInPlace(90,true);
        theta -= 90;
      }
      //Else if right open and not facing "up" turn right
      else if (theta < 80 && (LR[1] >= 60 || LR[1] == 0)) {
        delay(1000);
        turnInPlace(90,false);
        theta += 90;
      }
      //Else spin 180
      else {
        delay(1000);
        turnInPlace(90,true);
        turnInPlace(90,true);
        theta = theta - 180;
        setVel(true);
        delay(200);
      }
    }
    else {
      //If right open and not facing "up" turn right
      if (theta < 80 && (LR[1] >= 60 || LR[1] == 0)) {
        delay(1000);
        turnInPlace(90,false);
        theta += 90;
      }
      //Else if left open and not facing "down" turn left
      else if (theta > -80 && (LR[0] >= 60 || LR[0] == 0)) {
        delay(1000);
        turnInPlace(90,true);
        theta -= 90;
      }
      //Else spin 180
      else {
        delay(1000);
        turnInPlace(90,true);
        turnInPlace(90,true);
        theta = theta - 180;
        setVel(true);
        delay(200);
      }
    }
    objs++;
  }
  //Keeping the angle between -180 and 180
  if (theta > 180) {
    theta = 360 - theta;
  }
  else if (theta < -180) {
    theta = theta + 360;
  }
  //Starting going again
  setVel(true);
  return theta;
}
//////////////PARALLEL PARK CODE

void ParallelPark(){

  //Position Ultrasonic/Servos looking right
  //Move car forward indefinitely
  

  analogWrite(mot1, 255);
  analogWrite(mot2, 0);
  analogWrite(mot3, 255);
  analogWrite(mot4, 0);
  analogWrite(enL, 255);
  analogWrite(enR, 255);
//  delay(5000);

  
  float initialTime = millis();
  
  float currentTime = millis() - initialTime;
currentDistance1 = sonR.ping_cm();
lastDistance1 = sonR.ping_cm();


  //Find a parking spot 
  while(parkingSpotFound == false){
    
  //Get current depth and distance
  lastDistance1 = currentDistance1;
  
  currentDistance1 = sonR.ping_cm();

  depth1 = currentDistance1 - lastDistance1;
        //  Serial.println(currentDistance1);
   Serial.println("parking not found yet");  
  Serial.println(depth1);

  if(depth1 > 25){
    initialTime = millis();
      while(depth1 > 25)
      {
      delay(250);
      currentDistance2 = sonR.ping_cm();
      depth1 = currentDistance2 - lastDistance1;
      //lastDistance1 = currentDistance1;
      currentTime = millis() - initialTime;
      if(currentTime>250){
        parkingSpotFound = true;
        
        Serial.println("found Parking");
        break;

      }
      }
    
  }
  delay(250);
    lastDistance1 = currentDistance1;

  }

  //Stop Car
  Serial.println("we out");
  analogWrite(mot1, 0);
  analogWrite(mot3, 0);

  delay(2000);

  currentTime = millis();

//go backward
  analogWrite(mot1, 0);
  analogWrite(mot2, 255);
  analogWrite(mot3, 0);
  analogWrite(mot4, 255);

  
  delay(550);

  //Stop Car
  analogWrite(mot2, 0);
  analogWrite(mot4, 0);
delay(1000);

  //Turn90degrees
//  analogWrite(mot1, 255);
//  //analogWrite(mot2, 0);
// //analogWrite(mot3, 0);
// analogWrite(mot4, 255);
turnInPlace2(85,true);
 


  delay (425);


//stop
  analogWrite(mot1, 0);
  analogWrite(mot2, 0);
  analogWrite(mot3, 0);
  analogWrite(mot4, 0);
  
  delay (1000);
//back up
  analogWrite(mot1, 0);
  analogWrite(mot2, 255);
  analogWrite(mot3, 0);
  analogWrite(mot4, 255);
Serial.println("backing");

currentDistance1 = sonR.ping_cm();
int currentBackDistance = sonB.ping_cm();
//while ((currentDistance1 > 20 || currentDistance1 == 0) && (currentBackDistance > 10 || currentBackDistance == 0))
//{
//  
//  currentDistance1 = sonR.ping_cm();
//  currentBackDistance = sonB.ping_cm();
//    //go back x distance
//   Serial.println(currentDistance1);
//Serial.println(currentBackDistance);
//  
//}

while ((currentBackDistance > 10 || currentBackDistance == 0))
{
  
  currentDistance1 = sonR.ping_cm();
  currentBackDistance = sonB.ping_cm();
    //go back x distance
   Serial.println(currentDistance1);
Serial.println(currentBackDistance);
  
}



  analogWrite(mot1, 0);
  analogWrite(mot2, 0);
  analogWrite(mot3, 0);
  analogWrite(mot4, 0);
  delay (1000);

  //turn 90
 // analogWrite(mot1, 255);
  turnInPlace(85,false);
 //analogWrite(mot4, 255);
 


  delay (425);


  analogWrite(mot1, 0);
  analogWrite(mot2, 0);
  analogWrite(mot3, 0);
  analogWrite(mot4, 0);

  
  


  int back = sonB.ping_cm();
  int front =  sonF.ping_cm();
  //parkingSpotFound = false;

  if((front < 80 || front == 0) && (back  < 80 || back == 0))
  { 
    centerYourself();
  }
  else
  {
     parkingSpotFound = false;
    int currentTimeBackingUp = millis();
    
    //Need to find the spot and redo the back in sequence
    //First test if the spot is behind the car
    currentDistance1 = sonR.ping_cm();
    lastDistance1 = sonR.ping_cm();

     analogWrite(mot1, 0);
    analogWrite(mot2, 255);
    analogWrite(mot3, 0);
    analogWrite(mot4, 255);


  //Find a parking spot 
  while(parkingSpotFound == false){
   
  nowTimeBackingUp = millis() - currentTimeBackingUp;
  currentDistance1 = sonR.ping_cm();

  depth1 = currentDistance1 - lastDistance1;
        //  Serial.println(currentDistance1);
   Serial.println("parking not found yet");  
  Serial.println(depth1);

  if(nowTimeBackingUp > 3000){
    break;
  }

  if(depth1 > 15){
        parkingSpotFound = true;
        delay (100);


        Serial.println("found Parking");

          //Stop Car
      Serial.println("we out");
      analogWrite(mot2, 0);
      analogWrite(mot4, 0);
      delay(1000);

      //turn 90
       turnInPlace2(90,true);

      delay(425);

      analogWrite(mot1, 0);
      analogWrite(mot2, 255);
      analogWrite(mot3, 0);
      analogWrite(mot4, 255);

      currentBackDistance = sonB.ping_cm();
      while(currentBackDistance > 4)
      {
        currentBackDistance = sonB.ping_cm();

      }

      analogWrite(mot2, 0);
      analogWrite(mot4, 0);

      delay(1000);

       turnInPlace2(90,false);
       
      delay(425);
      centerYourself();

        break;
        
  }
//  delay(250);

  }

  if(parkingSpotFound == false && nowTimeBackingUp > 3000)
  {

    analogWrite(mot1, 255);
    analogWrite(mot2, 0);
    analogWrite(mot3, 255);
    analogWrite(mot4, 0);


 while(parkingSpotFound == false){
   
  
  currentDistance1 = sonR.ping_cm();

  depth1 = currentDistance1 - lastDistance1;
        //  Serial.println(currentDistance1);
   Serial.println("parking not found yet");  
  Serial.println(depth1);


  if(depth1 > 15){
        parkingSpotFound = true;
        delay (100);


        Serial.println("found Parking");

          //Stop Car
      Serial.println("we out");
      analogWrite(mot1, 0);
      analogWrite(mot3, 0);
      delay(1000);

      //turn 90
       turnInPlace2(90,true);

      delay(425);

      analogWrite(mot1, 0);
      analogWrite(mot2, 255);
      analogWrite(mot3, 0);
      analogWrite(mot4, 255);

      currentBackDistance = sonB.ping_cm();
      while(currentBackDistance > 4)
      {
        currentBackDistance = sonB.ping_cm();

      }

      analogWrite(mot2, 0);
      analogWrite(mot4, 0);

      delay(1000);

       turnInPlace2(90,false);
       
      delay(425);

      centerYourself();

      

      break;
        
  }
//  delay(250);

  }
    
  }


  

  //Stop Car
  Serial.println("we out");
  analogWrite(mot2, 0);
  analogWrite(mot3, 0);
  analogWrite(mot1, 0);
  analogWrite(mot4, 0);
   
  }
  
//Stop Car
  Serial.println("we out");
  analogWrite(mot2, 0);
  analogWrite(mot3, 0);
  analogWrite(mot1, 0);
  analogWrite(mot4, 0);


  
  
  
}

void centerYourself()
{
  int back = sonB.ping_cm();
  int front =  sonF.ping_cm();

  int diff = front-back;
  if(diff < -5)
  {
    while(diff < -5){
      analogWrite(mot1, 0);
      analogWrite(mot2, 255);
      analogWrite(mot3, 0);
      analogWrite(mot4, 255);
      back = sonB.ping_cm();
      front =  sonF.ping_cm();
      diff = front-back;
      
    }
    
  }
  else if(diff > 5)
  {

      while(diff > 5){
      analogWrite(mot1, 255);
      analogWrite(mot2, 0);
      analogWrite(mot3, 255);
      analogWrite(mot4, 0);
      back = sonB.ping_cm();
      front =  sonF.ping_cm();
      diff = front-back;
      
    }
    
  }
 
      analogWrite(mot1, 0);
      analogWrite(mot2, 0);
      analogWrite(mot3, 0);
      analogWrite(mot4, 0);

      delay(100000);
  
}


/*turnInPlace takes a theta in degrees and dir as a bool with True being
 * interpreted as right and False being interpreted as left
 */
void turnInPlace2(double theta, bool dir) {
  //Constant that holds the rate at which the cart turns at full speed
  double omega = 232;
  //Turning the motors off
    analogWrite(mot1, 0);
    analogWrite(mot2, 0);  
    analogWrite(mot3, 0);
    analogWrite(mot4, 0);
  //If the direction variable denotes left, set the left motors to back
  // and the right motors to forwards
  if (!(dir)) {
    analogWrite(mot1, 0);
    analogWrite(mot2, 130);  
    analogWrite(mot3, 130);
    analogWrite(mot4, 0);
  } else {
    analogWrite(mot1, 130);
    analogWrite(mot2, 0);  
    analogWrite(mot3, 0);
    analogWrite(mot4, 130);
  }
  //Determining the time for the turn
  double t = theta/omega;
  //Setting the pwm to maximum, waiting, and then setting to 0
  delay(static_cast<int>(t*1000));
  analogWrite(mot1, 0);
    analogWrite(mot2, 0);  
    analogWrite(mot3, 0);
    analogWrite(mot4, 0);
}

void loop()
{
  int leftint = digitalRead(left);
  int rightint = digitalRead(right);
  int centerint = digitalRead(center);
  int theta = 0;
  while(!(centerint) && !(leftint) && !(rightint))
  { 
    theta = 0;
    wallFollow(theta);
    leftint = digitalRead(left);
    rightint = digitalRead(right);
    centerint = digitalRead(center);
    
  }
  //default is to line follow
  linefollow();
  //transition to obstacle course
  int count = 0;
  int transition = 2;
  theta = 0;
  while((centerint) && (leftint) && (rightint))
  {
    Stop();
    delay(1000);
    GoForwardSlow();
    delay(250);
    Stop();
    leftint = digitalRead(left);
    rightint = digitalRead(right);
    centerint = digitalRead(center);
    if((centerint) && (leftint) && (rightint))
    {
      count = count +1;
    }
    while(count >= transition)
    {
      theta = obstacleCourse(theta);
      leftint = digitalRead(left);
      rightint = digitalRead(right);
      centerint = digitalRead(center);
      
      if((centerint) && (leftint) && (rightint))
      {
      count = 0;
      transition = 2;
          while((centerint) && (leftint) && (rightint))
          {
            Stop();
            delay(1000);
            GoForwardSlow();
            delay(250);
            Stop();
            leftint = digitalRead(left);
            rightint = digitalRead(right);
            centerint = digitalRead(center);
            if((centerint) && (leftint) && (rightint))
            {
              count = count +1;
            }
            //transition to paralell park
            if(count >= transition)
            {
              servo.write(90);
              ParallelPark();
            }
          }
      }
    }
