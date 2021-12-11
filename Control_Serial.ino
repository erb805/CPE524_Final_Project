// constants won't change. Used here to set a pin number:
//#include <Wire.h>
#include <SoftwareSerial.h>
#include "Adafruit_Keypad.h"

const int motorPin = 6;
const byte ROWS = 4;
const byte COLS = 3;
//define symbols on the buttons of keypad
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {13,12,11,10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9,8,7}; //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);


SoftwareSerial mySerial(3,2);
void setup() {  
  Serial.begin(9600);
  while (!Serial) {};
  mySerial.begin(4800);
  customKeypad.begin();
  password();
  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, HIGH);
}

void loop() {
  byte data;
  delay(100);
  if (mySerial.available()) {
    data = mySerial.read();
    Serial.println(data);
    if (data <= 90)
    {
      digitalWrite(motorPin, HIGH);
    }
    else
    {
      digitalWrite(motorPin, LOW);
    }
  }
}

void password()
{
  String passcode = "";
  String correct_code = "1544";

  while (passcode != correct_code)
  {
    customKeypad.tick();

    while(customKeypad.available())
    {
      keypadEvent e = customKeypad.read();
      if(e.bit.EVENT == KEY_JUST_PRESSED)
      {
        delay(200);
        passcode += ((char)e.bit.KEY);
        Serial.println(passcode);
        if (((char)e.bit.KEY) == '*')
        {
          passcode = "";
        }
      }
    }
  }
  
  
}
