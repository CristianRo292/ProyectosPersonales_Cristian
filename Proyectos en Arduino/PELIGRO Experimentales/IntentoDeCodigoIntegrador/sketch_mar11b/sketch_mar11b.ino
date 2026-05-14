#include <IRremote.hpp>
#include <SPI.h>
#include <MFRC522.h>

// --- Configuración RFID RC522 ---
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

// --- Pines Reasignados para evitar conflicto con SPI ---
const int RECV_PIN = A0;   
const int triggerPin = A1; 
const int echoPin = A2;    
const int LED_PIN = 3;

// --- Pines Motores ---
const int motor1A = 2; 
const int motor1B = 5; // PWM
const int motor2A = 4; 
const int motor2B = 6; // PWM

int velocidadMax = 250;
String op = ""; 
char comando;

void setup() {
  Serial.begin(9600);
  SPI.begin();           // Inicia bus SPI
  rfid.PCD_Init();       // Inicia RC522
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK); 
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(motor1A, OUTPUT); pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT); pinMode(motor2B, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.println("Robot Multimodal Listo. Esperando comando IR...");
}

// --- Función para medir distancia ---
long obtenerDistancia() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  long duracion = pulseIn(echoPin, HIGH);
  return duracion * 0.034 / 2;
}

void loop() {
  if (IrReceiver.decode()) {
    op = reconocimiento();
    if (op == "1") controlAutonomo();
    else if (op == "2") controlBluetooth();
    else if (op == "3") activacionRFID();
    else if (op == "Ok") controlInfraRojo();
    IrReceiver.resume(); 
  }
}

// --- MODO SUMO (Perseguir y Atacar) ---
void controlAutonomo() {
  Serial.println("Modo SUMO Activo");
  while (true) {
    long distancia = obtenerDistancia();
    
    if (distancia > 0 && distancia < 30) {
      frente(); // "Ataca" si está a menos de 30cm
    } else {
      girarDerecha(); // Gira sobre su eje para buscar objetivos
      delay(100);
    }

    if (IrReceiver.decode()) {
      if (reconocimiento() == "3") { detener(); break; }
      IrReceiver.resume();
    }
  }
}

// --- MODO RFID (Esquivador + Lectura) ---
void activacionRFID() {
  Serial.println("Modo Esquivador con RFID");
  while (true) {
    long distancia = obtenerDistancia();

    if (distancia > 0 && distancia < 15) {
      detener();
      delay(200);
      retroceder();
      delay(400);
      girarIzquierda();
      delay(500);
    } else {
      frente();
    }

    // Si detecta una tarjeta, se detiene y parpadea el LED
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      Serial.println("¡Tarjeta Detectada!");
      detener();
      digitalWrite(LED_PIN, HIGH);
      delay(2000);
      digitalWrite(LED_PIN, LOW);
      rfid.PICC_HaltA(); // Detiene lectura de la tarjeta actual
    }

    if (IrReceiver.decode()) {
      if (reconocimiento() == "3") { detener(); break; }
      IrReceiver.resume();
    }
  }
}

void controlBluetooth() {
  while (true) {
    if (Serial.available() > 0) {
      comando = (char)Serial.read();
      if (comando == '3') frente();
      else if (comando == '4') retroceder();
      else if (comando == '1') girarDerecha();
      else if (comando == '2') girarIzquierda();
      else if (comando == '5') detener();
    }
    if (IrReceiver.decode()) {
      if (reconocimiento() == "3") { detener(); break; }
      IrReceiver.resume();
    }
  }
}

void controlInfraRojo() {
  while (true) {
    if (IrReceiver.decode()) {
      String opMov = reconocimiento();
      if (opMov == "Arriba") frente();
      else if (opMov == "Abajo") retroceder();
      else if (opMov == "Izquierda") girarIzquierda();
      else if (opMov == "Derecha") girarDerecha();
      else if (opMov == "Ok") detener();
      else if (opMov == "3") break;
      IrReceiver.resume();
    }
  }
}

String reconocimiento() {
  unsigned long valor = IrReceiver.decodedIRData.decodedRawData;
  if (valor == 0) return ""; 
  switch(valor) {
    case 3108437760: return "Arriba";
    case 3141861120: return "Izquierda";
    case 3208707840: return "Ok";
    case 3158572800: return "Derecha";
    case 3927310080: return "Abajo";
    case 3910598400: return "1";
    case 3860463360: return "2";
    case 4061003520: return "3";
    default: return ""; 
  }
}

// --- Funciones de Movimiento ---
void frente() {
  digitalWrite(motor1A, HIGH); analogWrite(motor1B, velocidadMax);
  digitalWrite(motor2A, LOW);  analogWrite(motor2B, velocidadMax);
}
void retroceder() {
  digitalWrite(motor1A, LOW);  analogWrite(motor1B, velocidadMax);
  digitalWrite(motor2A, HIGH); analogWrite(motor2B, velocidadMax);
}
void girarDerecha() {
  digitalWrite(motor1A, HIGH); analogWrite(motor1B, velocidadMax);
  digitalWrite(motor2A, HIGH); analogWrite(motor2B, velocidadMax);
}
void girarIzquierda() {
  digitalWrite(motor1A, LOW);  analogWrite(motor1B, velocidadMax);
  digitalWrite(motor2A, LOW);  analogWrite(motor2B, velocidadMax);
}
void detener() {
  digitalWrite(motor1A, LOW);  analogWrite(motor1B, 0);
  digitalWrite(motor2A, LOW);  analogWrite(motor2B, 0);
}