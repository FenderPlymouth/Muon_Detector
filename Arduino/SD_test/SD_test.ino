//Libraries
#include <SPI.h>
#include <Wire.h>
#include <SD.h>

//Keep track of elapsed time
unsigned long timeStamp;

//Store all of the data in a string and print all at once
String dataString;

//Setup SD card file namer
char filename[24];

void setup() {
  //Start RX & TX ports
  Serial.begin(38400); //xBee and serial monitor 
  
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
}

void loop() {
  String dataString = "";
  Serial.println();
  
  {//Timestamp
    timeStamp = millis();
    timeStamp = timeStamp/1000;
    dataString += String(timeStamp) += ", ";
    dataString += String(timeStamp) += ", ";
    dataString += String(timeStamp) += ", ";
    dataString += String(timeStamp) += ", ";
    dataString += String(timeStamp) += ", ";
    dataString += String(timeStamp);
  }

  {//Write data to SD card
    File dataFile = SD.open(filename, FILE_WRITE);
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
    }
  } 
  
  Serial.print(dataString);
  delay(1000);
}







