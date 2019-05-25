void setup() {

  pinMode(4, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println("hi.");
  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second

}
