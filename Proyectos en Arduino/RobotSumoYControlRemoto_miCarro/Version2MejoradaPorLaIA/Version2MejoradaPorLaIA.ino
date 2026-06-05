// =====================================
//          ROBOT SUMO V2
// Bluetooth + Autónomo
// Adaptado a puente H de 6 pines
// =====================================


// =====================================
//            MOTORES
// =====================================

// PWM
const int habilDerecha   = 5;
const int habilIzquierda = 6;

// Motor derecho
const int motorDerA = 2;
const int motorDerR = 4;

// Motor izquierdo
const int motorIzqA = 7;
const int motorIzqR = 8;


// =====================================
//        SENSOR ULTRASONICO
// =====================================

const int Trigger = A2;
const int Echo    = A1;


// =====================================
//           VELOCIDADES
// =====================================

const int velocidadMax      = 250;
const int velocidadAtaque  = 220;
const int velocidadBusqueda = 90;


// =====================================
//            VARIABLES
// =====================================

char comando;

bool modoSumoActivo = false;


// =====================================
//          TEMPORIZADORES
// =====================================

unsigned long tiempoPrevioBusqueda = 0;
unsigned long ultimoAvistamiento   = 0;

const long intervaloBusqueda = 1800;
const long memoriaObjetivo  = 700;


// =====================================
//       DIRECCION DE BUSQUEDA
// =====================================

int direccionBusqueda = 1;


// =====================================
//           PROTOTIPOS
// =====================================

void frente(int velocidad);
void retroceder(int velocidad);

void girarDerecha(int velocidad);
void girarIzquierda(int velocidad);

void detener();

void modoSumo();

float tomarDistancia();

bool temporizadorBusqueda();


// =====================================
//               SETUP
// =====================================

void setup() {

  // Pines PWM
  pinMode(habilDerecha, OUTPUT);
  pinMode(habilIzquierda, OUTPUT);

  // Pines motores
  pinMode(motorDerA, OUTPUT);
  pinMode(motorDerR, OUTPUT);

  pinMode(motorIzqA, OUTPUT);
  pinMode(motorIzqR, OUTPUT);

  // Sensor ultrasónico
  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);

  digitalWrite(Trigger, LOW);

  // Bluetooth
  Serial.begin(9600);

  // Semilla aleatoria
  randomSeed(analogRead(A0));
}


// =====================================
//                LOOP
// =====================================

void loop() {

  // =====================================
  //       CONTROL BLUETOOTH
  // =====================================

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


  // =====================================
  //          MODO AUTONOMO
  // =====================================

  if (modoSumoActivo) {

    modoSumo();
  }
}


// =====================================
//            MODO SUMO
// =====================================

void modoSumo() {

  float distancia = tomarDistancia();


  // =====================================
  //      ENEMIGO DETECTADO
  // =====================================

  if (distancia > 0 && distancia <= 80) {

    ultimoAvistamiento = millis();

    // Muy cerca
    if (distancia <= 25) {

      frente(velocidadAtaque);
      return;
    }

    // Distancia media
    frente(velocidadMax);
    return;
  }


  // =====================================
  //      MEMORIA DEL OBJETIVO
  // =====================================

  if (millis() - ultimoAvistamiento <= memoriaObjetivo) {

    frente(velocidadAtaque);
    return;
  }


  // =====================================
  //            BUSQUEDA
  // =====================================

  if (temporizadorBusqueda()) {

    direccionBusqueda = random(1, 3);
  }


  if (direccionBusqueda == 1) {

    girarDerecha(velocidadBusqueda);

  } else {

    girarIzquierda(velocidadBusqueda);
  }
}


// =====================================
//      SENSOR ULTRASONICO
// =====================================

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

  // Sin lecturas válidas
  if (lecturasValidas == 0) {

    return 0;
  }

  float distanciaFinal =
    suma / lecturasValidas;

  Serial.println(distanciaFinal);

  return distanciaFinal;
}


// =====================================
//          TEMPORIZADOR
// =====================================

bool temporizadorBusqueda() {

  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoPrevioBusqueda >= intervaloBusqueda) {

    tiempoPrevioBusqueda = tiempoActual;
    return true;
  }

  return false;
}


// =====================================
//      FUNCIONES DE MOVIMIENTO
// =====================================

void frente(int velocidad) {

  // Derecho adelante
  digitalWrite(motorDerA, LOW);
  digitalWrite(motorDerR, HIGH);

  // Izquierdo adelante
  digitalWrite(motorIzqA, LOW);
  digitalWrite(motorIzqR, HIGH);

  analogWrite(habilDerecha, velocidad);
  analogWrite(habilIzquierda, velocidad);
}


void retroceder(int velocidad) {

  // Derecho atrás
  digitalWrite(motorDerA, HIGH);
  digitalWrite(motorDerR, LOW);

  // Izquierdo atrás
  digitalWrite(motorIzqA, HIGH);
  digitalWrite(motorIzqR, LOW);

  analogWrite(habilDerecha, velocidad);
  analogWrite(habilIzquierda, velocidad);
}


void girarDerecha(int velocidad) {

  // Derecho adelante
  digitalWrite(motorDerA, LOW);
  digitalWrite(motorDerR, HIGH);

  // Izquierdo atrás
  digitalWrite(motorIzqA, HIGH);
  digitalWrite(motorIzqR, LOW);

  analogWrite(habilDerecha, velocidad);
  analogWrite(habilIzquierda, velocidad);
}


void girarIzquierda(int velocidad) {

  // Derecho atrás
  digitalWrite(motorDerA, HIGH);
  digitalWrite(motorDerR, LOW);

  // Izquierdo adelante
  digitalWrite(motorIzqA, LOW);
  digitalWrite(motorIzqR, HIGH);

  analogWrite(habilDerecha, velocidad);
  analogWrite(habilIzquierda, velocidad);
}


void detener() {

  digitalWrite(motorDerA, LOW);
  digitalWrite(motorDerR, LOW);

  digitalWrite(motorIzqA, LOW);
  digitalWrite(motorIzqR, LOW);

  analogWrite(habilDerecha, 0);
  analogWrite(habilIzquierda, 0);
}
