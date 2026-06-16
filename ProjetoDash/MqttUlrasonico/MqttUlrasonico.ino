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


const int mqtt_port = 8883;  // Porta criptografada


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
// MQTT
// ======================
WiFiClientSecure espClient;  //ALTERAÇÃO PARA CLIENTE QUE SUPORTA AUTENTICAÇÃO
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


// ======================
// MQTT
// ======================
void reconnect() {
  String clientId = "ESP8266-" + String(ESP.getChipId());
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    // CONEXAO MQTT COM USUARIO E SENHA
    if (
      client.connect(
        clientId.c_str(),
        mqtt_user,
        mqtt_password)) {


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
  espClient.setInsecure();  // Ignora a validação da credencial do servidor
  client.setServer(mqtt_server, mqtt_port);

  pinMode(vermeio, OUTPUT);
  pinMode(relo, OUTPUT);
  pinMode(verdes, OUTPUT);

  Serial.println("Lendo dados do sensor...");
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  // Variáveis para armazenar seus dados
  float cmMsec;
  long microsec = ultrasonic.timing();

  //lendo seu dado
  
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  Serial.print("Distancia em cm: ");
  Serial.println(cmMsec);

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


  // Exibe os valores no monitor serial
  Serial.print("Dado: ");
  Serial.println(cmMsec);


  // ======================
  // ENVIO MQTT
  // ======================
  client.publish(topico, String(cmMsec).c_str());
  Serial.println("Dados enviados MQTT!");
  // Aguarda 2 segundos
  delay(2000);
}
