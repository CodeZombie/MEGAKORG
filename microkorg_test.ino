#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "SPI.h"
#include "webpages.h"

#include "multiplexercontroller.h"
#include "digipot.h"
#include "bankcontroller.h"
#include "microkorg.h"


ESP8266WebServer server(80);

Microkorg *microkorg;
byte mkChipSelect = 5;
byte mkSel_0 = 4;
byte mkSel_1 = 0;
byte mkSel_2 = 2;

byte test_bank = 0;

void setup() {
  Serial.begin(115200);

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  
  server.on("/", handleRoot);
  server.on("/changeParam", changeParam);
  server.begin();
  
  //HARDWARE SPI:
  SPI.begin();
  SPI.setFrequency(8000000); //8Mhz
  
  microkorg = new Microkorg(mkSel_0, mkSel_1, mkSel_2, mkChipSelect);
}

void loop() {
  server.handleClient(); 
}

void handleRoot() {
  server.send(200, "text/html", Resources::index_html);
}

void changeParam() {
  String parameter = server.arg("parameter");
  String value = server.arg("value");

  microkorg->setParameter(parameter.toInt(), value.toInt());

  server.send(200, "text/plain", "success");
}
