#include <Servo.h>
#include <SoftwareSerial.h>

#define SERVO 5
#define LED 13
#define BLUETOOTH_RX 0
#define BLUETOOTH_TX 1

SoftwareSerial BlueTooth(BLUETOOTH_RX, BLUETOOTH_TX);
Servo servo;

int pos;

void setup() {
  Serial.begin(9600);
  BlueTooth.begin(9600);
  
  servo.attach(SERVO);
  servo.write(0);
  
  pinMode(LED, OUTPUT);
}

void loop() {
  if (BlueTooth.available() >= 2) {
    int key = BlueTooth.read();
    int val = BlueTooth.read();

    digitalWrite(key, val);

    if (val == 1) {
      bloqTrava();
    } else {
      desbloqTrava();
    }
  }
}

void bloqTrava() {
  for (pos = 0; pos < 90; pos++) {
    servo.write(pos);
    delay(1);
  }
  
  delay(1000);
}

void desbloqTrava() {
  for (pos = 90; pos >= 0; pos--) {
    servo.write(pos);
    delay(1);
  }

  delay(1000);
}
