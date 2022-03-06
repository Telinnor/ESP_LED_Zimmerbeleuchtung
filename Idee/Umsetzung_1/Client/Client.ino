
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include "./WIFIsettings.cpp"

#include <Adafruit_NeoPixel.h>
#define PIN 2

//#define numLED 100
Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, PIN, NEO_GRB + NEO_KHZ800);

int numLED=20;
int waitingTime=10;

String host = "http://192.168.4.1";
String url = "/LED";
unsigned long previousTime = 0; //use for a delay without the delay() function

void setup()
{
  delay(1000);

  Serial.begin(115200);

  WIFIconnect();
  Serial.print("My IP-Adress is:  ");
  Serial.println(WiFi.localIP());

  strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();
  numLED =clientAktion(host+"/numLED").toInt();
  Serial.print("numLED:  ");
  Serial.println(numLED);
  strip.updateLength(numLED);
  

}

void loop()
{
  if (previousTime + waitingTime < millis()) {
    previousTime = millis();
    ReadLEDs(clientAktion(host+url));
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
    Serial.print(".");
    if (trys>=30){
      Serial.println("RESTART...");
      ESP.restart();
    }
  }
}

String clientAktion(String address) { //open Website and return its content
  WiFiClient client;

  HTTPClient http;
  String payload;
  Serial.print("[HTTP] begin...\n");
  if (http.begin(client, address))
  { // HTTP

    //Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0)
    {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

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
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      WiFi.mode(WIFI_OFF);
      WIFIconnect();
    }
    
    http.end();
    return payload;
  }
}

void ReadLEDs(String line) { //take the string of values and set the Pixel Colour
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
  Serial.printf("color in int: %d,%x\n", strip.getPixelColor(0), strip.getPixelColor(0));
}

int StrToHex(char str[])
{
  if(!isNumber(str)){
    return (int) strtol(str, 0, 16);
  }else{
    return 0;
  }
}
int isNumber(char s[])
{
  for (int i = 0; s[i] != '\0'; i++)
  {
    if (isdigit(s[i]) == 0){
      //Serial.printf("noINT: %s\n",s);
      return 0;
    }
  }
  return 1;
}
