
int posr = 5; //positive ng right gulong
int negr = 6; // negative ng right gulong
int posl = 9; //positive ng left gulong
int negl = 10; // negative ng left gulong

void setup() {
  // put your setup code here, to run once:
  pinMode(posr, OUTPUT);
  pinMode(negr, OUTPUT);
  pinMode(posl, OUTPUT);
  pinMode(negr, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 char command = Serial.read();
switch(command)
  {
    case 'f': 
     {
      Serial.println("forward");
      digitalWrite(posr, HIGH);
      digitalWrite(negr, LOW);
      digitalWrite(posl, HIGH);
      digitalWrite(negl, LOW);
      delay(100);
      digitalWrite(posr, LOW);
      digitalWrite(negr, LOW);
      digitalWrite(posl, LOW);
      digitalWrite(negl, LOW);
      break;
    }
    case 'b':
    {
      Serial.println("backward");
      digitalWrite(posr, LOW);
      digitalWrite(negr, HIGH);
      digitalWrite(posl, LOW);
      digitalWrite(negl, HIGH);
      delay(100);
      digitalWrite(posr, LOW);
      digitalWrite(negr, LOW);
      digitalWrite(posl, LOW);
      digitalWrite(negl, LOW);
      break;
    }
    case 'l':
    {
      Serial.println("left");
      digitalWrite(posr, LOW);
      digitalWrite(negr, LOW);
      digitalWrite(posl, HIGH);
      digitalWrite(negl, LOW);
      delay(100);
      digitalWrite(posr, LOW);
      digitalWrite(negr, LOW);
      digitalWrite(posl, LOW);
      digitalWrite(negl, LOW);
      break;
    }
    case 'r':
    {
      Serial.println("right");
      digitalWrite(posr, HIGH);
      digitalWrite(negr, LOW);
      digitalWrite(posl, LOW);
      digitalWrite(negl, LOW);
      delay(100);
      digitalWrite(posr, LOW);
      digitalWrite(negr, LOW);
      digitalWrite(posl, LOW);
      digitalWrite(negl, LOW);
      break;
    }
    case 's':
    { 
      Serial.println("stop");
      digitalWrite(posr, LOW);
      digitalWrite(negr, LOW);
      digitalWrite(posl, LOW);
      digitalWrite(negl, LOW);
      break;
    }
    default: break;
  }
}
