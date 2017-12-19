/*
   Empfänger des Kiwi Projekts "Infrarot Übertragung"
   Matrix-Text-Code von https://github.com/riyas-org/max7219/blob/master/MaxMatrix/example/max7219test_scrol.ino
*/
#include "MaxMatrix.h"
#include "IRremote.h"
#include "avr/pgmspace.h"

// Alphabet definieren
PROGMEM const unsigned char CH[] = {
  4, 8, B00111110, B01000001, B01000001, B00111110, B00000000, // 0
  3, 8, B01000010, B01111111, B01000000, B00000000, B00000000, // 1
  4, 8, B01100010, B01010001, B01001001, B01000110, B00000000, // 2
  4, 8, B00100010, B01000001, B01001001, B00110110, B00000000, // 3
  4, 8, B00011000, B00010100, B00010010, B01111111, B00000000, // 4
  4, 8, B00100111, B01000101, B01000101, B00111001, B00000000, // 5
  4, 8, B00111110, B01001001, B01001001, B00110000, B00000000, // 6
  4, 8, B01100001, B00010001, B00001001, B00000111, B00000000, // 7
  4, 8, B00110110, B01001001, B01001001, B00110110, B00000000, // 8
  4, 8, B00000110, B01001001, B01001001, B00111110, B00000000  // 9
};

const unsigned char leer[] = {8, 8, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};
const unsigned char startIMG[7][10] = {{8, 8, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000100, B00000000}, {8, 8, B00000000, B00000000, B00000000, B00000000, B00000000, B00001000, B00000100, B00000000}, {8, 8, B00000000, B00000000, B00000000, B00000000, B00010000, B00001000, B00000100, B00000000}, {8, 8, B00000000, B00000000, B00000000, B00100000, B00010000, B00001000, B00000100, B00000000}, {8, 8, B00000000, B00000000, B00010000, B00100000, B00010000, B00001000, B00000100, B00000000}, {8, 8, B00000000, B00001000, B00010000, B00100000, B00010000, B00001000, B00000100, B00000000}};
const unsigned char fehlerIMG[12][10] = {{B00000000, B00000010, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000}, { 8, 8, B00000000, B00000010, B00000100, B00000000, B00000000, B00000000, B00000000, B00000000}, { 8, 8, B00000000, B00000010, B00000100, B00001000, B00000000, B00000000, B00000000, B00000000}, { 8, 8, B00000000, B00000010, B00000100, B00001000, B00010000, B00000000, B00000000, B00000000}, { 8, 8, B00000000, B00000010, B00000100, B00001000, B00010000, B00100000, B00000000, B00000000}, { 8, 8, B00000000, B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B00000000}, { 8, 8, B00000000, B01000010, B00000100, B00001000, B00010000, B00100000, B01000000, B00000000}, { 8, 8, B00000000, B01000010, B00100100, B00001000, B00010000, B00100000, B01000000, B00000000}, { 8, 8, B00000000, B01000010, B00100100, B00011000, B00010000, B00100000, B01000000, B00000000}, { 8, 8, B00000000, B01000010, B00100100, B00011000, B00011000, B00100000, B01000000, B00000000}, { 8, 8, B00000000, B01000010, B00100100, B00011000, B00011000, B00100100, B01000000, B00000000}, { 8, 8, B00000000, B01000010, B00100100, B00011000, B00011000, B00100100, B01000010, B00000000}};

// Benötigte Zahlen als Chars
const char numbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

