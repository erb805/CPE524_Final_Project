//#include <Wire.h>
#include <OneWire.h>
#include<DallasTemperature.h>
#include <SoftwareSerial.h>


//defined constants
const int solar_panel_1 = A0;
const int solar_panel_2 = A3;
const int motor = A2;
const int ONE_WIRE_BUS = 2;
const float MAX_VOLTAGE = 12.0;
const float MAX_TEMP = 90.0;

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

SoftwareSerial mySerial(11,10); //RX, TX

void setup(void)
{
  sensors.begin();  // Start up the library\\
  Serial.begin(9600);
  while (!Serial) {};
  mySerial.begin(4800);
  
}

float readVoltage(int pin)
{
  float sensorValue;
  sensorValue = analogRead(pin)*1.0;
  sensorValue = (sensorValue*25.0)/1024.0;
  return sensorValue;
}

void loop() {
  //variables to store sensor values
  float solar_panel_1_voltage;
  float solar_panel_2_voltage;
  float motor_voltage;
  float panel_temp;
  bool keep_running;

  //read voltages from voltage sensors
  solar_panel_1_voltage = readVoltage(solar_panel_1);
  solar_panel_2_voltage = readVoltage(solar_panel_2);
  motor_voltage = readVoltage(motor);

  //read temperature from temp sensor
  sensors.requestTemperatures(); 
  //read panel temp in farenheit
  panel_temp = (sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0;
  
  //determine if we should keep operating
  keep_running = true;
  if (solar_panel_1_voltage >= MAX_VOLTAGE)
  {
    Serial.println("OVERVOLTAGE ON PANEL 1");
    keep_running = false;
  }
  if (solar_panel_2_voltage >= MAX_VOLTAGE)
  {
    Serial.println("OVERVOLTAGE ON PANEL 2");
    keep_running = false;
  }
  if (motor_voltage >= MAX_VOLTAGE)
  {
    Serial.println("OVERVOLTAGE ON MOTOR");
    keep_running = false;
  }
  if (panel_temp >= MAX_TEMP)
  {
    Serial.println("OVERTEMP ON PANEL");
    keep_running = false;
  }

  //debug print statements
  Serial.println("Panel 1 voltage");
  Serial.println(solar_panel_1_voltage);
  Serial.println("Panel 2 voltage");
  Serial.println(solar_panel_2_voltage);
  Serial.println("Motor voltage");
  Serial.println(motor_voltage);
  Serial.println("Panel Temp");
  Serial.println(panel_temp);
  
  
  if (keep_running)
  {
    mySerial.write(byte(panel_temp));
  }
//  else
//  {
//    mySerial.write(byte(panel_temp));
//  }
  // stop the program for for <sensorValue> milliseconds: 
  delay(500);
}
