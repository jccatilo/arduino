l#include <dht11.h>
#include <NewPing.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <Wire.h>
#include <BH1750.h>

#define StartConvert 0
#define ReadTemperature 1
#define StartConvert 0
#define ReadTemperature 1
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}
#define ReceivedBufferLength 20
#define SCOUNT  30
#define SlopeValueAddress 0
#define InterceptValueAddress (SlopeValueAddress+4) 
#define VREF 5000
#define moisturePin   A0
#define dhtPin        A1 
#define triggerPin    50
#define echoPin       52
#define phPin         A5
#define ECsensorPin   A6 
#define MAX_DISTANCE  250
//start alarm
int one = 22;
int two = 24;
int three = 26;
int four = 28;
int five = 30;
int six = 32;
int seven = 34;
int eight = 36;
int nine = 38;
int ten = 40;
int eleven = 42;
int twelve = 44;
int thirteen = 46;
int fourteen = 48;
int fifteen = 47;
int sixteen = 49;
//end alarm
int analogBuffer[SCOUNT];
int analogBufferIndex = 0;
int moistureValue =   0;
//int lightValue =    0;
uint16_t lightValue = 0;
boolean enterCalibrationFlag = 0;
float slopeValue, interceptValue, averageVoltageph;
float pingValue = 		0.00;
float phValue = 0.00;
float ECcurrent  = 0.00;
float ecValue = 0.00;
float tempValue = 0.00;
float humidValue = 0.00;
float temperature;
//double z = 0;
String commandString;
char receivedBuffer[ReceivedBufferLength+1];

const byte numReadings = 20;
unsigned int AnalogSampleInterval=25,printInterval=700,tempSampleInterval=850;
unsigned int readings[numReadings];
unsigned int AnalogAverage = 0,averageVoltage=0;
unsigned long AnalogValueTotal = 0;
unsigned long AnalogSampleTime,printTime,tempSampleTime;
byte index = 0; 
byte receivedBufferIndex = 0;
byte DS18B20_Pin = 4; 

NewPing sonar(triggerPin, echoPin, MAX_DISTANCE);
OneWire ds(DS18B20_Pin);
dht11 DHT;
BH1750 lightMeter;

void setup(){
  //alarm
  Serial.begin(9600);
  pinMode(one, OUTPUT);
  pinMode(two, OUTPUT);
  pinMode(three, OUTPUT);
  pinMode(four, OUTPUT);
  pinMode(five, OUTPUT);
  pinMode(six, OUTPUT);
  pinMode(seven, OUTPUT);
  pinMode(eight, OUTPUT);
  pinMode(nine, OUTPUT);
  pinMode(ten, OUTPUT);
  pinMode(eleven, OUTPUT);
  pinMode(twelve, OUTPUT);
  pinMode(thirteen, OUTPUT);
  pinMode(fourteen, OUTPUT);
  pinMode(fifteen, OUTPUT);
  pinMode(sixteen, OUTPUT);
  //end alarm
  readCharacteristicValues();
  lightMeter.begin(BH1750_CONTINUOUS_HIGH_RES_MODE_2);
  for (byte thisReading = 0; thisReading < numReadings; thisReading++)
  readings[thisReading] = 0;
  TempProcess(StartConvert);
  AnalogSampleTime=millis();
  printTime=millis();
  tempSampleTime=millis();
}

