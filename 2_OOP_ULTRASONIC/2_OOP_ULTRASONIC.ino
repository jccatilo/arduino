class Ultrasonic {
    //declaration of trig and echo pin for our sensor
    int TrigPin;
    int EchoPin;
  public:
    Ultrasonic(int trigpin, int echopin) {
      //Takes in the parameters passed during instantiation
      TrigPin = trigpin;
      EchoPin = echopin;
      //Sets the necessary pin modes for trigger and echo pin
      pinMode(TrigPin, OUTPUT);
      pinMode(EchoPin, INPUT);
      digitalWrite(TrigPin, LOW);//Sets trig pin to low. Needed.
    }
    int getDistance() {
      digitalWrite(TrigPin, LOW);//ensure trigger is low
      delayMicroseconds(2);
      digitalWrite(TrigPin, HIGH);//trigger sends a pulse
      delayMicroseconds(10);
      digitalWrite(TrigPin, LOW);//trigger goes back to low
      long duration = pulseIn(EchoPin, HIGH); //activate echo pin to wait for pulse coming back
      int distance = duration * 0.034 / 2; //formula to get distance in centimeters
      return distance;
    }
};

//Instantiate your objects
// Ultrasonic <your_desired_object_name> (<trigger_pin>,<echo_pin>)
Ultrasonic us_1(12, 11);
Ultrasonic us_2(9, 8);
Ultrasonic us_3(6, 5);
Ultrasonic us_4(3, 2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("1st: ");
  Serial.println(us_1.getDistance());
  delay(500);
  Serial.print("2nd: ");
  Serial.println(us_2.getDistance());
  delay(500);
  Serial.print("3rd: ");
  Serial.println(us_3.getDistance());
  delay(500);
  Serial.print("4th: ");
  Serial.println(us_4.getDistance());
  delay(500);
  Serial.println("==============");
  delay(500);
}
