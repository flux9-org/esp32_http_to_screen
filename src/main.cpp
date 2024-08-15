#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WebServer.h>
#include <WiFi.h>

#include "WiFiType.h"

TFT_eSPI tft = TFT_eSPI();
WebServer server(80);

IPAddress staticIP(192, 168, 1, 45);
IPAddress primaryDNS(192, 168, 1, 1); // Primary DNS (optional)
IPAddress secondaryDNS(0, 0, 0, 0);   // Secondary DNS (optional)

const char *ENDPOINT = "/data";

const char *SSID = "";          // TODO
const char *WIFI_PASSWORD = ""; // TODO

uint16_t BACKGROUND = tft.color565(60, 115, 180);

void header(const char *string) {
  tft.setTextSize(3);
  tft.setTextColor(tft.color565(255, 255, 255), BACKGROUND);

  // If you don't refill the screen old text can stay
  tft.fillRect(0, 0, tft.width(), tft.height(), BACKGROUND);

  tft.setTextDatum(TC_DATUM);
  tft.setCursor(10, 20);
  tft.print(string);
}

void handleRoot() { server.send(200, "text/plain", "Hello from ESP32!"); }

void handleData() {
  if (server.args() > 0) {
    for (int i = 0; i < server.args(); i++) {
      if (server.argName(i) == "headerValue") {
        String headerValue = server.arg(i);
        Serial.println("Received data: " + headerValue);
        header(headerValue.c_str());
        server.send(200, "text/plain", "Data received");
        return;
      }
    }
  }
  server.send(400, "text/plain", "No valid data received");
}

void setup() {
  Serial.begin(9600);
  Serial.println("Setup start");

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, WIFI_PASSWORD);
  Serial.println("\nConnecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("\nConnected to the WiFi network");
  delay(2000);

  if (WiFi.config(staticIP, WiFi.gatewayIP(), WiFi.subnetMask(), primaryDNS,
                  secondaryDNS)) {
    Serial.println("Static IP successfully configured");
  } else {
    Serial.println("Static IP configuration failed");
  }

  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on(ENDPOINT, handleData);
  server.begin();
  Serial.println("HTTP server started");

  tft.init();
  tft.invertDisplay(0);
  tft.fillScreen(tft.color565(0, 0, 0));
  Serial.println("TFT initialized");

  header("Some Text");
}

void loop() { server.handleClient(); }
