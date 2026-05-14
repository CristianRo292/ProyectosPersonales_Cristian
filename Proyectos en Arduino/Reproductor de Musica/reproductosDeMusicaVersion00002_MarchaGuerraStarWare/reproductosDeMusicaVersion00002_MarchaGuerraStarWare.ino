// --- DEFINICIÓN DE FRECUENCIAS ---
#define C3 131
#define CS3 139
#define D3 147
#define DS3 156
#define E3 165
#define F3 175
#define FS3 185
#define G3 196
#define GS3 208
#define A3 220
#define AS3 233
#define B3 247
#define C4 262
#define CS4 277
#define D4 294
#define DS4 311
#define E4 330
#define F4 349
#define FS4 370
#define G4 392
#define GS4 415
#define A4 440
#define AS4 466
#define B4 494
// #define C5 523
// #define CS5 554
// #define D5 587
// #define DS5 622
// #define E5 659
//
#define C5 523
#define CS5 554
#define D5 587
#define DS5 622
#define E5 659
#define F5 698
#define FS5 740
#define G5 784
#define GS5 831
#define A5 880
#define AS5 932
#define B5 988

#define REST 0

// Escala de tiempos de alta precisión
#define blanca      2.0
#define blanca_p      3.0
#define negra       1.0
#define negra_p     1.5
#define corchea     0.5
#define corchea_p   0.75
#define semicorchea 0.25
#define fusa        0.125
#define redonda 4.0

int pinBuzzer = 8;
float bpm = 124.0;

void setup() {
  pinMode(pinBuzzer, OUTPUT);
}

// Función con resolución de articulación dinámica
void tocar(int frecuencia, float duracion) {
  float ms_por_negra = 60000.0 / bpm;
  float tiempo_total = duracion * ms_por_negra;
  
  // Articulación al 90% para claridad máxima en velocidades altas
  float tiempo_sonido = tiempo_total * 0.90; 
  float tiempo_silencio = tiempo_total * 0.10;

  if (frecuencia > 0) {
    tone(pinBuzzer, frecuencia, tiempo_sonido);
  } else {
    noTone(pinBuzzer);
  }
  
  delay(tiempo_total); // Mantiene el pulso rítmico exacto
  noTone(pinBuzzer);
}

void loop() {
  bpm = 100.0; // Tempo de marcha militar

  // --- MOTIVO PRINCIPAL (La-La-La...) ---
  tocar(A4, negra); tocar(A4, negra); tocar(A4, negra);
  tocar(F4, corchea_p); tocar(C5, semicorchea);
  tocar(A4, negra); tocar(F4, corchea_p); tocar(C5, semicorchea);
  tocar(A4, blanca);

  // --- SEGUNDA PARTE (El ascenso) ---
  tocar(E5, negra); tocar(E5, negra); tocar(E5, negra);
  tocar(F5, corchea_p); tocar(C5, semicorchea);
  tocar(GS4, negra); tocar(F4, corchea_p); tocar(C5, semicorchea);
  tocar(A4, blanca);

  // --- EL SALTO ÉPICO ---
  tocar(A5, negra); tocar(A4, corchea_p); tocar(A4, semicorchea);
  tocar(A5, negra); tocar(GS5, corchea_p); tocar(G5, semicorchea);
  tocar(FS5, semicorchea); tocar(F5, semicorchea); tocar(FS5, corchea);
  
  tocar(REST, corchea);
  tocar(AS4, corchea); tocar(DS5, negra); tocar(D5, corchea_p); tocar(CS5, semicorchea);
  
  delay(3000); 
}