int pinone = 4;
void setup() {
  // put your setup code here, to run once:
pinMode(pinone, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(pinone, HIGH);
delay(1000);
digitalWrite(pinone, LOW);
delay(500);
}
