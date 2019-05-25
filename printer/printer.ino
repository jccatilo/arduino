#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

String ssid;
String password;
bool APMode=false;

bool printing = false;
IPAddress ip;
IPAddress gateway;
IPAddress subnet;

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/

const int eepromIPAddr[] PROGMEM = {0,1,2,3};
const int eepromSubnetAddr[] PROGMEM = {4,5,6,7};
const int eepromGatewayAddr[] PROGMEM = {8,9,10,11};
const int eepromBaudRateAddr[] PROGMEM = {12,13};
const int eepromSSIDAddr[] PROGMEM = 
{14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};
const int eepromWifiPassAddr[] PROGMEM = 
{30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45};

const char WIFIConfig[] PROGMEM = 
"<!DOCTYPE html>"
"<html>"
"<head>"

"</head>"
"<body>"
"<style>"
"div{display:inline-block}"
"body{text-align: center;background: antiquewhite}"
"</style>"
"<h1> WIFI CONFIGURATION! </h1>"
"<h2>#</h2>"
"<form action=\"/config\" method=\"post\">"
"<div><h4>IP Address: </h4></div> <input name='ip' value=\"#\"/><br/>"
"<div><h4>Subnet Mask: </h4></div> <input name='subnet' value=\"#\"/><br/>"
"<div><h4>Default Gateway: </h4></div> <input name='gateway' value=\"#\"/><br/>"
"<div><h4>TTL Baud Rate: </h4></div> <input name='baudrate' value=\"#\"/><br/>"
"<div><h4>SSID: </h4></div> <input name='ssid' value=\"#\"/><br/>"
"<div><h4>Wifi Password: </h4></div> <input name='wifipass' value=\"#\"/><br/>"
"<div><input type=\"submit\" value=\"Save\" /><br/>"
"</form>"
"</body>"
"</html>";

void handleRoot() {
  EEPROM.begin(4096);
  String msg="";
  if (server.method() == HTTP_POST) {
    if (server.hasArg("ip")) {
      eepromSetIP(eepromIPAddr, server.arg("ip").c_str());
    }
    if (server.hasArg("subnet")) {
      eepromSetIP(eepromSubnetAddr, server.arg("subnet").c_str());
    }
    if (server.hasArg("gateway")) {
      eepromSetIP(eepromGatewayAddr, server.arg("gateway").c_str());
    }
    if (server.hasArg("baudrate")) {
      eepromSetUint16(eepromBaudRateAddr, atoi(server.arg("baudrate").c_str()));
    }
    if (server.hasArg("ssid")) {
      eepromSetString(eepromSSIDAddr, server.arg("ssid").c_str());
    }
    if (server.hasArg("wifipass")) {
      eepromSetString(eepromWifiPassAddr, server.arg("wifipass").c_str());
    }
    msg = "Configrations Saved. Please restart for confiration to work";
    eepromGetIPAddress(eepromIPAddr);
    eepromGetIPAddress(eepromGatewayAddr);
    eepromGetIPAddress(eepromSubnetAddr);
    ssid = eepromGetString(eepromSSIDAddr);
    password = eepromGetString(eepromWifiPassAddr);

    //connectWifi();
    EEPROM.commit();
  }
  int addr=0;
  char * pch;

  pch=strchr(WIFIConfig,'#');
  for (int i=0; i<pch-WIFIConfig; i++){
    server.sendContent(String(WIFIConfig[i]));
  }
  addr = pch-WIFIConfig;
  // Read the message send it
  server.sendContent(msg);
  
  pch=strchr(pch+1,'#');
  for (int i=addr+1; i<pch-WIFIConfig; i++){
    server.sendContent(String(WIFIConfig[i]));
  }
  addr = pch-WIFIConfig;
  
  // Read the IP from EEPROM and send it
  server.sendContent(eepromGetIP(eepromIPAddr));

  // Send more HTML
  pch=strchr(pch+1,'#');
  for (int i=addr+1; i<pch-WIFIConfig; i++){
    server.sendContent(String(WIFIConfig[i]));
  }
  addr = pch-WIFIConfig;
  
  // Read the IP from EEPROM and send it
  server.sendContent(eepromGetIP(eepromSubnetAddr));

  pch=strchr(pch+1,'#');
  for (int i=addr+1; i<pch-WIFIConfig; i++){
    server.sendContent(String(WIFIConfig[i]));
  }
  addr = pch-WIFIConfig;

  // Read the IP from EEPROM and send it
  server.sendContent(eepromGetIP(eepromGatewayAddr));

  pch=strchr(pch+1,'#');
  for (int i=addr+1; i<pch-WIFIConfig; i++){
    server.sendContent(String(WIFIConfig[i]));
  }
  addr = pch-WIFIConfig;

  // Read the IP from EEPROM and send it
  uint16 baudRate = eepromGetUint16(eepromBaudRateAddr);
  server.sendContent(String(baudRate));

  pch=strchr(pch+1,'#');
  for (int i=addr+1; i<pch-WIFIConfig; i++){
    server.sendContent(String(WIFIConfig[i]));
  }
  addr = pch-WIFIConfig;

  // Read the IP from EEPROM and send it
  server.sendContent(eepromGetString(eepromSSIDAddr));

  pch=strchr(pch+1,'#');
  for (int i=addr+1; i<pch-WIFIConfig; i++){
    server.sendContent(String(WIFIConfig[i]));
  }
  addr = pch-WIFIConfig;

  // Read the IP from EEPROM and send it
  server.sendContent(eepromGetString(eepromWifiPassAddr));

  for (int i=addr+1; i<sizeof(WIFIConfig); i++){
    server.sendContent(String(WIFIConfig[i]));
  }
  EEPROM.end();
}

