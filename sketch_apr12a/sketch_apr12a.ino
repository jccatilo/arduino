
#include "HX711.h"

#define calibration_factor -7050.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

#define DOUT  3
#define CLK  2

HX711 scale;

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 scale demo");
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  Serial.println("Readings:");
}

void loop() {
  Serial.print("Reading: ");
  float weight = abs(scale.get_units()*-18.10);
  Serial.print(weight, 2); //scale.get_units() returns a float
  Serial.print(" grams"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();
  if(weight>=30){
    Serial.print("ok");
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);
    }
 else{
  if (weight<=29.99&&weight>=15){
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);
    delay(500);
    digitalWrite(5, LOW);
    delay(500);
    }
    else{
      digitalWrite(4,LOW);
      digitalWrite(5,HIGH);
      delay(5);
      }
  }
    

  delay(500);
   
  
}
