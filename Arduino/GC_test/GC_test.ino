//Keep track of elapsed time
unsigned long timeStamp;

//Counter variables
int timer;
int start;
int GC1_value = 0;
int GC2_value = 0;
int GC3_value = 0;
int GC_max = 0;
unsigned long count_GC1;
unsigned long count_GC2;
unsigned long count_GC3;

void setup()
{
  //Start RX & TX ports
  Serial.begin(38400); //Serial Monitor 
  Serial1.begin(9600); //Giger Counter 1
  Serial2.begin(9600); //Giger Counter 2
  Serial3.begin(9600); //Giger Counter 3
}

void loop() {
  Serial.println();
  
  {//Timestamp
    timeStamp = millis();
    timeStamp = timeStamp/1000;
    Serial.print(timeStamp);
    Serial.print(", ");
  }
  
  {//Configure counter
    start = millis();
    timer = start;
    count_GC1 = 0;
    count_GC2 = 0;
    count_GC3 = 0;
  
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
      
      timer = millis();
    }
       
    Serial.print(count_GC1);
    Serial.print(", ");
    Serial.print(count_GC2);
    Serial.print(", ");
    Serial.print(count_GC3);
  }
}


