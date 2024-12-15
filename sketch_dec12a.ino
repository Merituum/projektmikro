#include <LiquidCrystal.h> // Biblioteka obsługująca LCD
#include <Keypad.h>        // Biblioteka obsługująca klawiaturę

// Inicjalizacja LCD (piny RS, EN, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Definicja rozmiaru klawiatury (4 wiersze, 4 kolumny)
const byte ROWS = 4;
const byte COLS = 4;

// Mapowanie klawiszy klawiatury 4x4
char keys[ROWS][COLS] = {
  {'*', '0', '#', 'Z'}, //'*' - reset, 0 - Zagraj od początku, # zwiększenie tonacji, 'Z' - zmniejszenie tonacji
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

// Funkcja do odtwarzania dźwięku i zwracania nazwy nuty w literach
char playToneAndGetNoteName(char key) {
  int baseFrequencies[] = {262, 294, 330, 349, 392, 440, 494, 523, 587, 698, 784, 880};
  char note = '?';

  // Wylicz częstotliwość na podstawie klawisza i przypisz odpowiednią nazwę nuty
  int index = -1;
  if (key >= '1' && key <= '9') {
    index = key - '1'; // Mapowanie 1-9 na indeksy 0-8
  } else if (key == 'A') {
    index = 3; // F
    note = 'F';
  } else if (key == 'B') {
    index = 4; // G
    note = 'G';
  } else if (key == 'C') {
    index = 5; // A
    note = 'A';
  }

  if (index != -1) {
    int frequency = baseFrequencies[index] + toneShift;
    note = "CDEFGABCDFA"[index]; // Pobierz nazwę nuty z tablicy
    tone(buzzerPin, frequency, 500); // Graj dźwięk przez 500 ms
    delay(500); // Dłuższe wybrzmiewanie dźwięku
    noTone(buzzerPin); // Wyłączenie dźwięku
    delay(100); // Przerwa między dźwiękami
  }

  return note;
}

void playSequence() {
  lcd.clear();
  lcd.setCursor(0, 0);
  // lcd.print("Odtwarzanie...");

  for (int i = 0; i < sequence.length(); i++) {
    char currentNote = sequence[i];
    if (currentNote != ' ') { // Ignoruj spacje w sekwencji
      char noteName = playToneAndGetNoteName(currentNote);
      lcd.setCursor(0, 1);
      lcd.print("Nuta: ");
      lcd.print(noteName);
      delay(600); // Krótkie opóźnienie na wyświetlanie nazwy nuty
    }
  }
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("Koniec!       "); // Wyczyść linię po zakończeniu
  delay(1000);
}

void setup() {
  // Inicjalizacja LCD
  lcd.begin(16, 2); // Wyświetlacz 16x2
  lcd.setCursor(0, 0);
  lcd.print("Gotowy!");

  // Ustawienie pinu buzzera jako wyjście
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Odczyt naciśnięcia klawisza
  char key = keypad.getKey();

  // Jeśli klawisz został naciśnięty
  if (key) {
    if (key == '*') { // Resetowanie
      sequence = "";
      toneShift = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Reset sekwencji");
      delay(1000);
    } else if (key == '0') { // Odtwarzanie sekwencji
      playSequence();
    } else if (key == '#') { // Zwiększenie tonacji
      toneShift += 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Zwiekszono ton");
      lcd.setCursor(0, 1);
      lcd.print("Shift: ");
      lcd.print(toneShift);
      delay(1000);
    } else if (key == 'Z') { // Zmniejszenie tonacji
      toneShift -= 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Zmniejszono ton");
      lcd.setCursor(0, 1);
      lcd.print("Shift: ");
      lcd.print(toneShift);
      delay(1000);
    } else { // Dodanie klawisza do sekwencji
      char noteName = playToneAndGetNoteName(key); // Graj dźwięk i pobierz nazwę nuty
      if (noteName != '?') {
        
        sequence += key; // Dodaj do sekwencji
        sequence += " "; // Dodaj spację dla czytelności

        // Wyświetl sekwencję na LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        // lcd.print("Sekwencja:");
        lcd.setCursor(0, 1);
        lcd.print(sequence);
      }
    }
  }
}