void setup() {
  delay(1000);
  Serial.begin(19200);
  EEPROM.begin(4096);

  uint32 address=0;
  address = eepromGetIPAddress(eepromIPAddr);
  ip = IPAddress(byte(address>>24), byte(address>>16), byte(address>>8), byte(address));
  address = eepromGetIPAddress(eepromGatewayAddr);
  gateway = IPAddress(byte(address>>24), byte(address>>16), byte(address>>8), byte(address));
  address = eepromGetIPAddress(eepromSubnetAddr);
  subnet = IPAddress(byte(address>>24), byte(address>>16), byte(address>>8), byte(address));
  
  ssid = eepromGetString(eepromSSIDAddr);
  password = eepromGetString(eepromWifiPassAddr);

  EEPROM.end();
  
  if (ssid == "") {
    AP();
  } else {
    connectWifi();
    if (WiFi.status() != WL_CONNECTED) {
      AP();
    }
  }

  server.on("/config", handleRoot);
  server.on("/print/", handlePrint);
  server.begin();
}

int errCount=0;

void loop() {
  if (!APMode) {
    if (WiFi.status() != WL_CONNECTED) {
      errCount++;
      if (errCount == 10) {
        Serial.println("######################");
        Serial.println("######################");
        Serial.println("Unable to connect to WIFI");
        Serial.println("Please contact tech support");
        Serial.println("######################");
        Serial.println("######################");
        Serial.println("");
        Serial.println("");
        AP();
      } else if (errCount < 10){
        Serial.println("WIFI connection lost");
        Serial.println("");
        Serial.println("");
        connectWifi();
      }
    } else {
//      bool ret = Ping.ping(gateway, 3);
//      if (!ret){
//        Serial.println("Gateway unreachable");
//        Serial.println("");
//        Serial.println("");
//        connectWifi();
//      } else {
//        errCount = 0;
//      }
      errCount = 0;
    }
  }
  server.handleClient();
}

void AP() {
  WiFi.disconnect();
  ssid = "Printer Wifi";
  password = "";
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("Printer started in AP mode. Please contact tech support");
  Serial.print("Connect to AP (");
  Serial.print(ssid);
  Serial.println(")");
  Serial.println("Browse to http://192.168.4.1/config/");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  APMode = true;
}

void connectWifi(){
  APMode=false;
  WiFi.disconnect();
  WiFi.config(ip, gateway, subnet);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  for (int i=0; i<10; i++){
    if (WiFi.status() != WL_CONNECTED) {
      delay(1000);
    } else {
      Serial.println("WIFI connection is OK");
      Serial.print("IP: ");
      Serial.println(ip);
      Serial.print("Gateway: ");
      Serial.println(gateway);
      Serial.print("Subnet Mask: ");
      Serial.println(subnet);
      Serial.println("");
      Serial.println("");
      return;
    }
  }
  Serial.println("Unable to connect to Wifi");
  Serial.println("");
  Serial.println("");
}

uint16 eepromGetUint16(const int addr[]) {
  int lsb = EEPROM.read(addr[0]);
  int msb = EEPROM.read(addr[1]);
  uint16 val = lsb;
  val += msb<<8;
  return val;
}


void eepromSetUint16(const int addr[], uint16 val) {
  EEPROM.write(addr[0], (val<<8)>>8);
  EEPROM.write(addr[1], val>>8);
}

String eepromGetIP(const int addr[]) {
  String val = String(EEPROM.read(addr[0]));
  val += "." + String(EEPROM.read(addr[1]));
  val += "." + String(EEPROM.read(addr[2]));
  val += "." + String(EEPROM.read(addr[3]));
  return val;
}

uint32 eepromGetIPAddress(const int addr[]) {
  
  byte o1 = EEPROM.read(addr[0]);
  byte o2 = EEPROM.read(addr[1]);
  byte o3 = EEPROM.read(addr[2]);
  byte o4 = EEPROM.read(addr[3]);

  uint32 val = (o1<<24) + (o2<<16) + (o3<<8) + (o4);
  return val;
}

