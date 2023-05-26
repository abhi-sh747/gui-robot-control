#include <SoftwareSerial.h>

// defining motor driver's input pins
#define md1 4
#define md2 5
#define md3 6
#define md4 7
#define enA 9
#define enB 10  

String input, inMoveDir;
int inSpeed = 0, inTime = 0;
unsigned long timeSinceInput = 0;
unsigned long timeSinceReceived = 0;

SoftwareSerial bluetooth(2,3);

void setup()
{
  pinMode(md1, OUTPUT);
  pinMode(md2, OUTPUT);
  pinMode(md3, OUTPUT);
  pinMode(md4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  Serial.begin(9600);
  bluetooth.begin(38400);
  bluetooth.setTimeout(100);

  while(!bluetooth) {;}

  Serial.println("Started");
}

void loop()
{
  if(bluetooth.available())
  {
    input = bluetooth.readString();
    input.trim();
    Serial.println("Actual input: ");
    Serial.println(input);
  
    timeSinceInput = millis();

    int break1 = input.indexOf('-');
    int break2 = input.indexOf('-', break1 + 1);
    int break3 = input.indexOf('-', break2 + 1);

    inMoveDir = input.substring(0, break1);
    inSpeed = input.substring(break1 + 1, break2).toInt();
    inTime = input.substring(break2 + 1, break3).toInt();
    if(inMoveDir == "stop")
    {
      Serial.println("IMMEDIATE STOP!!");
      stop();
      return;
    }

    if(inSpeed < 0) inSpeed = 0;
    if(inSpeed > 255) inSpeed = 255;
    if(inTime < 0) inTime = 0;
    if(inTime == 0 && inMoveDir != "stop")
    {
      Serial.println("Time 0: so stopped");
      stop();
      return;
    }

    printInput(inMoveDir, inSpeed, inTime);

    moveInDirection(inMoveDir);
    speedControl(inSpeed);
    do
    {
      if(bluetooth.available())
      {
        input = bluetooth.readStringUntil('-');
        input.trim();
        int breakS = input.indexOf('-');
        String tempMoveDir = input.substring(0, breakS);
    
        if(tempMoveDir == "stop")
        { 
          Serial.println("Mid input: ");
          Serial.println(input);
          Serial.println("Interrupt STOP!!");
          inTime = 0;
          inSpeed = 0;
          stop();
          return;
        }
      }

    } while(millis() - timeSinceInput < inTime);
    
  }
}

void printInput(String direction, int speed, int time)
{
  String tempDir = "stop";
  if(direction == "forward") tempDir = "forward";
  if(direction == "backward") tempDir = "backward";
  if(direction == "left") tempDir = "left";
  if(direction == "right") tempDir = "right";

  Serial.print("|");
  Serial.print(direction);
  Serial.print("|");
  Serial.print(speed);
  Serial.print("|");
  Serial.print(time);
  Serial.println("|");
  Serial.print("Direction: ");
  Serial.print(tempDir);
  Serial.print(", Speed: ");
  Serial.print(speed);
  Serial.print(", Time: ");
  Serial.print(time);
  Serial.println(" ms");
  Serial.println();
}

void moveInDirection(String direction)
{
  if(direction == "forward")        forward();
  else if(direction == "backward")  backward();
  else if(direction == "left")      left();
  else if(direction == "right")     right();
  else                              stop();
}

void speedControl(int speed)
{
  analogWrite(enA, speed);
  analogWrite(enB, speed);
}

// making the robot move forward
void forward()
{
  digitalWrite(md1, HIGH);
  digitalWrite(md2, LOW);
  digitalWrite(md3, HIGH);
  digitalWrite(md4, LOW);

}
 
// making the robot move backward
void backward()
{
  digitalWrite(md1, LOW);
  digitalWrite(md2, HIGH);
  digitalWrite(md3, LOW);
  digitalWrite(md4, HIGH);

}
 
// making the robot move left
void left()
{
  digitalWrite(md1, LOW);
  digitalWrite(md2, HIGH);
  digitalWrite(md3, HIGH);
  digitalWrite(md4, LOW);

}

// making the robot move right
void right()
{
  digitalWrite(md1, HIGH);
  digitalWrite(md2, LOW);
  digitalWrite(md3, LOW);
  digitalWrite(md4, HIGH);

}
 
// making the robot stop
void stop()
{
  digitalWrite(md1, LOW);
  digitalWrite(md2, LOW);
  digitalWrite(md3, LOW);
  digitalWrite(md4, LOW);

}