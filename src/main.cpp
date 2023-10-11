

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WiFi Network Credentials
const char *ssid = "goldenbriquette";   // name of your WiFi network
const char *password = "9840782066"; // password of the WiFi network

// Home Assistant Credentials
const char *HA_USER = "akash";
const char *HA_PASS = "*^7KNF!7fsSnPq";

// -------------------   MQTT Network
IPAddress broker(192,168,0,139); // IP address of your MQTT broker eg. 192.168.1.50
const byte LIGHT_PIN = 2;           // Pin to control the light with
const char *ID = "Gesture";  // Name of our device, must be unique
const char *CLASS1 = "Gesture/class1";  // Topic to subcribe to
const char *STATE_CLASS1 = "Gesture/class1/state";  // Topic to publish the light state to
const char *CLASS2 = "Gesture/class2";  // Topic to subcribe to
const char *STATE_CLASS2 = "Gesture/class1/state";  // Topic to publish the light state to
const char *CLASS3 = "Gesture/class3";  // Topic to subcribe to
const char *STATE_CLASS3 = "Gesture/class3/state";  // Topic to publish the light state to
const char *CLASS4 = "Gesture/class4";  // Topic to subcribe to
const char *STATE_CLASS4 = "Gesture/class4/state";  // Topic to publish the light state to

WiFiClient wclient;
PubSubClient client(wclient); // Setup MQTT client
// -------------------   END MQTT Network


// Handle incomming messages from the broker
void callback(char* topic, byte* payload, unsigned int length) {
  String response;

  for (int i = 0; i < length; i++) {
    response += (char)payload[i];
  }
  // Serial.print("Message arrived [");
  // Serial.print(topic);
  // Serial.print("] ");
  // Serial.println(response);

  if (String(topic) == String(CLASS1)){
    if(response == "on")  // Turn the Motor1 on
      {
        digitalWrite(LIGHT_PIN, HIGH);
        client.publish(STATE_CLASS1,"on");
      }
    else if(response == "off")  // Turn the light off
    {
      digitalWrite(LIGHT_PIN, LOW);
      client.publish(STATE_CLASS1,"off");
    }
  }

  if (String(topic) == String(CLASS2)){
    if(response == "on")  // Turn the Motor1 on
      {
        digitalWrite(LIGHT_PIN, HIGH);
        client.publish(STATE_CLASS2,"on");
      }
    else if(response == "off")  // Turn the light off
    {
      digitalWrite(LIGHT_PIN, LOW);
      client.publish(STATE_CLASS2,"off");
    }
  }
}

// Connect to WiFi network
void setup_wifi() {
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); // Connect to network

  while (WiFi.status() != WL_CONNECTED) { // Wait for connection
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Reconnect to client
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if(client.connect(ID,HA_USER,HA_PASS)) {
      client.subscribe(CLASS1);
      client.subscribe(CLASS2);
      client.subscribe(CLASS3);
      client.subscribe(CLASS4);
      Serial.println("connected");
    }
    else {
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200); // Start serial communication at 115200 baud
  pinMode(LIGHT_PIN, OUTPUT); // Configure LIGHT_PIN as an output
  delay(100);
  setup_wifi(); // Connect to network
  client.setServer(broker, 1883);
  client.setCallback(callback);// Initialize the callback routine
}

void loop() {
  if (!client.connected())  // Reconnect if connection is lost
  {
    reconnect();
  }
  client.loop();

}
