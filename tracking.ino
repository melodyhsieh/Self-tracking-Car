#include <require_cpp11.h>
#include <MFRC522Debug.h>
#include <MFRC522Hack.h>
#include <MFRC522.h>
#include <deprecated.h>
#include <MFRC522Extended.h>

#include <SoftwareSerial.h>
#include <SPI.h>

#define RST_PIN      2        // RFID resetpin
#define SS_PIN       10        // RFID selection pin
MFRC522 mfrc522(SS_PIN, RST_PIN);  // MFRC522 object declaration
SoftwareSerial I2CBT(9,8);   //bluetooth RX,TX

/*pin definition*/
/************************/
// L298N 馬達驅動板
// 宣告 MotorR 為右邊
// 宣告 MotorL 為左邊
#define MotorR_I1     1  //定義 I1 接腳
#define MotorR_I2     4  //定義 I2 接腳
#define MotorL_I3     6 //定義 I3 接腳
#define MotorL_I4     7 //定義 I4 接腳
#define MotorR_PWMR    3  //定義 ENA (PWM調速) 接腳
#define MotorL_PWML    5  //定義 ENB (PWM調速) 接腳
// 循線模組
#define L2  A0  // Define Second Right Sensor Pin
#define L1  A1  // Define First Right Sensor Pin
#define M   A2  // Define Middle Sensor Pin
#define R1  A3  // Define First Left Sensor Pin
#define R2  A4  // Define Second Leftt Sensor Pin

/**************************************/
/*   Variables Define Here            */
/*   Initialized in setup() Funciton  */
/**************************************/
double _Tp;                         // Velocity of Car
double _w2;                         // Weight Value for the Outer Sensor
double _w1;                         // Weight Value for the Inner Sensor
double _Kp;                         // _Kp Parameter
double _Kd;                         // _Kd Parameter
double _Ki;                         // _Ki Parameter
double _LastError;                  // Error in Last Stage
char   _cmd;                        // Command
bool  quit = false;
byte* idSize;
byte* id;
bool see = true;
double _integral;                   // Integral from Starting point
bool R_dir = true;                  // if dir == ture, mean right-motor is forwarding. On the other hand, backwarding.
bool L_dir = true;                  // if dir == ture, mean Left-motor is forwarding. On the other hand, backwarding.

void setup()
{    
    SPI.begin();      // Init SPI bus
    mfrc522.PCD_Init();   // Init MFRC522
    mfrc522.PCD_DumpVersionToSerial();

    /*define your pin mode*/

    //Serial.begin(9600);
    I2CBT.begin(9600);//bluetooth initialization
    pinMode(MotorR_I1,   OUTPUT);
    pinMode(MotorR_I2,   OUTPUT);
    pinMode(MotorL_I3,   OUTPUT);
    pinMode(MotorL_I4,   OUTPUT);
    pinMode(MotorR_PWMR, OUTPUT);
    pinMode(MotorL_PWML, OUTPUT);
    pinMode(R1, INPUT); 
    pinMode(R2, INPUT);
    pinMode(M,  INPUT);
    pinMode(L1, INPUT);
    pinMode(L2, INPUT);
     
    // Initialize Variables
    // You can Define you own value here.
    _cmd = 'n';                // Command
    _Tp    = 130;              // Velocity of Car
    _w2    = 13.5;             // Second Stage Weight
    _w1    = _w2/2.0;          // First Stage Weight
    _Kp    = 15;               // _Kp Parameter
    _Kd    = 0.03;             // _Kd Parameter
    _Ki    = 0.0;              // _Ki Parameter
    _LastError = 0;            // Error in Last Stage
    _integral  = 0; 
    Serial.println("Start!");
 }

//#include "track.h"
#include "node.h"
#include "bluetooth.h"
#include "RFID.h"

void loop()
{
  if(!quit){
    if((digitalRead(L2) == 1 & digitalRead(L1) == 1 & digitalRead(M) == 1 & digitalRead(R1) == 1) | (digitalRead(R2) == 1 & digitalRead(R1) == 1 & digitalRead(M) == 1 & digitalRead(L1) == 1)){
      id = rfid(idSize);
      if(id != 0 and see){
        send_byte(id,*idSize);
        see = node(ask_direction());
        while (!see){
            see = node(ask_direction());
        } 
      }
      else if(id == 0 and see){
        I2CBT.write('g');
        Serial.println("send 'g'");
        see = node(ask_direction());
        while (!see){
            see = node(ask_direction());
        }  
      }     
    }
    else {
      Tracing_Mode();
    };
  }
    /*id = rfid(idSize);
    if(id != 0){
      send_byte(id,*idSize);
    }
    //signal_to_python();
    /*TODO*/
    

       /*MotorWriting(80,80);
       delay (550);
       MotorWriting(0,0);
       delay(180);
       MotorWriting(-80,80);
       delay (600);
       MotorWriting(0,0);
       delay(180);
       MotorWriting(80,80);
       delay(550);
       MotorWriting(-45,-75);
       delay(650);

       MotorWriting(80,-80);
       delay (975);
       MotorWriting(0,0);
       delay(180);*/
    //Tracing_Mode();
    
}

