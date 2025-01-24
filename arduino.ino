#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#define DHTTYPE DHT11 // DHT11

const char* ssid = "cslab";
const char* password = "aksesg31";

// MQTT Broker settings
const char* mqtt_server = "34.42.195.199";
const int mqtt_port = 1883;
const char* mqtt_topic = "SmartHome";

// Pin Definitions
const int dhtPin = 21;            // DHT11 sensor pin
const int pirPin = 14;            // PIR motion sensor pin
const int ledPinG = 19;           // Green LED (Maker: Pin 9, NodeMCU: Pin 19)
DHT dht(dhtPin, DHT11);

// Variables
unsigned long lastMsg = 0;
bool isActive = false;
int pirState = LOW;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.println("failed, retrying...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  // Configure pins
  pinMode(dhtPin, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(ledPinG, OUTPUT); // Configure green LED pin as output
  
  digitalWrite(ledPinG, LOW); // Start with LED off
  
  dht.begin();  // Initialize DHT11 sensor
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 5000) {  // Send every 5 seconds
    lastMsg = now;
    
    // Read DHT11 sensor
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    // Read PIR motion sensor
    int motionDetected = digitalRead(pirPin);
    
    // Turn on the green LED if motion is detected
    if (motionDetected == HIGH) {
      digitalWrite(ledPinG, HIGH);
      Serial.println("Motion detected: Green LED ON");
    } else {
      digitalWrite(ledPinG, LOW);
      Serial.println("No motion detected: Green LED OFF");
    }
    
    // Create JSON string with sensor data
    String jsonString = "{\"temperature\":" + String(temperature) + 
                       ",\"humidity\":" + String(humidity) + 
                       ",\"motion\":" + String(motionDetected) +
                       ",\"timestamp\":" + String(now) + "}";
    
    // Publish to MQTT
    client.publish(mqtt_topic, jsonString.c_str());
    
    // Debug prints
    Serial.println("\nSensor Readings:");
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Motion Detected: ");
    Serial.println(motionDetected);
  }
}
