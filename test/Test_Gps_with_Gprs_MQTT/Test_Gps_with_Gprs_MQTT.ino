#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
#include <stdlib.h>
#include <SoftwareSerial.h>

const char apn[]  = "timbrasil.com";
const char user[] = "tim";
const char pass[] = "tim";

long tempoEnviaDadosAux = 0;

TinyGPSPlus gps;

SoftwareSerial SerialAT(10, 11);
SoftwareSerial Serial_GPS(3, 4);

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

const char* broker = "m13.cloudmqtt.com";

void setup() {
  Serial.begin(9600);
  while (!Serial);

  SerialAT.begin(9600);
  Serial_GPS.begin(9600);
  delay(3000);

  mqttConnect();

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

boolean mqttConnect() {
  Serial.println("Initializing modem...");
  Serial.print("Waiting for network...");

  SerialAT.listen();
  if (!modem.waitForNetwork()) {
    Serial.println(" fail");
    while (true);
  }
  Serial.println(" OK");

  Serial.print("Connecting to ");
  Serial.print(apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    Serial.println(" fail");
    while (true);
  }
  Serial.println(" OK");

  mqtt.setServer(broker, 11657);
  return mqtt.connected();
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

    unsigned long tempo = millis();
    if (tempo - tempoEnviaDadosAux > 30000) {
      tempoEnviaDadosAux = tempo;
      enviaLocalizacao(gps.location.lat(), gps.location.lng());
    }
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
  char charTeste[150];

  String txtLatitude = floatToString(latitude);
  String txtLongitude = floatToString(longitude);

  String stringTeste = "{\n\t\"id\": 1,\n\t\"position\":{\n\t\t\"latitude\": " + txtLatitude + ",\n\t\t\"longitude\": " + txtLongitude + "\n\t}\n}";
  stringTeste.toCharArray(charTeste, stringTeste.length() + 1);

  Serial.print("Connecting to ");
  Serial.print(broker);

  SerialAT.listen();
  if (!mqtt.connect("", "", "")) {
    Serial.println(" fail");
    return false;
  }

  Serial.println(" OK");

  if (mqtt.connected()) {
    mqtt.publish("iot_data",  charTeste);
    return mqtt.connected();
  }
}
