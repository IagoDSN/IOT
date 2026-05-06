#define LED_PIN D1 // TODO
void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT); // TODO
}
void loop() {
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
