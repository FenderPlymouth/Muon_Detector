//Libraries
#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <SparkFunMPL3115A2.h>

//Setup OneWire temperature sensors
OneWire e_one_wire(3);
OneWire i_one_wire(2);
DallasTemperature external(&e_one_wire);
DallasTemperature internal(&i_one_wire);
float external_temp;
float internal_temp;

//Setup GPS unit
TinyGPSPlus GPSdata;
float LAT;
float LNG;
float ALT;
int SAT;
int HRS;
int MIN;
int SEC;

//Setup pressure sensor
MPL3115A2 myPressure;
float pressure;
float temperature;

//Setup GPS serial
SoftwareSerial gpsSerial(69, 68); //RX: 69=A15, TX: 68=A14

//Setup SD card file namer
char filename[24];

//Keep track of elapsed time
unsigned long timeStamp;

//Counter variables
int timer;
int start;
int GC1_value;
int GC2_value;
int GC3_value;
int GC_max = 0;
unsigned long count_GC1;
unsigned long count_GC2;
unsigned long count_GC3;
unsigned long count_Muon;

//Store all of the data in a string and print all at once
String dataString;

void setup() {
  //Set the heater pin
  pinMode(4, OUTPUT);
  
  //Start RX & TX ports
  Serial.begin(38400); //Serial Monitor 
  Serial1.begin(9600); //Giger Counter 1
  Serial2.begin(9600); //Giger Counter 2
  Serial3.begin(9600); //Giger Counter 3
  gpsSerial.begin(4800); //GPS
  
  //Start OneWire ports
  external.begin(); //External temperature sensor
  internal.begin(); //Internal temperature sensor
  
  //Start SDA & SCL ports
  Wire.begin();

  //Configure pressure sensor
  myPressure.begin();
  myPressure.setModeBarometer();
  myPressure.setOversampleRate(128);
  myPressure.enableEventFlags();

  //Setup the SD sheild
  pinMode(8, OUTPUT);
  Serial.print("Initializing SD card...");
  delay(1000);
  if (!SD.begin(10, 11, 12, 13)) {
    Serial.println("Card failed or not present");
    return;
  }

  //Initialize the SD card and find the name of the last file, then create a new sequential file 
  Serial.println("card initialized.");
  delay(500);
  int n = 0;
  snprintf(filename, sizeof(filename), "log_%03d.txt", n);
  while(SD.exists(filename)) {
    n++;
    snprintf(filename, sizeof(filename), "log_%03d.txt", n);
  }
  
  //Begin a new data run
  File dataFile = SD.open(filename,FILE_WRITE);
  if (dataFile) {  
    Serial.print("Begin New Data Run: #");
    Serial.println(n);
    dataFile.close();
  }
    else {
    Serial.println("error opening datalog.txt");
  }
  
  delay(500);
  Serial.println("Timer, Hr:Min:Sec, Sat, Latitude, Longitude, Altitude, Pressure, Internal, External, GC-1, GC-2, GC-3, Muon");
}

void loop() {
  String dataString = "";
  GPSdelay(1000);
  
  {//Timestamp
    timeStamp = millis();
    timeStamp = timeStamp/1000;
    dataString += String(timeStamp) += ", ";
  }

  {//GPS
    SAT = GPSdata.satellites.value();
    LAT = GPSdata.location.lat();
    LNG = GPSdata.location.lng();
    ALT = GPSdata.altitude.feet();
    HRS = GPSdata.time.hour();
    MIN = GPSdata.time.minute();
    SEC = GPSdata.time.second();
    dataString += String(HRS) += ":";
    dataString += String(MIN) += ":";
    dataString += String(SEC) += ", ";
    dataString += String(SAT) += ", ";
    dataString += String(LAT, 6) += ", ";
    dataString += String(LNG, 6) += ", ";
    dataString += String(ALT, 2) += ", ";
  }

  {//Pressure inside of gieger counter box
    pressure = myPressure.readPressure();
    dataString += String(pressure, 2)+= ", ";
  }
                
  {//Temperature inside of gieger counter box
    temperature = myPressure.readTemp();
    dataString += String(temperature, 2)+= ", ";
  }

  {//Temperature inside of the payload
    internal.requestTemperatures();
    float internal_temp = internal.getTempCByIndex(0);
    dataString += String(internal_temp, 2)+= ", ";
  
  //Heaters
    if (internal_temp < 0) {
      digitalWrite(4, HIGH);  
    }
    else {
      digitalWrite(4, LOW);
    }
  }

  {//Temperature outside of the payload
    external.requestTemperatures();
    float external_temp = external.getTempCByIndex(0);
    dataString += String(external_temp, 2)+= ", ";
  }
  
  {//Muon Counter
    //Setup counters
    start = millis();
    timer = start;
    count_GC1 = 0;
    count_GC2 = 0;
    count_GC3 = 0;
    count_Muon = 0;
  
    while ((timer-start) <= 30000) {
      //Geiger Counter readers
      GC1_value = Serial1.read();  
      GC2_value = Serial2.read();
      GC3_value = Serial3.read(); 
        
      if (GC1_value > GC_max) {
        count_GC1++;
      }
      
      if (GC2_value > GC_max) {
        count_GC2++;
      }
      
      if (GC3_value > GC_max) {
        count_GC3++;
      }

      if ((GC1_value > GC_max && GC2_value > GC_max) || (GC2_value > GC_max && GC3_value > GC_max)) {
        count_Muon++;
      }
      
      timer = millis();
    }

    dataString += String(count_GC1 * 2) += ", ";
    dataString += String(count_GC2 * 2) += ", ";
    dataString += String(count_GC3 * 2) += ", ";
    dataString += String(count_Muon * 2);
  }
 
  {//Print data to Serial and SD card
    File dataFile = SD.open(filename, FILE_WRITE);
    if (dataFile) {
      Serial.println(dataString);
      dataFile.println(dataString);
      dataFile.close();
    }

    else {
      Serial.println("error opening datalog.txt");
      Serial.println(dataString);
    }
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