void eepromSetIP(const int addr[], const char val[]) {
  int i=0;
  char * pch;
  pch = strtok ((char*)val,".");
  while (pch != NULL)
  {
    EEPROM.write(addr[i], atoi(pch));
    i++;
    pch = strtok (NULL, ".");
  }
}

String eepromGetString(const int addr[]) {
  String val="";
  char b;
  for (int i=0; i<16; i++) {
    b = EEPROM.read(addr[i]);
    if (b == 255 || b == 0) {
      return val;
    }
    val += String(b);
  }
  return val;
}

void eepromSetString(const int addr[], String val) {
  for (int i=0; i<16; i++) {
    if (i > 16) {
      EEPROM.write(addr[i], 255);
    } else {
      EEPROM.write(addr[i], val[i]);
    }
  }
}


#include <avr/pgmspace.h>

const char PROGMEM b64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

/* 'Private' declarations */
inline void a3_to_a4(unsigned char * a4, unsigned char * a3);
inline void a4_to_a3(unsigned char * a3, unsigned char * a4);
inline unsigned char b64_lookup(char c);

int base64_encode(char *output, char *input, int inputLen) {
  int i = 0, j = 0;
  int encLen = 0;
  unsigned char a3[3];
  unsigned char a4[4];

  while(inputLen--) {
    a3[i++] = *(input++);
    if(i == 3) {
      a3_to_a4(a4, a3);

      for(i = 0; i < 4; i++) {
        output[encLen++] = pgm_read_byte(&b64_alphabet[a4[i]]);
      }

      i = 0;
    }
  }

  if(i) {
    for(j = i; j < 3; j++) {
      a3[j] = '\0';
    }

    a3_to_a4(a4, a3);

    for(j = 0; j < i + 1; j++) {
      output[encLen++] = pgm_read_byte(&b64_alphabet[a4[j]]);
    }

    while((i++ < 3)) {
      output[encLen++] = '=';
    }
  }
  output[encLen] = '\0';
  return encLen;
}

int base64_decode(char * output, char * input, int inputLen) {
  int i = 0, j = 0;
  int decLen = 0;
  unsigned char a3[3];
  unsigned char a4[4];


  while (inputLen--) {
    if(*input == '=') {
      break;
    }

    a4[i++] = *(input++);
    if (i == 4) {
      for (i = 0; i <4; i++) {
        a4[i] = b64_lookup(a4[i]);
      }

      a4_to_a3(a3,a4);

      for (i = 0; i < 3; i++) {
        output[decLen++] = a3[i];
      }
      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 4; j++) {
      a4[j] = '\0';
    }

    for (j = 0; j <4; j++) {
      a4[j] = b64_lookup(a4[j]);
    }

    a4_to_a3(a3,a4);

    for (j = 0; j < i - 1; j++) {
      output[decLen++] = a3[j];
    }
  }
  output[decLen] = '\0';
  return decLen;
}

int base64_enc_len(int plainLen) {
  int n = plainLen;
  return (n + 2 - ((n + 2) % 3)) / 3 * 4;
}

int base64_dec_len(char * input, int inputLen) {
  int i = 0;
  int numEq = 0;
  for(i = inputLen - 1; input[i] == '='; i--) {
    numEq++;
  }

  return ((6 * inputLen) / 8) - numEq;
}

inline void a3_to_a4(unsigned char * a4, unsigned char * a3) {
  a4[0] = (a3[0] & 0xfc) >> 2;
  a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
  a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
  a4[3] = (a3[2] & 0x3f);
}

inline void a4_to_a3(unsigned char * a3, unsigned char * a4) {
  a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
  a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
  a3[2] = ((a4[2] & 0x3) << 6) + a4[3];
}

inline unsigned char b64_lookup(char c) {
  if(c >='A' && c <='Z') return c - 'A';
  if(c >='a' && c <='z') return c - 71;
  if(c >='0' && c <='9') return c + 4;
  if(c == '+') return 62;
  if(c == '/') return 63;
  return -1;
}

void handlePrint() {
  while (printing){
    
  }
  printing = true;
  if (server.hasArg("data")){ //Check if body received
    String data = server.arg("data");
    //Serial.println(data);
    server.send(200,"application/json","{'status':'OK'}");
    int input2Len = data.length();
    char input2[input2Len];
    
    data.toCharArray(input2, input2Len+1);
  
    int decodedLen = base64_dec_len(input2, input2Len);
    char decoded[decodedLen];

    byte decodedByte[decodedLen];
    
    base64_decode(decoded, input2, input2Len);
    //Serial.print(decoded);
    for(int i=0; i<decodedLen; i++){
      //decodedByte[i] = byte(decoded[i]);
      Serial.print(decoded[i]);
    }
    printing = false;

    //Serial.println(decodedByte);
    return;
  }
  server.send(200,"text/plain","err");
}
