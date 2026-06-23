#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Ultrasonic.h>

// ======================
// WIFI
// ======================
const char* ssid = "ifmachado";
const char* password = "";

// ======================
// HIVEMQ CLOUD
// ======================
const char* mqtt_server = "c28d5a3ca24a4d259fca9e54afea7665.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;  // Porta nativa criptografada para microcontroladores

// ======================
// USUARIO MQTT
// ======================
const char* mqtt_user = "Dog_mals";

// ======================
// SENHA MQTT
// ======================
const char* mqtt_password = "Jurema10";

// ======================
// TOPICOS MQTT
// ======================
const char* topico = "HydroMonitor/Ultrassonico";

// ======================
// SENSOR Ultrasonico
// ======================
#define pino_trigger D1
#define pino_echo D2
#define vermeio D7
#define relo D6
#define verdes D5

Ultrasonic ultrasonic(pino_trigger, pino_echo);

// ======================
// MQTT CLIENT DEFINITION
// ======================
WiFiClientSecure espClient;  
PubSubClient client(espClient);

void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi conectado!");
}

void reconnect() {
  String clientId = "ESP8266-" + String(ESP.getChipId());
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    // Conexão autenticada ao broker cloud
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println(" conectado!");
    } else {
      Serial.print("Erro MQTT: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// ======================
// SETUP
// ======================
void setup() {
  Serial.begin(9600);
  setup_wifi();
  
  espClient.setInsecure();  // Ignora a validação rígida de certificado SSL no ESP para simplificar
  client.setServer(mqtt_server, mqtt_port);

  pinMode(vermeio, OUTPUT);
  pinMode(relo, OUTPUT);
  pinMode(verdes, OUTPUT);

  Serial.println("Sistema Pronto. Lendo dados do sensor...");
}

// ======================
// LOOP PRINCIPAL
// ======================
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Variáveis para captação física do pulso
  float cmMsec;
  long microsec = ultrasonic.timing();

  // Conversão de tempo para centímetros reais
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  Serial.print("Distancia em cm: ");
  Serial.println(cmMsec);

  // Lógica de atuação física dos LEDs indicadores locais
  if (cmMsec >= 15) {
    digitalWrite(vermeio, LOW);
    digitalWrite(relo, LOW);
    digitalWrite(verdes, HIGH);
  }
  if (cmMsec < 15 && cmMsec >= 8) {
    digitalWrite(vermeio, LOW);
    digitalWrite(relo, HIGH);
    digitalWrite(verdes, HIGH);
  }
  if (cmMsec < 8) {
    digitalWrite(vermeio, HIGH);
    digitalWrite(relo, HIGH);
    digitalWrite(verdes, HIGH);
  }

  // ======================
  // ENVIO MQTT REAL
  // ======================
  client.publish(topico, String(cmMsec).c_str());
  Serial.println("Dados enviados ao MQTT!");
  
  // Aguarda 2 segundos para a próxima leitura estruturada
  delay(2000);
}