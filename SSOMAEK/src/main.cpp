#include <SPI.h>
#include <MFRC522.h>
#include "ESP32Servo.h"

#define SS_PIN    21
#define RST_PIN   22

#define CARD_UID_SIZE 4

#define SOJUMOTOR 12
#define BEERMOTOR 32
#define OUT_UNIT_TIME 100
#define MOTOR_ON_TIME 600

Servo sojumotor;
Servo beermotor;

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

byte nuidPICC[4];
const uint16_t ssomaek_0_10[4]={0x3A, 0x44, 0xF9, 0x80};
const uint16_t ssomaek_1_9[4]={0xE4, 0xE2, 0x76, 0x1A};
// void printHex(byte *buffer, byte bufferSize);
// void printDec(byte *buffer, byte bufferSize);

int recognize_card(byte *buffer, byte bufferSize);
void somaek_out(int soju_ratio);
void run_soju_motor(int ratio);


void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 

  sojumotor.setPeriodHertz(50);// Standard 50hz servo
  sojumotor.attach(SOJUMOTOR, 1000, 2000);

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
  somaek_out(recognize_card(mfrc522.uid.uidByte, CARD_UID_SIZE));

  Serial.println();
  delay(1000);
}



int recognize_card(byte *buffer, byte bufferSize){
  if (buffer[0] == ssomaek_0_10[0] &&
      buffer[1] == ssomaek_0_10[1] && 
      buffer[2] == ssomaek_0_10[2] && 
      buffer[3] == ssomaek_0_10[3] ){
    Serial.print("soju 0 : beer 10 out");
    return 0; // = soju_ratio
  }
  if (buffer[0] == ssomaek_1_9[0] &&
      buffer[1] == ssomaek_1_9[1] && 
      buffer[2] == ssomaek_1_9[2] && 
      buffer[3] == ssomaek_1_9[3] ){
    Serial.print("soju 1 : beer 9 out");
    return 1; // = soju_ratio
  }
  return (0);
}

void somaek_out(int soju_ratio){
  int beer_ratio = 10 - soju_ratio;
  run_soju_motor(soju_ratio);
  // run_motor(beer_ratio);
}

void run_soju_motor(int ratio){
  if (ratio == 0){
    delay(100);
    return;
  }
  else{
  sojumotor.write(180);
  delay(MOTOR_ON_TIME);
  sojumotor.write(90);
  delay(2000);
  sojumotor.write(180);
  delay(MOTOR_ON_TIME);
  sojumotor.write(90);
  return;  
  }
}