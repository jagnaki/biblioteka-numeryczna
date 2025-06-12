# biblioteka-numeryczna

## Opis projektu

Projekt realizowany w ramach przedmiotu **Metody Numeryczne**.

Program stanowi interaktywne narzędzie konsolowe umożliwiające wykonywanie podstawowych operacji numerycznych. Program pobiera dane z plików wejściowych, które użytkownik wybiera z poziomu menu, i umożliwia wybór konkretnej funkcji matematycznej do analizy.

Biblioteka zawiera następujące funkcje numeryczne, pogrupowane według kategorii:

### 🔹 Interpolacja
- Interpolacja Lagrange'a  
- Interpolacja Newtona  

### 🔹 Rozwiązywanie układów równań liniowych
- Metoda Gaussa  
- Metoda LU  

### 🔹 Całkowanie numeryczne  
Funkcje porównujące metody:
- Metoda prostokątów  
- Metoda trapezów  
- Metoda Simpsona  
- Kwadratury Gaussa-Legendre’a (GL)  

### 🔹 Rozwiązywanie równań różniczkowych  
Funkcja porównująca metody:
- Metoda Eulera  
- Metoda Heuna  
- Metoda punktu środkowego  
- Metoda Rungego-Kutty IV rzędu  

### 🔹 Aproksymacja

### 🔹 Rozwiązywanie równań nieliniowych  
Funkcja porównująca metody:
- Metoda bisekcji  
- Metoda Newtona  
- Metoda siecznych  
---
## Przykład użycia:

1. Uruchom program.
2. W menu wybierz opcję, np. `1` — Interpolacja Newtona.
3. Z menu dostępnych plików wybierz odpowiedni plik dla wybranej metody.
4. Wybierz numer porządkowy zestawu danych.
5. Program wykona interpolację i wyświetli wynik.
---

## Struktura katalogów
biblioteka-numeryczna/

├── include/    # Pliki nagłówkowe

├── src/        # Kod źródłowy

├── tests/      # Testy

├── CMakeLists.txt

├── main.cpp

└──  README.md

---

## Tworzenie i uruchamianie w CLion

Projekt był rozwijany i testowany w środowisku [CLion](https://www.jetbrains.com/clion/).

Aby rozpocząć pracę:

1. Otwórz CLion.
2. Wybierz opcję **Open** i wskaż główny katalog projektu.
3. Zbuduj i uruchom projekt bezpośrednio z poziomu CLion.

**Alternatywa:**  
Można również kompilować i uruchamiać projekt z poziomu terminala za pomocą `CMake` i kompilatora C++.

---

## Wymagania

- Kompilator C++ (np. `g++`)  
- `CMake >= 3.31`
