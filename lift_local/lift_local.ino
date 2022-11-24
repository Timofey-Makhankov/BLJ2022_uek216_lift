#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_SSD1306.h>

#define BUTTON 15
#define LED 33

char mac[17];
const char* ssid = "GuestWLANPortal";
const char* mqtt_server = "142.93.174.193";

const char* topic1 = "zuerich/lift/local/button";
const char* topic2 = "zuerich/lift/local/led";
const char* topic3 = "zuerich/lift/status";

WiFiClient espClient;
PubSubClient client(espClient);

Adafruit_SSD1306 display(128, 32, &Wire, -1);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void setup_wifi() {
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("done!");
  byte m[6];
  WiFi.macAddress(m);
  sprintf(mac, "%02X:02X:02X:02X:02X:02X", m[0], m[1], m[2], m[3], m[4], m[5]);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("recieved");
  if (strcmp(topic, topic2) == 0) {
    if (atoi((char*)payload) == 1) {
      blinkLed();
    }
  }
  if (strcmp(topic, topic3) == 0) {
    display.clearDisplay();
    display.setCursor(0, 0);
    //display.print((char*)payload);
    display.display();
    delay(100);
  }
}

void reconnect() {
  Serial.print("Attempting MQTT connection...");
  while (!client.connected()) {
    if (client.connect("lift")) {
      Serial.println("done!");
      client.subscribe(topic1);
      client.subscribe(topic2);
    } else {
      delay(500);
      Serial.print(".");
    }
  }
}

void blinkLed() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED, HIGH);
  }
}

void loop() {
  if (!client.connected()) { reconnect(); }
  if (digitalRead(BUTTON) == HIGH) {
    client.publish(topic1, "triggerd");
  }
  client.loop();
  delay(500);
}