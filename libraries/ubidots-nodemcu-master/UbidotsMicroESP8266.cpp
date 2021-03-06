/*
Copyright (c) 2013-2016 Ubidots.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Made by Mateo Velez - Metavix for Ubidots Inc
Modified by: Maria Carlina Hernandez

*/

#include "UbidotsMicroESP8266.h"
/**
 * Constructor.
 */
Ubidots::Ubidots(char* token, char* server) {
    _token = token;
    _server = server;
    _dsTag = "ESP8266";
    _dsName = "ESP8266";
    maxValues = 5;
    currentValue = 0;
    val = (Value *)malloc(maxValues*sizeof(Value));
}
void Ubidots::setDataSourceName(char *dataSourceName) {
    _dsName = dataSourceName;
}
void Ubidots::setDataSourceLabel(char *dataSourceLabel) {
    _dsTag = dataSourceLabel;
}
/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::getValue(char* id) {
  float num;
  String response;
  uint8_t bodyPosinit;
  uint8_t bodyPosend;
  if (_client.connect(HTTPSERVER, HTTPPORT)) {
        Serial.println(F("Getting your variable"));
        _client.print(F("GET /api/v1.6/variables/"));
        _client.print(id);
        _client.println(F("/values?page_size=1 HTTP/1.1"));
        _client.println(F("Host: things.ubidots.com"));
        _client.println(F("User-Agent: ESP8266/1.0"));
        _client.print(F("X-Auth-Token: "));
        _client.println(_token);
        _client.println(F("Connection: close"));
        _client.println();
  } else {
        return NULL;
  }
    int timeout = 0;
    while(!_client.available() && timeout < 5000) {
        timeout++;
        delay(1);
    }
    while (_client.available()) {
        response = _client.readString();
    }
    Serial.println(response);
    bodyPosinit = 4 + response.indexOf("\r\n\r\n");
    response = response.substring(bodyPosinit);
    Serial.println(response);
    bodyPosinit = 9 + response.indexOf("\"value\":");
    bodyPosend = response.indexOf(", \"timestamp\"");
    num = response.substring(bodyPosinit,bodyPosend).toFloat();
    _client.flush();
    _client.stop();
    return num;
}
/** 
 * This function is to get variable timestamp from the Ubidots API
 * @arg id is the ID of the variable where you will get the timestamp
 * @return VarTimestamp is the timestamp of the variable that you get from the Ubidots API
 */
long Ubidots::getVarTimestamp(char* id) {
  String response;
  String str_seconds;
  char *seconds;
  char *milis;
  long VarTimestamp;
  int bodyPosinit;
  int bodyPosend;
  if (_client.connect(HTTPSERVER, HTTPPORT)) {
        Serial.println(F("Getting your variable timestamp"));
        _client.print(F("GET /api/v1.6/variables/"));
        _client.print(id);
        _client.println(F("/values?page_size=1 HTTP/1.1"));
        _client.println(F("Host: things.ubidots.com"));
        _client.println(F("User-Agent: ESP8266/1.0"));
        _client.print(F("X-Auth-Token: "));
        _client.println(_token);
        _client.println(F("Connection: close"));
        _client.println();
  } else {
        return NULL;
  }
    int timeout = 0;
    while(!_client.available() && timeout < 5000) {
        timeout++;
        delay(1);
    }
    while (_client.available()) {
        response = _client.readString();
    }
    Serial.println(response);
    bodyPosinit = 4 + response.indexOf("\r\n\r\n");
    Serial.print(response);
    response = response.substring(bodyPosinit);
    Serial.println(response);
    bodyPosinit =13+response.indexOf("\"timestamp\":");
    bodyPosend = response.indexOf(", \"context\":");
    bodyPosend -= 3;
    str_seconds = response.substring(bodyPosinit,bodyPosend); 
    seconds = new char[str_seconds.length() + 1];
    strcpy(seconds, str_seconds.c_str());
    VarTimestamp = atoi(seconds);
    delete [] seconds;
    delay(1000);
    _client.flush();
    _client.stop();
    return VarTimestamp;
}
/** 
 * This function is to get variable context from the Ubidots API
 * @arg id is the ID of the variable where you will get the context
 * @return VarContext is the context of the variable that you get from the Ubidots API
 */
