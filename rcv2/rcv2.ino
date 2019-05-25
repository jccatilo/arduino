int posr = 5; //positive ng right gulong
int negr = 6; // negative ng right gulong
int posl = 9; //positive ng left gulong
int negl = 10; // negative ng left gulong
int bl = 2; // blinker left front and back
int br = 3; // blinker right front and back
int horn = 4; //horn
int ultrasonic = A5; //ultrasonic
int servo = A1; //manibela
int pingPin = 7;
int distanceLimit = 10;
char pagIwas;
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
  pinMode(bl, OUTPUT);
  pinMode(br, OUTPUT);
  pinMode(horn, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 char command = Serial.read();
 switch(command){
  case 'f':
    forward();
    break;
  case 'b':
    reverse();
    break;
  case 'l':
    left();
    break;
  case 'r':
    right();
    break;
  case 's':
    stp();
    break;
  case 'q':
    hazard();
    break;
  case 'w':
    blinkLeft();
    break;
  case 'e':
    blinkRight();
    break;
  case 't':
    horning();
    break;
  case 'a':
    autoPilot();
    break;
  default: break;
 }
 delay(50);
}

void forward(){
      Serial.println("forward");
      digitalWrite(posr, HIGH);
      digitalWrite(negr, LOW);
      digitalWrite(posl, HIGH);
      digitalWrite(negl, LOW);
      ping();
    }

void reverse(){
      Serial.println("backward");
      digitalWrite(posr, LOW);
      digitalWrite(negr, HIGH);
      digitalWrite(posl, LOW);
      digitalWrite(negl, HIGH);
      ping();
    }

void left(){
      Serial.println("backward");
      digitalWrite(posr, HIGH);
      digitalWrite(negr, LOW);
      digitalWrite(posl, LOW);
      digitalWrite(negl, LOW);
      ping();
    }

void right(){
      Serial.println("right");
      digitalWrite(posr, LOW);
      digitalWrite(negr, LOW);
      digitalWrite(posl, HIGH);
      digitalWrite(negl, LOW);
      ping();
    }
void stp(){ 
      Serial.println("stop");
      digitalWrite(posr, LOW);
      digitalWrite(negr, LOW);
      digitalWrite(posl, LOW);
      digitalWrite(negl, LOW);
      digitalWrite(bl, HIGH);
      digitalWrite(br, HIGH);
      digitalWrite(horn, LOW);
    }

void hazard(){
      Serial.println("hazard mode on");
      digitalWrite(br, HIGH);
      digitalWrite(bl, HIGH);
      delay(1000);
      digitalWrite(br, LOW);
      digitalWrite(bl, LOW);
      delay(1000);
    }

void blinkLeft(){
      Serial.println("blinker left on");
      digitalWrite(br, LOW);
      digitalWrite(bl, HIGH);
      delay(1000);
      digitalWrite(br, LOW);
      digitalWrite(bl, LOW);
      delay(1000);
    }

void blinkRight(){
      Serial.println("blinker right on");
      digitalWrite(br, HIGH);
      digitalWrite(bl, LOW);
      delay(1000);
      digitalWrite(br, LOW);
      digitalWrite(bl, LOW);
      delay(1000);
    }

void horning(){
      Serial.println("Horning");
      digitalWrite(horn, HIGH);
    }

void autoPilot(){
      Serial.println("AutopilotMode");
/*      int distance = ping();
      if (distance < distanceLimit){
        stp();
        switch (pagIwas){
          case 'p': //faceLeft
            reverse();
            delay(1000);
            left();
            delay(500);
            stp();
            delay(500);
            break;
          case 'o': //faceRight
            reverse();
            delay(1000);
            right();
            delay(500);
            stp();
            delay(500);
            break;
          default:break;
        }
        forward();
        
      } */
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



