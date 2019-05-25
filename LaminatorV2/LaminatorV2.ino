int pinDir =8;
int pinAdd =A0;
int addVal = 0;
int pinSub =A1;
int subVal =0;
int numIt = 0;
int pinF=13;
int pinR=5;
int pinGo=A2;
int goVal=0;

void setup() {
  // put your setup code here, to run once:
pinMode(pinAdd, INPUT);
pinMode(pinSub, INPUT);
pinMode(pinGo, INPUT);
Serial.begin(9600);
}

void loop() {
goVal= analogRead(pinGo);
if(goVal>=800){
  Serial.println("Moving ");
  do{
  Serial.print(numIt);
  Serial.println(" left.");
  Serial.println("Moving Forward.");
  digitalWrite(pinF, HIGH);
  digitalWrite(pinR, LOW);
  delay(5000);
  Serial.println("Moving Backward.");
  digitalWrite(pinF, LOW);
  digitalWrite(pinR, HIGH);
  delay(5000);
  numIt=numIt-1;
  }while (numIt!=0);
 }

else{
  digitalWrite(pinF, LOW);
  digitalWrite(pinR, LOW);
getInput();
}
}

void getInput(){
    // put your main code here, to run repeatedly:
//Serial.println("Hi.");
addVal = analogRead(pinAdd);
//Serial.println(addVal);
subVal = analogRead(pinSub);
//Serial.println(subVal);
if(addVal>=800){
  numIt=numIt+1;
  delay(5);
  }
 else if(subVal>=800){
  if(numIt<=0){numIt=0;}
  else{numIt=numIt-1;}
  delay(5);
  }
 else{
    }    
delay(500);
    Serial.print("Number of Iterations = ");
    Serial.println(numIt);
    
  return numIt;
  }
