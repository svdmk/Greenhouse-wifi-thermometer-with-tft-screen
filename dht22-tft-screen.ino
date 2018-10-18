//    tft pins              esp8266 pins 
//    VCC and LED pins      to the 3V3 output of my esp8266
//    ground                to ground
//    SCK                   to GPIO14 and SDI (MOSI) to GPIO13
//    RST                   to GPIO15
//    CS                    to GPIO5
//    D/C                   to GPIO4



//esp8266 includies
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>


//TFT LCD includies

#include <UTFT.h>
#include <SPI.h> // not necessary if Arduino IDE version >=1.6.6

//Esp8266 and UDP defines

const char *ssid = "Greenhouse";
const char *password = "svalenik";

String string1;
String string2;
String string3;


// Function declarations
void tempScreen(void);
void alarmBlink(void);
void tempValue(void);

//global variables
int st;

//WiFiServer server(6000);

// UDP variables
unsigned int localPort = 6000;
WiFiUDP UDP;
boolean udpConnected = false;
//unsigned int ServerPort = 8266;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];


//UTFT defines

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

// Modify the line below to match your display and wiring:
UTFT myGLCD ( ILI9341_S5P, 15, 5, 4 );



void setup() {
  // put your setup code here, to run once:
//Esp8266 and UDP setups
    
    delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  //server.on("/", handleRoot);
  //server.begin();
  Serial.println("HTTP server started");

  yield();

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


  
// Setup the LCD
    myGLCD.InitLCD (  );
    myGLCD.setFont ( SmallFont );


   ////// welcome 

myGLCD.clrScr (  );
myGLCD.setColor ( 255, 255, 255 );
myGLCD.setFont ( BigFont );
myGLCD.print ("WELCOME",CENTER, 30 ); 
myGLCD.print ("to the SVILEN's",CENTER,72 );
myGLCD.print ("GREENHOUSE",CENTER, 114 );
myGLCD.print ("WIRELESS",CENTER, 156 ); 
myGLCD.print ("THERMOMETER V_0.2",CENTER, 190 ); 

delay(4000);

tempScreen();

}

void loop() {
  // put your main code here, to run repeatedly:
//Esp8266 and UDP loops



// if there’s data available, read a packet
int packetSize = UDP.parsePacket();
if(packetSize)
{
Serial.println("");
Serial.print("Received packet of size ");
Serial.println(packetSize);
Serial.print("From ");
IPAddress remote = UDP.remoteIP();
for (int i =0; i < 4; i++)
{
Serial.print(remote[i], DEC);
if (i < 3)
{
Serial.print(".");
}
}
Serial.print(", port ");
Serial.println(UDP.remotePort());

// read the packet into packetBufffer
UDP.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
Serial.println("Contents:");
//int value = packetBuffer[0]*256 + packetBuffer[1];

 
 Serial.println(packetBuffer);

  
}   

 //yield();

 string1 =  String( String(packetBuffer[0]) + String(packetBuffer[1]) );

//Serial.print(string1);

 string2 =  String( String(packetBuffer[2]) + String(packetBuffer[3]) );

 string3 =  String( String(packetBuffer[4]) + String(packetBuffer[5]) );

int a2 = string2.toInt();

 //koprina
 if(a2 > 35) alarmBlink();
 if((a2<=35) && (st==1)) tempValue();
 if((a2<=35) && (st==2)) tempScreen();
}

void alarmBlink() {

myGLCD.fillScr ( VGA_RED );
 myGLCD.setColor ( 255, 255, 255 );
myGLCD.setFont ( BigFont );
 myGLCD.setBackColor ( VGA_RED );
 myGLCD.print ("THE TEMPERATURE",CENTER, 30 ); 
myGLCD.print ("IS ABOVE 28 ",CENTER,72 );
myGLCD.print ("DEGREES CELSIUS",CENTER, 114 );
myGLCD.print ("OPEN",CENTER, 156 ); 
myGLCD.print ("THE GREENHOUSE",CENTER, 190 );

delay(2000);

yield();


//myGLCD.clrScr (  );

 myGLCD.fillScr ( 255, 255, 255 );
 myGLCD.setColor ( VGA_RED );
myGLCD.setFont ( BigFont );
 myGLCD.setBackColor ( 255 , 255 , 255  );
 myGLCD.print ("THE TEMPERATURE",CENTER, 30 ); 
myGLCD.print ("IS ABOVE 28 ",CENTER,72 );
myGLCD.print ("DEGREES CELSIUS",CENTER, 114 );
myGLCD.print ("OPEN",CENTER, 156 ); 
myGLCD.print ("THE GREENHOUSE",CENTER, 190 );

st = 2;

delay(2000);
}

void tempScreen() 
{

  //myGLCD.clrScr ( );
  myGLCD.fillScr ( 0 , 0 , 0 );
 myGLCD.setColor ( VGA_WHITE );
    myGLCD.drawRect ( 0, 0, 319, 79 );
    myGLCD.drawRect ( 1, 1, 318, 78 );
    //myGLCD.setColor ( VGA_RED );
    myGLCD.drawRect ( 0, 80, 319, 160 );
    myGLCD.drawRect ( 1, 81, 318, 159 );
    //myGLCD.setColor ( VGA_RED );
    myGLCD.drawRect ( 0, 161, 319, 239 );
    myGLCD.drawRect ( 1, 162, 318, 238 );

 myGLCD.setColor ( VGA_WHITE );
myGLCD.setFont ( BigFont );
myGLCD.setBackColor ( 0 , 0 , 0 );
myGLCD.print ("AIR",5, 20 );
myGLCD.print ("TEMPERATURE:",5, 40 );
//myGLCD.setBackColor ( VGA_LIME ); 
myGLCD.print ("AIR",5,100 );
myGLCD.print ("HUMIDITY: %",5,120 );
//myGLCD.setColor ( 255, 255, 255 );
//myGLCD.setBackColor ( VGA_OLIVE );
myGLCD.print ("SOIL",5, 180 );
myGLCD.print ("MOISTURE: %",5, 200 );

st = 1;

}


void tempValue() {
 myGLCD.setFont ( SevenSegNumFont );
myGLCD.setColor ( VGA_WHITE );
//myGLCD.setBackColor ( VGA_AQUA );
myGLCD.print ( string2, 220, 15 ) ;
myGLCD.print ( string1, 220, 95 ) ;
myGLCD.print ( string3, 220, 175 ) ;
//myGLCD.setFont ( BigFont );
//myGLCD.print ( "C", 280, 175 ) ;



yield(); 
}
