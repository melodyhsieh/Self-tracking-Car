#include<SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

//motor
#define MotorL_I1     2  //定義 I1 接腳（左）
#define MotorL_I2     6  //定義 I2 接腳（左）
#define MotorR_I3     9 //定義 I3 接腳（右）
#define MotorR_I4     4 //定義 I4 接腳（右）
#define MotorL_PWML   3  //定義 ENA (PWM調速) 接腳
#define MotorR_PWMR    5  //定義 ENB (PWM調速) 接腳
//tracking
#define R2  A4  // Define Second Right Sensor Pin
#define R1  A3  // Define First Right Sensor Pin
#define M   A2  // Define Middle Sensor Pin
#define L1  A1  // Define First Left Sensor Pin
#define L2  A0  // Define Second Left Sensor Pin

//// p control params
int _Tp;                         // Velocity of Car
double _w2 = 2 ;                       // Weight Value for the Outer Sensor
double _w1 = _w2/2.0;                     // Weight Value for the Inner Sensor
double _Kp = 50;                          // _Kp Parameter
double _Ki = 5.0;                         // _Ki Parameter
double _integral = 0;                     // Integral from Starting point


#define RST_PIN      A5        // RFID resetpin
#define SS_PIN       10       // RFID selection pin
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define Electromagnet 8


#define pace_fast 150
#define pace_slow 100

bool pickup = 0;
int state = 0;
//0 -> stop
//1 -> remote
//2 -> track
char c;


void setup() {
  #ifndef DEBUG
  #define DEBUG
  Serial.begin(9600);
  #endif
  pinMode(MotorL_I1,      OUTPUT);
  pinMode(MotorL_I2,      OUTPUT);
  pinMode(MotorR_I3,      OUTPUT);
  pinMode(MotorR_I4,      OUTPUT);
  pinMode(MotorL_PWML,    OUTPUT);
  pinMode(MotorR_PWMR,    OUTPUT);
  pinMode(Electromagnet,  OUTPUT);
  pinMode(R1, INPUT); 
  pinMode(R2, INPUT);
  pinMode(M,  INPUT);
  pinMode(L1, INPUT);
  pinMode(L2, INPUT);
  
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() { 
    if (state == 1) {
      if (Serial.available()){
        c=Serial.read();
        Serial.println(c);
        if (c == 'm') {
          state = 2;
          node('q');
          }
        else {
          node(c);
        }
      }
    }
    else if (state == 2) {
      tracking(R2,R1,M,L1,L2);
      if (mfrc522.PICC_IsNewCardPresent()  && mfrc522.PICC_ReadCardSerial()){
        node('q');
        state = 1;
        Serial.println("read");
      }
    }
    else {
      if (Serial.available()){
        c=Serial.read();
        Serial.println("state0");
        Serial.println(c);
        if (c == 'm') {     
            state = 1;
        }
      }
    }
}

void MotorInverter(int motor, bool& dir) {
    
    if(motor==MotorL_PWML){
        if(dir){
            digitalWrite(MotorL_I1,HIGH);
            digitalWrite(MotorL_I2,LOW);
        }
        else{
            digitalWrite(MotorL_I1,LOW);
            digitalWrite(MotorL_I2,HIGH);
        }
    }
    if(motor==MotorR_PWMR){
        if(dir){
            digitalWrite(MotorR_I3,HIGH);
            digitalWrite(MotorR_I4,LOW);
        }
        else{
            digitalWrite(MotorR_I3,LOW);
            digitalWrite(MotorR_I4,HIGH);
        }
    }
}

void MotorWriting(double vR, double vL) {
    bool R_dir,L_dir;
    if(vR<0)
    {
        R_dir=false;
        vR=-vR;
    }
    else R_dir=true;
    //Serial.println(vL);
    if(vL<0)
    {
        L_dir=false;
        vL=-vL;
    }
    else L_dir=true;
    MotorInverter(MotorR_PWMR,R_dir);
    MotorInverter(MotorL_PWML,L_dir);
    analogWrite(MotorL_PWML,vL);
    analogWrite(MotorR_PWMR,vR);
    //Serial.println(vL);
}

void node(char dir){
  if (dir == 'w'){
    MotorWriting(pace_fast, pace_fast);
    delay(100);
  }
  else if (dir == 's'){
    MotorWriting((-1) * pace_fast,(-1) * pace_fast);
    delay(100);
  }
  else if (dir == 'a'){
    MotorWriting(pace_fast, (-1) * pace_fast);
    delay(100);
  }
  else if (dir == 'd'){
    MotorWriting((-1) * pace_fast, pace_fast);
    delay(100);
  }
  else if (dir == '2'){
    MotorWriting(pace_slow, pace_slow);
    delay(100);
  }
  else if (dir == 'x'){
    MotorWriting((-1) * pace_slow,(-1) * pace_slow);
    delay(100);
  }
  else if (dir == 'z'){
    MotorWriting(pace_slow, (-1) * pace_slow);
    delay(100);
  }
  else if (dir == 'c'){
    MotorWriting((-1) * pace_slow, pace_slow);
    delay(100);
  }
  else if (dir == 'q' or dir == 'p') {
    MotorWriting(0, 0);
    delay(100);
  }
  else if(dir == 'f') {
    if (pickup == 0) {
      digitalWrite(Electromagnet, HIGH);
      Serial.println(pickup);
      pickup = 1;
    }
    else {
      digitalWrite(Electromagnet, LOW);
      Serial.println(pickup);
      pickup = 0;
    }
    delay(100);
  }
}

void tracking(int r2,int r1,int m,int l1,int l2){
    r1 = digitalRead(R1); // right-outer sensor
    r2 = digitalRead(R2); // right-inner sensor
    m = digitalRead(M);  // middle sensor
    l2 = digitalRead(L1); // left-inner sensor
    l1 = digitalRead(L2); // left-outer sensor
    _Tp = 140;
    double error=_w2*l2+_w1*l1-_w2*r2-_w1*r1;           //left 1 right 0 error>0 turn left, vice versa
    if((l1+l2==2)||(l1+m==2)||(r1+m==2)||(r1+r2==2)) error=error/2; 
    double revise = 2/3;                                           //need to test
    _integral = revise*_integral + error;      
    if(error>=1.5 || error<=-1.5 ) _Tp=0;      
    double power=_Kp*error + _Ki*_integral;
    double vR=_Tp+power;
    double vL=_Tp-power; 
    MotorWriting((int)vR, (int)vL); 

}

