#if defined(ESP8266)
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>          //https://github.com/esp8266/Arduino
#endif

//needed for library
#include <DNSServer.h>
#if defined(ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include <WiFi.h>
#include <PubSubClient.h>

HardwareSerial Serial2(2);

#define RXD2 16
#define TXD2 17

byte reading[] = { 0x01,0x03,0x00,0x00,0x00,0x02,0xC4,0x0B };
byte reset[] = { 0x01, 0x05, 0x00, 0x00, 0xFF, 0x00, 0x8C, 0x3A };
byte writing[] = { 0x01, 0x06, 0x00, 0x00, 0xFF, 0xFF, 0x88, 0x7A };

const char* ssid = "Keluarga Cendana";
const char* pass = "876543216";
const char* broker = "192.168.2.239";
const char* brokerUser = "dev";
const char* brokerPassword = "dev101";
const char* outTopic = "iot";

WiFiClient espClient;
PubSubClient client(espClient);

long currentTime, lastTime;
int count = 0;

String hexMessage;

void setupWifiManager(){
  
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("AutoConnectAP");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();

    
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
}

void setupWifi(){
  delay(100);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.print("\nConnected to ");
  Serial.println(ssid);

}

void setupMQTT(){
  client.setServer(broker, 1883);
}

void reconnect() {
  while(!client.connected()) {
    Serial.print("\nConnecting to ");
    Serial.println(broker);
    if(client.connect("ESP32", brokerUser, brokerPassword)) {
      Serial.print("\nConnected to ");
      Serial.println(broker);
    } else {
        Serial.print("\nReconnecting.. ");
        delay(3000);
    }
  }
}
void setupSerial() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: "+String(TX));
  Serial.println("Serial Rxd is on pin: "+String(RX));

}


void setup() {
  // put your setup code here, to run once:
  setupSerial();
  setupWifiManager();
  setupMQTT();
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial2.write(reading, sizeof(reading));
  //Serial2.write(writing, sizeof(writing));
  //Serial2.write(reset, sizeof(reset));
  
  String msgBuilder = "";
  while (Serial2.available()) {
    uint8_t inChar = Serial2.read();
    msgBuilder = String(msgBuilder + String(inChar, HEX)+ " ");
  }
  delay(100);
  Serial.println(msgBuilder);
  
  if(!client.connected()) {
    reconnect();
  }
  
  client.loop();
  
  currentTime = millis();
  if(currentTime - lastTime > 1000 && msgBuilder.length() > 0) {
    count++;
    String msgBuilderCount = msgBuilder + "#"+ String(count);

    Serial.print("sending message.. ");
    Serial.println(msgBuilderCount);
    
    char messages[msgBuilderCount.length() + 1];
    msgBuilderCount.toCharArray(messages, msgBuilderCount.length() + 1);
    client.publish(outTopic, messages);
    lastTime = millis();
  }

}
