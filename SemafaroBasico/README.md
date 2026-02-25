# Documentação — Semáforo com Arduino

## 1. Descrição do Projeto

Este projeto implementa um semáforo veicular completo com semáforo sonoro e visual para pedestres, utilizando Arduino. O sistema conta com três LEDs individuais (vermelho, amarelo e verde) para o semáforo de veículos, um LED RGB para o semáforo de pedestres e um buzzer piezoelétrico para emissão de som acessível.

---

## 2. Componentes Utilizados

| Componente         | Quantidade | Finalidade                              |
|--------------------|------------|-----------------------------------------|
| Arduino Uno        | 1          | Microcontrolador principal              |
| Protoboard         | 1          | Montagem do circuito                    |
| LED Verde          | 1          | Sinal verde do semáforo veicular        |
| LED Amarelo        | 1          | Sinal amarelo do semáforo veicular      |
| LED Vermelho       | 1          | Sinal vermelho do semáforo veicular     |
| LED RGB            | 1          | Semáforo visual para pedestres          |
| Piezo (Buzzer)     | 1          | Semáforo sonoro para pedestres          |
| Resistores 220Ω    | 6          | Proteção dos LEDs e do RGB              |
| Jumpers            | vários     | Conexões entre componentes              |

---

## 3. Mapeamento de Pinos

| Pino Arduino | Componente          | Descrição                  |
|--------------|---------------------|----------------------------|
| 2            | LED Amarelo         | Semáforo veicular          |
| 3            | LED Verde           | Semáforo veicular          |
| 4            | RGB — Verde         | Semáforo pedestre          |
| 5            | RGB — Azul          | Reservado (não utilizado)  |
| 6            | RGB — Vermelho      | Semáforo pedestre          |
| 7            | Piezo               | Buzzer sonoro              |
| 8            | LED Vermelho        | Semáforo veicular          |

---

## 4. Funcionamento do Sistema

O sistema opera em ciclos contínuos divididos em três estados principais, controlados pela função `loop()`.

### 4.1 Estado 1 — Pedestre VERDE / Veículo VERMELHO

O LED vermelho do semáforo veicular é ativado. O LED RGB exibe uma cor mista (semáforo colorido do pedestre), indicando que o pedestre pode atravessar. O buzzer emite um tom contínuo de 1500 Hz como sinal sonoro de liberação.

Nos últimos 10% do tempo total configurado para o pedestre verde (`PED_GREEN_TOTAL_MS * BLINK_PERCENT`), o LED verde do RGB começa a piscar rapidamente, avisando que o sinal está prestes a fechar. O som contínuo é mantido durante o pisca e cortado apenas ao final.

### 4.2 Estado 2 — Pedestre VERMELHO / Veículo AMARELO

O LED amarelo do semáforo veicular é ativado e o RGB passa para vermelho, indicando que o pedestre deve parar. O buzzer emite bipes intermitentes durante toda a duração do amarelo (`VEH_YELLOW_MS`), sinalizando a transição.

### 4.3 Estado 3 — Pedestre VERMELHO / Veículo VERDE

O LED verde do semáforo veicular é ativado. O RGB permanece vermelho. O buzzer continua com bipes intermitentes durante o verde do veículo (`VEH_GREEN_MS`), reforçando que o pedestre deve aguardar.

Ao final deste estado, o ciclo reinicia automaticamente pelo `loop()`.

---

## 5. Parâmetros Configuráveis

Todos os tempos do sistema são definidos no topo do código e podem ser ajustados sem alterar a lógica:

```cpp
const unsigned long PED_GREEN_TOTAL_MS = 10000;  // Tempo total do pedestre verde (ms)
const float         BLINK_PERCENT      = 0.10;   // Percentual final em modo pisca (10%)
const unsigned long VEH_YELLOW_MS      = 2000;   // Duração do amarelo veicular (ms)
const unsigned long VEH_GREEN_MS       = 8000;   // Duração do verde veicular (ms)
```

