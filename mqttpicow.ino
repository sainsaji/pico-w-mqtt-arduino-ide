//Written by: Sain Saji
//Program: MQTT pubsub client using pico w
//Notes: Change WiFi SSID and Password

#include <stdlib.h>
#include <PubSubClient.h>
#include "WizFi360.h"

// setup according to the device you use
#define WIZFI360_EVB_PICO

// Emulate Serial1 on pins 6/7 if not present
// remember that the wiznet is connected to 4 and 5 not 6 and 7
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
#if defined(ARDUINO_MEGA_2560)
SoftwareSerial Serial1(4, 5); // RX, TX
#elif defined(WIZFI360_EVB_PICO)
SoftwareSerial Serial2(4, 5); // RX, TX
#endif
#endif

/* Baudrate */
#define SERIAL_BAUDRATE   115200
#if defined(ARDUINO_MEGA_2560)
#define SERIAL1_BAUDRATE  115200
#elif defined(WIZFI360_EVB_PICO)
#define SERIAL2_BAUDRATE  115200
#endif



/* Wi-Fi info */
//Change accordingly
char ssid[] = "Daridryam";       // your network SSID (name)
char pass[] = "anoshinode chothikke";   // your network password

int status = WL_IDLE_STATUS;  // the Wifi radio's status



// Initialize the Ethernet client object
WiFiClient wclient;

//MQTT broker
const char* mqtt_server = "broker.emqx.io";

PubSubClient client(wclient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  }


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "picoclientabz";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopicab12", "hello world");
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
    //IPAddress ip = WiFi.localIP();
  Serial.println(WiFi.SSID());
 Serial.println("STOPED HERE  .... ");

  Serial.print("Signal strength (RSSI):");
  //Serial.print(rssi);
  Serial.println(" dBm");
}



void setup() {



  // initialize serial for debugging
  Serial.begin(SERIAL_BAUDRATE);
  // initialize serial for WizFi360 module
#if defined(ARDUINO_MEGA_2560)
  Serial1.begin(SERIAL1_BAUDRATE);
#elif defined(WIZFI360_EVB_PICO)
  Serial2.begin(SERIAL2_BAUDRATE);
#endif
  // initialize WizFi360 module
#if defined(ARDUINO_MEGA_2560)
  WiFi.init(&Serial1);
#elif defined(WIZFI360_EVB_PICO)
  WiFi.init(&Serial2);
#endif

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  Serial.println("You're connected to the network");
  printWifiStatus();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
 
}


void loop() {
  if (!wclient.connected()) {
    reconnect();
  }
  client.loop();
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopicab12", msg);
  }

}


