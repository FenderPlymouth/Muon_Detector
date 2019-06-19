//Libraries
#include <SPI.h>
#include <Wire.h>
#include <SparkFunMPL3115A2.h>

//Setup pressure sensor
MPL3115A2 myPressure;
float pressure;
float temperature;

//Keep track of elapsed time
unsigned long timeStamp;

void setup() {
  //Start RX & TX ports
  Serial.begin(38400); //xBee and serial monitor 
  
  //Start SDA & SCL ports
  Wire.begin();

  //Configure pressure sensor
  myPressure.begin();
  myPressure.setModeBarometer();
  myPressure.setOversampleRate(128);
  myPressure.enableEventFlags();
}

void loop() {
  Serial.println();
  
  {//Timestamp
    timeStamp = millis();
    timeStamp = timeStamp/1000;
    Serial.print(timeStamp);
    Serial.print(", ");
  }

  {//Pressure and Temperature
    pressure = myPressure.readPressure();
    temperature = myPressure.readTemp();
    Serial.print(pressure);
    Serial.print(", ");
    Serial.print(temperature);
  }
}