Com os valores acima, o ciclo completo dura aproximadamente **20 segundos**: 10s pedestre verde → 2s amarelo → 8s verde veicular.

---

## 6. Funções Principais

### `semafaroVeiculo(r, y, g)`
Controla os três LEDs do semáforo veicular de forma centralizada. Recebe booleanos para vermelho, amarelo e verde.

### `pedestre(red, green)`
Controla as cores do LED RGB. O pino azul permanece sempre em LOW (não utilizado).

### `somAberto()`
Ativa o buzzer com tom contínuo de 1500 Hz, indicando que o pedestre está livre para atravessar.

### `somFechadoIntermitente(duracaoMs)`
Emite bipes alternados (120ms ligado / 120ms desligado) durante o tempo especificado. Usa `millis()` para controlar o tempo sem ultrapassar a duração total.

### `piscaPedestreVerde(duracaoMs)`
Pisca o LED verde do RGB em intervalos de 300ms. Também respeita o tempo máximo usando `min(intervalo, restante)` para evitar estouro.

---

## 7. Diagrama de Estados

```
┌─────────────────────────────────────────────────────┐
│                   CICLO PRINCIPAL                   │
└─────────────────────────────────────────────────────┘

  ┌──────────────────────┐
  │  ESTADO 1            │  Veículo: VERMELHO
  │  Pedestre: VERDE     │  Buzzer: tom contínuo
  │  (90% do tempo)      │
  └──────────┬───────────┘
             │
  ┌──────────▼───────────┐
  │  ESTADO 1b           │  Veículo: VERMELHO
  │  Pedestre: PISCANDO  │  Buzzer: tom contínuo
  │  (10% final)         │
  └──────────┬───────────┘
             │
  ┌──────────▼───────────┐
  │  ESTADO 2            │  Veículo: AMARELO
  │  Pedestre: VERMELHO  │  Buzzer: bipes intermitentes
  │  (VEH_YELLOW_MS)     │
  └──────────┬───────────┘
             │
  ┌──────────▼───────────┐
  │  ESTADO 3            │  Veículo: VERDE
  │  Pedestre: VERMELHO  │  Buzzer: bipes intermitentes
  │  (VEH_GREEN_MS)      │
  └──────────┬───────────┘
             │
             └──── volta ao ESTADO 1
```

---

## 8. Código-Fonte

```cpp
// ====== TEMPOS (ajuste aqui) ======
const unsigned long PED_GREEN_TOTAL_MS = 10000;
const float         BLINK_PERCENT      = 0.10;
const unsigned long VEH_YELLOW_MS      = 2000;
const unsigned long VEH_GREEN_MS       = 8000;

// ====== PINOS ======
const int PIN_V_RED    = 8;
const int PIN_V_YELLOW = 2;
const int PIN_V_GREEN  = 3;
const int PIN_P_RED    = 6;
const int PIN_P_GREEN  = 4;
const int PIN_P_BLUE   = 5;
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

  // ESTADO 1: Pedestre VERDE | Veículo VERMELHO
  semafaroVeiculo(true, false, false);
  pedestre(false, true);
  somAberto();
  delay(steadyMs);

  // ESTADO 1b: Pedestre PISCANDO | Veículo VERMELHO
  piscaPedestreVerde(blinkMs);
  noTone(PIN_BUZZER);

  // ESTADO 2: Pedestre VERMELHO | Veículo AMARELO
  semafaroVeiculo(false, true, false);
  pedestre(true, false);
  somFechadoIntermitente(VEH_YELLOW_MS);

  // ESTADO 3: Pedestre VERMELHO | Veículo VERDE
  semafaroVeiculo(false, false, true);
  pedestre(true, false);
  somFechadoIntermitente(VEH_GREEN_MS);
}
```

---

## 9. Link Tinkercad

> 🔗 **[Inserir link do projeto no Tinkercad aqui]**

---
