// importing the necessary libraries
#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// defining the pins for the components and variables
SoftwareSerial sim(6, 7);
#define FLAME_SENSOR A0
#define BUZZER_PIN 2
#define trigPin 8
#define echoPin 9
long duration;
int distanceInch;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// specifying the phone number where the text message will be sent to (include country code)
String number = "+639151497837";

void setup() {
  Serial.begin(9600);
  sim.begin(9600);
  lcd.begin();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(FLAME_SENSOR, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // code block in measuring the distance of an object (in inches) from the ultrasonic sensor 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceInch = duration * 0.0133 / 2;

  // reads flame sensor every one second
  int flameValue = analogRead(FLAME_SENSOR);
  Serial.print("Analog reading = "); 
  Serial.println(flameValue);
  delay(1000);

  // if statement that prompts the buzzer to sound the alarm, send a fire alert message, and display a fire alert through the LCD when fire is detected
  // ideally, any value less than 1023 indicates the presence of fire but since the flame sensor measures wavelength ~760nm to 1100nm, threshold value set at 1010 instead
  if (flameValue < 1010) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("!! FIRE ALERT !!");
    lcd.setCursor(0, 1);
    lcd.print("PLEASE EVACUATE");
    SendMessage();
    soundAlarm();
    delay(10000);
    lcd.clear();
  }

  // if statement to display that the train in coming when ultrasonic sensor returns a distance of less than two inches
  if (distanceInch < 2){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TRAIN IS COMING TO");
    lcd.setCursor(1, 1);
    lcd.print("RECTO STATION");
    delay(7000);
    lcd.clear();
  }

  // if statement to display a default message when ultrasonic sensor does not detect a near object (presumably the train)
   if (distanceInch > 2){
    lcd.setCursor(0, 0);
    lcd.print("WELCOME TO LRT-2");
    lcd.setCursor(1, 1);
    lcd.print("RECTO STATION");
  }
}

// code block that sets the GSM module in text mode and specify the message that will be sent
void SendMessage(){
  sim.println("AT+CMGF=1"); 
  delay(200);
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(200);
  String SMS = "[FIRE ALERT] LRT-2 RECTO STATION";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);
  delay(200);
}

// code block that sets the behavior of the buzzer
void soundAlarm() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(5000);
  digitalWrite(BUZZER_PIN, LOW);
}
