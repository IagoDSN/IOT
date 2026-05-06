#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LED_PIN D1

const char* ssid = "ifmachado";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando ao WiFi...");


  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("\nWiFi conectado!");
}


void reconnect() {
  String clientId = "ESP8266-" + String(ESP.getChipId());
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado!");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup(){
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.publish("ifsuldeminas/Iago/ledstatus", "ON"); //TODO
}

void loop(){
  if (!client.connected()) {
reconnect();
}
client.loop();

if (Serial.available() > 0) {
    char comando = Serial.read();
    if (comando == '1') {
      digitalWrite(LED_PIN, HIGH);// TODO
      Serial.println("ACESSO!");// TODO
    } 
    else if (comando == '0') {
      digitalWrite(LED_PIN, LOW);// TODO
      Serial.println("APAGADO!");// TODO    }
  }
  }
}