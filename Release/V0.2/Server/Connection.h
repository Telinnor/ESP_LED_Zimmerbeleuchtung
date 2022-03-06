#pragma once

#include <Adafruit_NeoPixel.h>
#include <WiFiUdp.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <WebServer.h>
#endif

class Connection{
private:
    WiFiUDP UDP;
    Adafruit_NeoPixel *__ConnectionLEDStripe;
    bool ConnectionEnabled[10];
    unsigned long previousTime = 0;
    String iToString(u_int32_t value, int TargetLength);
    String iToHex(u_int32_t value, int TargetLength);
    String createLEDString(void);
    String checkUDP(void);
    void sendLEDData(IPAddress UDP_IP, uint16_t UDP_Port);
    void sendUDP(IPAddress UDP_IP, uint16_t UDP_Port, String message);

public:
    void beginConnection(Adafruit_NeoPixel *LED_strip);
    void handleConnection(void);

};
