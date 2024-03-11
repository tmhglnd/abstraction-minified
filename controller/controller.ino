/*
  Code for the .abstraction()-minified controller

  This sketch connects the ESP32 Wemos Lolin to another WiFi network and 
  broadcasts analogRead() osc-messages to all devices in the network

  Based on the WiFiUDPClient.ino example
  Using OSC by Adrian Freed & Yotam Mann
  Extended by Timo Hoogland, www.timohoogland.com, 2024
*/

// include libraries for WiFi and OSC
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

// WiFi network name and password
const char * networkName = "RPiAccessPoint";
const char * networkPswd = "RPiPassWord";

// Broadcast to all devices in the network on port
const char * udpAddress = "255.255.255.255";
const int udpPort = 9999;

// Are we currently connected?
boolean connected = false;

// The udp library class
WiFiUDP udp;

// Define the GPIO pin for a test LED or set the pin for built-in LED
#define LED_BUILTIN 5
// Keep track of time since the last blink for status LED
int sinceBlink = 0;

// define analog read gpio pins where the potmeters are connected to
#define POT_PIN1 32
#define POT_PIN2 33

void setup(){
  // set the pin modes
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(POT_PIN1, INPUT);
  pinMode(POT_PIN2, INPUT);

  // initilize hardware serial:
  Serial.begin(115200);
  
  // connect to the WiFi network and start udp
  connectToWiFi(networkName, networkPswd);
  udp.begin(udpPort);
}

// history for potmeters to filter noise with threshold
int _p1, _p2;
int thresh = 5;

// history for potmeter smoothing
float _s1, _s2;
float smooth = 0.75;

void loop(){
  // read the values from the potmeters
  int pot1 = analogRead(POT_PIN1);
  int pot2 = analogRead(POT_PIN2);

  // apply a lowpass filter on the readings
  _s1 = pot1 * (1-smooth) + _s1 * smooth;
  _s2 = pot2 * (1-smooth) + _s2 * smooth;

  // Uncomment for plotting in the Serial Plotter and Monitor
  // Serial.print("0 4096 ");
  // Serial.print(_p1);
  // Serial.print(" ");
  // Serial.println(_p2);

  // only send data when connected
  if (connected){
    // only send data when relative value changed above threshold
    if (abs(_s1 - _p1) > thresh){
      sendMessage("/control1/function", _s1);
      _p1 = _s1;
    }
    if (abs(_s2 - _p2) > thresh){
      sendMessage("/control1/value", _s2);
      _p2 = _s2;
    }
  }

  // status LED blinks every 500 on/off to indicate WiFi is connected
  // the status LED blinks rapidly every 100ms to indicate no WiFi connection
  if (millis() - sinceBlink >= (50 + connected * 450)){
    digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
    sinceBlink = millis();
  }

  // wait a little to reduce cpu cycles
  delay(40);
}

// A function that sends an OSC-message to specified IP and Port
// with variable address and value
void sendMessage(char addr[], int val){
  // the messages wants an OSC address as first argument
  OSCMessage msg(addr);
  // add the value to the message
  msg.add(val);

  udp.beginPacket(udpAddress, udpPort);
  // send the bytes to the SLIP stream
  msg.send(udp);
  // mark the end of the OSC Packet
  udp.endPacket();
  // free space occupied by message
  msg.empty();
}

// Function to connect to the WiFi network
void connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("Connecting to WiFi network: " + String(ssid));
  // delete old config
  WiFi.disconnect(true);
  // register event handler
  WiFi.onEvent(WiFiEvent);
  // initiate connection
  WiFi.begin(ssid, pwd);
  Serial.println("Waiting for WIFI connection...");
}

// WiFi event handler
void WiFiEvent(WiFiEvent_t event){
  switch(event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      // When connected set 
      Serial.print("WiFi connected! IP address: ");
      Serial.println(WiFi.localIP());
      // Initializes the UDP state
      // This initializes the transfer buffer
      udp.begin(WiFi.localIP(),udpPort);
      connected = true;
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      connected = false;
      break;
    default: break;
  }
}
