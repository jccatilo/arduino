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

Wheels right_wheels(2, 3);
Wheels left_wheels(4, 5);

Ultrasonic sonar_1(53, 52);

int sonar_values[10];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}
void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(get_sonar_value());
  long a = get_sonar_value();
  if (a <= 10) {
    Serial.println("Obstruction detected.");
    f_Stop();
    scan_area();
  }
  else {
    forward(a);
    f_Stop();
  }
  delay(250);
}

void forward(int duration) {
  Serial.print("Going forward for ");
  Serial.println(duration * 10); //for debugging
  right_wheels.Forward();
  left_wheels.Forward();
  delay(duration * 6);
}

void f_Stop() {
  Serial.println("Stop mode.");
  right_wheels.Stop();
  left_wheels.Stop();
  delay(250);
}

long get_sonar_value() {
  for (int i = 0; i < 10; i++) {
    int a = sonar_1.Check();
    sonar_values[i] = a;
    delay(10);
  }
  int count = sizeof(sonar_values) / sizeof(sonar_values[0]);
  int index = 0 ;
  for (int i = 1; i < count; i++) {
    if (sonar_values[i] < sonar_values[index]) {
      index = i;
    }
  }
  return sonar_values[index];
}

void look_left() {
  right_wheels.Forward();
  left_wheels.Reverse();
  delay(200);
  f_Stop();
}
void look_right() {
  right_wheels.Reverse();
  left_wheels.Forward();
  delay(200);
  f_Stop();
}

void reverse() {
  right_wheels.Reverse();
  left_wheels.Reverse();
  delay(200);
}

void turn_180() {
  right_wheels.Reverse();
  left_wheels.Forward();
  delay(600);
}

long scan_area() {
  reverse();
  f_Stop();
  long current = get_sonar_value();
  look_left();
  Serial.println("Scanning left....");
  long left = get_sonar_value();
  look_right();
  look_right();
  Serial.println("Scanning right....");
  long right = get_sonar_value();
  look_left();
  if ((left > right) && (left > current) && (left > 15)) {
    look_left();
  }
  else if ((right > left) && (right > current) && (right > 15)) {
    look_right();
  }
  else {
    turn_180();
    f_Stop();
    delay(1000);
  }
}
