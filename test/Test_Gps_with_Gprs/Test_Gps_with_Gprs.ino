#include <stdlib.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

int LED = 13;
int GPS_TXPin = 3;
int GPS_RXPin = 4;
int GPRS_TXPin = 10;
int GPRS_RXPin = 11;

TinyGPSPlus gps;

SoftwareSerial Serial_GPS(GPS_TXPin, GPS_RXPin);
SoftwareSerial Serial_GPRS(GPRS_TXPin, GPRS_RXPin);

void setup() {
  pinMode(LED, OUTPUT);

  Serial.begin(9600);
  while (!Serial);

  Serial_GPS.begin(9600);
  Serial_GPRS.begin(4800);

  Serial.println("Modulo GPS GY-NEO6MV2");
  Serial.print("Biblioteca TinyGPS++ v. ");
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println("Latitude e Longitude, Data, Hora");
  Serial.println("------------------------------------------");
}

void loop() {
  Serial_GPS.listen();
  while (Serial_GPS.available() > 0) {
    if (gps.encode(Serial_GPS.read())) {
      displayInfo();
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println("Erro com o componente GPS.");
    while (true);
  }
}

void displayInfo() {
  Serial.print("Date/Time: ");
  if (gps.date.isValid()) {
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.year());
  } else {
    Serial.print("ERRO");
  }

  Serial.print(" ");
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) Serial.print("0");
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print("0");
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print("0");
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print("0");
    Serial.print(gps.time.centisecond());
  } else {
    Serial.print("ERRO");
  }

  Serial.print("  Localizacao: ");
  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(",");
    Serial.println(gps.location.lng(), 6);

    enviaLocalizacao(gps.location.lat(), gps.location.lng());
  } else {
    Serial.println("ERRO");
  }
}

String floatToString(float x, byte precision = 6) {
  char tmp[50];
  dtostrf(x, 0, precision, tmp);
  return String(tmp);
}

void enviaLocalizacao(float latitude, float longitude) {
  String txtLatitude = floatToString(latitude);
  String txtLongitude = floatToString(longitude);
  
  Serial.println("------------------------------------------------------");

  Serial_GPRS.listen();
  Serial.println("Enviando localizacao para o servidor");
  
  Serial_GPRS.println("AT");

  delay(100);
  ShowSerialData();

  Serial_GPRS.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(1000);
  ShowSerialData();

  Serial_GPRS.println("AT+SAPBR=3,1,\"APN\",\"timbrasil.br\"");
  delay(4000);
  ShowSerialData();

  Serial_GPRS.println("AT+SAPBR=3,1,\"USER\",\"tim\"");
  delay(4000);
  ShowSerialData();

  Serial_GPRS.println("AT+SAPBR=3,1,\"PWD\",\"tim\"");
  delay(4000);
  ShowSerialData();

  Serial_GPRS.println("AT+SAPBR=1,1");
  delay(2000);
  ShowSerialData();

  Serial_GPRS.println("AT+SAPBR=2,1");
  delay(2000);
  ShowSerialData();

  Serial_GPRS.println("AT+HTTPINIT");
  delay(2000);
  ShowSerialData();

  Serial_GPRS.println("AT+HTTPPARA=\"CID\",1");
  delay(2000);
  ShowSerialData();

  Serial_GPRS.print("AT+HTTPPARA=\"URL\",\"localhost/add.php?field1="+ txtLatitude +"&fiel2="+ txtLongitude +"\"");
  delay(1000);
  ShowSerialData();

  Serial_GPRS.println("AT+HTTPACTION=0");
  delay(10000);
  ShowSerialData();

  Serial_GPRS.println("AT+HTTPREAD");
  delay(100);
  ShowSerialData(); 

  Serial_GPRS.println("AT+HTTPTERM");
  delay(100);
  ShowSerialData(); 

  Serial.println("------------------------------------------------------");

  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
}

void ShowSerialData() {
  while (Serial_GPRS.available() != 0) {
    Serial.write(Serial_GPRS.read());
  }
}
