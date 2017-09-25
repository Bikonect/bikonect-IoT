#include <Servo.h>

#define SERVO_1 5
#define SERVO_2 6

Servo servo1;
Servo servo2;

int pos;

void setup() {
  servo1.attach(SERVO_1);
  servo2.attach(SERVO_2);
  
  Serial.begin(9600);

  servo1.write(0);
  servo2.write(0);
  
  pinMode(13, OUTPUT);
}

void loop() {
  if (Serial.available() >= 2) {
    int key = Serial.read();
    int val = Serial.read();

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
    servo1.write(pos);
    servo2.write(pos);
    delay(1);
  }
  
  delay(1000);
}

void desbloqTrava() {
  for (pos = 90; pos >= 0; pos--) {
    servo1.write(pos);
    servo2.write(pos);
    delay(1);
  }

  delay(1000);
}
