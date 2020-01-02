#include <SPI.h>
#include <MFRC522.h>     // 引用程式庫

byte* rfid(byte* idSize) {
  
    //Serial.println("in rfid");
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
    {
      mfrc522.PICC_HaltA();
    }

}