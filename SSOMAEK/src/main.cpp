#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN    21
#define RST_PIN   22

#define CARD_UID_SIZE 4

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

byte nuidPICC[4];
const uint16_t ssomaek_0_10[4]={0x3A, 0x44, 0xF9, 0x80};
const uint16_t ssomaek_1_9[4]={0xE4, 0xE2, 0x76, 0x1A};
// void printHex(byte *buffer, byte bufferSize);
// void printDec(byte *buffer, byte bufferSize);
int recognize_card(byte *buffer, byte bufferSize);

void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 
}
 
void loop() 
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  recognize_card(mfrc522.uid.uidByte, CARD_UID_SIZE);
  Serial.println();
  delay(1000);
}



int recognize_card(byte *buffer, byte bufferSize){
  if (buffer[0] == ssomaek_0_10[0] &&
      buffer[1] == ssomaek_0_10[1] && 
      buffer[2] == ssomaek_0_10[2] && 
      buffer[3] == ssomaek_0_10[3] ){
    Serial.print("soju 0 : beer 10 out");
    return 0;
  }
  if (buffer[0] == ssomaek_1_9[0] &&
      buffer[1] == ssomaek_1_9[1] && 
      buffer[2] == ssomaek_1_9[2] && 
      buffer[3] == ssomaek_1_9[3] ){
    Serial.print("soju 1 : beer 9 out");
    return 1;
  }
};


// /**
//  * Helper routine to dump a byte array as hex values to Serial. 
//  */
// void printHex(byte *buffer, byte bufferSize) {
//   for (byte i = 0; i < bufferSize; i++) {
//     Serial.print(buffer[i] < 0x10 ? " 0" : " ");
//     Serial.print(buffer[i], HEX);
//   }
// }


// /**
//  * Helper routine to dump a byte array as dec values to Serial.
//  */
// void printDec(byte *buffer, byte bufferSize) {
//   for (byte i = 0; i < bufferSize; i++) {
//     Serial.print(' ');
//     Serial.print(buffer[i], DEC);
//   }
// }