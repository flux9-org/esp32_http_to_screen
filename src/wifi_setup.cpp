#include <WiFi.h>

IPAddress primaryDNS(192, 168, 1, 1); // Primary DNS (optional)
IPAddress secondaryDNS(0, 0, 0, 0);   // Secondary DNS (optional)

void connectToWifi(const char *ssid, const char *password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  delay(5000);
  Serial.print("Connected to WiFi. Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void assignStaticIP(IPAddress ipAddr) {
  if (WiFi.config(ipAddr, WiFi.gatewayIP(), WiFi.subnetMask(), primaryDNS,
                  secondaryDNS)) {
    Serial.println("Static IP successfully configured");
  } else {
    Serial.println("Static IP configuration failed");
  }
}
