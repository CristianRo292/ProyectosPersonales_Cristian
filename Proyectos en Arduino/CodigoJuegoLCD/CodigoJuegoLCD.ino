#include <LiquidCrystal.h>

// ==================== CONFIGURACIÓN DE PINES ====================
LiquidCrystal lcd(12, 11, 6, 5, 7, 3); // (RS, E, D4, D5, D6, D7)

const int pinJoyX = A0;   // Joystick eje X
const int pinJoyY = A1;   // Joystick eje Y
const int pinBoton = 2;   // Botón del joystick

// ==================== VARIABLES DEL JUGADOR ====================
int posX = 7;   // Posición X inicial (0-15)
int posY = 0;   // Posición Y inicial (0-1)

// ==================== OBJETOS DEL JUEGO ====================
// Muros (obstáculos fijos)
const int muro1X = 3, muro1Y = 0;
const int muro2X = 9, muro2Y = 1;

// Premio (colectible)
int premioX = 15, premioY = 1;

// Enemigo (se mueve automáticamente)
int enemigoX = 8, enemigoY = 1;
bool direccionDerecha = true;

// ⚡ CONTROL DE VELOCIDAD DEL ENEMIGO
int enemyMoveCounter = 0;
const int enemySpeed = 8;  // ⬅️ AJUSTA ESTO: Mayor = más lento (8 = ~400ms por movimiento)

// ==================== CONFIGURACIÓN ====================
const int umbral = 400;
int puntuacion = 0;

// ==================== PROTOTIPOS DE FUNCIÓN ====================
void dibujarObjeto(int x, int y, char caracter = '#');
void refrescarPantalla();
void movimientoEnemigo();
void reiniciarJuego();
void generarPremio();

// ==================== SETUP ====================
void setup() {
  lcd.begin(16, 2);
  pinMode(pinBoton, INPUT_PULLUP);
  randomSeed(analogRead(3));
  
  lcd.setCursor(0, 0);
  lcd.print("  JUEGO LCD  ");
  lcd.setCursor(0, 1);
  lcd.print(" Joy:Move Btn:R ");
  delay(2000);
  
  reiniciarJuego();
}

// ==================== LOOP PRINCIPAL ====================
void loop() {
  // 🔹 1. Leer joystick (siempre responsive)
  int valorX = analogRead(pinJoyX);
  int valorY = analogRead(pinJoyY);
  int estadoBoton = digitalRead(pinBoton);

  // 🔹 2. Calcular nueva posición tentativa del jugador
  int nuevoX = posX;
  int nuevoY = posY;

  if (valorX < umbral) nuevoX--;
  else if (valorX > (1023 - umbral)) nuevoX++;
  
  if (valorY < umbral) nuevoY--;
  else if (valorY > (1023 - umbral)) nuevoY++;

  nuevoX = constrain(nuevoX, 0, 15);
  nuevoY = constrain(nuevoY, 0, 1);

  // 🔹 3. Colisión con MUROS
  bool chocoMuro = (nuevoX == muro1X && nuevoY == muro1Y) || 
                   (nuevoX == muro2X && nuevoY == muro2Y);
  
  if (!chocoMuro) {
    posX = nuevoX;
    posY = nuevoY;
  }

  // 🔹 4. Colisión con PREMIO
  if (posX == premioX && posY == premioY) {
    puntuacion++;
    generarPremio();
  }

  // 🔹 5. Colisión con ENEMIGO → Game Over
  if (posX == enemigoX && posY == enemigoY) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  GAME OVER  ");
    lcd.setCursor(0, 1);
    lcd.print(" Pts: ");
    lcd.print(puntuacion);
    delay(2000);
    reiniciarJuego();
  }

  // 🔹 6. Dibujar escena
  refrescarPantalla();
  dibujarObjeto(posX, posY, 'O');

  // 🔹 7. Botón: Reiniciar
  if (estadoBoton == LOW) {
    reiniciarJuego();
    delay(300);
  }

  delay(50);  // Loop estable (joystick responsive)
}

// ==================== FUNCIONES AUXILIARES ====================

void dibujarObjeto(int x, int y, char caracter) {
  lcd.setCursor(x, y);
  lcd.print(caracter);
}

void refrescarPantalla() {
  lcd.clear();
  
  dibujarObjeto(muro1X, muro1Y, '#');
  dibujarObjeto(muro2X, muro2Y, '#');
  dibujarObjeto(premioX, premioY, '+');
  
  movimientoEnemigo();  // Aquí se aplica la velocidad lenta
  
  lcd.setCursor(12, 0);
  lcd.print("P:");
  lcd.print(puntuacion);
}

// 👾 MOVIMIENTO DEL ENEMIGO CON VELOCIDAD CONTROLADA
void movimientoEnemigo() {
  enemyMoveCounter++;  // Contar frames
  
  // Solo mover al enemigo cada 'enemySpeed' frames
  if (enemyMoveCounter >= enemySpeed) {
    enemyMoveCounter = 0;  // Reset contador
    
    // Cambiar dirección en extremos
    if (enemigoX >= 14) direccionDerecha = false;
    if (enemigoX <= 2) direccionDerecha = true;
    
    // Mover enemigo
    enemigoX += direccionDerecha ? 1 : -1;
  }
  
  // Dibujar enemigo siempre (aunque no se haya movido)
  dibujarObjeto(enemigoX, enemigoY, 'E');
}

void generarPremio() {
  int nuevoX, nuevoY;
  do {
    nuevoX = random(0, 16);
    nuevoY = random(0, 2);
  } while (
    (nuevoX == muro1X && nuevoY == muro1Y) ||
    (nuevoX == muro2X && nuevoY == muro2Y) ||
    (nuevoX == posX && nuevoY == posY) ||
    (nuevoX == enemigoX && nuevoY == enemigoY)
  );
  premioX = nuevoX;
  premioY = nuevoY;
}

void reiniciarJuego() {
  posX = 7; posY = 0;
  enemigoX = 8; enemigoY = 1;
  direccionDerecha = true;
  enemyMoveCounter = 0;  // Reset contador del enemigo
  puntuacion = 0;
  generarPremio();
  refrescarPantalla();
  dibujarObjeto(posX, posY, 'O');
}