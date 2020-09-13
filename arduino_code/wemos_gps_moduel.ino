#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#ifndef STASSID
#define STASSID "SSID"
#define STAPSK  "PWD"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

static const int RXPin = D6, TXPin = D5;
static const uint32_t GPSBaud = 9600;

//Replace the ngrok url here
const char* host = "http://410d5092123d.ngrok.io";
const uint16_t port = 80;

String assetnumber = "1245645";


TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
HTTPClient http;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  ss.begin(GPSBaud);

}


void loop()
{

  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }

 if(client.connected() && ss.available() > 0){
    while (ss.available() > 0)
      if (gps.encode(ss.read())){
        if (gps.location.isValid()){
      
          double latitude = (gps.location.lat());
          double longitude = (gps.location.lng());
          String json;
          
          String latbuf;
          latbuf += (String(latitude, 6));
          Serial.println(latbuf);
      
          String lonbuf;
          lonbuf += (String(longitude, 6));
          Serial.println(lonbuf);
      
          DynamicJsonDocument doc(200);
 
          doc["assetnumber"] = assetnumber;
          doc["latitude"] = latbuf;
          doc["longitude"] = lonbuf;

          
          serializeJson(doc, json);
          Serial.println(json);
          //Replace the ngrok url here
          http.begin("http://410d5092123d.ngrok.io/addgpsdata");      //Specify request destination
          http.addHeader("Content-Type", "application/json");
          int httpCode = http.POST(json);   //Send the request
          String payload = http.getString();                                        //Get the response payload
 
          Serial.println(httpCode);   //Print HTTP return code
          Serial.println(payload);    //Print request response payload
 
          http.end(); 
          delay(10000);
        }
      
        else{
          Serial.println("GPS data not valid");
        }
      }
  }
  
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  
}
