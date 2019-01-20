#include <Wire.h>
#include <Adafruit_INA219.h> // You will need to download this library

#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

Adafruit_INA219 sensor219; // Declare and instance of INA219


#define I2C_ADDRESS 0x3C
SSD1306AsciiAvrI2c oled;


//#include "SdFat.h"
//SdFat SD;

unsigned long previousMillis = 0;
unsigned long interval = 1000;
//const int chipSelect = 10;
//File LogFile;

void setup(void) 
{
   Serial.begin(9600);    
  // Serial.println('SD');
  // SD.begin(chipSelect);
  // Serial.println('SD started');
   
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
  
    //voltage =  sensor219.getBusVoltage_V()/20;
    shuntvoltage = shuntvoltage /20;
    busvoltage = busvoltage /20;

     //shuntvoltage = sensor219.getShuntVoltage_mV();
    // busvoltage = sensor219.getBusVoltage_V();
    // current = current + sensor219.getCurrent_mA();
        
    voltage = voltage + (busvoltage + (shuntvoltage/1000));
    //voltage = busvoltage ;
    current = sensor219.getCurrent_mA();
    power = voltage * (current/1000); // Calculate the Power
    energy = energy + voltage * current / 3600;
  
    displaydata(voltage, current, power, energy);
    displaySerial(shuntvoltage, busvoltage,voltage, current, power, energy);

/*
    LogFile = SD.open("LOG.csv", FILE_WRITE);
    if (LogFile) {
      Serial.print("Write to file"); 
      LogFile.print(currentMillis);
      LogFile.print(';');
      LogFile.print(voltage);
      LogFile.print(';');
      LogFile.print('current');
      
      LogFile.close();
    }
*/    
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
    Serial.println(" mW");  

    Serial.print("Energy:         "); 
    Serial.print(E); 
    Serial.println(" mWh");  
    Serial.println("");  
}

void displaydata(float& U, float& A, float& P, float& E) {
  showUnit(0,U,"U","V");
  showUnit(12,A,"I","mA");
  showUnit(24,P,"P","mW");
  showUnit(36,E,"E","mWh");
}

void showUnit(int line, float value,String symbol, String unit)
{
  oled.setCursor(5, line);
  oled.setFont(Verdana12);
  oled.print(symbol);
  oled.setCursor(17, line);
  oled.print(":");

  int x = 32;
  if (value < 0)
    x = x-4;
  oled.setCursor(x, line);
  oled.setFont(Verdana12_bold);
  oled.print(value);
  
  oled.setCursor(85, line);
  oled.setFont(Verdana12);
  oled.print(" ");
  oled.println(unit);
}
