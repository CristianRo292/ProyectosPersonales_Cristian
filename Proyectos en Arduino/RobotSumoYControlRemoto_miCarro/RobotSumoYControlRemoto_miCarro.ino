// ------- PINES MOTORES (Adaptados a tu nuevo puente H) -------
const int abilDerecha = 5;     // ENA (PWM para velocidad Derecha)
const int abiilIzquierda = 6;  // ENB (PWM para velocidad Izquierda)
const int motorDerA = 2;       // IN1 (Derecha Adelante)
const int motorDerR = 4;       // IN2 (Derecha Atrás)
const int motorIzqA = 7;       // IN3 (Izquierda Adelante)
const int motorIzqR = 8;       // IN4 (Izquierda Atrás)

// ------- PINES SENSOR ULTRASÓNICO -------
const int Trigger = A2; // Adaptado a A2
const int Echo = A1;    // Adaptado a A1

float t = 0.0; 
float dist = 0.0;

// Velocidades (Convertidas a const para evitar errores de compilación)
const int velocidadMax = 250;    // Tu velocidad máxima actual
const int velocidadMinima = 80;  // Velocidad base para aproximaciones

char comando;
unsigned long tiempoPrevio = 0;
const long intervalo = 2000; // Cambio de rumbo aleatorio cada 2 segundos

bool modoSumoActivo = false; 

// Prototipos de función con los valores por defecto
void frente(int velocidad = velocidadMax);
void retroceder(int velocidad = velocidadMax);
void girarDerecha(int velocidad = velocidadMax);
void girarIzquierda(int velocidad = velocidadMax);
void detener();
void modoSumo();
float tomarDistancia();
bool temporizador();

void setup() {
  // Configuración de pines de motores
  pinMode(abilDerecha, OUTPUT);
  pinMode(abiilIzquierda, OUTPUT);
  pinMode(motorDerA, OUTPUT);
  pinMode(motorDerR, OUTPUT);
  pinMode(motorIzqA, OUTPUT);
  pinMode(motorIzqR, OUTPUT);
  
  // Configuración del sensor ultrasónico
  pinMode(Trigger, OUTPUT); 
  pinMode(Echo, INPUT);  
  digitalWrite(Trigger, LOW);
  
  Serial.begin(9600); // Bluetooth
}

void loop() {
  // 1. Recepción de comandos por Bluetooth (Mapeado con tu App)
  if (Serial.available() > 0) {
    comando = (char)Serial.read();
    
    // Si usas control manual, se desactiva el modo autónomo inmediatamente
    if (comando == '3')      { modoSumoActivo = false; frente(); }
    else if (comando == '4') { modoSumoActivo = false; retroceder(); }
    else if (comando == '1') { modoSumoActivo = false; girarDerecha(); }
    else if (comando == '2') { modoSumoActivo = false; girarIzquierda(); }
    else if (comando == '5') { modoSumoActivo = false; detener(); }
    
    // Botón "Modo Sumo" presionado en la App (Envía '6')
    else if (comando == '6') { 
      modoSumoActivo = true; 
    }
  }
  // modoSumo();
  // 2. Bucle continuo del modo autónomo
  if (modoSumoActivo) {
    modoSumo();
  }
}

// =========================================================
// IMPLEMENTACIÓN DE FUNCIONES DE MOVIMIENTO (NUEVA LÓGICA DE 6 PINES)
// =========================================================

void frente(int velocidad) {
  digitalWrite(motorDerA, LOW);
  digitalWrite(motorDerR, HIGH);
  digitalWrite(motorIzqA, LOW);
  digitalWrite(motorIzqR, HIGH);
  analogWrite(abilDerecha, velocidad);
  analogWrite(abiilIzquierda, velocidad);
}

void retroceder(int velocidad) {
  digitalWrite(motorDerA, HIGH);
  digitalWrite(motorDerR, LOW);
  digitalWrite(motorIzqA, HIGH);
  digitalWrite(motorIzqR, LOW);
  analogWrite(abilDerecha, velocidad);
  analogWrite(abiilIzquierda, velocidad);
}

void girarDerecha(int velocidad) {
  digitalWrite(motorDerA, LOW);
  digitalWrite(motorDerR, HIGH);
  digitalWrite(motorIzqA, HIGH);
  digitalWrite(motorIzqR, LOW);
  analogWrite(abilDerecha, velocidad);
  analogWrite(abiilIzquierda, velocidad);
}

void girarIzquierda(int velocidad) {
  digitalWrite(motorDerA, HIGH);
  digitalWrite(motorDerR, LOW);
  digitalWrite(motorIzqA, LOW);
  digitalWrite(motorIzqR, HIGH);
  analogWrite(abilDerecha, velocidad);
  analogWrite(abiilIzquierda, velocidad);
}
void detener() {
  digitalWrite(motorDerA, LOW);
  digitalWrite(motorDerR, LOW);
  digitalWrite(motorIzqA, LOW);
  digitalWrite(motorIzqR, LOW);
  analogWrite(abilDerecha, 0);
  analogWrite(abiilIzquierda, 0);
}

// =========================================================
// LÓGICA DE COMBATE Y SENSORES (MANTIENE LA COMPATIBILIDAD CON TU APP)
// =========================================================

void modoSumo() {
  float distancia = tomarDistancia();
  static int numeroAleatorio = 1; 

  // Si la pista está despejada o el sensor no recibe eco (distancia == 0)
  if (distancia >= 80 || distancia == 0) { 
    if (temporizador()) {
      numeroAleatorio = random(1, 3); // Cambia de rumbo cada 2 segundos
    }
    
    if (numeroAleatorio == 2) {
      girarDerecha(velocidadMinima);
      return;
    }
    girarIzquierda(velocidadMinima);
    return;
  } 
  
  // Oponente detectado encima (Embestida total)
  else if (distancia <= 35) {
    frente(180); 
    return;
  }
  
  // Oponente en rango medio
  else {
    frente(velocidadMax); 
  }
}

float tomarDistancia() {
  delay(10);
  digitalWrite(Trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10); 
  digitalWrite(Trigger, LOW);
  
  
  t = pulseIn(Echo, HIGH, 30000); 
  dist = t / 59.0;
  Serial.println(dist);
  return dist;
  
}

bool temporizador() {
  unsigned long tiempoActual = millis();
  if (tiempoActual - tiempoPrevio >= intervalo) {
    tiempoPrevio = tiempoActual; 
    return true;
  }
  return false;
}
