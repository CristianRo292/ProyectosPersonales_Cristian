#include <IRremote.hpp>

// // Configuración de los motores
// int abilIzquierda = 6; // Corregido: abil con una sola 'i'
// int motorIzqA = 7;
// int motorIzqR = 8;

int velocidadMax = 250;
int velocidadMedia = 100;
int velocidadMin = 50;

const int triggerPin = 12;
const int echoPin = 13;

int RECV_PIN = 11;
int LED_PIN = 3;
String op = ""; 

// ------- PINES MOTORES (Basados en tu primer robot) -------
const int motor1A = 2; // Izquierdo A
const int motor1B = 5; // Izquierdo B (PWM)
const int motor2A = 4; // Derecho A
const int motor2B = 6; // Derecho B (PWM)

// int velocidadMax = 200; 
char comando;


void setup() {
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK); 
  
  pinMode(LED_PIN, OUTPUT);
  // pinMode(abilIzquierda, OUTPUT);
  // pinMode(motorIzqA, OUTPUT);
  // pinMode(motorIzqR, OUTPUT);
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  // control del eco
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.println("Sistema de Control IR listo...");
}

void loop() {
  if (IrReceiver.decode()) {
    op = reconocimiento();
    
    if (op == "1") {
      controlAutonomo();
    }
    else if (op == "2") {
      controlBluetooth();
    }
    else if (op == "3") {
      activacionRFID();
    }
    else if (op == "Ok") { // Corregido: "Ok" con O mayúscula para coincidir con el return
      // girar(0);
      controlInfraRojo();
    }
    
    IrReceiver.resume(); 
  }
}

void controlInfraRojo()
{
  while (true)
  {
    String opMovimiento = reconocimiento();
    if (opMovimiento == "Arriba")
    {
      frente();
    }
    else if (opMovimiento == "Abajo")
    {
      retroceder();
    }
    else if (opMovimiento == "Izquierda")
    {
      girarIzquierda();
    }
    else if (opMovimiento == "Derecha")
    {
      girarDerecha();
    }
    else if (opMovimiento == "3")
    {
      break;
    }
  }
  
}

void controlAutonomo()
{
  while (true)
  {
    if (reconocimiento() == "3")
    {
      break;
    }
  }
}

void controlBluetooth()
{
  while (true)
  {
    if (Serial.available() > 0) 
    {
      comando = (char)Serial.read();
      
      if (comando == '3') frente();
      else if (comando == '4') retroceder();
      else if (comando == '1') girarDerecha();
      else if (comando == '2') girarIzquierda();
      else if (comando == '5') detener();
    }
    if (reconocimiento() == "3")
    {
      break;
    }
  }
}

void activacionRFID()
{

}

String reconocimiento() {
  unsigned long valor = IrReceiver.decodedIRData.decodedRawData;
  
  // Si el valor es 0, a veces es un error de lectura o tecla repetida
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
// =====================================================


void frente() {
  digitalWrite(motor1A, HIGH);
  analogWrite(motor1B, velocidadMax);
  digitalWrite(motor2A, LOW);
  analogWrite(motor2B, velocidadMax);
}

void retroceder() {
  digitalWrite(motor1A, LOW);
  analogWrite(motor1B, velocidadMax);
  digitalWrite(motor2A, HIGH);
  analogWrite(motor2B, velocidadMax);
}

void girarDerecha() {
  digitalWrite(motor1A, HIGH);
  analogWrite(motor1B, velocidadMax);
  digitalWrite(motor2A, HIGH);
  analogWrite(motor2B, velocidadMax);
}

void girarIzquierda() {
  digitalWrite(motor1A, LOW);
  analogWrite(motor1B, velocidadMax);
  digitalWrite(motor2A, LOW);
  analogWrite(motor2B, velocidadMax);
}

void detener() {
  digitalWrite(motor1A, LOW);
  analogWrite(motor1B, 0);
  digitalWrite(motor2A, LOW);
  analogWrite(motor2B, 0);
}