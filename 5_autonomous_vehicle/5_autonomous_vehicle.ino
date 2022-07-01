#include "my_classes.h"

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
