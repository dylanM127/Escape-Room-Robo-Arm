#include <Servo.h>
#include <Stepper.h>

Servo myservoX, myservoY;

int valueX = 0;
int valueY = 0;

const int servoX = 2;
const int servoY = 3;
const int button = 40;

const int stepsPerRevolution = 2048;

Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);


void setup() {
	//pinMode(A0, INPUT);
	//pinMode(A1, INPUT);
	Serial.begin(9600);

  myservoX.attach(servoX);
  myservoY.attach(servoY);

  pinMode(button, INPUT);

}

void loop() {
  valueX = analogRead(A0);	// read X axis value [0..1023]
 // Serial.println("X:");
  //Serial.print(valueX, DEC);

  valueY = analogRead(A1);	// read Y axis value [0..1023]
  //Serial.println(" | Y:");
  //Serial.print(valueY, DEC);

  int angleX = map(valueX, 0, 1023, 0, 180);
  myservoX.write(angleX);

  //int angleY = map(valueY, 0, 1023, 0, 180);
  //myservoY.write(angleY);

  int speed = map(valueY, 530, 1023, 0, 15);
  
  Serial.println(speed);

  if (speed > 5){
    myStepper.setSpeed(speed);
    myStepper.step(100);
  }

  if (speed < -5){
    myStepper.setSpeed(-speed);
    myStepper.step(-100);
  }
  delay(50);
}
