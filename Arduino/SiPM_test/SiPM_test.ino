//Keep track of elapsed time
unsigned long timeStamp;

//Setup SiPM
const int SiPM = A0;

//Counter variables
int timer;
int start;
int SiPM_value;
int SiPM_max = 1004;
unsigned long count_SiPM;

void setup() {
  //Start RX & TX ports
  Serial.begin(38400); //xBee and serial monitor 
}

void loop() {
  Serial.println();
  
  {//Timestamp
    timeStamp = millis();
    timeStamp = timeStamp/1000;
    Serial.print(timeStamp);
    Serial.print(", ");
  }

  {//Counters
    //Setup counters
    start = millis();
    timer = start;
    count_SiPM = 0;
    
    while ((timer-start) <= 5000) {
      //SiPM and Geiger Counter readers
      SiPM_value = analogRead(SiPM);

      if (SiPM_value < SiPM_max) {
        count_SiPM++;
      }
      
      timer = millis();
    }
    
    Serial.print(count_SiPM);
  }
}


