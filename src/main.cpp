#include "Arduino.h"
#include "MathService.h"
#include "LampService.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

//Wifi variables and objects
const char* _wifiName = "wifiName";
const char* _wifiPassword = "wifiPassword";
ESP8266WebServer server(80);
void restServerRouting();
void handleNotFound();
void connectToWiFi();

//Services
MathService mathService;
LampService lampService;

#define lampGPIO D7


void setup() 
{
  Serial.begin(9600);
  connectToWiFi();
  pinMode(lampGPIO, OUTPUT);
  lampService.TurnLampOff(lampGPIO);
}

void loop() 
{
  server.handleClient();
}


//  ------------ API ------------

void TurnOnLamp()
{
  lampService.TurnLampOn(lampGPIO);
  server.send(200);
}

void TurnOffLamp()
{
  lampService.TurnLampOff(lampGPIO);
  server.send(200);
}

void TurnOffLampDelayed()
{

  String arg = "delay";

  int delaySeconds = server.arg(arg).toInt();

  if(delay == 0)
  {
    server.send(400, "text/json", "Missing argument: " + arg);
  }

  unsigned int currentTime = mathService.ConvertMillisToSeconds(millis());
  unsigned int timePassed = currentTime;

  while(timePassed - currentTime < delaySeconds)
  {
    timePassed = mathService.ConvertMillisToSeconds(millis());
    delay(15); //Avoid watchdog in ESP8266 12E

    if(timePassed < currentTime)
    {
      server.send(500, "text/json", "System reset...");
      return;
    }
  }

  lampService.TurnLampOff(lampGPIO);

  server.send(200);

}

// Define routing
void restServerRouting() 
{
    server.on(F("/turn-lamp-on"), HTTP_PUT, TurnOnLamp);
    server.on(F("/turn-lamp-off"), HTTP_PUT, TurnOffLamp);
    server.on(F("/turn-lamp-off-delayed"), HTTP_PUT, TurnOffLampDelayed);

}

// Manage not found URL
void handleNotFound() 
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) 
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

void connectToWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(_wifiName, _wifiPassword);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(_wifiName);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  // Activate mDNS this is used to be able to connect to the server
  // with local DNS hostmane esp8266.local
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
 
  // Set server routing
  restServerRouting();
  // Set not found response
  server.onNotFound(handleNotFound);
  // Start server
  server.begin();
  Serial.println("HTTP server started");
}
 
