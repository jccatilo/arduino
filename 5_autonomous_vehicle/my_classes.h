class Wheels {
    int Pin1;
    int Pin2;
  public:
    Wheels(int pin1, int pin2) {
      Pin1 = pin1;
      Pin2 = pin2;
      pinMode(Pin1, OUTPUT);
      pinMode(Pin2, OUTPUT);
    }
    void Forward() {
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, LOW);
    }
    void Stop() {
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
    }
    void Reverse() {
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, HIGH);
    }
};

class Ultrasonic {
    int TrigPin;
    int EchoPin;
  public:
    Ultrasonic(int trigpin, int echopin) {
      TrigPin = trigpin;
      EchoPin = echopin;
      pinMode(TrigPin, OUTPUT);
      pinMode(EchoPin, INPUT);
      digitalWrite(TrigPin, LOW);
    }
    int Check() {
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
