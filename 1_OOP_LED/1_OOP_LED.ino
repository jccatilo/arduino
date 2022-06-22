// C++ code
//
class LED { // this will be the parent object name you created
    int pin; //this can be any variable name
  public:
    LED(int led_pin) {
      pin = led_pin;
      pinMode(pin, OUTPUT);// this line replaces the pinMode in setup() in the previous code
    }
    void Blink() { // this is a subclass which can be used by the objects under this parent object you will create
      digitalWrite(pin, HIGH);
      delay(500);
      digitalWrite(pin, LOW);
      delay(500);
    }
};

//Creating objects
// format: <Parent object name> <child object>(parameter/pin_number);
LED led_green(10);
LED led_yellow(11);
LED led_red(12);
LED led_blue(13);

void setup()
{
  //the instantiation and set up of pins are now done above.
}

void loop()
{
  // format: <child>.<subclass>(parameter)
  led_green.Blink(); //no parameter was passed in our curent case but we can utilize this in other projects
  led_yellow.Blink();
  led_red.Blink();
  led_blue.Blink();
}
