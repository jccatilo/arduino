#include <SoftwareSerial.h>
SoftwareSerial SIM900(8,7);

void setup() {
  // put your setup code here, to run once:
SIM900.begin(115200);
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
if (SIM900.available()){
  Serial.write(SIM900.read());  
  }

  if(Serial.available()){
    SIM900.write(Serial.read());
    }
}