void loop(){   
 Serial.flush();
  while (Serial.available()){
    delay(10);
    char receivedChar = Serial.read();
    commandString += receivedChar;
    Serial.flush();
    
    //send TEMP value
    if (commandString.charAt(0) == 'a'){ 
      getTempValue();
      Serial.print(tempValue, 1);

                if (tempValue<21){
                  digitalWrite(nine, HIGH);
                  digitalWrite(one,LOW);
                  delay(500);
                  }
                else if (tempValue>27){
                  digitalWrite(nine, LOW);
                  digitalWrite(one, HIGH);
                  delay(500);
                  }
                else{
                  digitalWrite(one, LOW);
                  digitalWrite(nine, LOW);
                  delay(500);}
    }

    //send HUMID value
    if (commandString.charAt(0) == 'b'){ 
      getHumidValue();
      Serial.print(humidValue, 1);
                  if (humidValue<50){
                    digitalWrite(ten, HIGH);
                    digitalWrite(two,LOW);
                 //   Serial.println("humidity low");
                    delay(500);
                    }
                  else if (humidValue>80){
                    digitalWrite(ten, LOW);
                    digitalWrite(two, HIGH);
                 //   Serial.println("humidity high");
                    delay(500);
                    }
                  else{
                    digitalWrite(ten, LOW);
                    digitalWrite(two, LOW);
                //    Serial.println("humidity ok");
                    delay(500);
                    }
    }

    //send LIGHT value
    if (commandString.charAt(0) == 'c'){ 
      getLightValue();
      Serial.print(lightValue, 1);
                  if (lightValue<20000){
                    digitalWrite(eleven, HIGH);
                    digitalWrite(three,LOW);
                 //   Serial.println("lux low");
                    delay(500);
                    }
                  else if (lightValue>30000){
                    digitalWrite(eleven, LOW);
                    digitalWrite(three, HIGH);
                 //   Serial.println("lux high");
                    delay(500);
                    }
                  else{
                    digitalWrite(eleven, LOW);
                    digitalWrite(three, LOW);
                //    Serial.println("lux ok");
                    delay(500);
                    }
    }

    //send MOISTURE value
    if (commandString.charAt(0) == 'd'){ 
      getMoistureValue();
      Serial.print(moistureValue);
                  if (moistureValue<40){
                    digitalWrite(twelve, HIGH);
                    digitalWrite(four,LOW);
                 //   Serial.println("moisture low");
                    delay(500);
                    }
                  else if (moistureValue>80){
                    digitalWrite(twelve, LOW);
                    digitalWrite(four, HIGH);
                 //   Serial.println("moisture high");
                    delay(500);
                    }
                  else{
                    digitalWrite(twelve, LOW);
                    digitalWrite(four, LOW);
                //    Serial.println("moisture ok");
                    delay(500);
                    }
    }

    //send PING value
    if (commandString.charAt(0) == 'e'){ 
      getPingValue();
      Serial.print(pingValue, 2);
                    if (pingValue<8){
                      //Serial.println("high solution level");
                      digitalWrite(five, HIGH);
                      digitalWrite(thirteen, LOW);
                      }
                    else if (pingValue>20){
                      //Serial.println("low solution level");
                      digitalWrite(five, LOW);
                      digitalWrite(thirteen, HIGH);
                      }
                    else{
                    digitalWrite(five,LOW);
                    digitalWrite(thirteen, LOW);
                    }
      }

    //send PH value
    if (commandString.charAt(0) == 'f'){ 
      Serial.print(phValue);
                  if (phValue<6.0){
                    digitalWrite(fourteen, HIGH);
                    digitalWrite(six,LOW);
                 //   Serial.println("ph low");
                    delay(500);
                    }
                  else if (phValue>6.5){
                    digitalWrite(fourteen, LOW);
                    digitalWrite(six, HIGH);
                 //   Serial.println("ph high");
                    delay(500);
                    }
                  else{
                    digitalWrite(fourteen, LOW);
                    digitalWrite(six, LOW);
                //    Serial.println("ph ok");
                    delay(500);
                    }
    }

    //send EC value
    if (commandString.charAt(0) == 'g'){
      Serial.print(ECcurrent, 2);
                  if (ECcurrent<1.2){
                    digitalWrite(fifteen, HIGH);
                    digitalWrite(seven,LOW);
                 //   Serial.println("ec low");
                    delay(500);
                    }
                  else if (ECcurrent>2.4){
                    digitalWrite(fifteen, LOW);
                    digitalWrite(seven, HIGH);
                 //   Serial.println("ec high");
                    delay(500);
                    }
                  else{
                    digitalWrite(fifteen, LOW);
                    digitalWrite(seven, LOW);
                //    Serial.println("ec ok");
                    delay(500);
                    }
    }

    //send Water Temp value
    if (commandString.charAt(0) == 'h'){
      Serial.print(temperature, 2);
                  if (temperature<20){
                    digitalWrite(sixteen, HIGH);
                    digitalWrite(eight,LOW);
                 //   Serial.println("solution temp low");
                    delay(500);
                    }
                  else if (temperature>35){
                    digitalWrite(sixteen, LOW);
                    digitalWrite(eight, HIGH);
                 //   Serial.println("solution temp high");
                    delay(500);
                    }
                  else{
                    digitalWrite(sixteen, LOW);
                    digitalWrite(eight, LOW);
                //    Serial.println("solution temp ok");
                    delay(500);
                    }
    }
    }    
  
  commandString = "";
  getPhValue();
  getEcValue();

  delay(10);

}


