#include <IRremote.hpp>
#include "NotasMusicales.h" // Tus frecuencias de piano
#include "cancion1.h"    // Tu base de datos musical

const int RECV_PIN = 11;
const int BUZZER_PIN = 8;
const int leds[] = {2, 3, 4, 5};

// Comandos del control
const uint32_t COD_MODO_PIANO = 0xFF6897; 
const uint32_t COD_MODO_MIDI  = 0xE916FF00; 
const uint32_t COD_SALIR      = 0xFF42BD; 

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  for (int i = 0; i < 4; i++) pinMode(leds[i], OUTPUT);
  Serial.println("Sistema Iniciado - Esperando comando...");
}

void loop() {
  menuEspera(); // Función para el parpadeo de LEDs

  if (IrReceiver.decode()) {
    uint32_t comando = IrReceiver.decodedIRData.decodedRawData;
    IrReceiver.resume();
    string teclaPres = mapeoTeclas(comando);

    if (comando == COD_MODO_PIANO) {
      Serial.println("Modo Piano Activo");
      ejecutarModoPiano();
    } 
    else if (comando == COD_MODO_MIDI) {
      Serial.println("Reproduciendo Cancion...");
      ejecutarModoMidi();
    }
  }
}

// --- Lógica de los Modos ---

void ejecutarModoMidi() {
  for (uint16_t i = 0; i < MELODY_LEN; i++) {
    // Escuchar si queremos salir con "*"
    if (IrReceiver.decode()) {
      if (IrReceiver.decodedIRData.decodedRawData == COD_SALIR) {
        IrReceiver.resume();
        noTone(BUZZER_PIN);
        return; 
      }
      IrReceiver.resume();
    }

    // Lectura de Flash (PROGMEM)
    uint16_t f = pgm_read_word(&midi1[i][0]);
    uint16_t d = pgm_read_word(&midi1[i][1]);
    uint16_t p = pgm_read_word(&midi1[i][2]);
    
    if (f > 0) {
      tone(BUZZER_PIN, f);
      gestionarLedsPorFreq(f);
    }
    delay(d); 
    noTone(BUZZER_PIN);
    apagarLeds();
    delay(p);  
  }
}

void ejecutarModoPiano() {
  while (true) {
    if (IrReceiver.decode()) {
      uint32_t tecla = IrReceiver.decodedIRData.decodedRawData;
      
      if (tecla == COD_SALIR) { IrReceiver.resume(); break; }
      
      // Ejemplo: Tecla 1 toca Do
      if (tecla == 0xFF6897) tone(BUZZER_PIN, C4, 200);
      
      IrReceiver.resume();
    }
  }
}

// --- Funciones Auxiliares ---

void menuEspera() {
  static unsigned long t = 0;
  static int l = 0;
  if (millis() - t > 150) {
    apagarLeds();
    digitalWrite(leds[l], HIGH);
    l = (l + 1) % 4;
    t = millis();
  }
}

void gestionarLedsPorFreq(uint16_t freq) {
  if (freq < 150) digitalWrite(leds[0], HIGH);
  else if (freq < 350) digitalWrite(leds[1], HIGH);
  else if (freq < 550) digitalWrite(leds[2], HIGH);
  else digitalWrite(leds[3], HIGH);
}

void apagarLeds() {
  for (int i = 0; i < 4; i++) digitalWrite(leds[i], LOW);
}
string mapeoTeclas(uint32_t valor)
{
   switch (valor) {
    // --- REEMPLAZA LOS '0x0000' CON LOS CODIGOS QUE SALGAN EN TU MONITOR ---
    case 0xB946FF00: Serial.println("FLECHA ARRIBA"); return valor; 
    case 0xEA15FF00: Serial.println("FLECHA ABAJO"); break;
    case 0xBB44FF00: Serial.println("FLECHA IZQUIERDA"); break;
    case 0xBC43FF00: Serial.println("FLECHA DERECHA"); break;
    case 0xBF40FF00: Serial.println("OK"); break;
    
    case 0xE916FF00: Serial.println("1"); break;
    case 0xE619FF00: Serial.println("2"); break;
    case 0xF20DFF00: Serial.println("3"); break;
    case 0xF30CFF00: Serial.println("4"); break;
    case 0xE718FF00: Serial.println("5"); break;
    case 0xA15EFF00: Serial.println("6"); break;
    case 0xF708FF00: Serial.println("7"); break;
    case 0xE31CFF00: Serial.println("8"); break;
    case 0xA55AFF00: Serial.println("9"); break;
    case 0xAD52FF00: Serial.println("0"); break;
    
    case 0xBD42FF00: Serial.println("* (ASTERISCO)"); break;
    case 0xB54AFF00: Serial.println("# (NUMERAL)"); break;

    default:
      Serial.println("TECLA NO REGISTRADA");
      Serial.println("Codigo: ");
      Serial.print(codigo);
      break;

}