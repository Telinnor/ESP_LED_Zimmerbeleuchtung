
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

//IPAddress host= host.fromString("192.168.4.1");
IPAddress host = IPAddress(192, 168, 4, 1); // 192.168.004.00

void setup()
{
  pinMode(1,OUTPUT);
  delay(1000);

  Serial.begin(115200);
  Serial.println("startConnection");
  delay(300);
  Serial.println("---------------------");
  Serial.println("Version: V0.2  Client");
  Serial.println("https://github.com/Telinnor/ESP_LED_Zimmerbeleuchtung");
  Serial.println("---------------------");
  WIFIconnect();
  // Serial.print("My IP-Adress is:  ");
  // Serial.println(WiFi.localIP());

  strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();
  //numLED =clientAktion(host+"/numLED").toInt();

  UDP.begin(UDP_PORT);
  // Serial.print("Listening on UDP port ");
  // Serial.println(UDP_PORT);
  delay(300);
  sendUDP(host, UDP_PORT, "1");
  String num;
  while (1)
  {
    num = checkUDP();
    if (num != "0")
    {
      numLED = num.toInt();
      if (numLED>0)
        break;
    }
  }

  // Serial.print("numLED:  ");
  // Serial.println(numLED);
  strip.updateLength(numLED);
  strip.clear();
  strip.show();
}

void loop()
{
  WIFIcheck();

  if (checkUDP()[0]!=64)
  {
    digitalWrite(1, HIGH);
    ReadLEDs(UDP_packet);
    strip.show();
    digitalWrite(1, LOW);
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
    // Serial.print(".");
    if (trys>=30){
      // Serial.println("RESTART...");
      ESP.restart();
    }
  }
}

void WIFIcheck(){
  if (WiFi.status() != WL_CONNECTED){
    strip.clear();
    strip.show();
    delay(10);
    ESP.restart();
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
  // Serial.printf("color in int: %d,%x\n", strip.getPixelColor(0), strip.getPixelColor(0));
}

int StrToHex(char str[])
{
  return (int) strtol(str, 0, 16);
}

String checkUDP(void)
{ //überprüfen ob ein UDP Packet vorhanden ist und es im Monitor ausgeben
  int packetSize = UDP.parsePacket();
  if (packetSize)
  {
    // Serial.print("Received packet! Size: ");
    // Serial.println(packetSize);
    int len = UDP.read(UDP_packet, packetSize);
    if (len > 0)
    {
      UDP_packet[len] = '\0';
    }
    // Serial.print("Packet received: ");
    // Serial.println(UDP_packet);
    return (String)UDP_packet; // Received data
  }
  return "@"; // Dont received data
}

void sendUDP(IPAddress UDP_IP, uint16_t UDP_Port, String message)
{ // Sendet eine UDP Nachricht an IP und Port
  UDP.beginPacket(UDP_IP, UDP_Port);
  // UDP.write("message received");
  UDP.write(message.c_str());
  UDP.endPacket();
}