void getTempValue(){
  int chk;
  chk = DHT.read(dhtPin);
  tempValue = DHT.temperature;
}



void getHumidValue(){
  int chk;
  chk = DHT.read(dhtPin);
  humidValue = DHT.humidity;
}

void getMoistureValue(){
  //moistureValue=analogRead(moisturePin);
  
  if (analogRead(moisturePin)==0) {
    moistureValue= 0;}
  if (analogRead(moisturePin)>=1 && analogRead(moisturePin)<= 110){
  moistureValue= 10;}
  if (analogRead(moisturePin)>=111 && analogRead(moisturePin)<=220){
  moistureValue= 20;}
   if (analogRead(moisturePin)>=221 && analogRead(moisturePin)<= 330){
  moistureValue= 30;}
   if (analogRead(moisturePin)>=331 && analogRead(moisturePin)<= 440){
  moistureValue= 40;}
   if (analogRead(moisturePin)>=441 && analogRead(moisturePin)<= 550){
  moistureValue= 50;}
   if (analogRead(moisturePin)>=551 && analogRead(moisturePin)<= 660){
  moistureValue= 60;}
   if (analogRead(moisturePin)>=661 && analogRead(moisturePin)<= 770){
  moistureValue= 70;}
   if (analogRead(moisturePin)>=771 && analogRead(moisturePin)<= 880){
  moistureValue= 80;}
   if (analogRead(moisturePin)>=881 && analogRead(moisturePin)<= 990){
  moistureValue= 90;}
   if (analogRead(moisturePin)>=991 && analogRead(moisturePin)<= 1100){
  moistureValue= 100;} 

}

void getLightValue(){
  lightValue  = lightMeter.readLightLevel();
 //float z = lightMeter.readLightLevel();
 //lightValue  = 0.00000000015444*pow (z,3) - 0.000017538*pow (z,2) + 1.1169*z - 22.686;
}

void getPingValue(){
  pingValue  =   sonar.ping_cm();

}

void getEcValue(){
  /*
   Every once in a while,sample the analog value and calculate the average.
  */
  if(millis()-AnalogSampleTime>=AnalogSampleInterval)  
  {
    AnalogSampleTime=millis();
     // subtract the last reading:
    AnalogValueTotal = AnalogValueTotal - readings[index];
    // read from the sensor:
    readings[index] = analogRead(ECsensorPin);
    // add the reading to the total:
    AnalogValueTotal = AnalogValueTotal + readings[index];
    // advance to the next position in the array:
    index = index + 1;
    // if we're at the end of the array...
    if (index >= numReadings)
    // ...wrap around to the beginning:
    index = 0;
    // calculate the average:
    AnalogAverage = AnalogValueTotal / numReadings;
  }
  /*
   Every once in a while,MCU read the temperature from the DS18B20 and then let the DS18B20 start the convert.
   Attention:The interval between start the convert and read the temperature should be greater than 750 millisecond,or the temperature is not accurate!
  */
   if(millis()-tempSampleTime>=tempSampleInterval) 
  {
    tempSampleTime=millis();
    temperature = TempProcess(ReadTemperature);  // read the current temperature from the  DS18B20
    TempProcess(StartConvert);                   //after the reading,start the convert for next reading
  }
   /*
   Every once in a while,print the information on the serial monitor.
  */
  if(millis()-printTime>=printInterval)
  {
    printTime=millis();
    averageVoltage=AnalogAverage*(float)5000/1024;

    float TempCoefficient=1.0+0.0185*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.0185*(fTP-25.0));
    float CoefficientVolatge=(float)averageVoltage/TempCoefficient;   
    if(CoefficientVolatge<150)ECcurrent = 0; //Serial.println("0");   //25^C 1413us/cm<-->about 216mv  if the voltage(compensate)<150,that is <1ms/cm,out of the range
    else if(CoefficientVolatge>3300)ECcurrent = 0; //Serial.println("Out of the range!");  //>20ms/cm,out of the range
    else
    { 
      if(CoefficientVolatge<=448)ECcurrent=6.84*CoefficientVolatge-64.32;   //1ms/cm<EC<=3ms/cm
      else if(CoefficientVolatge<=1457)ECcurrent=6.98*CoefficientVolatge-127;  //3ms/cm<EC<=10ms/cm
      else ECcurrent=5.3*CoefficientVolatge+2278;                           //10ms/cm<EC<20ms/cm

 
      ECcurrent/=1000;    //convert us/cm to ms/cm

    }
  }
}

