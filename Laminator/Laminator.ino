int inA = 2;
int inB = 3;
int inC = 4;
int inD = 5; 

void setup() {
  // put your setup code here, to run once:
  pinMode(inA, OUTPUT);
  pinMode(inB, OUTPUT);
  pinMode(inC, OUTPUT);
  pinMode(inD, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(inA, HIGH);
digitalWrite(inB, LOW);
digitalWrite(inC, LOW);
digitalWrite(inD, HIGH);
}
