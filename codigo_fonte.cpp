#include <LiquidCrystal.h>

// LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Sensor ultrassônico
const int trigPin = 7;
const int echoPin = 6;

const float velocidadeSom = 34000.0; // em cm/s
const int leiturasCount = 3;

float leituras[leiturasCount];
int indice = 0;
float soma = 0;
float media = 0;
bool calculouMedia = false;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  lcd.begin(16, 2);
  lcd.clear();
}

void loop() {
  soma -= leituras[indice];

  float distancia = medirDistancia();
  float distanciaM = distancia / 100.0;

  leituras[indice] = distanciaM;
  soma += distanciaM;
  indice++;

  if (indice >= leiturasCount) {
    indice = 0;
    calculouMedia = true;
  }

  media = soma / leiturasCount;

  lcd.clear();

  if (calculouMedia) {
    lcd.setCursor(0, 0);
    lcd.print("Dist: ");
    lcd.print(media, 2);
    lcd.print(" m");

    lcd.setCursor(0, 1);

    if (media >= 2.5) {
      lcd.print("Nivel baixo");
    } else if (media >= 1.6) {
      lcd.print("Nivel normal");
    } else if (media >= 0.8) {
      lcd.print("Risco enchente");
    } else {
      lcd.print("Enchente!");
    }

    Serial.print("Distancia: ");
    Serial.print(media, 2);
    Serial.println(" m");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Medindo...");
  }

  delay(800);
}

float medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH);
  float distanciaCm = duracao * 0.000001 * velocidadeSom / 2.0;
  return distanciaCm;
}
