#include <PS2X_lib.h> 
//Khai báo chân PS2 kết nối với Arduino 
#define PS2_DAT     13  
#define PS2_CMD     11 
#define PS2_SEL     10 
#define PS2_CLK     12
//Khai báo chân Motor1
#define enA 6 // Chân điều khiển PWM Motor 1
#define in1 7
#define in2 8
//Khai báo chân Motor2 
#define enB 5 // Chân điều khiển PWM Motor 2
#define in3 3
#define in4 4

//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

PS2X ps2x; // create PS2 Controller Class
int error = 0;
byte type = 0;
byte vibrate = 0;

int lx=0;
int ly=0;
int rx=0;
int ry=0;
//------------------------------------------------------------------------

void moveForward(int speed_motor)  // Hàm robot đi tiến với tốc độ tùy chỉnh
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, speed_motor);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, speed_motor);
  }

//-----------------------------------------------------------------

void moveBack(int speed_motor)// Hàm robot đi lùi với tốc độ tùy chỉnh
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, speed_motor);
    digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, speed_motor);
}

//-------------------------------------------------------

void turnLeft(int speed_motor)          //turn robot to the left || Quay robot sang trái
{
  digitalWrite(in1,HIGH );
  digitalWrite(in2, LOW);
  analogWrite(enA, speed_motor);
   digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, speed_motor);
}

//----------------------------------------------

void turnRight(int speed_motor)            // turn to the right   || Quay robot sang phải
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, speed_motor);
   digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, speed_motor);
 }
 
//-------------------------------------------------

void moveStop()           // stop the robot       || Hàm dừng robot
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

//------------------------------------------------

void moveForward_2Motor(int speed_Right,int speed_Left) // Hàm robot đi tiến với tốc độ 2 động cơ tùy chỉnh
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, speed_Right);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, speed_Left);
}

//--------------------------------------------------------

void moveBack_2Motor(int speed_Right,int speed_Left) // Hàm robot đi lùi với tốc độ 2 động cơ tùy chỉnh
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, speed_Right);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, speed_Left);
}

void setup(){
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  Serial.begin(57600);
  
  delay(300);  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true)");
  else
    Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
  case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }
}
void loop() {
   if(error == 1) //skip loop if no controller found
    return; 
  
  if(type == 2){
    ps2x.read_gamepad();
  }
    else {
    ps2x.read_gamepad(false, vibrate);
   
    vibrate = ps2x.Analog(PSB_CROSS);
    
    lx= ps2x.Analog(PSS_LX);
    ly= ps2x.Analog(PSS_LY);
    rx= ps2x.Analog(PSS_RX);
    ry= ps2x.Analog(PSS_RY);
    
    if ( lx != 123 || ly != 132 || rx != 123 || ry != 132)
    {
        if ((lx == 123) && (ly ==0) || (rx == 123) && (ry == 0))
        {
            moveForward(200);
        }
        else if ((lx == 123) && (ly == 255) || (rx == 123) && (ry == 255))
        {
            moveBack(200);
        }
        else if ((lx == 0) && (ly == 132) || (rx == 0) && (ry == 132))
        {
            turnLeft(200); 
        }
         else if ((lx == 255) && (ly == 132) || (rx == 255) && (ry == 132))
        {
            turnRight(200);
        }
        else if ((lx == 255) && (ly == 0) || (rx == 255) && (ry == 0))
        {
            moveForward_2Motor(150, 200);
        }
        else if ((lx == 0) && (ly == 0) || (rx == 0) && (ry == 0))
        {
            moveForward_2Motor(200, 150);
        }
        else if ((lx == 0) && (ly == 255) || (rx == 0) && (ry == 255))
        {
            moveBack_2Motor(150, 200);
        }
        else if ((lx == 255 ) && (ly ==255) || (rx == 255) && (ry ==255))
        {
            moveBack_2Motor(200, 150);
        }
    }
    else if(ps2x.Button(PSB_PAD_UP))
    {     
        moveForward(200); 
    }
    else if(ps2x.Button(PSB_PAD_RIGHT))
    {
        turnRight(200);
    }
    else if(ps2x.Button(PSB_PAD_LEFT))
    {
        turnLeft(200);
    }
    else if(ps2x.Button(PSB_PAD_DOWN))
    { 
        moveBack(200);     
    }   
    else {
      moveStop();
    }
    
  }
}
