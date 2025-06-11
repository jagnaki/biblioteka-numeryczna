#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include "../include/interpolacja.h"
using namespace std;

using namespace std;

// Struktura przechowująca informacje o plikach
struct PlikDanych {
    int numer;
    string nazwa;
    string opis;
};

// Tablica dostępnych plików
const vector<PlikDanych> DOSTEPNE_PLIKI = {
    {1, "A_daneH.txt", "Dane H"},
    {2, "A_daneN.txt", "Dane N"},
    {3, "A_newton.txt", "Newton"},
    {4, "A_dane_LU.txt", "Dane LU"},
    {5, "A_daneA_LU.txt", "Dane A LU"},
    {6, "A_daneB_LU.txt", "Dane B LU"},
    {7, "A_daneC_LU.txt", "Dane C LU"},
    {8, "A_lagrange.txt", "Lagrange"},
    {9, "A_daneA_gauss.txt", "Dane A Gauss"},
    {10, "A_daneB_gauss.txt", "Dane B Gauss"},
    {11, "A_daneC_gauss.txt", "Dane C Gauss"},
    {12, "A_dane_calkowanie.txt", "Dane całkowanie"}
};

void wyswietlPlikiFolderu() {
    cout << "\n" << string(50, '=') << endl;
    cout << "         DOSTĘPNE PLIKI Z DANYMI" << endl;
    cout << string(50, '=') << endl;

    for (const auto& plik : DOSTEPNE_PLIKI) {
        cout << setw(2) << plik.numer << ". "
             << setw(25) << left << plik.nazwa
             << " - " << plik.opis << endl;
    }
    cout << string(50, '=') << endl;
}

string wybierzPlik() {
    int numer;

    wyswietlPlikiFolderu();

    cout << "\nWybierz numer pliku (1-" << DOSTEPNE_PLIKI.size() << "): ";

    if (!(cin >> numer)) {
        cout << "Błąd: Wprowadź prawidłową liczbę!" << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        return "";
    }

    // Sprawdzenie czy numer jest w prawidłowym zakresie
    if (numer < 1 || numer > static_cast<int>(DOSTEPNE_PLIKI.size())) {
        cout << "Błąd: Nieprawidłowy numer pliku! Wybierz liczbę od 1 do "
             << DOSTEPNE_PLIKI.size() << endl;
        return "";
    }

    // Znalezienie pliku o podanym numerze
    for (const auto& plik : DOSTEPNE_PLIKI) {
        if (plik.numer == numer) {
            cout << "Wybrano plik: " << plik.nazwa << " (" << plik.opis << ")" << endl;
            return plik.nazwa;
        }
    }

    return "";
}

int pobierzNumerPorzadkowy() {
    int numer;
    cout << "\nPodaj numer porządkowy (l.p.): ";

    if (!(cin >> numer)) {
        cout << "Błąd: Wprowadź prawidłową liczbę!" << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        return -1;
    }

    if (numer <= 0) {
        cout << "Błąd: Numer porządkowy musi być większy od 0!" << endl;
        return -1;
    }

    return numer;
}