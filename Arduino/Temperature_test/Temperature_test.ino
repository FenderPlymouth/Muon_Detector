#include <OneWire.h>
#include <DallasTemperature.h>

//Setup OneWire temperature sensors
OneWire e_one_wire(2);
OneWire i_one_wire(3);
DallasTemperature external(&e_one_wire);
DallasTemperature internal(&i_one_wire);
float external_temp;
float internal_temp;

void setup() {
  //Start RX & TX ports
  Serial.begin(38400); //Serial Monitor 

  //Start OneWire ports
  external.begin(); //External temperature sensor
  internal.begin(); //Internal temperature sensor
}

void loop() {
  //Temperature outside of the payload
  external.requestTemperatures();
  float external_temp = external.getTempCByIndex(0);
  Serial.println(external_temp);

  //Temperature inside of the payload
  internal.requestTemperatures();
  float internal_temp = internal.getTempCByIndex(0);
  Serial.println(internal_temp);
}
