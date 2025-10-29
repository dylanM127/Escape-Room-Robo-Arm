// I have been updateing the repositroy as I go 
// so you can see the history. 
#include <Servo.h>
#include <Stepper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 53

byte readCard[4];
String MasterTag = "5A8D981A";  // REPLACE this Tag ID with your Tag ID!!!
String tagID = "";

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);

Servo myservoX, myservoY, myservoC;

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte zero[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};
byte five[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

int valueX = 0;
int valueY = 0;
int valueC = 0;


unsigned long currentMillis = 0;
unsigned long previousP = 0;
unsigned long previousN = 0;

int pInterval = 50;
int nInterval = 200;
int i = 0;
bool access = false;
int speed = 0;
int angleX = 90;
int angleY = 90;
int angleC = 90;
int angleXAdd = 0;
int angleYAdd = 0;

const int servoX = 2;
const int servoY = 3;
const int servoC = 4;
const int button = 40;
const int backlight = 33;


const int stepsPerRevolution = 2048;

Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);


void setup() {

  SPI.begin();         // SPI bus
  mfrc522.PCD_Init();  // MFRC522

  //pinMode(A0, INPUT);
  //pinMode(A1, INPUT);
  Serial.begin(9600);

  lcd.begin();
  lcd.backlight();

  lcd.createChar(0, zero);
  lcd.createChar(5, five);

  myservoX.attach(servoX);
  myservoY.attach(servoY);
  myservoC.attach(servoC);

  pinMode(button, INPUT);
  pinMode(backlight, OUTPUT);
}

void loop() {
  currentMillis = millis();
if (access == true){
  valueX = analogRead(A0);  // read X axis value [0..1023]
  valueY = analogRead(A1);  // read Y axis value [0..1023]
  //valueC = analogRead(A2); // read the X axis val of joystick 2
  
  angleXAdd = map(valueX, 511, 1023, 0, 2);
  angleYAdd = map(valueY, 511, 1023, 0, 5);
  //angleC = map(valueC, 0, 1023, 0, 180);

  angleX = angleX + angleXAdd;
  angleY = angleY + angleYAdd;

  

  if (i > 0){
    myservoX.write(angleX);
    myservoY.write(angleY);
    myservoC.write(angleC);

    speed = map(valueY, 530, 1023, 0, 15);

  
    
    if (speed > 5) {
      myStepper.setSpeed(speed);
      myStepper.step(100);
    }

    if (speed < -5) {
      myStepper.setSpeed(-speed);
      myStepper.step(-100);
    }
  }

 // Serial.println(angleY);
  //Serial.println(angleX);

  if (i > 0 && (speed > 5 || speed < -5 || angleXAdd != 0 || angleYAdd != 0 /*||*angleC != 90*/)) { //Detects if there is any input. 
    if (currentMillis - previousN >= nInterval) {                                               // And if so decrese. 
      previousN = currentMillis;

      i = i - 1;
      lcd.setCursor(0, 0);
      lcd.print(i);
      lcd.print("%  ");
      updateBar(i);
    }
  }else {
      if (i == 0 && (speed > 5 || speed < -5 || angleXAdd != 0 || angleYAdd != 0 /*|| angleC != 90*/)) {
        digitalWrite(backlight, LOW);
        }else {  // Otherwise increse. 
      if (i < 100 && currentMillis - previousP >= pInterval) {
        previousP = currentMillis;

        digitalWrite(backlight, HIGH);

        i = i + 1;
        lcd.setCursor(0, 0);
        lcd.print(i);
        lcd.print("%  ");
        updateBar(i);
      } 
     }
   }
  }
    while (getID()) {

    if (tagID == MasterTag) {

      Serial.println("Access Granted");
      access = true;
      Serial.println(access);

      // You can write any code here like opening doors, switching on a relay, lighting up an LED, or anything else you can think of.
    } else {
      Serial.println(" Access Denied!");
      Serial.println(tagID);
      access = false;
      Serial.println(access);
    }
  }
  
}

void updateBar(unsigned long count) {
  int bar = 0.8 * count;
  int val = bar / 5;

  lcd.setCursor(val - 1, 1);
  lcd.write(5);

  lcd.setCursor(val, 1);
  lcd.write(0);
}

//Read new tag if available
boolean getID() {
  // Getting ready for Reading PICCs
  if (!mfrc522.PICC_IsNewCardPresent()) {  //If a new PICC placed to RFID reader continue
    return false;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {  //Since a PICC placed get Serial and continue
    return false;
  }
  tagID = "";
  for (uint8_t i = 0; i < 4; i++) {  // The MIFARE PICCs that we use have 4 byte UID
    //readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));  // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA();  // Stop reading
  return true;
}
