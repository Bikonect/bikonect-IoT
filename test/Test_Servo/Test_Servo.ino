#include <Servo.h>

#define SERVO_1 5
#define SERVO_2 6

Servo s1;
Servo s2;
int pos;

void setup () {
  s1.attach(SERVO_1);
  s2.attach(SERVO_2);
  
  Serial.begin(9600);
  
  s1.write(0);
  s2.write(0);
}

void loop() {
  for (pos = 0; pos < 180; pos++) {
    s1.write(pos);
    s2.write(pos);
    delay(1);
  }
  
  delay(1000);
  
  for (pos = 180; pos >= 0; pos--) {
    s1.write(pos);
    s2.write(pos);
    delay(1);
  }

  delay(1000);
}
