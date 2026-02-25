
const unsigned long PED_GREEN_TOTAL_MS = 10000;  // tempo total pedestre verde
const float         BLINK_PERCENT      = 0.10;   // 10% final piscando
const unsigned long VEH_YELLOW_MS      = 2000;   // amarelo do veículo
const unsigned long VEH_GREEN_MS       = 8000;   // verde do veículo


const int PIN_V_RED    = 8;
const int PIN_V_YELLOW = 2;
const int PIN_V_GREEN  = 3;
// Pedestre (RGB)
const int PIN_P_RED    = 6;
const int PIN_P_GREEN  = 4;
const int PIN_P_BLUE   = 5;  
// Piezo
const int PIN_BUZZER   = 7;


void semafaroVeiculo(bool r, bool y, bool g) {
  digitalWrite(PIN_V_RED,    r ? HIGH : LOW);
  digitalWrite(PIN_V_YELLOW, y ? HIGH : LOW);
  digitalWrite(PIN_V_GREEN,  g ? HIGH : LOW);
}

void pedestre(bool red, bool green) {
  digitalWrite(PIN_P_RED,   red   ? HIGH : LOW);
  digitalWrite(PIN_P_GREEN, green ? HIGH : LOW);
  digitalWrite(PIN_P_BLUE,  LOW);
}

void somAberto() {
  tone(PIN_BUZZER, 1500);
}


void somFechadoIntermitente(unsigned long duracaoMs) {
  const int          freq  = 1200;
  const unsigned long onMs  = 120;
  const unsigned long offMs = 120;
  unsigned long fim = millis() + duracaoMs;
  while (millis() < fim) {
    tone(PIN_BUZZER, freq);
    delay(onMs);
    noTone(PIN_BUZZER);

    unsigned long restante = fim - millis();
    delay(min(offMs, restante));
  }
  noTone(PIN_BUZZER);
}


void piscaPedestreVerde(unsigned long duracaoMs) {
  const unsigned long intervaloMs = 300;
  unsigned long fim = millis() + duracaoMs;
  while (millis() < fim) {
    digitalWrite(PIN_P_GREEN, HIGH);
    unsigned long restante = fim - millis();
    delay(min(intervaloMs, restante));

    digitalWrite(PIN_P_GREEN, LOW);
    restante = fim - millis();
    delay(min(intervaloMs, restante));
  }
  digitalWrite(PIN_P_GREEN, LOW);
}
void setup() {
  pinMode(PIN_V_RED,    OUTPUT);
  pinMode(PIN_V_YELLOW, OUTPUT);
  pinMode(PIN_V_GREEN,  OUTPUT);
  pinMode(PIN_P_RED,    OUTPUT);
  pinMode(PIN_P_GREEN,  OUTPUT);
  pinMode(PIN_P_BLUE,   OUTPUT);
  pinMode(PIN_BUZZER,   OUTPUT);
}


void loop() {
  unsigned long blinkMs  = (unsigned long)(PED_GREEN_TOTAL_MS * BLINK_PERCENT);
  unsigned long steadyMs = PED_GREEN_TOTAL_MS - blinkMs;


  semafaroVeiculo(false, false, true);
  pedestre(true, false);
  somFechadoIntermitente(VEH_GREEN_MS);

  semafaroVeiculo(false, true, false);
  pedestre(true, false);
  somFechadoIntermitente(VEH_YELLOW_MS);

  piscaPedestreVerde(blinkMs);
  noTone(PIN_BUZZER);     

  semafaroVeiculo(true, false, false);
  pedestre(false, true);
  somAberto();             
  delay(steadyMs);


}