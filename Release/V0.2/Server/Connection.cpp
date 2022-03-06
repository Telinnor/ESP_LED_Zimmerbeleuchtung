#include <WiFiClient.h>
#include <WiFiUdp.h>
#include "./WIFIsettings.cpp"
#include "Connection.h"
#include <Adafruit_NeoPixel.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <WebServer.h>
#endif

void Connection::beginConnection(Adafruit_NeoPixel* LED_strip)
{
    Serial.println("startConnection");
    delay(300);
    Serial.println("---------------------");
    Serial.println("Version: V0.2  Server");
    Serial.println("https://github.com/Telinnor/ESP_LED_Zimmerbeleuchtung");
    Serial.println("---------------------");

    __ConnectionLEDStripe = LED_strip;


    WiFi.mode(WIFI_STA);
    WiFi.begin(HOMEssid, HOMEpsk);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("My IP-Adress is:  ");
    Serial.println(WiFi.localIP());

    // Start the AccesPoint
    Serial.println();
    Serial.print("Configuring access point...");
    WiFi.softAP(APssid, APpsk);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);


    Serial.println("HTTP server started");

    UDP.begin(UDP_PORT);
    Serial.print("Listening on UDP port ");
    Serial.println(UDP_PORT);


    Serial.println("finshed setup");
    Serial.println((*__ConnectionLEDStripe).numPixels());
}

void Connection::handleConnection()
{
    //Serial.println("Check1");
    //Serial.println(checkUDP());
    switch ((int)checkUDP()[0])
    {
    case 65:    //A: zeige Zustand LEDs
        Serial.println("resend Package");
        sendLEDData(UDP.remoteIP(), UDP.remotePort());
        break;
    case 49:  //1:  nenne Pixel von Strip 1
        sendUDP(UDP.remoteIP(), UDP.remotePort(),(String)(*__ConnectionLEDStripe).numPixels());
        ConnectionEnabled[1]=1;
        Serial.println("numLED1");
        break;
    case 2:
        break;
    default:
        // do something
        break;
    }

    //Serial.println("Check2");
    //server.handleClient();
    //Serial.println("Check3");
    //ArduinoOTA.handle(); // OTA Upload via ArduinoIDE

    if (millis() -previousTime > 10 && ConnectionEnabled[1]==1)
    {
       previousTime = millis();
       //Serial.println("Check4");
       sendLEDData(IPAddress(192, 168, 4, 2), 4210);
        //Serial.println("Send Data");
    }
}

String Connection::iToString(u_int32_t value, int TargetLength)
{

    String result;
    String valueString=(String) value;
    int lengthString=valueString.length();
    //int lengthString=sizeof(valueString)/sizeof(valueString[0]);
    // Serial.println(valueString);
    // Serial.println(valueString.length());
    // Serial.println(valueString[0]);
    // Serial.println(sizeof(valueString[0]));
    // Serial.println(lengthString);
    for (int i = 0;i<(TargetLength-lengthString);i++){
        result+=String("0");
    }
    
    result+=valueString;
    Serial.println(result);
    return result;
}

String Connection::iToHex(u_int32_t value, int TargetLength)
{
    String result;
    String valueString=String(value,HEX);
    for (int i=valueString.length(); i<TargetLength;i++){
        result += String("0");
    }
    result+=valueString;
    return result;
}

String Connection::createLEDString()
{ // erstellt einen langen String mit der Farbe aller einzelnen Strings im Format RRGGBB in Hex angezeigt
    String message;
    for (int i = 0; i < (*__ConnectionLEDStripe).numPixels(); i++)
    {
        // u_int32_t color = (*__ConnectionLEDStripe).getPixelColor(i);
        u_int8_t *color = (*__ConnectionLEDStripe).getPixels();
        uint32_t colorCombined = ((uint32_t)color[i * 3 + 0] << 16) | ((uint32_t)color[i * 3 + 1] << 8) | (uint32_t)color[i * 3 + 2];
        // message+=iToString(color,10)+"|";
        message += iToHex(colorCombined, 6) + "";

        // Serial.println(message);
    }
    return message;
}

String Connection::checkUDP(void)
{ //überprüfen ob ein UDP Packet vorhanden ist und es im Monitor ausgeben
    char packet[255];
    int packetSize = UDP.parsePacket();
    if (packetSize)
    {
        Serial.print("Received packet! Size: ");
        Serial.println(packetSize);
        int len = UDP.read(packet, 255);
        if (len > 0)
        {
            packet[len] = '\0';
        }
        Serial.print("Packet received: ");
        Serial.println(packet);
        return (String)packet; // Received data
    }
    return "nothing"; //Dont received data
}

void Connection::sendLEDData(IPAddress UDP_IP, uint16_t UDP_Port)
{
   sendUDP(UDP_IP, UDP_Port, createLEDString());
}

void Connection::sendUDP(IPAddress UDP_IP, uint16_t UDP_Port, String message)
{ // Sendet eine UDP Nachricht an IP und Port
    UDP.beginPacket(UDP_IP, UDP_Port);
    //UDP.write("message received");
    UDP.write(message.c_str());
    UDP.endPacket();
    //Serial.println(millis());
}

