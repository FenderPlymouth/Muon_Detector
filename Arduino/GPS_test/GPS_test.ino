//Libraries
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

//Setup sensor
TinyGPSPlus GPSdata;

//Setup GPS serial
SoftwareSerial gpsSerial(69, 68); //RX: 69=A15, TX: 68=A14

//Keep track of elapsed time
unsigned long timeStamp;

void setup() {
  //Start RX & TX ports
  Serial.begin(38400); //xBee and serial monitor 
  gpsSerial.begin(9600); //GPS
}

void loop() {
  Serial.println();
  
  {//Timestamp
    timeStamp = millis();
    timeStamp = timeStamp/1000;
    Serial.print(timeStamp);
    Serial.print(", ");
  }

  {//GPS
    Serial.print(GPSdata.satellites.value());
    Serial.print(", ");
    Serial.print(GPSdata.location.lat(), 6);
    Serial.print(", ");
    Serial.print(GPSdata.location.lng(), 6);
    Serial.print(", ");
    Serial.print(GPSdata.altitude.feet());
    Serial.print(", ");
    Serial.print(GPSdata.time.hour());
    Serial.print(":");
    Serial.print(GPSdata.time.minute());
    Serial.print(":");
    Serial.print(GPSdata.time.second());
    Serial.print(", ");
    GPSdelay(1000);
  }
}





//Extra functions
  //Feeds the gps data from the receiver to the arduino, refreshes at 1Hz which adds 1 second to the cycle time
  static void GPSdelay(unsigned long ms) {
    
    unsigned long delay_start = millis();
    
    do {
      while (gpsSerial.available())
      GPSdata.encode(gpsSerial.read());
    } while (millis() - delay_start < ms);
  }
