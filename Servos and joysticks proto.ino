#include <Servo.h>

Servo myservoX;
Servo myservoY;

int valueX = 0;
int valueY = 0;


void setup() {
	//pinMode(A0, INPUT);
	//pinMode(A1, INPUT);
  	pinMode(7, INPUT_PULLUP); //set pin 7 as an input and enable the internal pull-up resistor
	Serial.begin(9600);

  myservoX.attach(3);
  myservoY.attach(2);

}

void loop() {
  valueX = analogRead(A0);	// read X axis value [0..1023]
  Serial.println("X:");
  Serial.print(valueX, DEC);

  valueY = analogRead(A1);	// read Y axis value [0..1023]
  Serial.println(" | Y:");
  Serial.print(valueY, DEC);

 

  int angleX = map(valueX, 0, 1023, 0, 180);
  myservoX.write(angleX);

  int angleY = map(valueY, 0, 1023, 0, 180);
  myservoY.write(angleX);

  delay(50);
}
