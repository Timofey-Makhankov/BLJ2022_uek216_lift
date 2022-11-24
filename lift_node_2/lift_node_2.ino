#include <WiFi.h>
#include <PubSubClient.h>

char mac[17];
const char* ssid = "GuestWLANPortal";
const char* mqtt_server = "142.93.174.193";

const char* topic1 = "m216demo/lift/sensor2";

#define LIGHT 33

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  pinMode(LIGHT, INPUT);
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

void reconnect() {
  Serial.print("Attempting MQTT connection...");
  while (!client.connected()) {
    if (client.connect("lift Sensor")) {
      Serial.println("done!");
      client.subscribe(topic1);
    } else {
      delay(500);
      Serial.print(".");
    }
  }
}

void loop() {
  if (!client.connected()) { reconnect(); }
  char tempBuffer[15];
  Serial.println(analogRead(LIGHT));
  sprintf(tempBuffer, "%f", analogRead(LIGHT));
  client.publish(topic1, tempBuffer);
  delay(500);
  client.loop();
}