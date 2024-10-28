#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "DHT.h"

// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

/*Put your SSID & Password*/
const char* ssid = "Helugape";  // Enter SSID here
const char* password = "helugape";  //Enter Password here

const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String formattedDate;
String hours;
String minuts; 
String date; 


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// DHT Sensor
uint8_t DHTPin = D4; 
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);               

float Temperature;
float Humidity;
String mac_adress;

void setup() {
 
  Serial.begin(115200); //Serial connection
  delay(100);
  dht.begin();   
  pinMode(DHTPin, INPUT);
  WiFi.begin(ssid,password);   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
    delay(500);
    Serial.println("Waiting for connection");
  }
  timeClient.begin();
  timeClient.setTimeOffset(-3600*3);
 
}
 
void loop() {
 
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 
   Temperature = dht.readTemperature(); // Gets the values of the temperature
   Humidity = dht.readHumidity(); // Gets the values of the humidity 
   Serial.println("Temperature");
   Serial.println(Temperature);
   Serial.println("Humidity");
   Serial.println(Humidity);
   Serial.println("------- /n -------");
   Serial.println();

   mac_adress = WiFi.macAddress(); 
   Serial.println();
   Serial.print("MAC: ");
   Serial.println(mac_adress);

   timeClient.forceUpdate();
   Serial.print(daysOfTheWeek[timeClient.getDay()]);
   Serial.print(", ");
   Serial.print(timeClient.getHours());
   Serial.print(":");
   Serial.print(timeClient.getMinutes());
   Serial.print(":");
   Serial.println(timeClient.getSeconds());
   
   hours = timeClient.getHours();
   minuts  = timeClient.getMinutes();
   date = timeClient.getFormattedTime();

   StaticJsonDocument<300> doc;
   // Add values in the document
   doc["MAC"] = String(mac_adress);
   doc["Humidade"] = String(Humidity);
   doc["Temperatura"] = String(Temperature);
   doc["Date"] = String(date);

   // Generate the minified JSON and send it to the Serial port.
   serializeJson(doc, Serial);
   // Start a new line
   Serial.println();
   // Generate the prettified JSON and send it to the Serial port.
   serializeJsonPretty(doc, Serial);

   HTTPClient http;    //Declare object of class HTTPClient
   http.begin("http://35.225.101.233:80/api/dados");      //Specify request destination
   http.addHeader("Content-Type", "application/json");  //Specify content-type header

   String json_file;
   serializeJson(doc, json_file);

   int httpCode = http.POST(json_file);   //Send the request
   String payload = http.getString(); //Get the response payload
   
   Serial.println("------- /n -------");
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload

   http.end();  //Close connection
 
 }
 else{
    Serial.println("Error in WiFi connection");   
 }
  delay(60000);  //Send a request every 60 seconds
}
