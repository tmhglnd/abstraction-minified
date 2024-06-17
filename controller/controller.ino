/*
  .abstraction().minified()

  Code for the .abstraction().minified() controller.
  For the interactive coding artwork by Timo Hoogland, 2024
  www.timohoogland.com

  This sketch connects the ESP32 Wemos Lolin32 to another WiFi network and 
  broadcasts analogRead() osc-messages to over the network at port 9999

  The RGB display shows the selected function and potmeter value as float 0-1

  Based on the WiFiUDPClient.ino example
  Using OSC by Adrian Freed & Yotam Mann
  Using Waveshare LCD1602 RGB library
  Extended by Timo Hoogland, www.timohoogland.com, 2024
*/

// include libraries for WiFi and OSC
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

// include libraries for LCD1602 RGB
#include <Wire.h>
#include "Waveshare_LCD1602_RGB.h"

// WiFi network name and password
// const char * networkName = "./the-matrix.exe";
// const char * networkPswd = "qnhk4mcVkmdk";
const char * networkName = "RPiAccessPoint";
const char * networkPswd = "RPiPassWord";

// Broadcast to all devices in the network on port
// const char * udpAddress = "255.255.255.255";
// Destionation IP is raspberry pi's static ip address
const char * udpAddress = "192.168.4.1";
const int udpPort = 9999;

// Are we currently connected?
boolean connected = false;

// The udp library class
WiFiUDP udp;

// the display is 16x2 characters
Waveshare_LCD1602_RGB lcd(16, 2);

// include library for rotary encoder
// Using https://github.com/brianlow/Rotary by Brian Low
#include <Rotary.h>

// S1 = CLK pin, click signal, LOW > HIGH > LOW
#define CLK_PIN 34
// S2 = DT pin, lags behind CLK by 90 degrees 
#define DT_PIN 35

// create instance of rotary for clock and data pin
Rotary rot = Rotary(CLK_PIN, DT_PIN);

// Define the GPIO pin for a test LED or set the pin for built-in LED
#define LED_BUILTIN 5
// Keep track of time since the last blink for status LED
int sinceBlink = 0;
// Keep track of time since last screen update
int sinceUpdate = 0;
// Keep track of time since last sensor poll
int sincePoll = 0;
// Keep track of time since last osc message send
int sinceOSC = 0;

// define analog read gpio pins where the potmeters are connected to
#define POT_PIN1 33

// history for potmeters to filter noise with threshold
int _p1;
int thresh = 10;

// history for potmeter smoothing
float _s1;
float smooth = 0.7;

// value for rotary position
int pos = 0;
int _pos = 0;

void setup(){
  // set the pin modes
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(POT_PIN1, INPUT);

  pinMode(CLK_PIN, INPUT);
  pinMode(DT_PIN, INPUT);

  // initilize hardware serial:
  Serial.begin(9600);

  // initialize LCD
  lcd.init();

  // enable cursor blinking
  // lcd.blink();

  // run the startscreen
  lcd.setRGB(255, 255, 255);
  startScreen();

  // connect to the WiFi network and start udp
  connectToWiFi(networkName, networkPswd);
  // udp.begin(udpPort);

  // initialize the rotary encoder
  rot.begin();
}

void loop(){
  // read rotary direction change and inc/dec the position
  int dir = rot.process();

  if (dir == DIR_CW){
    pos++;
    if (pos > 4){ pos = 0; }
  } else if (dir == DIR_CCW){
    pos--;
    if (pos < 0){ pos = 4; }
  }

  // Uncomment for posting rotary position in the monitor
  // if (pos != _pos){
  //   _pos = pos;
  //   Serial.print(" rotary: ");
  //   Serial.println(pos);
  // }

  // read the values from the potmeters
  int pot1 = analogRead(POT_PIN1);

  // apply a lowpass filter on the readings
  _s1 = pot1 * (1-smooth) + _s1 * smooth;

  // Uncomment for plotting in the Serial Plotter and Monitor
  // Serial.print("0 4096 ");
  // Serial.print(_p1);
  
  // only send data when connected
  if (connected){
    // only poll send osc messages every 20 ms
    if ((millis() - sinceOSC) >= 20){
      sinceOSC = millis();
      // only send data when relative value changed above threshold

      if (pos != _pos){
        _pos = pos;
        sendMessage("/control1/function", _pos);
      }
      if (abs(_s1 - _p1) > thresh){
        sendMessage("/control1/value", _s1);
        _p1 = _s1;
      }
    }

    // only update the screen every 100 milliseconds
    if (millis() - sinceUpdate >= 250){
      sinceUpdate = millis();
      lcd.setRGB(255, (_pos * 102) % 256, _p1/16);
      // lcd.setRGB(255, _p1/16, _p2/16);
      displayFunction(_pos);
      // displayFunction(_p1);
      displayValue(_p1);
    }
  } else {
    // not connected yet
    lcd.setCursor(0, 0);
    lcd.send_string("connecting...");
  }

  // status LED blinks every 1000 on/off to indicate WiFi is connected
  // the status LED blinks rapidly every 50ms to indicate no WiFi connection
  if (millis() - sinceBlink >= (50 + connected * 950)){
    sinceBlink = millis();
    digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
  }

  // wait a little to reduce cpu cycles
  delay(1);
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

// function names for display
char *functionNames[] = { "squiggle", "mosaic", "smear", "glass", "paint" };
// history for values to adjust only when changing.
int _f = -1;
int _v = -1;

// a function that displays the function name
void displayFunction(int f){
  // print the function name on first line
  // only when it changed
  // int func = int(float(f) / 4096 * 5);
  int func = f;
  if (_f != func){
    _f = func;
    lcd.setCursor(0, 0);
    lcd.send_string(" .");
    lcd.send_string(functionNames[_f]);
    lcd.send_string("(   ");
  }
}

// a function that displays the value as float 0-1
void displayValue(int v){
  // downscale the value range
  int val = int(float(v) / 4096 * 110);

  // only when it changed
  if (_v != val){
    _v = val;
    // generate a char array for number displaying
    char displayNumber[10];
    // convert float value to string with fixed digits
    dtostrf(float(val) / 110, 8, 3, displayNumber);

    // print the variable number from the knob as float 0-1
    lcd.setCursor(0, 1);
    lcd.send_string("   ");
    // lcd.send_string(char(val));
    lcd.send_string(displayNumber);

    // end the line of code
    lcd.send_string(" );");
  }
}

// A function that displays the startscreen message
void startScreen(){
  // display the installation name and wait a little
  lcd.setCursor(0, 0);
  lcd.send_string(".abstraction()");
  delay(1000);
  
  // display my name and wait a little
  lcd.setCursor(0, 1);
  lcd.send_string("  by tmhglnd");
  delay(5000);

  // clear the screen
  lcd.clear();
}
