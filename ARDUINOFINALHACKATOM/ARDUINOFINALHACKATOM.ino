/*
   --------------------------------------------------------------------------------------------------------------------
   Example sketch/program showing how to read new NUID from a PICC to serial.
   --------------------------------------------------------------------------------------------------------------------
   This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid

   Example sketch/program showing how to the read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
   Reader on the Arduino SPI interface.

   When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
   then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
   you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
   will show the type, and the NUID if a new card has been detected. Note: you may see "Timeout in communication" messages
   when removing the PICC from reading distance too early.

   @license Released into the public domain.

   Typical pin layout used:
   -----------------------------------------------------------------------------------------
               MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
               Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin          Pin           Pin       Pin        Pin              Pin
   -----------------------------------------------------------------------------------------
   RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
   SPI SS      SDA(SS)      10            53        D10        10               10
   SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
   SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
   SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
*/

#include <SPI.h>
#include <MFRC522.h>
#include <Sensorino.h>
#include <LiquidCrystal.h>

//#define DEBUG
//#define SHOW_ON_MONITOR



#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

LiquidCrystal lcd(6, 7, 5, 4, 3, 2);

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];
bool flag = 0;
bool flag2 = 0;

String content = "";
String last = "";
String bentotag = "50 3E 7B 26";
String whitetag = "13 0E 40 D5";
String vemtag = "B1 8D E6 05";

float valorW = 0;
float valorB = 0;
float valorV = 0;

US us1(8, A4);
US us0(A2, A3);

void setup() {
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  lcd.begin(16, 2);
#ifdef DEBUG
  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
#endif
}

void loop() {
  if (!flag2) {
    lcd.print("Aproxime o vem:");
    flag2 = true;
  }

  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  flag2 = false;
  lcd.clear();

#ifdef DEBUG
  Serial.print(F("PICC type: "));
#endif
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
#ifndef DEBUG
  rfid.PICC_GetTypeName(piccType);
#endif
#ifdef DEBUG
  Serial.println(rfid.PICC_GetTypeName(piccType));
#endif



  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
#ifdef DEBUG
    Serial.println(F("Your tag is not of type MIFARE Classic."));
#endif
    return;
  }


#ifdef DEBUG
  Serial.print("Ulstrassonic0:\t");
  Serial.print(us0.getDistance(), DEC);
  Serial.print("\tUlstrassonic1:\t");
  Serial.println(us1.getDistance(), DEC);
#endif
  lcd.clear();
  lcd.print("Esperando lixo!");
  while (!flag)
  {
#ifdef DEBUG2
    Serial.println("Waiting for the tresh");
#endif
    if (us0.getDistance() < 41 || us1.getDistance() < 41)
    {
      lcd.clear();
      lcd.print("Lixo detectado!");
      delay(2000);
      lcd.clear();
      lcd.print("Muito Obrigad@!");
      delay(2000);
      lcd.clear();
      lcd.print("Voce ganhou:");
      lcd.setCursor(0, 1);
      lcd.print(" 15 cents no VEM");
      delay(4000);
      lcd.clear();

#ifdef DEBUG3
      Serial.print("O primeiro: ");
      Serial.print(rfid.uid.uidByte[0]);
      Serial.print(" ");
      Serial.print(rfid.uid.uidByte[1]);
      Serial.print(" ");
      Serial.print(rfid.uid.uidByte[2]);
      Serial.print(" ");
      Serial.print(rfid.uid.uidByte[3]);
      Serial.println(" ");

      Serial.print("O segundo: ");
      Serial.print(nuidPICC[0]);
      Serial.print(" ");
      Serial.print(nuidPICC[1]);
      Serial.print(" ");
      Serial.print(nuidPICC[2]);
      Serial.print(" ");
      Serial.print(nuidPICC[3]);
      Serial.println(" ");
#endif

      if (rfid.uid.uidByte[0] != nuidPICC[0] ||
          rfid.uid.uidByte[1] != nuidPICC[1] ||
          rfid.uid.uidByte[2] != nuidPICC[2] ||
          rfid.uid.uidByte[3] != nuidPICC[3] ) {

        for (byte i = 0; i < 4; i++) {
          nuidPICC[i] = rfid.uid.uidByte[i];
        }
        content = "";
        for (byte i = 0; i < rfid.uid.size; i++)
        {
          content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
          content.concat(String(rfid.uid.uidByte[i], HEX));
        }

        content.toUpperCase();

        last = content.substring(1);

        if (last == whitetag) //MR WHITE
        {
          valorW = valorW + 0.15;
          Serial.println("MR_White");
          Serial.println(valorW);
#ifdef SHOW_ON_MONITOR
          Serial.print("Bem Vindo MR White, voce tem:");
          Serial.print(valorW);
          Serial.println(" Reais");
#endif
        }

        if (last == vemtag) //MR WHITE
        {
          valorV = valorV + 0.15;
          Serial.println("Otacilio");
          Serial.println(valorV);
#ifdef SHOW_ON_MONITOR
          Serial.print("Bem Vindo Otacilio, voce tem:");
          Serial.print(valorV);
          Serial.println(" Reais");
#endif
        }

        if (last == bentotag) //MR WHITE
        {
          valorB = valorB + 0.15;
          Serial.println("Bento");
          Serial.println(valorB);
#ifdef SHOW_ON_MONITOR
          Serial.print("Bem Vindo Bento, voce tem:");
          Serial.print(valorB);
          Serial.println(" Reais");
#endif
        }
      }
      else {
        if (last == whitetag) //MR WHITE
        {
          valorW = valorW + 0.15;
          Serial.println("MR_White");
          Serial.println(valorW);
#ifdef SHOW_ON_MONITOR
          Serial.print("Bem Vindo MR White, voce tem:");
          Serial.print(valorW);
          Serial.println(" Reais");
#endif
        }

        if (last == vemtag) //MR WHITE
        {
          valorV = valorV + 0.15;
          Serial.println("Otacilio");
          Serial.println(valorV);
#ifdef SHOW_ON_MONITOR
          Serial.print("Bem Vindo Otacilio, voce tem:");
          Serial.print(valorV);
          Serial.println(" Reais");
#endif
        }

        if (last == bentotag) //MR WHITE
        {
          valorB = valorB + 0.15;
          Serial.println("Bento");
          Serial.println(valorB);
#ifdef SHOW_ON_MONITOR
          Serial.print("Bem Vindo Bento, voce tem:");
          Serial.print(valorB);
          Serial.println(" Reais");
#endif
        }
      }




#ifdef DEBUG
      Serial.print("last:");
      Serial.println(last);
      Serial.println(whitetag);
      printHex(rfid.uid.uidByte, rfid.uid.size);
      Serial.println(" ");
      Serial.println("Tresh detected");
#endif
      flag = true;
    }
  }

  flag = false;


  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}


/**
   Helper routine to dump a byte array as hex values to Serial.
*/
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
   Helper routine to dump a byte array as dec values to Serial.
*/
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