void getPhValue(){
 static unsigned long sampleTimepoint = millis();
   if(millis()-sampleTimepoint>40U)
   {
     sampleTimepoint = millis();
     analogBuffer[analogBufferIndex] = analogRead(phPin)/1024.0*VREF;    //read the voltage and store into the buffer,every 40ms
     analogBufferIndex++;
     if(analogBufferIndex == SCOUNT) 
         analogBufferIndex = 0;
     averageVoltageph = getMedianNum(analogBuffer,SCOUNT);   // read the stable value by the median filtering algorithm
   }
   
   static unsigned long printTimepoint = millis();
   if(millis()-printTimepoint>1000U)
   {
     printTimepoint = millis();
     phValue = averageVoltageph/1000.0*slopeValue+interceptValue;
   }   
}

boolean serialDataAvailable(void){
  char receivedChar;
  static unsigned long receivedTimeOut = millis();
  while (Serial.available()>0) 
  {   
    if (millis() - receivedTimeOut > 1000U) 
    {
      receivedBufferIndex = 0;
      memset(receivedBuffer,0,(ReceivedBufferLength+1));
    }
    receivedTimeOut = millis();
    receivedChar = Serial.read();
    if (receivedChar == '\n' || receivedBufferIndex==ReceivedBufferLength){
    receivedBufferIndex = 0;
    strupr(receivedBuffer);
    return true;
    }
    else{
      receivedBuffer[receivedBufferIndex] = receivedChar;
      receivedBufferIndex++;
    }
  }
  return false;
}

byte uartParse(){
  byte modeIndex = 0;
  if(strstr(receivedBuffer, "CALIBRATION") != NULL) 
      modeIndex = 1;
  else if(strstr(receivedBuffer, "EXIT") != NULL) 
      modeIndex = 4;
  else if(strstr(receivedBuffer, "ACID:") != NULL)   
      modeIndex = 2;  
  else if(strstr(receivedBuffer, "ALKALI:") != NULL)
      modeIndex = 3;
  return modeIndex;
}

void phCalibration(byte mode){
    char *receivedBufferPtr;
    static byte acidCalibrationFinish = 0, alkaliCalibrationFinish = 0;
    static float acidValue,alkaliValue;
    static float acidVoltage,alkaliVoltage;
    float acidValueTemp,alkaliValueTemp,newSlopeValue,newInterceptValue;
    switch(mode)
    {
      case 0:
      if(enterCalibrationFlag)
         Serial.println(F("Command Error"));
      break;
      
      case 1:
      receivedBufferPtr=strstr(receivedBuffer, "CALIBRATION");
      enterCalibrationFlag = 1;
      acidCalibrationFinish = 0;
      alkaliCalibrationFinish = 0;
      Serial.println(F("Enter Calibration Mode"));
      break;
     
      case 2:
      if(enterCalibrationFlag)
      {
          receivedBufferPtr=strstr(receivedBuffer, "ACID:");
          receivedBufferPtr+=strlen("ACID:");
          acidValueTemp = strtod(receivedBufferPtr,NULL);
          if((acidValueTemp>3)&&(acidValueTemp<5))        //typical ph value of acid standand buffer solution should be 4.00
          {
             acidValue = acidValueTemp;
             acidVoltage = averageVoltage/1000.0;        // mV -> V
             acidCalibrationFinish = 1;
             Serial.println(F("Acid Calibration Successful"));
           }else {
             acidCalibrationFinish = 0;
             Serial.println(F("Acid Value Error"));
           }
      }
      break;
 
       case 3:
       if(enterCalibrationFlag)
       {
           receivedBufferPtr=strstr(receivedBuffer, "ALKALI:");
           receivedBufferPtr+=strlen("ALKALI:");
           alkaliValueTemp = strtod(receivedBufferPtr,NULL);
           if((alkaliValueTemp>8)&&(alkaliValueTemp<11))        //typical ph value of alkali standand buffer solution should be 9.18 or 10.01
           {
                 alkaliValue = alkaliValueTemp;
                 alkaliVoltage = averageVoltage/1000.0;
                 alkaliCalibrationFinish = 1;
                 Serial.println(F("Alkali Calibration Successful"));
            }else{
               alkaliCalibrationFinish = 0;
               Serial.println(F("Alkali Value Error"));
            }
       }
       break;

        case 4:
        if(enterCalibrationFlag)
        {
            if(acidCalibrationFinish && alkaliCalibrationFinish)
            {
              newSlopeValue = (acidValue-alkaliValue)/(acidVoltage - alkaliVoltage);
              EEPROM_write(SlopeValueAddress, newSlopeValue);
              newInterceptValue = acidValue - (slopeValue*acidVoltage);
              EEPROM_write(InterceptValueAddress, newInterceptValue);
              Serial.print(F("Calibration Successful"));
            }
            else Serial.print(F("Calibration Failed"));       
            Serial.println(F(",Exit Calibration Mode"));
            acidCalibrationFinish = 0;
            alkaliCalibrationFinish = 0;
            enterCalibrationFlag = 0;
        }
        break;
    }
}

