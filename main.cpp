#include <iostream>
#include <vector>

#include "../biblioteka-numeryczna/include/aproksymacja.h"
#include "../biblioteka-numeryczna/include/ukladyRownanLiniowych.h"
#include "../biblioteka-numeryczna/include/calkowanieNumeryczne.h"
#include "../biblioteka-numeryczna/include/rownaniaRozniczkowe.h"
#include "../biblioteka-numeryczna/include/interpolacja.h"
#include "../biblioteka-numeryczna/include/rownaniaNieliniowe.h"
#include "../biblioteka-numeryczna/include/funkcje.h"
#include "../biblioteka-numeryczna/include/wczytanieDanych.h"

using namespace std;


int main() {
    int wybor;

    do {
        cout << "\n=== MENU GLOWNE ===\n";
        cout << "1. Interpolacja Newtona\n";
        cout << "2. Interpolacja Lagrange'a\n";
        cout << "3. Uklad rownan liniowych - metoda LU\n";
        cout << "4. Uklad rownan liniowych - metoda Gaussa\n";
        cout << "5. Calkowanie - metoda prostokatow\n";
        cout << "6. Calkowanie - metoda trapezow\n";
        cout << "7. Calkowanie - metoda Simpsona\n";
        cout << "8. Calkowanie - 2-wezlowe kwadratury G-L\n";
        cout << "9. Calkowanie - 3-wezlowe kwadratury G-L\n";
        cout << "10. Calkowanie - 4-wezlowe kwadratury G-L\n";
        cout << "11. Rownania rozniczkowe - metoda Eulera\n";
        cout << "12. Rownania rozniczkowe - metoda Heuna\n";
        cout << "13. Rownania rozniczkowe - metoda punktu srodkowego\n";
        cout << "14. Rownania rozniczkowe - metoda Rungego-Kutty IV rzedu\n";
        cout << "0. Wyjscie\n";
        cout << "Wybierz opcje: ";
        cin >> wybor;

        switch (wybor) {
            //case 1: interpolacja_newtona(); break;
            case 2: {
                int x;
                funkcja_t wybranaFunkcja = pobierz_funkcje();

                string nazwaPliku = wybierzPlik();
                if (nazwaPliku.empty()) {
                    cout << "Nie wybrano pliku. Zamykanie programu." << endl;
                    return 1;
                }

                int numerPorzadkowy = pobierzNumerPorzadkowy();
                do_interpolacja_lagrangea(nazwaPliku, numerPorzadkowy, wybranaFunkcja);
                break;
            }

            // case 3: metoda_lu(); break;
            // case 4: metoda_gaussa(); break;
            // case 5: metoda_prostokatow(); break;
            // case 6: metoda_trapezow(); break;
            // case 7: metoda_simpsona(); break;
            // case 8: kwadratura_gaussa_legendre(2); break;
            // case 9: kwadratura_gaussa_legendre(3); break;
            // case 10: kwadratura_gaussa_legendre(4); break;
            // case 11: metoda_eulera(); break;
            // case 12: metoda_heuna(); break;
            // case 13: metoda_punktu_srodkowego(); break;
            // case 14: metoda_rungego_kutty_4(); break;
            case 0: cout << "Koniec programu.\n"; break;
            default: cout << "Nieprawidlowy wybor. Sprobuj ponownie.\n";
        }

    } while (wybor != 0);

    return 0;
}