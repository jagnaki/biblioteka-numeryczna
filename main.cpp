#include <iostream>
#include <vector>
#include <locale>
#include "../biblioteka-numeryczna/include/aproksymacja.h"
#include "../biblioteka-numeryczna/include/ukladyRownanLiniowych.h"
#include "../biblioteka-numeryczna/include/calkowanieNumeryczne.h"
#include "../biblioteka-numeryczna/include/rownaniaRozniczkowe.h"
#include "../biblioteka-numeryczna/include/interpolacja.h"
#include "../biblioteka-numeryczna/include/rownaniaNieliniowe.h"
#include "../biblioteka-numeryczna/include/wczytanieDanych.h"

using namespace std;


int main() {
#ifdef _WIN32
    //Ustawienie strony kodowej konsoli na UTF-8 (65001)
    system("chcp 65001 > nul");
#endif
    int wybor;

    do {
        cout << "\n=== MENU GLOWNE ===\n";
        cout << "1. Interpolacja Newtona\n";
        cout << "2. Interpolacja Lagrange'a\n";
        cout << "3. Uklad rownan liniowych - metoda LU\n";
        cout << "4. Uklad rownan liniowych - metoda Gaussa\n";
        cout << "5. Calkowanie - czas\n";
        cout << "6. Calkowanie - kwadratury\n";
        cout << "*7. Rownania rozniczkowe - metoda Eulera\n";
        cout << "*8. Rownania rozniczkowe - metoda Heuna\n";
        cout << "*9. Rownania rozniczkowe - metoda punktu srodkowego\n";
        cout << "*10. Rownania rozniczkowe - metoda Rungego-Kutty IV rzedu\n";
        cout << "11. Aproksymacja funkcji\n";
        cout << "*12. Rownania nieliniowe - metoda bisekcji\n";
        cout << "*13. Rownania nieliniowe - metoda Newtona\n";
        cout << "*14. Rownania nieliniowe - metoda siecznych\n";
        cout << "0. Wyjscie\n";
        cout << "Wybierz opcje: ";
        cin >> wybor;

        switch (wybor) {
            case 1: {
                funkcja_t wybranaFunkcja = pobierz_funkcje();
                string nazwaPliku = wybierzPlik();
                if (nazwaPliku.empty()) {
                    cout << "Nie wybrano pliku. Zamykanie programu." << endl;
                    return 1;
                }
                int numerPorzadkowy = pobierzNumerPorzadkowy();
                do_interpolacja_newtona(nazwaPliku, numerPorzadkowy, wybranaFunkcja);
                break;
            }


            case 2: {
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

            case 3: {
                string nazwaPliku = wybierzPlik();
                if (nazwaPliku.empty()) {
                    cout << "Nie wybrano pliku. Zamykanie programu." << endl;
                    return 1;
                }
                int numerPorzadkowy = pobierzNumerPorzadkowy();
                do_LU(nazwaPliku, numerPorzadkowy);
                break;
            }
            case 4: {
                string nazwaPliku = wybierzPlik();
                if (nazwaPliku.empty()) {
                    cout << "Nie wybrano pliku. Zamykanie programu." << endl;
                    return 1;
                }
                int numerPorzadkowy = pobierzNumerPorzadkowy();
                do_gauss(nazwaPliku, numerPorzadkowy);
                break;
            }
            case 5: {
                string nazwaPliku = wybierzPlik();
                if (nazwaPliku.empty()) {
                    cout << "Nie wybrano pliku. Zamykanie programu." << endl;
                    return 1;
                }
                int numerPorzadkowy = pobierzNumerPorzadkowy();
                do_calkowanie_czas(nazwaPliku, numerPorzadkowy);
                break;
            }
            case 6: {
                string nazwaPliku = wybierzPlik();
                if (nazwaPliku.empty()) {
                    cout << "Nie wybrano pliku. Zamykanie programu." << endl;
                    return 1;
                }
                int numerPorzadkowy = pobierzNumerPorzadkowy();
                do_calkowanie_kwadratury(nazwaPliku, numerPorzadkowy);
                break;
            }
            // case 7: metoda_eulera(); break;
            // case 8: metoda_heuna(); break;
            // case 9: metoda_punktu_srodkowego(); break;
            // case 10: metoda_rungego_kutty_4(); break;
            case 11: {
                do_aproksymacja();
                break;
            }
            // case 12: metoda_bisekcji(); break;
            // case 13: metoda_newtona(); break;
            // case 14: metoda_siecznych(); break;
            case 0: cout << "Koniec programu.\n"; break;
            default: cout << "Nieprawidlowy wybor. Sprobuj ponownie.\n";
        }

    } while (wybor != 0);

    return 0;
}