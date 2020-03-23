/*
 Publishing in the callback
  - connects to an MQTT server
  - subscribes to the topic "inTopic"
  - when a message is received, republishes it to "outTopic"
  This example shows how to control pins using Ethernet shield via MQTT
*/

//#include <SPI.h>
//#include <Ethernet.h>


#include <UIPEthernet.h>
// The connection_data struct needs to be defined in an external file.
#include <UIPServer.h>
#include <UIPClient.h>

#include <PubSubClient.h>
  
#include <SoftwareSerial.h>

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xEE };
IPAddress ip(192, 168, 178, 178);
IPAddress server(192, 168, 178, 128);

SoftwareSerial SerialCom (3,2);
int myDelay = 1000;
 
byte addArray[] = {0xFF, 0x01, 0x86,0x00, 0x00, 0x00,0x00, 0x00, 0x79};
 
char dataValue[9];
String dataString = "";
String ppm = " ";
char ppmc[50];
int resHigh;
int resLow;
int pulse;
  

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

// Callback function
void callback(char* topic, byte* payload, unsigned int length) {
    
  //turn the LED ON if the payload is '1' and publish to the MQTT server a confirmation message
  if(payload[0] == '1'){
    //digitalWrite(lightPin, HIGH);
    client.publish("testTopic", "Light On"); }
    
  //turn the LED OFF if the payload is '0' and publish to the MQTT server a confirmation message
  if (payload[0] == '0'){
    //digitalWrite(lightPin, LOW);
    client.publish("testTopic", "Light Off");
    }
  if (payload[0] == '2'){
  client.publish("testTopic", ppmc);
    }
} // void callback


//---

//---


void setup()
{
//  pinMode(lightPin, OUTPUT);
//  digitalWrite(lightPin, LOW);
  
  Ethernet.begin(mac, ip);
  if (client.connect("arduinoClient")) {
    client.publish("testTopic","CO2 Sensor - Online");
    client.subscribe("inTopic");
  }
//Serial.begin(9600);
SerialCom.begin(9600);
//Serial.println("16CORE | MH-Z16 C02 Sensor Test Code");
//Serial.println("------------------------------------");
}

void loop()
{
  client.loop();
  
  SerialCom.write(addArray, 9);
  SerialCom.readBytes(dataValue, 9);
  resHigh = (int) dataValue[2];
  resLow  = (int) dataValue[3];
  pulse = (256*resHigh)+resLow;
  dataString = String(pulse);
  //Serial.print("PPM : ");
  //Serial.println(pulse);

  ppm = String(pulse); //converting Humidity (the float variable above) to a string
  ppm.toCharArray(ppmc, ppm.length() + 1); //packaging up the data to publish to mqtt whoa...

  //client.publish("testTopic", ppmc);
  //delay(myDelay);
}
