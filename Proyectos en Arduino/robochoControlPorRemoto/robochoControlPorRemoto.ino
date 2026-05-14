// ------- PINES MOTORES (Basados en tu primer robot) -------
const int motor1A = 2; // Izquierdo A
const int motor1B = 5; // Izquierdo B (PWM)
const int motor2A = 4; // Derecho A
const int motor2B = 6; // Derecho B (PWM)

int velocidadMax = 200; 
char comando;

void setup() {
Filtre su búsqueda...
Tipo:

Todo
Tema:

Todo






  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  
  Serial.begin(9600); // Para el módulo Bluetooth
}

void loop() {
  if (Serial.available() > 0) {
    comando = (char)Serial.read();
    
    if (comando == '3') frente();
    else if (comando == '4') retroceder();
    else if (comando == '1') girarDerecha();
    else if (comando == '2') girarIzquierda();
    else if (comando == '5') detener();
  }
}

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
