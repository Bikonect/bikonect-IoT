#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h>
#include <PubSubClient.h>

const char apn[]  = "timbrasil.com";
const char user[] = "tim";
const char pass[] = "tim";

#include <SoftwareSerial.h>
SoftwareSerial SerialAT(10, 11);

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

const char* broker = "m13.cloudmqtt.com";
const char* stringTeste = "{\n\t\"id\": 1,\n\t\"position\":{\n\t\t\"latitude\": -50.5953251,\n\t\t\"longitude\": -50.6875112\n\t}\n}";

void setup() {
  Serial.begin(9600);
  delay(10);

  SerialAT.begin(9600);
  delay(3000);

  Serial.println("Initializing modem...");
  Serial.print("Waiting for network...");
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
  mqttConnect();
}

boolean mqttConnect() {
  Serial.print("Connecting to ");
  Serial.print(broker);

  if (!mqtt.connect("", "", "")) {
    Serial.println(" fail");
    return false;
  }

  Serial.println(" OK");

  if (mqtt.connected()) {
    mqtt.publish("iot_data", stringTeste);
    return mqtt.connected();
  }
}

void loop() {
  mqtt.loop();
}
