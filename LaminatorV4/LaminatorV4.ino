
int pinAdd =A0;
int addVal = 0;
int pinSub =A1;
int subVal =0;
int numIt = 0;
int pinF=5;
int pinR=13;
int pinGo=A2;
int goVal=0;
int pushB1=A3;
int b1Val = 0;
int pushB2=A4;
int b2Val = 0;

void setup() {
  // put your setup code here, to run once:
pinMode(pinAdd, INPUT);
pinMode(pinSub, INPUT);
pinMode(pinGo, INPUT);
pinMode(2, OUTPUT);
pinMode(3, OUTPUT);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
pinMode(6, OUTPUT);
pinMode(7, OUTPUT);
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
pinMode(10, OUTPUT);
pinMode(11, OUTPUT);
pinMode(12, OUTPUT);
pinMode(13, OUTPUT);




Serial.begin(9600);
}

void loop() {
goVal= analogRead(pinGo);
if(goVal>=800){
  Serial.println("Moving.");
  do{
    b2Val = analogRead(pushB2);
          Serial.println("b2val: ");
      Serial.println(b2Val);
    if(b2Val<=799){
      Serial.println("unangf");
      Serial.println("b2val: ");
      Serial.println(b2Val);
      
      digitalWrite(pinF, HIGH);
      digitalWrite(pinR, LOW);
      }
     else{
        Serial.println("NASA ELSE KA");
        digitalWrite(pinF, LOW);
        digitalWrite(pinR, HIGH);
        if(b1Val>=800){
          digitalWrite(pinF, HIGH);
          digitalWrite(pinR, LOW);
          numIt=numIt-1;          
          }
        }
  
  }while (numIt!=0);
 }

else{

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



void WriteDigit(int d, int v){
  digitalWrite(2,d==1);
  digitalWrite(3, d==2);
  digitalWrite(4, d==3);

  //A (0,2,3,5,6,7,8,9)
  digitalWrite(6, v!=1 && v!=4);
  //B (0,1,2,3,4,5,8,9
  digitalWrite(7, v!=5 &&v!=6);
  //C
  digitalWrite(8, v!=2);
  //d
  digitalWrite(9, v!=1 && v!=4 && v!=7);
  //e
  digitalWrite(10, v==0||v==2||v==6||v==8);
  //f
  digitalWrite(11, v!=1 && v!=2 && v!=3 && v!=7);
  //g
  digitalWrite(12, v!=0 && v!=1 && v!=7);
 
  
  }
