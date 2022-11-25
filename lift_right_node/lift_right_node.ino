#include <WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

#define SERVO_PIN 15
#define LIGHT_PIN 33

char mac[17];
const char* ssid = "GuestWLANPortal";
const char* mqtt_server = "142.93.174.193";

const char* topic1 = "zuerich/lift/lightsensor";
const char* topic2 = "zuerich/lift/button";
const char* topic3 = "zuerich/lift/conformation";

WiFiClient espClient;
PubSubClient client(espClient);

Servo servo;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  servo.attach(SERVO_PIN);
  servo.write(90);

  pinMode(LIGHT_PIN, INPUT);
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
    if (atoi((char*)payload) == 1){
      pressButton();
    }
  }
}

void reconnect() {
  Serial.print("Attempting MQTT connection...");
  while (!client.connected()) {
    if (client.connect("lift1")) {
      Serial.println("done!");
      client.subscribe(topic1);
      client.subscribe(topic2);
      client.subscribe(topic3);
    } else {
      delay(500);
      Serial.print(".");
    }
  }
}

void pressButton(){
  servo.write(110);
  delay(450);
  servo.write(90);
  delay(100);
  servo.write(45);
  delay(100);
  servo.write(90);
  client.publish(topic3, "recieved");
}

void loop() {
  if (!client.connected()) { reconnect(); }
  char buffer[15];
  float reading = analogRead(LIGHT_PIN);
  sprintf(buffer, "%f", reading);
  client.loop();
  client.publish(topic1, buffer);
  client.loop();
  delay(500);
}