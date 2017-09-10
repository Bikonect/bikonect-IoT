#include <TinyGPS++.h>
#include <SoftwareSerial.h>

int TXPin = 3;
int RXPin = 4;

TinyGPSPlus gps;
SoftwareSerial Serial_GPS(TXPin, RXPin);

void setup() {
  Serial.begin(9600);
  Serial_GPS.begin(9600);

  Serial.println(F("Data, Hora, Latitude e Longitude"));
  Serial.println(F("Modulo GPS GY-NEO6MV2"));
  Serial.print(F("Biblioteca TinyGPS++ v. "));
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println();
}

void loop() {
  while (Serial_GPS.available() > 0) {
    if (gps.encode(Serial_GPS.read())) {
      displayInfo();
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
}

void displayInfo() {
  Serial.print(F("Location: "));

  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid()) {
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
