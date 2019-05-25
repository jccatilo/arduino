
int ledPins[]= { 22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38};

void setup() {
  // put your setup code here, to run once:
  for ( int i = 0; i<16; i ++)
  pinMode(ledPins[i], OUTPUT);
}

void loop() {
  
if ((tempValue,1)<19)
{digitalWrite(ledPins, HIGH);}
if ((tempValue,1)>25)
{digitalWrite(ledPins, HIGH);}

if ((humidValue,1)<50)
{digitalWrite(ledPins, HIGH);}
if((humidValue,1))>70)
{digitalWrite(ledPins, HIGH);}

if ((lightValue, 1)<500)
{digitalWrite(ledPins, HIGH);}
if((lightValue,1))>10800)
{digitalWrite(ledPins, HIGH);}

if ((moistureValue, 1)<40)
{digitalWrite(ledPins, HIGH);}
if((moistureValue, 1))>80)
{digitalWrite(ledPins, HIGH);}

if ((pingValue, 2)<500)
{digitalWrite(ledPins, HIGH);}
if((pingValue, 2))>10800)
{digitalWrite(ledPins, HIGH);}

if ((phValue)<6)
{digitalWrite(ledPins, HIGH);}
if((phValue))>6.5)
{digitalWrite(ledPins, HIGH);}

if ((ECcurrent, 2)<0.6)
{digitalWrite(ledPins, HIGH);}
if((ECcurrent, 2))>1.4)
{digitalWrite(ledPins, HIGH);}

if ((temperature, 2)<18)
{digitalWrite(ledPins, HIGH);}
if((temperature, 2))>26)
{digitalWrite(ledPins, HIGH);}

}
