/*
  Web client

  This sketch connects to a website (http://www.google.com)
  using a WiFi shield.

  This example is written for a network using WPA encryption. For
  WEP or WPA, change the WiFi.begin() call accordingly.

  This example is written for a network using WPA encryption. For
  WEP or WPA, change the WiFi.begin() call accordingly.

  Circuit:
   WiFi shield attached

  created 13 July 2010
  by dlf (Metodo2 srl)
  modified 31 May 2012
  by Tom Igoe
*/


#include <SPI.h>
#include <WiFi101.h>
#include "arduino_secrets.h"
#include <PZEM004T.h>
#include "DHT.h"
#include <Scheduler.h>
#define DHTPIN 0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "Mail Hotspot";        // your network SSID (name)
char pass[] = "wfue0225";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(103, 10, 145, 6); // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;
PZEM004T* pzem;
IPAddress ipPower(192, 168, 1, 1);

///////////////////////////////////////////////////////////////////////////
//Variable to be exposed to the API
//unsigned long prevMillisTask1 = 0;
//long task1 = 5000;
float v, i, p, e, h, t;
//char c;

void setup() {

  Serial.begin(115200);
  pzem = new PZEM004T(&Serial1);
  pzem->setAddress(ipPower);
  dht.begin();

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  while (status != WL_CONNECTED) {
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(2000);
  }
    Scheduler.startLoop(loop2);
}

void loop() {
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
      v = pzem->voltage(ipPower);
      i = pzem->current(ipPower);
      p = pzem->power(ipPower);
      e = pzem->energy(ipPower);
      h = dht.readHumidity();
      t = dht.readTemperature();

      Serial.println("connected to server");
      // Make a HTTP request:
      client.println("GET /telkomsat_iot/index.php/postdata?id=TSAT01&voltage=" + String(v) + "&current=" + String(i) + "&power=" + String(p) + "&energy=" + String(e) + "&humidity=" + String(h) + "&temperature=" + String(t*0.85));
      client.println("Host: http://103.10.145.6");
      client.println("Connection: close");
      client.println();
      delay(2000);
    }
}

void loop2(){  
  if (Serial.available()) {
   char c = Serial.read();
    Serial.write(c);
    if (c == '0') {
      Serial.println("Led turned off!");
    }
    if (c == '1') {
      Serial.println("Led turned on!");
    }
  }
    yield();
  }








