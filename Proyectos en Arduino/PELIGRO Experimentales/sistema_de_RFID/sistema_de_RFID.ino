#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define RST_PIN         9          
#define SS_PIN          10         
#define LED_PIN         7

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); 

MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  
  pinMode(LED_PIN, OUTPUT);
  
  lcd.init();
  lcd.backlight();

  // Cargamos la llave estándar (FF FF FF FF FF FF)
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  resetPantalla();
}

void loop() {
  // 1. Buscar tarjetas
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  digitalWrite(LED_PIN, HIGH);
  lcd.clear();
  lcd.print("Leyendo...");

  // 2. Intentar leer Bloque 4
  byte bloque = 4;
  byte buffer[18];
  byte tamano = sizeof(buffer);
  
  // Autenticación obligatoria para MIFARE
  MFRC522::StatusCode status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, bloque, &key, &(mfrc522.uid));
  
  if (status == MFRC522::STATUS_OK) {
    status = mfrc522.MIFARE_Read(bloque, buffer, &tamano);
    
    if (status == MFRC522::STATUS_OK) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Datos:");
      lcd.setCursor(0, 1);
      
      String datos = "";
      for (uint8_t i = 0; i < 16; i++) {
        // Solo mostrar caracteres que se puedan leer (letras/números)
        if (buffer[i] >= 32 && buffer[i] <= 126) {
          datos += (char)buffer[i];
        }
      }
      
      if(datos.length() == 0) lcd.print("[Vacio/Nulo]");
      else lcd.print(datos);
      
      Serial.println("Leido: " + datos);
    } else {
      lcd.print("Error Lectura");
    }
  } else {
    lcd.print("Error Llave/Auth");
    Serial.println(mfrc522.GetStatusCodeName(status));
  }

  // 3. Pausa y reset
  delay(3000);
  digitalWrite(LED_PIN, LOW);
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  resetPantalla();
}

void resetPantalla() {
  lcd.clear();
  lcd.print("Lector de Datos");
  lcd.setCursor(0, 1);
  lcd.print("Acerque Tag");
}