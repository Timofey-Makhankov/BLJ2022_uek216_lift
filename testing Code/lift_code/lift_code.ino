#include <Servo.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define LED 15
#define LIGHTSENSORPIN 13
#define BUTTON 14

Servo servo;
Adafruit_SSD1306 display(128, 32, &Wire, -1);

int pos;

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(LIGHTSENSORPIN, INPUT);
  pinMode(BUTTON, INPUT);

  servo.attach(12);
  servo.write(90);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    while (1) delay(1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
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

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(reading);
  display.print(" LuX");

  display.display();
  delay(100);
}
