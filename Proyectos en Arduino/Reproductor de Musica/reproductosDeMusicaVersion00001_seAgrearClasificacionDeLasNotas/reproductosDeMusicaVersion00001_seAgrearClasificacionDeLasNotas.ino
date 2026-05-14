#include "FrecuenciasDeNotas.h"

struct NotaMapa {
  const char* nombre;
  int frecuencia;
};

// Mapa dinámico con valores de frecuencia reales (Hercios)
NotaMapa mapa[] = {
  // Octava 1
  {"C1", 33}, {"Cs1", 35}, {"D1", 37}, {"Ds1", 39}, {"E1", 41}, {"F1", 44}, {"Fs1", 46}, {"G1", 49}, {"Gs1", 52}, {"A1", 55}, {"As1", 58}, {"B1", 62},
  // Octava 2
  {"C2", 65}, {"Cs2", 69}, {"D2", 73}, {"Ds2", 78}, {"E2", 82}, {"F2", 87}, {"Fs2", 93}, {"G2", 98}, {"Gs2", 104}, {"A2", 110}, {"As2", 117}, {"B2", 123},
  // Octava 3
  {"C3", 131}, {"Cs3", 139}, {"D3", 147}, {"Ds3", 156}, {"E3", 165}, {"F3", 175}, {"Fs3", 185}, {"G3", 196}, {"Gs3", 208}, {"A3", 220}, {"As3", 233}, {"B3", 247},
  // Octava 4
  {"C4", 262}, {"Cs4", 277}, {"D4", 294}, {"Ds4", 311}, {"E4", 330}, {"F4", 349}, {"Fs4", 370}, {"G4", 392}, {"Gs4", 415}, {"A4", 440}, {"As4", 466}, {"B4", 494},
  // Octava 5
  {"C5", 523}, {"Cs5", 554}, {"D5", 587}, {"Ds5", 622}, {"E5", 659}, {"F5", 698}, {"Fs5", 740}, {"G5", 784}, {"Gs5", 831}, {"A5", 880}, {"As5", 932}, {"B5", 988},
  // Octava 6
  {"C6", 1047}, {"Cs6", 1109}, {"D6", 1175}, {"Ds6", 1245}, {"E6", 1319}, {"F6", 1397}, {"Fs6", 1480}, {"G6", 1568}, {"Gs6", 1661}, {"A6", 1760}, {"As6", 1865}, {"B6", 1976},
  // Especial
  {"REST", 0}
};

// Cálculo dinámico del tamaño del mapa para el bucle de búsqueda
const int totalNotasMapa = sizeof(mapa) / sizeof(NotaMapa);

// Definimos tamaños máximos para evitar desbordar la memoria
const int MAX_NOTAS = 70;
int melody[MAX_NOTAS];
float durations[MAX_NOTAS];
// variables del tempo
const int minuto = 60000;
// int tem = minuto;
float tempo = 700;

void setup() {
  // No es necesario pinMode para la función tone()
  Serial.begin(9600);
  Serial.setTimeout(3000);
}

void loop() {
  // 1. Verificar si hay datos esperando en el buffer
  if (Serial.available() > 0) {
    
    // 2. Leer la cadena completa hasta encontrar un salto de línea (Enter)
    String entradaUsuario = Serial.readStringUntil('\n');
    entradaUsuario.trim(); // Limpia espacios o caracteres basura al final

    if (entradaUsuario.length() > 0) {
      Serial.print("Procesando: ");
      Serial.println(entradaUsuario);

      // 3. Llamar a tu función de parsing
      int totalNotasEncontradas = parsearMusica(entradaUsuario);
      

      // 4. Reproducir inmediatamente lo recibido
      tocarMelodia(totalNotasEncontradas);
    }
  }
}

// #include "pitches.h"

void tocarMelodia(int cont) {
  for(int i = 0; i < cont; i++) {
    Serial.print("Nota ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(melody[i]);

    // Multiplicamos primero (float * float) y luego convertimos a long
    Serial.println(durations[i]);
    Serial.print(tempo);

    float duracionMilis = (float)(durations[i] * tempo);
    
    tone(8, melody[i]);
    delay(duracionMilis); 
    noTone(8);
    // Agregamos un pequeño respiro constante de 20ms o proporcional
    delay(10); 
  }
}
int parsearMusica(String entrada) {
  int indice = 0;
  int posInicio = 0;

  while (indice < MAX_NOTAS) {
    int posPlus = entrada.indexOf('+', posInicio);
    
    // Seguridad: Si no hay un '+', la cadena está mal formateada
    if (posPlus == -1) break;

    int posEspacio = entrada.indexOf(' ', posPlus);
    if (posEspacio == -1) posEspacio = entrada.length();

    // 1. Extraer la Nota (Ej: "C3")
    String notaStr = entrada.substring(posInicio, posPlus);
    notaStr.trim(); // Limpia espacios accidentales

    // --- BÚSQUEDA EN EL MAPA ---
    int frecuenciaEncontrada = 0; 
    for (int i = 0; i < totalNotasMapa; i++) {
      if (notaStr.equals(mapa[i].nombre)) {
        frecuenciaEncontrada = mapa[i].frecuencia;
        break; 
      }
    }
    melody[indice] = frecuenciaEncontrada;
    // ---------------------------

    // 2. Extraer la Duración (Ej: "0.25")
    String durStr = entrada.substring(posPlus + 1, posEspacio);
    durations[indice] = durStr.toFloat();

    indice++;
    posInicio = posEspacio + 1;
    if (posInicio >= entrada.length()) break;
  }
  return indice; // Devuelve cuántas notas se procesaron
}
