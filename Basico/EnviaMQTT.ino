#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LED_PIN D1

const char* ssid = "ifmachado";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const char* topico_status = "ifsuldeminas/iago/ledstatus";
const char* topico_comando = "ifsuldeminas/iago/ledcomando";

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
      client.subscribe(topico_comando);
      Serial.println("Tópico Assinado!");

    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 2 segundos");
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String mensagem = "";

  for (int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }
  Serial.print("Mensagem recebida");
  Serial.println(mensagem);
  if (mensagem ==  "ON") {
    digitalWrite(LED_PIN, HIGH);
    client.publish(topico_comando, "ON");
    Serial.println("LED LIGADO");
  } else if (mensagem == "OFF") {
    digitalWrite(LED_PIN, LOW);
    client.publish(topico_comando, "OFF");
    Serial.println("LED DESLIGADO");
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (Serial.available() > 0) {
    char comando = Serial.read();
    if (comando == '1') {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("-> ACESO pelo Serial!");
      client.publish("ifsuldeminas/Iago/ledstatus", "ON");
    } else if (comando == '0') {
      digitalWrite(LED_PIN, LOW);
      Serial.println("-> APAGADO pelo Serial!");
      client.publish("ifsuldeminas/Iago/ledstatus", "OFF");
    }
  }
}