int buttonToLeft = 2;
int buttonToRight = 3;
int andarLeft = 4;
int andarRight = 5;
int pinindotLeft = 0;
int pinindotRight = 0;
int errorCommand =0;


void setup() {
pinMode(buttonToLeft, INPUT);
pinMode(buttonToRight, INPUT);
pinMode(andarLeft, OUTPUT);
pinMode(andarRight, OUTPUT);
pinMode(errorCommand, OUTPUT);
}

void loop() {
 pinindotLeft =  digitalRead(buttonToLeft);
 pinindotRight = digitalRead(buttonToRight);

if ((pinindotLeft ==  LOW)&&(pinindotRight== LOW)){
  digitalWrite(andarLeft, LOW);
  digitalWrite(andarRight, LOW);
  digitalWrite(errorCommand, LOW);}
  
else if ((pinindotLeft ==  HIGH)&&(pinindotRight== LOW)){
  digitalWrite(andarLeft, HIGH);
  digitalWrite(andarRight, LOW);
  digitalWrite(errorCommand, LOW);}

else if ((pinindotLeft ==  LOW)&&(pinindotRight== HIGH)){
  digitalWrite(andarLeft, LOW);
  digitalWrite(andarRight, HIGH);
  digitalWrite(errorCommand, LOW);}
  
else if ((pinindotLeft ==  HIGH)&&(pinindotRight== HIGH)){
  digitalWrite(andarLeft, LOW);
  digitalWrite(andarRight, LOW);
  digitalWrite(errorCommand, HIGH);}

else {
  //pogi si jc
  }
}
