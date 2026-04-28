#define verde D1
#define amarelo D2
#define vermelho D3


void setup() {
  pinMode(verde, OUTPUT);
  pinMode(amarelo, OUTPUT);
  pinMode(vermelho, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  digitalWrite(verde, HIGH);
  digitalWrite(amarelo, LOW);
  digitalWrite(vermelho, LOW);
  Serial.println("Verde acesso!");
  delay(5000);
  digitalWrite(verde, LOW);
  digitalWrite(amarelo, HIGH);
  digitalWrite(vermelho, LOW);
  Serial.println("Amarelo acesso!");
  delay(2000);
  digitalWrite(verde, LOW);
  digitalWrite(amarelo, LOW);
  digitalWrite(vermelho, HIGH);
  Serial.println("Vermelho acesso!");
  delay(3000);
  digitalWrite(verde, LOW);
  digitalWrite(amarelo, HIGH);
  digitalWrite(vermelho, LOW);
  Serial.println("Amarelo acesso!");
  delay(2000);              
}
