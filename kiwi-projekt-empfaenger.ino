/*
   Empfänger des Kiwi Projekts "Infrarot Übertragung"
   Matrix-Text-Code von https://github.com/riyas-org/max7219/blob/master/MaxMatrix/example/max7219test_scrol.ino
*/
#include "MaxMatrix.h"
#include "IRremote.h"
#include "avr/pgmspace.h"

// Alphabet definieren
PROGMEM const unsigned char CH[] = {
  3, 8, B00000000, B00000000, B00000000, B00000000, B00000000, // space
  1, 8, B01011111, B00000000, B00000000, B00000000, B00000000, // !
  3, 8, B00000011, B00000000, B00000011, B00000000, B00000000, // "
  5, 8, B00010100, B00111110, B00010100, B00111110, B00010100, // #
  4, 8, B00100100, B01101010, B00101011, B00010010, B00000000, // $
  5, 8, B01100011, B00010011, B00001000, B01100100, B01100011, // %
  5, 8, B00110110, B01001001, B01010110, B00100000, B01010000, // &
  1, 8, B00000011, B00000000, B00000000, B00000000, B00000000, // '
  3, 8, B00011100, B00100010, B01000001, B00000000, B00000000, // (
  3, 8, B01000001, B00100010, B00011100, B00000000, B00000000, // )
  5, 8, B00101000, B00011000, B00001110, B00011000, B00101000, // *
  5, 8, B00001000, B00001000, B00111110, B00001000, B00001000, // +
  2, 8, B10110000, B01110000, B00000000, B00000000, B00000000, // ,
  4, 8, B00001000, B00001000, B00001000, B00001000, B00000000, // -
  2, 8, B01100000, B01100000, B00000000, B00000000, B00000000, // .
  4, 8, B01100000, B00011000, B00000110, B00000001, B00000000, // /
  4, 8, B00111110, B01000001, B01000001, B00111110, B00000000, // 0
  3, 8, B01000010, B01111111, B01000000, B00000000, B00000000, // 1
  4, 8, B01100010, B01010001, B01001001, B01000110, B00000000, // 2
  4, 8, B00100010, B01000001, B01001001, B00110110, B00000000, // 3
  4, 8, B00011000, B00010100, B00010010, B01111111, B00000000, // 4
  4, 8, B00100111, B01000101, B01000101, B00111001, B00000000, // 5
  4, 8, B00111110, B01001001, B01001001, B00110000, B00000000, // 6
  4, 8, B01100001, B00010001, B00001001, B00000111, B00000000, // 7
  4, 8, B00110110, B01001001, B01001001, B00110110, B00000000, // 8
  4, 8, B00000110, B01001001, B01001001, B00111110, B00000000, // 9
  2, 8, B01010000, B00000000, B00000000, B00000000, B00000000, // :
  2, 8, B10000000, B01010000, B00000000, B00000000, B00000000, // ;
  3, 8, B00010000, B00101000, B01000100, B00000000, B00000000, // <
  3, 8, B00010100, B00010100, B00010100, B00000000, B00000000, // =
  3, 8, B01000100, B00101000, B00010000, B00000000, B00000000, // >
  4, 8, B00000010, B01011001, B00001001, B00000110, B00000000, // ?
  5, 8, B00111110, B01001001, B01010101, B01011101, B00001110, // @
  4, 8, B01111110, B00010001, B00010001, B01111110, B00000000, // A
  4, 8, B01111111, B01001001, B01001001, B00110110, B00000000, // B
  4, 8, B00111110, B01000001, B01000001, B00100010, B00000000, // C
  4, 8, B01111111, B01000001, B01000001, B00111110, B00000000, // D
  4, 8, B01111111, B01001001, B01001001, B01000001, B00000000, // E
  4, 8, B01111111, B00001001, B00001001, B00000001, B00000000, // F
  4, 8, B00111110, B01000001, B01001001, B01111010, B00000000, // G
  4, 8, B01111111, B00001000, B00001000, B01111111, B00000000, // H
  3, 8, B01000001, B01111111, B01000001, B00000000, B00000000, // I
  4, 8, B00110000, B01000000, B01000001, B00111111, B00000000, // J
  4, 8, B01111111, B00001000, B00010100, B01100011, B00000000, // K
  4, 8, B01111111, B01000000, B01000000, B01000000, B00000000, // L
  5, 8, B01111111, B00000010, B00001100, B00000010, B01111111, // M
  5, 8, B01111111, B00000100, B00001000, B00010000, B01111111, // N
  4, 8, B00111110, B01000001, B01000001, B00111110, B00000000, // O
  4, 8, B01111111, B00001001, B00001001, B00000110, B00000000, // P
  4, 8, B00111110, B01000001, B01000001, B10111110, B00000000, // Q
  4, 8, B01111111, B00001001, B00001001, B01110110, B00000000, // R
  4, 8, B01000110, B01001001, B01001001, B00110010, B00000000, // S
  5, 8, B00000001, B00000001, B01111111, B00000001, B00000001, // T
  4, 8, B00111111, B01000000, B01000000, B00111111, B00000000, // U
  5, 8, B00001111, B00110000, B01000000, B00110000, B00001111, // V
  5, 8, B00111111, B01000000, B00111000, B01000000, B00111111, // W
  5, 8, B01100011, B00010100, B00001000, B00010100, B01100011, // X
  5, 8, B00000111, B00001000, B01110000, B00001000, B00000111, // Y
  4, 8, B01100001, B01010001, B01001001, B01000111, B00000000, // Z
  2, 8, B01111111, B01000001, B00000000, B00000000, B00000000, // [
  4, 8, B00000001, B00000110, B00011000, B01100000, B00000000, // \ backslash
  2, 8, B01000001, B01111111, B00000000, B00000000, B00000000, // ]
  3, 8, B00000010, B00000001, B00000010, B00000000, B00000000, // hat
  4, 8, B01000000, B01000000, B01000000, B01000000, B00000000, // _
  2, 8, B00000001, B00000010, B00000000, B00000000, B00000000, // `
  4, 8, B00100000, B01010100, B01010100, B01111000, B00000000, // a
  4, 8, B01111111, B01000100, B01000100, B00111000, B00000000, // b
  4, 8, B00111000, B01000100, B01000100, B00101000, B00000000, // c
  4, 8, B00111000, B01000100, B01000100, B01111111, B00000000, // d
  4, 8, B00111000, B01010100, B01010100, B00011000, B00000000, // e
  3, 8, B00000100, B01111110, B00000101, B00000000, B00000000, // f
  4, 8, B10011000, B10100100, B10100100, B01111000, B00000000, // g
  4, 8, B01111111, B00000100, B00000100, B01111000, B00000000, // h
  3, 8, B01000100, B01111101, B01000000, B00000000, B00000000, // i
  4, 8, B01000000, B10000000, B10000100, B01111101, B00000000, // j
  4, 8, B01111111, B00010000, B00101000, B01000100, B00000000, // k
  3, 8, B01000001, B01111111, B01000000, B00000000, B00000000, // l
  5, 8, B01111100, B00000100, B01111100, B00000100, B01111000, // m
  4, 8, B01111100, B00000100, B00000100, B01111000, B00000000, // n
  4, 8, B00111000, B01000100, B01000100, B00111000, B00000000, // o
  4, 8, B11111100, B00100100, B00100100, B00011000, B00000000, // p
  4, 8, B00011000, B00100100, B00100100, B11111100, B00000000, // q
  4, 8, B01111100, B00001000, B00000100, B00000100, B00000000, // r
  4, 8, B01001000, B01010100, B01010100, B00100100, B00000000, // s
  3, 8, B00000100, B00111111, B01000100, B00000000, B00000000, // t
  4, 8, B00111100, B01000000, B01000000, B01111100, B00000000, // u
  5, 8, B00011100, B00100000, B01000000, B00100000, B00011100, // v
  5, 8, B00111100, B01000000, B00111100, B01000000, B00111100, // w
  5, 8, B01000100, B00101000, B00010000, B00101000, B01000100, // x
  4, 8, B10011100, B10100000, B10100000, B01111100, B00000000, // y
  3, 8, B01100100, B01010100, B01001100, B00000000, B00000000, // z
  3, 8, B00001000, B00110110, B01000001, B00000000, B00000000, // {
  1, 8, B01111111, B00000000, B00000000, B00000000, B00000000, // |
  3, 8, B01000001, B00110110, B00001000, B00000000, B00000000, // }
  4, 8, B00001000, B00000100, B00001000, B00000100, B00000000, // ~
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

// Matrix-Daten initialisieren
int data = 8;    // DIN pin of MAX7219 module
int load = 9;    // CS pin of MAX7219 module
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
  matrixScroll(7, 100);
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
    }
    irrecv.resume();
  }
}
