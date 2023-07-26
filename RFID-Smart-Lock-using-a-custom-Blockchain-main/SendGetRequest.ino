

//old test code
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>


WiFiClient wificlient;
ESP8266WiFiMulti WiFiMulti;

String serverName = "http://192.168.137.1:5000/";
String addRoute = serverName+"add_card?cardNum=";
String verifyRoute = serverName+"verify_card?cardNum=";
void wificonnect();
void setup() {
    Serial.begin(115200); // speed of communication between laptop & microcontroller
    Serial.flush();
    pinMode(D0, OUTPUT); 
    wificonnect();         
}

void loop() {
  //GETsend("Hello world");
  sendAdd("123");
  sendVerify("123");
}

void wificonnect()
  {
    WiFiMulti.addAP("Laputopu", "8y;8H480");// Change this with your WiFi Credentials 
    Serial.println("Connecting");
    while(WiFiMulti.run()!=WL_CONNECTED)
    {
      Serial.print(".");
      digitalWrite(D0, LOW);
      delay(100);
      digitalWrite(D0, HIGH);
      delay(100);
    }
     Serial.print("Connected to HOTSPOT..\n");// This line will execute if your device is connected to Access Pass
     
  }
  void sendAdd(String cardNum){
      HTTPClient http;    //Declare object of class HTTPClient
      if (WiFi.status() == WL_CONNECTED) 
      { //Check WiFi connection status
        // Send and get Data
        http.begin(wificlient,addRoute+cardNum);
  
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
        
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
      }
      
      else 
      {
        Serial.println("Error in WiFi connection");
      }
      delay(16000); // interval
  }
  void sendVerify(String cardNum){
      HTTPClient http;    //Declare object of class HTTPClient
      if (WiFi.status() == WL_CONNECTED) 
      { //Check WiFi connection status
        // Send and get Data
        http.begin(wificlient,verifyRoute+cardNum);
  
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
        
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
      }
      
      else 
      {
        Serial.println("Error in WiFi connection");
      }
      delay(16000); // interval
  }

void GETsend(String data)
{
        HTTPClient http;    //Declare object of class HTTPClient
      if (WiFi.status() == WL_CONNECTED) 
      { //Check WiFi connection status
        // Send and get Data
        http.begin(wificlient,serverName);
  
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
        
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
      }
      
      else 
      {
        Serial.println("Error in WiFi connection");
      }
      delay(16000); // interval
}
