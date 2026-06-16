#include <Ultrasonic.h>

#define pino_trigger D1
#define pino_echo D2
#define vermeio D7
#define relo D6
#define verdes D5

Ultrasonic ultrasonic(pino_trigger, pino_echo);

void setup() {
  pinMode(vermeio, OUTPUT);
  pinMode(relo, OUTPUT);
  pinMode(verdes, OUTPUT);

  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");
}

void loop() {
  float cmMsec;
  long microsec = ultrasonic.timing();
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

  delay(300);
}