#include <Servo.h>

#define LED 15
#define LIGHTSENSORPIN 13
#define BUTTON 14

Servo servo;

int pos;

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(LIGHTSENSORPIN, INPUT);
  pinMode(BUTTON, INPUT);
  servo.attach(12);
  servo.write(90);
}
void loop() {
  float reading = analogRead(LIGHTSENSORPIN);

  Serial.println(reading);

  if (reading > 750) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }

  if (digitalRead(BUTTON) == HIGH) {
    servo.write(135);
    delay(400);
    servo.write(90);
    delay(500);
    servo.write(45);
    delay(400);
    servo.write(90);
  }
}
