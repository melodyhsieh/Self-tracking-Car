#include<SoftwareSerial.h>

//TODO: return the direction based on the command you read
int ask_direction(){ 
    char cmd;
    cmd = char(I2CBT.read());
    Serial.print("receive the cmd:");
    Serial.println(cmd);
    // dir = 0 : advance
    // dir = 1 : left
    // dir = 2 : right
    // dir = 3 : back
    // dir = 4 : halt

    if(cmd == 'f') return 0;
    else if(cmd == 'b') return 3;
    else if(cmd == 'r') return 2;
    else if(cmd == 'l') return 1;
    else return 4;
}

char convert_to_HEX(int a){
     if(a == 0) return '0';
     else if(a == 1) return '1';
     else if(a == 2) return '2';
     else if(a == 3) return '3';
     else if(a == 4) return '4';
     else if(a == 5) return '5';
     else if(a == 6) return '6';
     else if(a == 7) return '7';
     else if(a == 8) return '8';
     else if(a == 9) return '9';
     else if(a == 10) return 'A';
     else if(a == 11) return 'B';
     else if(a == 12) return 'C';
     else if(a == 13) return 'D';
     else if(a == 14) return 'E';
     else if(a == 15) return 'F';
     else return 'Q';
}
// TODO: send the id back by BT
void send_byte(byte *id, byte idSize){
  for(byte i = 0;i < 4;i++){
    char a = convert_to_HEX(id[i]/16);
    char b = convert_to_HEX(id[i]-16*(id[i]/16));
    Serial.print(a);
    Serial.println(b);
    I2CBT.write(a);
    I2CBT.write(b);
    }
}
