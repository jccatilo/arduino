int pinDir = 8;
int pinAdd = A0;
int addVal = 0;
int pinSub = A1;
int subVal = 0;
int numIt = 0;
int pinF = 5;
int pinR = 13;
int pinGo = A2;
int goVal = 0;
String bcdVal = "";

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
  goVal = analogRead(pinGo);
  WriteDigit(1, numIt / 10);
  WriteDigit(2, numIt % 10);

  if (goVal >= 800) {
    Serial.println("Moving ");
    do {
      WriteDigit(1, numIt / 10);
      WriteDigit(2, numIt % 10);
      Serial.print(numIt);
      Serial.println(" left.");
      Serial.println("Moving Forward.");
      digitalWrite(pinF, HIGH);
      digitalWrite(pinR, LOW);
      delay(2000);
      Serial.println("Moving Backward.");
      digitalWrite(pinF, LOW);
      digitalWrite(pinR, HIGH);
      delay(2000);
      numIt = numIt - 1;
    } while (numIt != 0);
  }

  else {
    digitalWrite(pinF, LOW);
    digitalWrite(pinR, LOW);
    getInput();
  }
}

void getInput() {
  addVal = analogRead(pinAdd);
  subVal = analogRead(pinSub);
  if (addVal >= 800) {
    numIt = numIt + 1;
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    delay(500);
  }
  else if (subVal >= 800) {
    if (numIt <= 0) {
      numIt = 0;
    }
    else {
      numIt = numIt - 1;
    }
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    delay(500);
  }
  else {
  }
  //    Serial.print("Number of Iterations = ");
  //    Serial.println(numIt);
  WriteDigit(2, numIt % 10);
  WriteDigit(1, numIt / 10);

  return numIt;
}




void WriteDigit(int d, int v) {
  digitalWrite(2, d == 1);
  digitalWrite(3, d == 2);
  digitalWrite(4, d == 3);

  //A (0,2,3,5,6,7,8,9)
  digitalWrite(6, v != 1 && v != 4);
  //B (0,1,2,3,4,5,8,9
  digitalWrite(7, v != 5 && v != 6);
  //C
  digitalWrite(8, v != 2);
  //d
  digitalWrite(9, v != 1 && v != 4 && v != 7);
  //e
  digitalWrite(10, v == 0 || v == 2 || v == 6 || v == 8);
  //f
  digitalWrite(11, v != 1 && v != 2 && v != 3 && v != 7);
  //g
  digitalWrite(12, v != 0 && v != 1 && v != 7);
  delay(5);

}
