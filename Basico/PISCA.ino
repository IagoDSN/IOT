#define led D1// TO DO


void setup() {
  pinMode(led, OUTPUT);// TO DO
}
void loop() {
  digitalWrite(led, HIGH); // liga o LED
  delay(1000);             // espera 1 segundo
  digitalWrite(led, LOW);  // desliga o LED
  delay(1000);             // espera 2 segundos
}
