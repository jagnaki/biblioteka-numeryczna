# biblioteka-numeryczna

## Opis projektu

Projekt realizowany w ramach przedmiotu **Metody Numeryczne**.

Biblioteka zawiera następujące funkcje numeryczne, pogrupowane według kategorii:

### 🔹 Interpolacja
- Interpolacja Lagrange'a  
- Interpolacja Newtona  

### 🔹 Rozwiązywanie układów równań liniowych
- Metoda Gaussa  
- Metoda LU  

### 🔹 Całkowanie numeryczne  
Funkcje porównujące następujące metody:
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

## 📁 Struktura katalogów
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
