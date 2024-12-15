#include <LiquidCrystal.h> // Biblioteka obsługująca LCD
#include <Keypad.h>        // Biblioteka obsługująca klawiaturę

// Inicjalizacja LCD (piny RS, EN, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Definicja rozmiaru klawiatury (4 wiersze, 4 kolumny)
const byte ROWS = 4;
const byte COLS = 4;

// Mapowanie klawiszy klawiatury 4x4
char keys[ROWS][COLS] = {
  {'*', '0', '#', 'D'}, //'*' - reset, 0 - Zagraj od początku, # zwiększenie tonacji, 'D' - zmniejszenie tonacji
  {'1', '2', '3', 'A'}, // C, D, E, F
  {'4', '5', '6', 'B'}, // F, G, A, G (wysokie Sol)
  {'7', '8', '9', 'C'}  // B, C, D, A
};

// Piny wierszy i kolumn klawiatury podłączone do Arduino
byte rowPins[ROWS] = {6, 7, 8, 9};    // Piny wierszy
byte colPins[COLS] = {10, 13, A1, A2}; // Piny kolumn

// Inicjalizacja klawiatury
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Pin podłączony do buzzera
const int buzzerPin = A0;

// Tablica do przechowywania sekwencji klawiszy
String sequence = "";

// Bieżąca tonacja
int toneShift = 0;

// Funkcja do odtwarzania dźwięku dla różnych klawiszy i zwracania odpowiadającej litery
char playToneAndGetNoteName(char key) {
  int baseFrequencies[] = {262, 294, 330, 349, 392, 440, 494, 523, 587, 698, 784, 880};
  char note = '?';

  switch (key) {
    case '1': case '2': case '3': case '4': case '5': case '6':
    case '7': case '8': case '9': case 'A': case 'B': case 'C': {
      int index = (key == 'A') ? 3 : (key == 'B') ? 4 : (key == 'C') ? 5 : (key - '1');
      int frequency = baseFrequencies[index] + toneShift;
      note = "CDEFGABCDFA"[index];
      tone(buzzerPin, frequency, 300); // Graj dźwięk przez 300 ms
      break;
    }
    case '*':
      sequence = ""; // Resetowanie sekwencji
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Reset sekwencji");
      delay(1000);
      break;

    case '0': {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Odtwarzanie");
      for (int i = 0; i < sequence.length(); i++) {
        if (sequence[i] != ' ') {
          playToneAndGetNoteName(sequence[i]);
          delay(500);
        }
      }
      break;
    }

    case '#':
      toneShift += 80; // Zwiększ tonację
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Zwiekszono ton");
      delay(1000);
      break;

    case 'D':
      toneShift -= 10; // Zmniejsz tonację
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Zmniejszono ton");
      delay(1000);
      break;

    default:
      break;
  }

  return note;
}

void setup() {
  // Inicjalizacja LCD
  lcd.begin(16, 2); // Wyświetlacz 16x2
  lcd.setCursor(0, 0);
  // lcd.print("Nacisnij klawisz");

  // Ustawienie pinu buzzera jako wyjście
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Odczyt naciśnięcia klawisza
  char key = keypad.getKey();

  // Jeśli klawisz został naciśnięty
  if (key) {
    // Pobierz odpowiadającą literę dla klawisza
    char note = playToneAndGetNoteName(key);

    // Dodaj literę do sekwencji, jeśli nie jest to klawisz specjalny
    if (note != '?' && key != '*' && key != '0' && key != '#' && key != 'D') {
      if (sequence.length() < 16) { // Maksymalna długość jednej linii LCD
        sequence += note;
        sequence += " "; // Dodaj spacje między literami
      } else {
        sequence = note; // Rozpocznij nową linię sekwencji
        sequence += " ";
      }

      // Wyświetl sekwencję na LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      // lcd.print("Sekwencja:");
      lcd.setCursor(0, 1);
      lcd.print(sequence);
    }

    delay(500); // Krótkie opóźnienie, aby zobaczyć efekt
  }
}