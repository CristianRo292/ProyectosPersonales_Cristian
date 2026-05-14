#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN  10

MFRC522 mfrc522(SS_PIN, RST_PIN);
byte llaveD3[] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println(F(">>> LECTURA TOTAL DE DATOS (HASTA BLOQUE 15) <<<"));
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = llaveD3[i];

  Serial.println(F("\n--- INICIO DE CADENA RECONSTRUIDA ---"));

  // Recorremos del bloque 4 al 14 (el 15 es Trailer y se omite)
  for (byte b = 4; b < 15; b++) {
    // Regla de Oro: Saltamos los bloques Trailer (7, 11)
    if ((b + 1) % 4 == 0) continue; 

    MFRC522::StatusCode status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, b, &key, &(mfrc522.uid));

    if (status == MFRC522::STATUS_OK) {
      byte buffer[18];
      byte size = sizeof(buffer);
      if (mfrc522.MIFARE_Read(b, buffer, &size) == MFRC522::STATUS_OK) {
        // Imprimimos los 16 caracteres de este bloque
        for (byte i = 0; i < 16; i++) {
          if (buffer[i] >= 32 && buffer[i] <= 126) {
            Serial.print((char)buffer[i]);
          }
        }
      }
    }
  }

  Serial.println(F("\n--- FIN DEL MENSAJE ---"));
  
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  delay(10000); // Pausa de 10 segundos para que puedas copiar el texto
}