#include <LiquidCrystal.h> // Biblioteka obsługująca LCD
#include <Keypad.h>        // Biblioteka obsługująca klawiaturę

// Inicjalizacja LCD (piny RS, EN, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Definicja rozmiaru klawiatury (4 wiersze, 4 kolumny)
const byte ROWS = 4;
const byte COLS = 4;

// Mapowanie klawiszy klawiatury 4x4
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
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

// Funkcja do odtwarzania dźwięku dla różnych klawiszy i zwracania odpowiadającej litery
char playToneAndGetNoteName(char key) {
  int frequency = 0;
  char note = '?';

  // Przypisanie częstotliwości dźwięku do klawiszy
  switch (key) {
    case '1': frequency = 262; note = 'C'; break; // C (Do)
    case '2': frequency = 294; note = 'D'; break; // D (Re)
    case '3': frequency = 330; note = 'E'; break; // E (Mi)
    case '4': frequency = 349; note = 'F'; break; // F (Fa)
    case '5': frequency = 392; note = 'G'; break; // G (Sol)
    case '6': frequency = 440; note = 'A'; break; // A (La)
    case '7': frequency = 494; note = 'B'; break; // B (Si)
    case '8': frequency = 523; note = 'C'; break; // C (Do wysokie)
    case '9': frequency = 587; note = 'D'; break; // D (Re wysokie)
    case '0': frequency = 659; note = 'E'; break; // E (Mi wysokie)
    case 'A': frequency = 698; note = 'F'; break; // F (Fa wysokie)
    case 'B': frequency = 784; note = 'G'; break; // G (Sol wysokie)
    case 'C': frequency = 880; note = 'A'; break; // A (La wysokie)
    case 'D': frequency = 988; note = 'B'; break; // B (Si wysokie)
    case '*': frequency = 1047; note = '*'; break; // Specjalny dźwięk
    case '#': frequency = 1175; note = '#'; break; // Specjalny dźwięk
    default: frequency = 0; note = '?'; break;    // Brak dźwięku dla nieznanego klawisza
  }

  if (frequency > 0) {
    tone(buzzerPin, frequency, 300); // Graj dźwięk przez 300 ms
  }

  return note;
}

void setup() {
  // Inicjalizacja LCD
  lcd.begin(16, 2); // Wyświetlacz 16x2
  lcd.setCursor(0, 0);
  lcd.print("Nacisnij klawisz");

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

    // Dodaj literę do sekwencji
    if (sequence.length() < 16) { // Maksymalna długość jednej linii LCD
      sequence += note;
      sequence += " "; // Dodaj spacje między literami
    } else {
      sequence = note; // Rozpocznij nową linię sekwencji
      sequence += " ";
    }

    // Wyświetl sekwencję na LCD
    lcd.clear();                 // Wyczyść ekran
    lcd.setCursor(0, 0);         // Ustaw kursor na początku
    lcd.print("Sekwencja:");     // Tekst informacyjny
    lcd.setCursor(0, 1);         // Przejdź do drugiego wiersza
    lcd.print(sequence);         // Wyświetl sekwencję

    delay(500); // Krótkie opóźnienie, aby zobaczyć efekt
  }
}
