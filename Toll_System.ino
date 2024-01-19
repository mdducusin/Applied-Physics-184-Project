// importing the necessary libraries
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// defining the pins for the components and variables
#define green 6
#define red 7
#define buzzer 2
MFRC522 mfrc522(10, 9);
Servo myServo;

void setup() {
  Serial.begin(9600);  
  SPI.begin();
  mfrc522.PCD_Init();
  myServo.attach(3);
  myServo.write(0);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(buzzer, OUTPUT);
  noTone(buzzer);
  Serial.println("Put your card to the reader:");
  Serial.println();
}

void loop() {
  // scanning for RFID cards/tags and obtaining its unique identifier (UID)
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  // if statement for RFID card emulating a Beep card with sufficient balance
  // prompts the green LED to light up, buzzer to make a sound, and servo motor to open then close after five seconds
  if (content.substring(1) == "79 AA 92 16")
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    digitalWrite(green, HIGH);
    tone(buzzer, 500);
    delay(300);
    noTone(buzzer);
    myServo.write(90);
    delay(5000);
    myServo.write(0);
    digitalWrite(green, LOW);
  }

  // if statement for RFID tag emulating a master key to open the servo motor indefinitely
  if (content.substring(1) == "8A 7B 45 80") {
    Serial.println("Master access");
    Serial.println();
    delay(500);
    digitalWrite(green, HIGH);
    tone(buzzer, 500);
    delay(300);
    noTone(buzzer);
    myServo.write(90);
  }

  // if statement for RFID tag emulating a master key to close the servo motor
  if (content.substring(1) == "4C 87 82 64") {
    Serial.println("Master access");
    Serial.println();
    delay(500);
    digitalWrite(red, HIGH);
    tone(buzzer, 500);
    delay(300);
    noTone(buzzer);
    myServo.write(0);
  }

  // else statement for RFID tag emulating a Beep card with insufficient balance
  // prompts the red LED to light up and the buzzer to make a sound
  else 
  {
    Serial.println(" Access denied");
    digitalWrite(red, HIGH);
    tone(buzzer, 300);
    delay(1000);
    digitalWrite(red, LOW);
    noTone(buzzer);
  }
}