char* Ubidots::getVarContext(char* id) {
  String response;
  String str_context;
  char* VarContext;
  int bodyPosinit;
  int bodyPosend;
  if (_client.connect(HTTPSERVER, HTTPPORT)) {
        Serial.println(F("Getting your variable context"));
        _client.print(F("GET /api/v1.6/variables/"));
        _client.print(id);
        _client.println(F("/values?page_size=1 HTTP/1.1"));
        _client.println(F("Host: things.ubidots.com"));
        _client.println(F("User-Agent: ESP8266/1.0"));
        _client.print(F("X-Auth-Token: "));
        _client.println(_token);
        _client.println(F("Connection: close"));
        _client.println();
  } else {
        return NULL;
  }
    int timeout = 0;
    while(!_client.available() && timeout < 5000) {
        timeout++;
        delay(1);
    }
    while (_client.available()) {
        response = _client.readString();
    }
    Serial.println(response);
    bodyPosinit = 4 + response.indexOf("\r\n\r\n");
    Serial.print(response);
    response = response.substring(bodyPosinit);
    Serial.println(response);
    bodyPosinit =13+response.indexOf("\"context\":");
    bodyPosend = response.indexOf(", \"created_at\":");
    bodyPosend -= 1;
    str_context = response.substring(bodyPosinit,bodyPosend); 
    VarContext = new char [str_context.length() + 1];
    strcpy(VarContext, str_context.c_str());
    _client.flush();
    _client.stop();
    return VarContext;
}
/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */

void Ubidots::add(char *variable_id, float value) {
    return add(variable_id, value, NULL, NULL);
}
void Ubidots::add(char *variable_id, float value, char *ctext) {
    return add(variable_id, value, ctext, NULL);   
}
void Ubidots::add(char *variable_id, float value, unsigned long timestamp) {
    return add(variable_id, value, NULL, timestamp);
}
void Ubidots::add(char *variable_id, float value, char *ctext, unsigned long timestamp) {
    (val+currentValue)->id = variable_id;
    (val+currentValue)->value_id = value;
    (val+currentValue)->context = ctext;
    (val+currentValue)->timestamp = timestamp;
    currentValue++;
    if (currentValue>maxValues) {
        Serial.println(F("You are sending more than 5 consecutives variables, you just could send 5 variables. Then other variables will be deleted!"));
        currentValue = maxValues;
    }
}
/**
 * Send all data of all variables that you saved
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll(bool type) {
    if (type) {
        return sendTLATE();
    } else {
        return sendHTTP();
    }
}
bool Ubidots::sendTLATE() {
    uint8_t i;
    String str;
    char data[256];
    sprintf(data, "=>");
    for (i = 0; i < currentValue; i++) {
        str = String(((val+i)->value_id), 5);
        sprintf(data, "%s%s:%s", data, (val+i)->id, str.c_str());
        if ((val+currentValue)->timestamp != NULL) {
            sprintf(data, "%s@%s", data, (val+currentValue)->timestamp);
        }
        if ((val+currentValue)->context != NULL) {
            sprintf(data, "%s$%s", data, (val+i)->context);
        }
        sprintf(data, "%s,", data);
    }
    Serial.println(data);
    if (_client.connect(SERVER, PORT)) {
        Serial.println(F("POSTING YOUR VARIABLES"));
        _client.print(USER_AGENT);
        _client.print(F("/"));
        _client.print(VERSION);
        _client.print(F("|POST|"));
        _client.print(_token);
        _client.print(F("|"));
        _client.print(_dsTag);
        _client.print(F(":"));
        _client.print(_dsName);
        _client.print(data);
        _client.println(F("|end"));
    } else {
        Serial.println(F("Connection to server failed"));
        currentValue = 0;
        return false;
    }
    int timeout = 0;
    while(!_client.available() && timeout < 5000) {
        timeout++;
        delay(1);
    }
    while (_client.available()) {
        char c = _client.read();
        Serial.write(c);
    }
    currentValue = 0;
    return true;
}
bool Ubidots::sendHTTP() {
    uint16_t i;
    String all;
    String str;
    all = "[";
    for (i = 0; i < currentValue; ) {
        str = String(((val+i)->value_id), 4);
        all += "{\"variable\": \"";
        all += String((val + i)->id);
        all += "\", \"value\":";
        all += str;
        all += "}";
        i++;
        if (i < currentValue) {
            all += ", "; 
        }
    }
    all += "]";
    i = all.length();
	String toPost = "POST /api/v1.6/collections/values/?force=true HTTP/1.1\r\n"
					"Host: things.ubidots.com\r\n"
					"User-Agent: ESP8266/1.0\r\n"
					"X-Auth-Token: " + String(_token)	 + "\r\n"
					"Connection: close\r\n"
					"Content-Type: application/json\r\n"
					"Content-Length: " + String(i) + "\r\n"
					"\r\n"
					+ all +
					"\r\n";
	if (_client.connect(HTTPSERVER, HTTPPORT)) {
        Serial.println(F("Posting your variables"));
		Serial.println(toPost);
        _client.print(toPost);
    }
	
    int timeout = 0;
    while(!_client.available() && timeout < 5000) {
        timeout++;
        delay(1);
    }
    while (_client.available()) {
        char c = _client.read();
        Serial.write(c);
    }
    currentValue = 0;
    _client.stop();
    return true;
}
bool Ubidots::wifiConnection(char* ssid, char* pass) {
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    WiFi.setAutoReconnect(true);
    Serial.println(F("WiFi connected"));
    Serial.println(F("IP address: "));
    Serial.println(WiFi.localIP());
}


/*
 * ?? Francesco Potort?? 2013 - GPLv3 - Revision: 1.13
 *
 * Send an NTP packet and wait for the response, return the Unix time
 *
 * To lower the memory footprint, no buffers are allocated for sending
 * and receiving the NTP packets.  Four bytes of memory are allocated
 * for transmision, the rest is random garbage collected from the data
 * memory segment, and the received packet is read one byte at a time.
 * The Unix time is returned, that is, seconds from 1970-01-01T00:00.
 */
