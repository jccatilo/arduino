int posr = 7; //positive ng right gulong
int negr = 4; // negative ng right gulong
int posl = 8; //positive ng left gulong
int negl = 12; // negative ng left gulong
int limit = 15;
int trigPin = 9;
int echoPin = 10;
long duration;
int distance;

void setup() {
  // put your setup code here, to run once:
  pinMode(posr, OUTPUT);
  pinMode(negr, OUTPUT);
  pinMode(posl, OUTPUT);
  pinMode(negr, OUTPUT);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
Serial.begin(9600);
}

int ping(){
digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
}

void loop() {
  // put your main code here, to run repeatedly:

int limit =15;
if (distance > limit){
  Serial.println("forward");
  digitalWrite(posr, HIGH);
  digitalWrite(negr, LOW);
  digitalWrite(posl, HIGH);
  digitalWrite(negl, LOW);
  ping();
  delay(100);
  }
  
 else if (distance<= limit)  {
  Serial.println("stop");
  digitalWrite(posr, LOW);
  digitalWrite(negr, LOW);
  digitalWrite(posl, LOW);
  digitalWrite(negl, LOW);
  ping();
  delay(100);
  }
  delay(100);
}
