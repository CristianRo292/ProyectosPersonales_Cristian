// ------- PINES MOTORES -------
const int motor1A = 2; // Izquierdo A
const int motor1B = 5; // Izquierdo B (PWM)
const int motor2A = 4; // Derecho A
const int motor2B = 6; // Derecho B (PWM)

// Sensores de proximidad
const int Trigger = 12;
const int Echo = 13;

float t = 0.0; // Ancho del pulso enviado
float dist = 0.0;

// SOLUCIÓN AL ERROR: Convertidas a const
const int velocidadMax = 250; 
const int velocidadMinima = 80;

char comando;
bool estCon = true;

// Parámetros del temporizador
unsigned long tiempoPrevio = 0;
const long intervalo = 2000;
bool modoSumoActivo = false; 

// declaramos metodo
void frente(int velocidad = velocidadMax);
void retroceder(int velocidad = velocidadMax);
void girarDerecha(int velocidad = velocidadMax);
void girarIzquierda(int velocidad = velocidadMax);
void detener();
void modoSumo();
float tomarDistancia();
bool temporizador();

void setup() {
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  
  pinMode(Trigger, OUTPUT); 
  pinMode(Echo, INPUT);  
  digitalWrite(Trigger, LOW);
  
  Serial.begin(9600); // Bluetooth
}

void loop() {
  if (Serial.available() > 0) {
    comando = (char)Serial.read();
    
    if (comando == '3') { modoSumoActivo = false; frente(); }
    else if (comando == '4') { modoSumoActivo = false; retroceder(); }
    else if (comando == '1') { modoSumoActivo = false; girarDerecha(); }
    else if (comando == '2') { modoSumoActivo = false; girarIzquierda(); }
    else if (comando == '5') { modoSumoActivo = false; detener(); }
    
    // Botón "Modo Sumo" presionado en la App (Envía '6')
    else if (comando == '6') 
    { 
      modoSumoActivo = true; 
    }
  }
  if (modoSumoActivo) 
  {
    modoSumo();
  }
  
}

// Funciones de movimiento con parámetros por defecto (Ahora válidos)
void frente(int velocidad = velocidadMax) {
  digitalWrite(motor1A, HIGH);
  analogWrite(motor1B, velocidad);
  digitalWrite(motor2A, LOW);
  analogWrite(motor2B, velocidad);
}

void retroceder(int velocidad = velocidadMax) {
  digitalWrite(motor1A, LOW);
  analogWrite(motor1B, velocidad);
  digitalWrite(motor2A, HIGH);
  analogWrite(motor2B, velocidad);
}

void girarDerecha(int velocidad = velocidadMax) {
  digitalWrite(motor1A, HIGH);
  analogWrite(motor1B, velocidad);
  digitalWrite(motor2A, HIGH);
  analogWrite(motor2B, velocidad);
}

void girarIzquierda(int velocidad = velocidadMax) {
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

void modoSumo() {
  float distancia = tomarDistancia();
  static int numeroAleatorio = 1; 

  
  if (distancia >= 80 || distancia == 0) { 
    if (temporizador()) {
      numeroAleatorio = random(1, 3);
    }
    
    if (numeroAleatorio == 2) {
      girarDerecha(velocidadMinima);
      return;
    }
    girarIzquierda(velocidadMinima);
    return;
  } 
  
  
  else if (distancia <= 35) {
    frente(180); 
    return;
  }
  
  
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
  
  t = pulseIn(Echo, HIGH, 30000); // Timeout de 30ms por si el sensor no lee nada
  dist = t / 59.0;
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