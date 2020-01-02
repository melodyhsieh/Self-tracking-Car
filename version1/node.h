// dir = 0 : advance
// dir = 1 : left
// dir = 2 : right
// dir = 3 : back

// TODO: determine the behavior of each port when occuring a node(here represented as an integer)
#include "track.h"
bool node(int dir){
    if(dir == 0){
      Serial.println("go ahead");
      MotorWriting(80,80);
      delay(800);
      Tracing_Mode();
      return true;
    }
    else if(dir == 1){
       Serial.println("turn left");
       MotorWriting(80,80);
       delay (475);
       MotorWriting(0,0);
       delay(180);
       MotorWriting(80,-80);
       delay (525);
       MotorWriting(0,0);
       delay(180);
       MotorWriting(80,80);
       delay(500);
       Tracing_Mode();
       return true;
    }
    else if(dir == 2){
       Serial.println("turn right");
       MotorWriting(80,80);
       delay (475);
       MotorWriting(0,0);
       delay(180);
       MotorWriting(-80,80);
       delay (525);
       MotorWriting(0,0);
       delay(180);
       MotorWriting(80,80);
       delay(500);
       Tracing_Mode();
       return true;
    }
    else if(dir == 3){
       Serial.println("go back");
       MotorWriting(80,-80);
       delay (975);
       MotorWriting(0,0);
       delay(180);
       Tracing_Mode();
       return true;
    }
    else if(dir == 4){
       Serial.println("halt");
       MotorWriting(0,0);
       delay(200);
       return false;
    }
    else{
       Serial.println("Nothing");
       MotorWriting(0,0);
       delay(200);
       return false;
    }
}


