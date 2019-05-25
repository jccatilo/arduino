#include <ESP8266WiFi.h>
const char WiFiPassword[] = "12345678";
const char AP_NameChar[] = "Engineer's door" ;
 
WiFiServer server(80);
 
String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
String html_1 = "<!DOCTYPE html><html><head><title>Engineer's Door Control</title></head><body><div id='main'><h2>Door Control</h2>";
String html_2 = "<form id='F1' action='LOCKOFF'><input class='button' type='submit' value='Unlock The Door' ></form><br>";
//String html_3 = "<form id='F2' action='LOCKOFF'><input class='button' type='submit' value='Close The Door' ></form><br>";
String html_4 = "</div></body></html>";
 
String request = "";
int LED_Pin = 4;
 
void setup() 
{
    pinMode(LED_Pin, OUTPUT); 
 
    boolean conn = WiFi.softAP(AP_NameChar, WiFiPassword);
    server.begin();
 
} // void setup()
 
 
void loop() 
{
 
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client)  {  return;  }
 
    // Read the first line of the request
    request = client.readStringUntil('\r');
 
    if  ( request.indexOf("LOCKOFF") > 0 ) { digitalWrite(LED_Pin, HIGH); delay(1500); digitalWrite(LED_Pin, LOW);  }
    else{}
    client.flush();
 
    client.print( header );
    client.print( html_1 );
    client.print( html_2 );
//    client.print( html_3 );
    client.print( html_4);
 
    delay(5);

 
} // void loop()
