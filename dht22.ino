/////////////////////////////////////////////////////////////////
// DHT22 pins       esp8266 pins
// vcc              to 3.3v
// data             to to GPIO4
// ground           to ground
////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// cheep chinese soil moisture sensor hookup ( only soil part without comparator part)
// A0 esp8266 pin and ground
//////////////////////////////////////////

#include <DHT.h>


#include <ESP8266WiFi.h>

#include <WiFiUdp.h>

#define DHTPIN 4     // what digital pin the DHT22 is conected to 
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors

DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "Greenhouse";
const char* password = "svalenik";



// UDP variables
unsigned int localPort = 8267;
WiFiUDP UDP;
boolean udpConnected = false;
unsigned int ServerPort = 6000;
IPAddress IP_REMOTE(192, 168, 4, 1);
IPAddress ipClient(192, 168, 4, 20);
IPAddress Subnet(255, 255, 255, 0);




void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  //Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA); // para que solo sea STA y no genere la IP 192.168.4.1
  WiFi.config(ipClient, IP_REMOTE, Subnet);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // connect to UDP – returns true if successful or false if not


Serial.println("");
Serial.println("Connecting to UDP");

if(UDP.begin(localPort) == 1){
Serial.println("Connection successful");

}
else{
Serial.println("Connection failed");
}

yield();  

}

void loop() {
  
char valueTemp[7];


float h = dht.readHumidity();
float t = dht.readTemperature();

int h1 = (int) h;

int t1 = (int) t;

String str1;
String str2;
str1=String(h1);
str2 = String(t1);

Serial.println("Humidity:");
Serial.println(h1);
Serial.println("Temperature:");
Serial.println(t1);


int sm = analogRead(A0);
sm = map( sm, 0, 1024, 0, 99 );
String str3;
str3 = String(sm);
Serial.println(sm);


yield();


String stringOne =  String( str1 + str2 + str3 );
//String stringOne =  String( str1 + str2 );

Serial.println(stringOne);

stringOne.toCharArray(valueTemp,7); 




// send UDP to server

UDP.beginPacket(IP_REMOTE, ServerPort);
UDP.write(valueTemp);
UDP.endPacket();

Serial.println("Temp sended");

delay(2000);
}


