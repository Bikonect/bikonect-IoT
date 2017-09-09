#include <Servo.h>

#define PINSERVO 6
#define PINPWD 13

Servo servo;
int pos;

void setup() {
  servo.attach(PINSERVO);

  Serial.begin(9600);

  servo.write(0);

  pinMode(PINPWD, OUTPUT);
}

void loop() {
  if (Serial.available() >= 2) {
    int key = Serial.read();
    int val = Serial.read();

    if (val == 0) {
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
  
  digitalWrite(PINPWD, HIGH);
  delay(1000);
}

void desbloqTrava() {
  for (pos = 90; pos >= 0; pos--) {
    servo.write(pos);
    delay(1);
  }

  digitalWrite(PINPWD, LOW);
  delay(1000);
}
