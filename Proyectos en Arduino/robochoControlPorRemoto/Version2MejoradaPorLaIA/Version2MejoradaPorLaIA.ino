// ===============================
//        ROBOT SUMO
// Manual por Bluetooth + Autónomo
// ===============================


// ---------- PINES MOTORES ----------
const int motor1A = 2; // Izquierdo dirección
const int motor1B = 5; // Izquierdo PWM

const int motor2A = 4; // Derecho dirección
const int motor2B = 6; // Derecho PWM


// ---------- SENSOR ULTRASONICO ----------
const int Trigger = 12;
const int Echo = 13;


// ---------- VELOCIDADES ----------
const int velocidadMax = 180;
const int velocidadAtaque = 250;
const int velocidadBusqueda = 90;


// ---------- VARIABLES ----------
char comando;

bool modoSumoActivo = false;


// ---------- TEMPORIZADORES ----------
unsigned long tiempoPrevioBusqueda = 0;
unsigned long ultimoAvistamiento = 0;

const long intervaloBusqueda = 1800;
const long memoriaObjetivo = 700;


// ---------- DIRECCION DE BUSQUEDA ----------
int direccionBusqueda = 1;


// ---------- PROTOTIPOS ----------
void frente(int velocidad);
void retroceder(int velocidad);

void girarDerecha(int velocidad);
void girarIzquierda(int velocidad);

void detener();

void modoSumo();

float tomarDistancia();

bool temporizadorBusqueda();


// ===================================
//              SETUP
// ===================================
void setup() {

  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);

  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);

  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);

  digitalWrite(Trigger, LOW);

  Serial.begin(9600);

  // Semilla aleatoria
  randomSeed(analogRead(A0));
}


// ===================================
//               LOOP
// ===================================
void loop() {

  // ---------- CONTROL BLUETOOTH ----------
  if (Serial.available() > 0) {

    comando = Serial.read();

    switch (comando) {

      case '1':
        modoSumoActivo = false;
        girarDerecha(velocidadMax);
      break;

      case '2':
        modoSumoActivo = false;
        girarIzquierda(velocidadMax);
      break;

      case '3':
        modoSumoActivo = false;
        frente(velocidadMax);
      break;

      case '4':
        modoSumoActivo = false;
        retroceder(velocidadMax);
      break;

      case '5':
        modoSumoActivo = false;
        detener();
      break;

      case '6':
        modoSumoActivo = true;
      break;
    }
  }

  // ---------- MODO AUTONOMO ----------
  if (modoSumoActivo) {
    modoSumo();
  }
}


// ===================================
//          MODO SUMO
// ===================================
void modoSumo() {

  float distancia = tomarDistancia();
  Serial.println(distancia);

  // ===================================
  //   ENEMIGO DETECTADO
  // ===================================
  if (distancia > 0 && distancia <= 40) {

    ultimoAvistamiento = millis();

    // Muy cerca → control para no patinar
    if (distancia <= 25) {
      frente(velocidadAtaque);
      return;
    }

    // Media distancia → máxima potencia
    frente(velocidadMax);
    return;
  }


  // ===================================
  // MEMORIA DEL OBJETIVO
  // Sigue avanzando aunque pierda
  // momentáneamente al enemigo
  // ===================================
  if (millis() - ultimoAvistamiento <= memoriaObjetivo) {

    frente(velocidadAtaque);
    return;
  }


  // ===================================
  // BUSQUEDA
  // ===================================

  // Cambiar dirección aleatoriamente
  if (temporizadorBusqueda()) {

    direccionBusqueda = random(1, 3);
  }

  // Girar
  if (direccionBusqueda == 1) {

    girarDerecha(velocidadBusqueda);

  } else {

    girarIzquierda(velocidadBusqueda);
  }
}


// ===================================
//      SENSOR ULTRASONICO
// ===================================
float tomarDistancia() {

  float suma = 0;
  int lecturasValidas = 0;

  for (int i = 0; i < 3; i++) {

    digitalWrite(Trigger, LOW);
    delayMicroseconds(2);

    digitalWrite(Trigger, HIGH);
    delayMicroseconds(10);

    digitalWrite(Trigger, LOW);

    unsigned long tiempo =
      pulseIn(Echo, HIGH, 30000);

    float distancia = tiempo / 59.0;

    // Filtrar lecturas inválidas
    if (distancia > 0 && distancia < 400) {

      suma += distancia;
      lecturasValidas++;
    }

    delay(5);
  }

  // Si no hubo lecturas válidas
  if (lecturasValidas == 0) {
    return 0;
  }

  return suma / lecturasValidas;
}


// ===================================
//         TEMPORIZADOR
// ===================================
bool temporizadorBusqueda() {

  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoPrevioBusqueda >= intervaloBusqueda) {

    tiempoPrevioBusqueda = tiempoActual;
    return true;
  }

  return false;
}


// ===================================
//      FUNCIONES MOVIMIENTO
// ===================================

void frente(int velocidad) {

  digitalWrite(motor1A, HIGH);
  analogWrite(motor1B, velocidad);

  digitalWrite(motor2A, LOW);
  analogWrite(motor2B, velocidad);
}


void retroceder(int velocidad) {

  digitalWrite(motor1A, LOW);
  analogWrite(motor1B, velocidad);

  digitalWrite(motor2A, HIGH);
  analogWrite(motor2B, velocidad);
}


void girarDerecha(int velocidad) {

  digitalWrite(motor1A, HIGH);
  analogWrite(motor1B, velocidad);

  digitalWrite(motor2A, HIGH);
  analogWrite(motor2B, velocidad);
}


void girarIzquierda(int velocidad) {

  digitalWrite(motor1A, LOW);
  analogWrite(motor1B, velocidad);

  digitalWrite(motor2A, LOW);
  analogWrite(motor2B, velocidad);
}


void detener() {

  digitalWrite(motor1A, LOW);
  analogWrite(motor1B, 0);

  digitalWrite(motor2A, LOW);
  analogWrite(motor2B, 0);
}
