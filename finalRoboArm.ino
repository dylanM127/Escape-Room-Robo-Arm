
// I have been updateing this repositroy as I go 
// so you can see the history. 

#include <Servo.h>
#include <Stepper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo myservoX, myservoY;

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

unsigned long currentMillis = 0;
unsigned long previousP = 0;
unsigned long previousN = 0;

int pInterval = 50;
int nInterval = 200;
int i = 0;

const int servoX = 2;
const int servoY = 3;
const int button = 40;

const int stepsPerRevolution = 2048;

Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);


void setup() {
  //pinMode(A0, INPUT);
  //pinMode(A1, INPUT);
  Serial.begin(9600);

  lcd.begin();
  lcd.backlight();

  lcd.createChar(0, zero);
  lcd.createChar(5, five);

  myservoX.attach(servoX);
  myservoY.attach(servoY);

  pinMode(button, INPUT);
}

void loop() {
  currentMillis = millis();

  valueX = analogRead(A0);  // read X axis value [0..1023]
  valueY = analogRead(A1);  // read Y axis value [0..1023]

  int angleX = map(valueX, 0, 1023, 0, 180);
  myservoX.write(angleX);
 

  //int angleY = map(valueY, 0, 1023, 0, 180);
  //myservoY.write(angleY);

  int speed = map(valueY, 530, 1023, 0, 15);

  Serial.println(speed);

  if (speed > 5) {
    myStepper.setSpeed(speed);
    myStepper.step(100);
  }

  if (speed < -5) {
    myStepper.setSpeed(-speed);
    myStepper.step(-100);
  }
  
  if (i > 0 && (speed > 5 || speed < -5 || angleX > 90 || angleX < 90)) { //Detects if there is any input. 
     if (currentMillis - previousN >= nInterval) {                       // And if so decrese. 
      previousN = currentMillis;

      i = i - 1;
      lcd.setCursor(0, 0);
      lcd.print(i);
      lcd.print("%  ");
      updateBar(i);
    }
  }else {// Otherwise increse. 
    if (i < 100 && currentMillis - previousP >= pInterval) {
      previousP = currentMillis;

      i = i + 1;
      lcd.setCursor(0, 0);
      lcd.print(i);
      lcd.print("%  ");
      updateBar(i);
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