// RAM-Buffer für die aktuelle Zahlengrafik
byte buffer[10];
// Aktuelle Position in der Zahlenliste
int currpos = 0;
// Ob die Übertragung gestartet wurde oder nicht
bool gestartet = false;
// Zahlenliste
int toshow[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// "Reparierter" Zahlenwert
int rvalue = 0;
// Zeit des letzten Empfangs
// (Um zu überprüfen ob es sich um einen wirklichen Übertragungsfehler handelt)
int lasttime;
// Zeit zwische Zahlenwerten ab der ein Fehler gewertet wird
const int maxtime = 100;

// Matrix-Daten initialisieren
int data = 8;    // DIN pin of MAX7219 module
int load = 9;    //  CS pin of MAX7219 module
int clock = 10;  // CLK pin of MAX7219 module
MaxMatrix m(data, load, clock, 1);

// Infrarot Emfänger Daten initialisieren
IRrecv irrecv(11);
decode_results results;

/* FUNKTIONEN */

// Animation auf der Matrix anzeigen
void matrixAnim(byte frames[][10], int count, int ms) {
  for (int i = 0; i < count; i++) {
    m.writeSprite(0, 0, frames[i]);
    delay(ms);
  }
}

// Matrix um amount Schritte nach rechts scrollen
void matrixScroll(int amount, int shift_speed) {
  for (int i = 0; i < amount; i++) {
    delay(shift_speed);
    m.shiftLeft(false, false);
  }
}

// Buchstabe auf der Matrix anzeigen und nach links wescrollen
void matrixLetter(char c, int shift_speed) {
  if (c < 32) return;
  c -= 32;
  memcpy_P(buffer, CH + 7 * c, 7);
  m.writeSprite(8, 0, buffer);
  m.setColumn(8 + buffer[0], 0);
  m.writeSprite(16, 0, leer);
  matrixScroll(buffer[0] + 1, shift_speed);
}

// String auf der Matrix als Scrolling-Text anzeigen
void matrixString(char* s, int shift_speed) {
  while (*s != 0) {
    matrixLetter(*s, shift_speed);
    s++;
  }
  matrixScroll(7, shift_speed);
}

// X - Animation auf Matrix anzeigen
void fehlerAnim() {
  matrixAnim(fehlerIMG, 12, 100);
  m.writeSprite(8, 0, leer);
  matrixScroll(8, 200);
  m.writeSprite(0, 0, leer);
}

// Die Zahlenliste auf der Matrix ausgeben
void matrixNumArr(int *arr) {
  for (int i = 0; i < 10; i++) {
    if (arr[i] == 10) {
      i = 10;
    } else {
      matrixLetter(numbers[arr[i]], 100);
    }
  }
  m.writeSprite(8, 0, leer);
  matrixScroll(7, 100);
  m.writeSprite(0, 0, leer);
}

// Zahlenliste löschen
void lZahlen(int *currpos, bool *gestartet, int (*zahlen)[10]) {
  *currpos = 0;
  *gestartet = false;
  for (int i = 0; i < 10; i++) {
    (*zahlen)[i] = 0;
  }
}

/* PROGRAMM */

void setup() {
  Serial.begin(9600);
  Serial.println("Gestartet");

  // Matrix initialisieren
  m.init();
  m.setIntensity(0);

  // Empfänger initialisieren
  irrecv.enableIRIn();

  // Matrixausgabe
  matrixAnim(startIMG, 7, 100);
  matrixScroll(8, 100);

  lasttime = millis();
}

void loop() {
  if (irrecv.decode(&results)) {
    rvalue = results.value;
    // Zurückwandeln der Zahlen, die sonst falsch übertragen wurden (ja, ja, es ist ziemlich hacky)
    switch (rvalue) {
      case 21: rvalue = 1; break;
      case 23: rvalue = 3; break;
      case 27: rvalue = 7; break;
      case 20: rvalue = 0; break;
    }
    if (rvalue == 12) {
      Serial.println("---START---");
      lZahlen(&currpos, &gestartet, &toshow);
      gestartet = true;
    } else if (rvalue == 13) {
      Serial.println("---ENDE---");
      if (currpos != 10 && gestartet) {
        fehlerAnim();
        Serial.println("Fehler: Nicht alle Zahlen erhalten");
      } else {
        matrixNumArr(toshow);
        Serial.println("Erfolgreich");
      }
      lZahlen(&currpos, &gestartet, &toshow);
    } else {
      Serial.print("Wert: ");
      Serial.print(rvalue);
      Serial.print(" Typ: ");
      Serial.print(results.decode_type);
      Serial.println("");
      if (millis() - lasttime < maxtime) {
        if (!gestartet) {
          Serial.println("Fehler: Nicht gestartet");
          fehlerAnim();
          lZahlen(&currpos, &gestartet, &toshow);
        } else if (currpos > 9 || rvalue > 10) {
          Serial.println("Fehler: Zu viele Zahlen oder kaputter Wert");
          fehlerAnim();
          lZahlen(&currpos, &gestartet, &toshow);
        } else {
          toshow[currpos] = rvalue;
          currpos++;
        }
      } else {
        Serial.println("Fehler: Zu langer Zeitabstand zwischen übertragenen Zahlen");
        lZahlen(&currpos, &gestartet, &toshow);
      }
    }
    irrecv.resume();
  }
}
