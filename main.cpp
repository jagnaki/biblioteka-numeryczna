#include <iostream>
using namespace std;

// === Deklaracje funkcji ===
// Równania liniowe
void metoda_gaussa() { cout << "Metoda Gaussa\n"; }
void metoda_lu() { cout << "Metoda LU\n"; }

// Interpolacja
void interpolacja_newtona() { cout << "Interpolacja Newtona\n"; }
void interpolacja_lagrangea() { cout << "Interpolacja Lagrange'a\n"; }

// Aproksymacja
void aproksymacja_przykladowa() { cout << "Aproksymacja - metoda przykladowa\n"; }

// Całkowanie
void metoda_prostokatow() { cout << "Metoda prostokatow\n"; }
void metoda_trapezow() { cout << "Metoda trapezow\n"; }
void metoda_simpsona() { cout << "Metoda Simpsona\n"; }
void kwadratura_gaussa_legendre(int wezly) {
    cout << wezly << "-wezlowa kwadratura Gaussa-Legendre'a\n";
}

// Równania różniczkowe
void metoda_eulera() { cout << "Metoda Eulera\n"; }
void metoda_heuna() { cout << "Metoda Heuna\n"; }
void metoda_punktu_srodkowego() { cout << "Metoda punktu srodkowego\n"; }
void metoda_rungego_kutty_4() { cout << "Metoda Rungego-Kutty IV rzedu\n"; }

// Równania nieliniowe
void rozwiazanie_nieliniowe() { cout << "Rozwiazanie rownan nieliniowych (przykladowa metoda)\n"; }

// === Menu podrzędne ===
void menu_uklady_liniowe() {
    int wybor;
    cout << "\n--- Rozwiazywanie ukladow rownan liniowych ---\n";
    cout << "1. Metoda Gaussa\n";
    cout << "2. Metoda LU\n";
    cout << "0. Powrot\n";
    cout << "Wybierz opcje: ";
    cin >> wybor;
    switch (wybor) {
        case 1: metoda_gaussa(); break;
        case 2: metoda_lu(); break;
    }
}

void menu_interpolacja() {
    int wybor;
    cout << "\n--- Interpolacja ---\n";
    cout << "1. Newtona\n";
    cout << "2. Lagrange'a\n";
    cout << "0. Powrot\n";
    cout << "Wybierz opcje: ";
    cin >> wybor;
    switch (wybor) {
        case 1: interpolacja_newtona(); break;
        case 2: interpolacja_lagrangea(); break;
    }
}

void menu_aproksymacja() {
    int wybor;
    cout << "\n--- Aproksymacja ---\n";
    cout << "1. Metoda przykladowa\n";
    cout << "0. Powrot\n";
    cout << "Wybierz opcje: ";
    cin >> wybor;
    if (wybor == 1) aproksymacja_przykladowa();
}

void menu_calkowanie() {
    int wybor;
    cout << "\n--- Calkowanie numeryczne ---\n";
    cout << "1. Prostokatow\n";
    cout << "2. Trapezow\n";
    cout << "3. Simpsona\n";
    cout << "4. Kwadratura Gaussa-Legendre'a (2 wezly)\n";
    cout << "5. Kwadratura Gaussa-Legendre'a (3 wezly)\n";
    cout << "6. Kwadratura Gaussa-Legendre'a (4 wezly)\n";
    cout << "0. Powrot\n";
    cout << "Wybierz opcje: ";
    cin >> wybor;
    switch (wybor) {
        case 1: metoda_prostokatow(); break;
        case 2: metoda_trapezow(); break;
        case 3: metoda_simpsona(); break;
        case 4: kwadratura_gaussa_legendre(2); break;
        case 5: kwadratura_gaussa_legendre(3); break;
        case 6: kwadratura_gaussa_legendre(4); break;
    }
}

void menu_rozniczkowe() {
    int wybor;
    cout << "\n--- Rownania rozniczkowe ---\n";
    cout << "1. Euler\n";
    cout << "2. Heuna\n";
    cout << "3. Punktu srodkowego\n";
    cout << "4. Rungego-Kutty IV rzedu\n";
    cout << "0. Powrot\n";
    cout << "Wybierz opcje: ";
    cin >> wybor;
    switch (wybor) {
        case 1: metoda_eulera(); break;
        case 2: metoda_heuna(); break;
        case 3: metoda_punktu_srodkowego(); break;
        case 4: metoda_rungego_kutty_4(); break;
    }
}

void menu_nieliniowe() {
    int wybor;
    cout << "\n--- Rownania nieliniowe ---\n";
    cout << "1. Metoda przykladowa (dopisz konkretna pozniej)\n";
    cout << "0. Powrot\n";
    cout << "Wybierz opcje: ";
    cin >> wybor;
    if (wybor == 1) rozwiazanie_nieliniowe();
}

// === MENU GŁÓWNE ===
int main() {
    int wybor;
    do {
        cout << "\n=== MENU GLOWNE ===\n";
        cout << "1. Rozwiazywanie ukladow rownan liniowych\n";
        cout << "2. Interpolacja\n";
        cout << "3. Aproksymacja\n";
        cout << "4. Calkowanie numeryczne\n";
        cout << "5. Rownania rozniczkowe\n";
        cout << "6. Rownania nieliniowe\n";
        cout << "0. Wyjscie\n";
        cout << "Wybierz opcje: ";
        cin >> wybor;

        switch (wybor) {
            case 1: menu_uklady_liniowe(); break;
            case 2: menu_interpolacja(); break;
            case 3: menu_aproksymacja(); break;
            case 4: menu_calkowanie(); break;
            case 5: menu_rozniczkowe(); break;
            case 6: menu_nieliniowe(); break;
            case 0: cout << "Koniec programu.\n"; break;
            default: cout << "Nieprawidlowy wybor. Sprobuj ponownie.\n";
        }

    } while (wybor != 0);

    return 0;
}
