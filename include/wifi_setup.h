#ifndef WIFI_H
#define WIFI_H

class IPAddress;

void connectToWifi(const char *ssid, const char *password);
void assignStaticIP(IPAddress ipAddr);

#endif