unsigned long Ubidots::ntpUnixTime () {
    static int udpInited = udp.begin(123); // open socket on arbitrary port

    // Only the first four bytes of an outgoing NTP packet need to be set
    // appropriately, the rest can be whatever.
    const long ntpFirstFourBytes = 0xEC0600E3; // NTP request header

    // Fail if WiFiUdp.begin() could not init a socket
    if (! udpInited)
        return 0;

    // Clear received data from possible stray received packets
    udp.flush();

    // Send an NTP request
    if (! (udp.beginPacket(TIME_SERVER, 123) // 123 is the NTP port
        && udp.write((byte *)&ntpFirstFourBytes, 48) == 48
        && udp.endPacket()))
            return 0;               // sending request failed

    // Wait for response; check every pollIntv ms up to maxPoll times
    const int pollIntv = 150;     // poll every this many ms
    const byte maxPoll = 15;      // poll up to this many times
    int pktLen;               // received packet length
    for (byte i = 0; i < maxPoll; i++) {
        if ((pktLen = udp.parsePacket()) == 48)
            break;
        delay(pollIntv);
    }
    if (pktLen != 48)
        return 0;               // no correct packet received

    // Read and discard the first useless bytes
    // Set useless to 32 for speed; set to 40 for accuracy.
    const byte useless = 40;
    for (byte i = 0; i < useless; ++i)
        udp.read();

    // Read the integer part of sending time
    unsigned long time = udp.read();  // NTP time
    for (byte i = 1; i < 4; i++)
        time = time << 8 | udp.read();

    // Round to the nearest second if we want accuracy
    // The fractionary part is the next byte divided by 256: if it is
    // greater than 500ms we round to the next second; we also account
    // for an assumed network delay of 50ms, and (0.5-0.05)*256=115;
    // additionally, we account for how much we delayed reading the packet
    // since its arrival, which we assume on average to be pollIntv/2.
    time += (udp.read() > 115 - pollIntv/8);
    // Discard the rest of the packet
    udp.flush();
    return time - 2208988800ul;       // convert NTP time to Unix time
}
