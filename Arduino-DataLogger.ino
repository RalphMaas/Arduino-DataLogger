#include <Wire.h>
#include <Adafruit_INA219.h> // You will need to download this library

#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

Adafruit_INA219 sensor219; // Declare and instance of INA219


#define I2C_ADDRESS 0x3C
SSD1306AsciiAvrI2c oled;

unsigned long previousMillis = 0;
unsigned long interval = 1000;

void setup(void) 
{
   Serial.begin(9600);    
   oled.begin(&Adafruit128x64, I2C_ADDRESS);
   oled.setFont(Verdana12);
   oled.clear();    
   
   // Initialize the INA219.
   // By default the initialization will use the largest range (32V, 2A).  However
   // you can call a setCalibration function to change this range (see comments).
   sensor219.begin();
   sensor219.setCalibration_16V_400mA();
   // To use a slightly lower 32V, 1A range (higher precision on amps):
  // sensor219.setCalibration_32V_1A();
   // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
   //ina219.setCalibration_16V_400mA();
   Serial.println('Start');
}

void loop(void) 
{



 unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    float shuntvoltage = 0;
    float busvoltage = 0;
    float voltage = 0; 
    float current = 0; // Measure in milli amps
  
    float power = 0;
    float energy = 0;
    int i;

    
    previousMillis = currentMillis;

     for (i = 0; i < 20; i++){
        shuntvoltage = shuntvoltage + sensor219.getShuntVoltage_mV();
        busvoltage = busvoltage + sensor219.getBusVoltage_V();
        current = current + sensor219.getCurrent_mA();
    };
  
    shuntvoltage = shuntvoltage /20;
    busvoltage = busvoltage /20;
   
    current = sensor219.getCurrent_mA();
    if (current <= 0 )
    {
        voltage = 0;
    }
    else
    {
        voltage = busvoltage + (shuntvoltage/1000);
    }
  
    power = voltage * (current/1000); // Calculate the Power
    energy = energy + voltage * current / 3600;
  
    displaydata(voltage, current, power, energy);
    displaySerial(shuntvoltage, busvoltage,voltage, current, power, energy);
 }
}

void displaySerial(float& Us, float& Ub, float& U, float& A, float& P, float& E) {
    Serial.print("Voltage Shunt:       "); 
    Serial.print(Us); 
    Serial.println(" mV");      
    
    Serial.print("Voltage Bus:       "); 
    Serial.print(Ub); 
    Serial.println(" V");  
      
    Serial.print("Voltage:       "); 
    Serial.print(U); 
    Serial.println(" V");  

    Serial.print("Current:       "); 
    Serial.print(A); 
    Serial.println(" mA");
  
    Serial.print("Power:         "); 
    Serial.print(P); 
    Serial.println(" W");  

    Serial.print("Energy:         "); 
    Serial.print(E); 
    Serial.println(" Wh");  
    Serial.println("");  
}

void displaydata(float& U, float& A, float& P, float& E) {
  showUnit(0,U,"U","V");
  showUnit(12,A,"I","mA");
  showUnit(24,P,"P","W");
  showUnit(36,E,"E","Wh");

}

void showUnit(int line, float value,String symbol, String unit)
{
  oled.setCursor(5, line);
  oled.setFont(Verdana12);
  oled.print(symbol);
  oled.setCursor(17, line);
  oled.print(":");

  int x = 32;
  oled.setCursor(x, line);
  oled.setFont(Verdana12_bold);
  oled.print(value);
  oled.print("   ");

  
  oled.setCursor(85, line);
  oled.setFont(Verdana12);
  oled.print(" ");
  oled.println(unit);
}
