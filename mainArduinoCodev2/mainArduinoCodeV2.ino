#include <dht11.h>
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

int ledPins[]= { 22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38};

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
  Serial.begin(9600);
  readCharacteristicValues();
  lightMeter.begin(BH1750_CONTINUOUS_HIGH_RES_MODE_2);
  
  for (byte thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;
  TempProcess(StartConvert);
  AnalogSampleTime=millis();
  printTime=millis();
  tempSampleTime=millis();
  
  for ( int i = 0; i<16; i ++)
  pinMode(ledPins[i], OUTPUT);
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
      Serial.println(tempValue, 1);
    }

    //send HUMID value
    if (commandString.charAt(0) == 'b'){ 
      getHumidValue();
      Serial.println(humidValue, 1);
    }

    //send LIGHT value
    //if (commandString.charAt(0) == 'c'){ 
    //  getLightValue();
    //  Serial.println(lightValue, 1);

    if (commandString.charAt(0) == 'c'){ 
      getLightValue();
      Serial.println(lightValue, 1);
    }

    //send MOISTURE value
    if (commandString.charAt(0) == 'd'){ 
      getMoistureValue();
      Serial.println(moistureValue);
    }

    //send PING value
    if (commandString.charAt(0) == 'e'){ 
      getPingValue();
      Serial.println(pingValue, 2);
    }
    

    //send PH value
    if (commandString.charAt(0) == 'f'){ 
      Serial.println(phValue);
    }

    //send EC value
    if (commandString.charAt(0) == 'g'){
      Serial.println(ECcurrent, 2);
    }

    //send Water Temp value
    if (commandString.charAt(0) == 'h'){
      Serial.println(temperature, 2);
    }    
  }
  commandString = "";
  getPhValue();
  getEcValue();
  delay(10);

  //*alarm start*//

if ((tempValue,1)<19)
{digitalWrite(ledPins, HIGH);}
if ((tempValue,1)>25)
{digitalWrite(ledPins, HIGH);}

if ((humidValue,1)<50)
{digitalWrite(ledPins, HIGH);}
if((humidValue,1)>70)
{digitalWrite(ledPins, HIGH);}

if ((lightValue, 1)<500)
{digitalWrite(ledPins, HIGH);}
if((lightValue,1)>10800)
{digitalWrite(ledPins, HIGH);}

if ((moistureValue, 1)<40)
{digitalWrite(ledPins, HIGH);}
if((moistureValue, 1)>80)
{digitalWrite(ledPins, HIGH);}

if ((pingValue, 2)>20)
{digitalWrite(ledPins, HIGH);}

if ((phValue)<6)
{digitalWrite(ledPins, HIGH);}
if((phValue)>6.5)
{digitalWrite(ledPins, HIGH);}

if ((ECcurrent, 2)<0.6)
{digitalWrite(ledPins, HIGH);}
if((ECcurrent, 2)>1.4)
{digitalWrite(ledPins, HIGH);}

if ((temperature, 2)<18)
{digitalWrite(ledPins, HIGH);}
if((temperature, 2)>26)
{digitalWrite(ledPins, HIGH);}

//alarm end
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
 // moistureValue =   analogRead(moisturePin);
 // moistureValue = 0.0931*analogRead(moisturePin) - 0.8898; first formula
  
 if (analogRead(moisturePin)>=1 && analogRead(moisturePin)<= 95){
  moistureValue= 10;}
  if (analogRead(moisturePin)>=96 && analogRead(moisturePin)<= 190){
  moistureValue= 20;}
   if (analogRead(moisturePin)>=191 && analogRead(moisturePin)<= 285){
  moistureValue= 30;}
   if (analogRead(moisturePin)>=286 && analogRead(moisturePin)<= 380){
  moistureValue= 40;}
   if (analogRead(moisturePin)>=381 && analogRead(moisturePin)<= 475){
  moistureValue= 50;}
   if (analogRead(moisturePin)>=476 && analogRead(moisturePin)<= 570){
  moistureValue= 60;}
   if (analogRead(moisturePin)>=571 && analogRead(moisturePin)<= 665){
  moistureValue= 70;}
   if (analogRead(moisturePin)>=666 && analogRead(moisturePin)<= 760){
  moistureValue= 80;}
   if (analogRead(moisturePin)>=761 && analogRead(moisturePin)<= 855){
  moistureValue= 90;}
   if (analogRead(moisturePin)>=856 && analogRead(moisturePin)<= 950){
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
//    Serial.print("Analog value:");
//    Serial.print(AnalogAverage);   //analog average,from 0 to 1023
//    Serial.print("    Voltage:");
//    Serial.print(averageVoltage);  //millivolt average,from 0mv to 4995mV
//    Serial.print("mV    ");
//    Serial.print("temp:");
//    Serial.print(temperature);    //current temperature
//    Serial.print("^C     EC:");
    
    float TempCoefficient=1.0+0.0185*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.0185*(fTP-25.0));
    float CoefficientVolatge=(float)averageVoltage/TempCoefficient;   
    if(CoefficientVolatge<150)ECcurrent = 0; //Serial.println("0");   //25^C 1413us/cm<-->about 216mv  if the voltage(compensate)<150,that is <1ms/cm,out of the range
    else if(CoefficientVolatge>3300)ECcurrent = 0; //Serial.println("Out of the range!");  //>20ms/cm,out of the range
    else
    { 
      if(CoefficientVolatge<=448)ECcurrent=6.84*CoefficientVolatge-64.32;   //1ms/cm<EC<=3ms/cm
      else if(CoefficientVolatge<=1457)ECcurrent=6.98*CoefficientVolatge-127;  //3ms/cm<EC<=10ms/cm
      else ECcurrent=5.3*CoefficientVolatge+2278;                           //10ms/cm<EC<20ms/cm

//      else ECcurrent=5.3*CoefficientVolatge+2278;  
      ECcurrent/=1000;    //convert us/cm to ms/cm
//      Serial.print(ECcurrent,2);  //two decimal
//      Serial.println("ms/cm");
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
