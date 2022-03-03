
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include "./WIFIsettings.cpp"

#include <Adafruit_NeoPixel.h>
#define PIN 2

//#define numLED 100
int numLED = 10;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLED, PIN, NEO_GRB + NEO_KHZ800);

WiFiUDP UDP;
char UDP_packet[600];


String host = "http://192.168.4.1";

void setup()
{
  delay(1000);

  //Serial.begin(115200);

  WIFIconnect();
  //Serial.print("My IP-Adress is:  ");
  //Serial.println(WiFi.localIP());

  strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();
  numLED =clientAktion(host+"/numLED").toInt();
  //Serial.print("numLED:  ");
  //Serial.println(numLED);
  strip.updateLength(numLED);
  strip.clear();
  strip.show();
  UDP.begin(UDP_PORT);
  //Serial.print("Listening on UDP port ");
  //Serial.println(UDP_PORT);
}

void loop()
{
  if (checkUDP()){
    ReadLEDs(UDP_packet);
    strip.show();
  }
}

void WIFIconnect(){ //connect to the master WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(APssid, APpsk);
byte trys=0;
  while (WiFi.status() != WL_CONNECTED)
  {
    trys++;
    delay(500);
    //Serial.print(".");
    if (trys>=30){
      //Serial.println("RESTART...");
      ESP.restart();
    }
  }
}

String clientAktion(String address) { //open Website and return its content
  WiFiClient client;

  HTTPClient http;
  String payload;
  //Serial.print("[HTTP] begin...\n");
  if (http.begin(client, address))
  { // HTTP

    //Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0)
    {
      // HTTP header has been send and Server response header has been handled
      //Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        payload = http.getString();
        //Serial.println(payload);
        //ReadLEDs(payload);
        
      }
    }
    else
    {
      //Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      WiFi.mode(WIFI_OFF);
      WIFIconnect();
    }
    
    http.end();
    return payload;
  }
}

void ReadLEDs(String line) { //take the string of values and set the Pixel Colour
//Serial.println(line);
  for (int i = 0; i < numLED; i++) {
    //char colorHEX[6] = {line.charAt(5 + i * 7 + 0) , line.charAt(5 + i * 7 + 1) , line.charAt(5 + i * 7 + 2) , line.charAt(5 + i * 7 + 3) , line.charAt(5 + i * 7 + 4) , line.charAt(5 + i * 7 + 5)};
    char colorHEX[6];
    for (int j=0;j<6;j++){
      colorHEX[j] = line.charAt(i * 6 + j);
    }
    
    uint32_t color = StrToHex(colorHEX);
    //Serial.printf("color in int: %d",color);
    if(color==0){
      //Serial.print(i);
    }
    strip.setPixelColor(i, color);
  }
  //Serial.printf("color in int: %d,%x\n", strip.getPixelColor(0), strip.getPixelColor(0));
}

int StrToHex(char str[])
{
  return (int) strtol(str, 0, 16);
}

bool checkUDP(){ //überprüfen ob ein UDP Packet vorhanden ist und es im Monitor ausgeben
  int packetSize = UDP.parsePacket();
  if (packetSize)
  {
    //Serial.print("Received packet! Size: ");
    //Serial.println(packetSize);
    int len = UDP.read(UDP_packet, packetSize);
    if (len > 0)
    {
      UDP_packet[len] = '\0';
    }
    //Serial.print("Packet received: ");
    //Serial.println(UDP_packet);
    return 1; // Received data
  }
  return 0; // Dont received data
}
