// importing library to create virtual hardware port which use the only hardware port available on UNO 
#include <SoftwareSerial.h>

// defining motor driver's input pins
#define md1 4
#define md2 5
#define md3 6
#define md4 7
#define enA 9
#define enB 10  

// defining required global variables
String input, inMoveDir;
int inSpeed = 0, inTime = 0;
unsigned long timeSinceInput = 0;
unsigned long timeSinceReceived = 0;

// creating a SoftwareSerial object for bluetooth with pin 2 as RX and pin 3 as TX 
SoftwareSerial bluetooth(2,3);

void setup()
{
  // initializing motor driver pins as output
  pinMode(md1, OUTPUT);
  pinMode(md2, OUTPUT);
  pinMode(md3, OUTPUT);
  pinMode(md4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  // initializing both the serial (b/w arduino and computer for testing) and 
  // bluetooth (b/w computer and arduino via bluetooth module)
  Serial.begin(9600);
  bluetooth.begin(38400);
  bluetooth.setTimeout(100);

  while(!bluetooth) {;}

  Serial.println("Started");
}

void loop()
{
  if(bluetooth.available()) // Non-blocking 
  {
    input = bluetooth.readString();                         // reading input from the user 
    input.trim();                                           // trimmig to remove any extra spaces at start and end
    Serial.println("Actual input: ");
    Serial.println(input);
  
    timeSinceInput = millis();                              // setting timeSinceInput variable to use as reference time to implement  
                                                            // a certain action (e.g., forward) for a certain time period (e.g., 5 sec)
    
    // storing the index of dashes in the string being used to separate different commands
    int break1 = input.indexOf('-');
    int break2 = input.indexOf('-', break1 + 1);
    int break3 = input.indexOf('-', break2 + 1);

    // separating out different commands from the input string
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
  
    // printing information for testing 
    printInput(inMoveDir, inSpeed, inTime);

     // making the car move in the required direction
    moveInDirection(inMoveDir);
    // making the car move at the desired speed
    speedControl(inSpeed);
    
//  making the certain action for the specified amount of time
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

// printing information regarding the received input
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

// changing direction of car based on the input
void moveInDirection(String direction)
{
  if(direction == "forward")        forward();
  else if(direction == "backward")  backward();
  else if(direction == "left")      left();
  else if(direction == "right")     right();
  else                              stop();
}

// changing speed of car based on the input
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