int getMedianNum(int bArray[], int iFilterLen) {
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
      {
    bTab[i] = bArray[i];
      }
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++) 
      {
    for (i = 0; i < iFilterLen - j - 1; i++) 
          {
      if (bTab[i] > bTab[i + 1]) 
            {
    bTemp = bTab[i];
          bTab[i] = bTab[i + 1];
    bTab[i + 1] = bTemp;
       }
    }
      }
      if ((iFilterLen & 1) > 0)
  bTemp = bTab[(iFilterLen - 1) / 2];
      else
  bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
}

void readCharacteristicValues(){
    EEPROM_read(SlopeValueAddress, slopeValue);
    EEPROM_read(InterceptValueAddress, interceptValue);
    if(EEPROM.read(SlopeValueAddress)==0xFF && EEPROM.read(SlopeValueAddress+1)==0xFF && EEPROM.read(SlopeValueAddress+2)==0xFF && EEPROM.read(SlopeValueAddress+3)==0xFF)
    {
      slopeValue = 3.5;   // If the EEPROM is new, the recommendatory slope is 3.5.
      EEPROM_write(SlopeValueAddress, slopeValue);
    }
    if(EEPROM.read(InterceptValueAddress)==0xFF && EEPROM.read(InterceptValueAddress+1)==0xFF && EEPROM.read(InterceptValueAddress+2)==0xFF && EEPROM.read(InterceptValueAddress+3)==0xFF)
    {
      interceptValue = 0;  // If the EEPROM is new, the recommendatory intercept is 0.
      EEPROM_write(InterceptValueAddress, interceptValue);
    }
}

float TempProcess(bool ch){
  //returns the temperature from one DS18B20 in DEG Celsius
  static byte data[12];
  static byte addr[8];
  static float TemperatureSum;
  if(!ch){
          if ( !ds.search(addr)) {
              //Serial.println("no more sensors on chain, reset search!");
              ds.reset_search();
              return 0;
          }      
          if ( OneWire::crc8( addr, 7) != addr[7]) {
              //Serial.println("CRC is not valid!");
              return 0;
          }        
          if ( addr[0] != 0x10 && addr[0] != 0x28) {
              //Serial.print("Device is not recognized!");
              return 0;
          }      
          ds.reset();
          ds.select(addr);
          ds.write(0x44,1); // start conversion, with parasite power on at the end
  }
  else{  
          byte present = ds.reset();
          ds.select(addr);    
          ds.write(0xBE); // Read Scratchpad            
          for (int i = 0; i < 9; i++) { // we need 9 bytes
            data[i] = ds.read();
          }         
          ds.reset_search();           
          byte MSB = data[1];
          byte LSB = data[0];        
          float tempRead = ((MSB << 8) | LSB); //using two's compliment
          TemperatureSum = tempRead / 16;
    }
          return TemperatureSum;  
}
