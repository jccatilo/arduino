
#include <Servo.h>

//Declare Servos

Servo leftservo;     //Left wheel servo

Servo rightservo;    //Right wheel servo

Servo scanservo;     //Ping Sensor Servo

const int turntime = 500;  //Number of milliseconds to turn when turning

const int pingPin = 7;     //Pin that the Ping sensor is attached to.

const int leftservopin = 9; //Pin number for left servo

const int rightservopin = 10; // Pin number for right servo

const int scanservopin = 6;   // Pin number for scan servo

const int distancelimit = 10;   //If something gets this many inched from

                                 // the robot it stops and looks for where to go.

//Setup function. Runs once when Arduino is turned on or restarted

void setup()

{

  leftservo.attach(leftservopin); //Attach left servo to its pin.

  rightservo.attach(rightservopin); // Attch the right servo

  scanservo.attach(scanservopin); // Attach the scan servo

  delay(2000);        // wait two seconds

}

void loop(){

  go();  // if nothing is wrong the go forward using go() function below.

  int distance = ping(); // us the ping() function to see if anything is ahead.

  if (distance < distancelimit){

    stopmotors(); // If something is ahead, stop the motors.

    char turndirection = scan(); //Decide which direction to turn.

    switch (turndirection){

      case 'l':

        turnleft(turntime);

        break;

      case 'r':

        turnright(turntime);

        break;

      case 's':

        turnleft(turntime);

        break;

    }

  }

}

 

int ping(){

  long duration, inches, cm;

  //Send Pulse

  pinMode(pingPin, OUTPUT);

  digitalWrite(pingPin, LOW);

  delayMicroseconds(2);

  digitalWrite(pingPin, HIGH);

  delayMicroseconds(5);

  digitalWrite(pingPin, LOW);

  //Read Echo

  pinMode(pingPin, INPUT);

  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance

  inches = microsecondsToInches(duration);

  cm = microsecondsToCentimeters(duration);

  Serial.print("Ping:  ");

  Serial.println(inches);

  return round(inches);

}

void go(){

  leftservo.write(30);

  rightservo.write(150);

}

void turnleft(int t){

  leftservo.write(150);

  rightservo.write(150);

  delay(t);

}

   

void turnright(int t){

  leftservo.write(30);

  rightservo.write(30);

  delay(t);

}      

void forward(int t){

  leftservo.write(30);

  rightservo.write(150);

  delay(t);

}

void backward(int t){

  leftservo.write(150);

  rightservo.write(30);

  delay(t);

}

void stopmotors(){

  leftservo.write(90);

  rightservo.write(90);

}      

 

char scan(){

  int leftscanval, centerscanval, rightscanval;

  char choice;

  //Look left

  scanservo.write(30);

  delay(300);

  leftscanval = ping();

  //Look right

  scanservo.write(150);

  delay(1000);

  rightscanval = ping();

  //center scan servo

  scanservo.write(88);

 

  if (leftscanval>rightscanval && leftscanval>centerscanval){

    choice = 'l';

  }

  else if (rightscanval>leftscanval && rightscanval>centerscanval){

    choice = 'r';

  }

  else{

    choice = 's';

  }

  Serial.print("Choice:  ");

  Serial.println(choice);

  return choice;

}

long microsecondsToInches(long microseconds){

  return microseconds / 74 / 2;

}

long microsecondsToCentimeters(long microseconds){

  return microseconds / 29 / 2;

}  


