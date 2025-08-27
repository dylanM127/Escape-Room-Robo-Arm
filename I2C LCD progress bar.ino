#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte zero [] = {
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
};
byte five [] = {
	B11111,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111
};

int prevCount = 0;

void setup()
{
	Serial.begin(9600);
	lcd.begin();
	lcd.backlight();

	lcd.createChar(0, zero);
	lcd.createChar(5, five);
}

void loop(){
	for (int i = 0; i <= 100; i++){
		lcd.setCursor(0,0);
		lcd.print(i);
		lcd.print("%  ");
		updateBar(i);
		delay(50);
		}

	for (int i = 100; i >= 0; i--){
		lcd.setCursor(0,0);
		lcd.print(i);
		lcd.print("%  ");
		updateBar(i);
		delay(200);
	}
}

void updateBar(unsigned long count){
	int bar = 0.8*count;
	int val = bar/5; 

	lcd.setCursor(val, 1);
	lcd.write(5);
	
	lcd.setCursor(val+1, 1);
	lcd.write(0);
}
