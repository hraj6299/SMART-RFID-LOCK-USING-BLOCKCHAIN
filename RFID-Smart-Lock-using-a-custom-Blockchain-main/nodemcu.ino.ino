#include<SoftwareSerial.h> //serial communication
SoftwareSerial mySUART(4, 5);  //D2, D1; recieve,transmit
//wifi
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>


WiFiClient wificlient;
ESP8266WiFiMulti WiFiMulti;

String serverName = "http://192.168.137.1:5000/";
String addRoute = serverName+"add_card?cardNum=";
String verifyRoute = serverName+"verify_card?cardNum=";
void wificonnect();

void setup()
{
  Serial.begin(115200);
  mySUART.begin(115200);
  Serial.flush();
  pinMode(D0, OUTPUT); 
  wificonnect();
  
}

void loop()
{
  //wait for input
  while(mySUART.available()<=0){
    delay(500);
  }
  delay(2000);
  //send to server
  String a = mySUART.readStringUntil('\n');
  Serial.println("DEBUG: Received: " + a);
  String b;
  if(a.charAt(0) == 'R'){
    b = sendAdd(a.substring(1));
  } else if(a.charAt(0) == 'V'){
    b = sendVerify(a.substring(1));
  } else{
    //TODO handle error, or send anyway?
    b = "";
    }
  //return the server output
    if(b == "True"){
      b = "1";
    } else{
      b = "0";
    }
    mySUART.println(b);
    Serial.println("DEBUG: Written: " + b);
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
  String sendAdd(String cardNum){
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
        return payload;
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        return String(httpResponseCode);
      }
      // Free resources
      http.end();
      }
      
      else 
      {
        Serial.println("Error in WiFi connection");
        return "Error";
      }
      delay(16000); // interval
  }
  String sendVerify(String cardNum){
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
        return payload;
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        return String(httpResponseCode);
      }
      // Free resources
      http.end();
      }
      
      else 
      {
        Serial.println("Error in WiFi connection");
        return "Error";
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