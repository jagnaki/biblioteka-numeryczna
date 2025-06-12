#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <locale>
#include <string>

using namespace std;

//Zmienne globalne do przechowywania parametrów
int i_global, j_global;

//Funkcja
double funkcja(double x) {
    //Implementacja funkcji z zadania: f(x) = e^x * cos(5x) - x^3
    return exp(x) * cos(5*x) - pow(x, 3);
}

//Funkcje bazowe (jednomiany): 1, x, x^2, x^3, ...
double funkcja_bazowa(int stopien, double x) {
    return pow(x, stopien);
}

//Funkcja obliczająca całkę metodą Simpsona
//Parametry:
//- func: wskaźnik do funkcji, którą całkujemy
//- a, b: granice całkowania
//- n: liczba podziałów przedziału całkowania (musi być parzysta)
template<typename F>
double calka_simpsona_template(F func, double a, double b, int n) {
    //n - liczba podprzedziałów (musi być parzysta)
    if (n % 2 != 0) n++; //Zapewniamy parzystą liczbę podprzedziałów

    double h = (b - a) / n; //Szerokość podprzedziału
    double suma = func(a) + func(b); //Wartości na krańcach

    //Sumowanie wartości funkcji dla współczynników 4 (nieparzyste indeksy)
    for (int i = 1; i < n; i += 2) {
        suma += 4 * func(a + i * h);
    }

    //Sumowanie wartości funkcji dla współczynników 2 (parzyste indeksy)
    for (int i = 2; i < n; i += 2) {
        suma += 2 * func(a + i * h);
    }

    //Ostateczny wynik całki
    return (h / 3) * suma;
}

//Funkcje pomocnicze używające szablonu
double calka_phi_i_phi_j(int i, int j, double a, double b, int n) {
    auto funkcja_do_calkowania = [i, j](double x) {
        return funkcja_bazowa(i, x) * funkcja_bazowa(j, x);
    };

    return calka_simpsona_template(funkcja_do_calkowania, a, b, n);
}

double calka_f_phi_i(int i, double a, double b, int n) {
    auto funkcja_do_calkowania = [i](double x) {
        return funkcja(x) * funkcja_bazowa(i, x);
    };

    return calka_simpsona_template(funkcja_do_calkowania, a, b, n);
}

//Funkcja rozwiązująca układ równań metodą eliminacji Gaussa
vector<double> rozwiaz_uklad_rownan(const vector<vector<double>>& A, const vector<double>& b) {
    int n = A.size();

    //Tworzymy macierz rozszerzoną [A|b]
    vector<vector<double>> Ab(n, vector<double>(n + 1));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Ab[i][j] = A[i][j];
        }
        Ab[i][n] = b[i];
    }

    //Eliminacja w przód
    for (int i = 0; i < n; i++) {
        //Szukamy elementu maksymalnego w kolumnie dla pivota (dla stabilności numerycznej)
        int max_row = i;
        for (int j = i + 1; j < n; j++) {
            if (abs(Ab[j][i]) > abs(Ab[max_row][i])) {
                max_row = j;
            }
        }

        //Zamieniamy wiersze
        if (max_row != i) {
            swap(Ab[i], Ab[max_row]);
        }

        //Eliminacja
        for (int j = i + 1; j < n; j++) {
            double coef = Ab[j][i] / Ab[i][i];
            for (int k = i; k <= n; k++) {
                Ab[j][k] -= coef * Ab[i][k];
            }
        }
    }

    //Podstawianie wstecz
    vector<double> x(n);
    for (int i = n - 1; i >= 0; i--) {
        x[i] = Ab[i][n];
        for (int j = i + 1; j < n; j++) {
            x[i] -= Ab[i][j] * x[j];
        }
        x[i] /= Ab[i][i];
    }

    return x;
}

//Funkcja wyznaczająca wartość aproksymowaną F(x) dla danego x i współczynników a
double aproksymacja(const vector<double>& a, double x) {
    double wynik = 0.0;
    for (size_t i = 0; i < a.size(); i++) {
        wynik += a[i] * funkcja_bazowa(i, x);
    }
    return wynik;
}

//Funkcja obliczająca błąd średniokwadratowy aproksymacji
double blad_srednio_kwadratowy(const vector<double>& a, double a_przedzial, double b_przedzial, int n_punktow) {
    double h = (b_przedzial - a_przedzial) / n_punktow;
    double suma_kwadratow_bledow = 0.0;

    for (int i = 0; i <= n_punktow; i++) {
        double x = a_przedzial + i * h;
        double blad = funkcja(x) - aproksymacja(a, x);
        suma_kwadratow_bledow += blad * blad;
    }

    return sqrt(suma_kwadratow_bledow / (n_punktow + 1));
}

void do_aproksymacja() {
#ifdef _WIN32
    //Ustawienie strony kodowej konsoli na UTF-8 (65001)
    system("chcp 65001 > nul");
#endif

    //Parametry aproksymacji
    double a_przedzial = 0.0;  //Początek przedziału
    double b_przedzial = 1.5;   //Koniec przedziału
    int n_calkowania = 1000;     //Liczba podziałów do całkowania
    int n_punktow_wykresu = 100; //Liczba punktów do wygenerowania danych

    vector<int> stopnie_aproksymacji = {2, 3, 4, 5, 6};

    string filename;

    for (int stopien : stopnie_aproksymacji) {
        cout << "Aproksymacja stopnia " << stopien << ":" << endl;

        //Tworzymy macierz A i wektor b
        int m = stopien + 1; //Liczba funkcji bazowych (1, c, x^2, ..., x^m)
        vector<vector<double>> A(m, vector<double>(m, 0.0));
        vector<double> b(m, 0.0);

        //Wypełnienie macierzy A i wektora b
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                A[i][j] = calka_phi_i_phi_j(i, j, a_przedzial, b_przedzial, n_calkowania);
            }
            b[i] = calka_f_phi_i(i, a_przedzial, b_przedzial, n_calkowania);
        }
        //Rozwiązanie układu równań - znalezienie współczynników a_i
        vector<double> a = rozwiaz_uklad_rownan(A, b);

        //Wypisanie współczynników
        cout << "Współczynniki: ";
        for (double coef : a) {
            cout << coef << " ";
        }
        cout << endl;

        //obliczanie błędu średniokwadratowego
        double blad = blad_srednio_kwadratowy(a, a_przedzial, b_przedzial, n_punktow_wykresu);
        cout << "Błąd średniokwadratowy: " << fixed << setprecision(10) << blad << endl;

        filename = "aproksymacja_stopien_" + to_string(stopien) + ".csv";
        ofstream file(filename);
        if (file.is_open()) {
            file << "x;f(x);F(x);blad\n"; // Nagłówki kolumn
            double h = (b_przedzial - a_przedzial) / n_punktow_wykresu;
            for (int i = 0; i <= n_punktow_wykresu; i++) {
                double x = a_przedzial + i * h;
                double fx = funkcja(x);
                double Fx = aproksymacja(a, x);
                double blad_punktowy = Fx - fx;
                //cout<< "x = " << x << ", f(x) = " << fx << ", F(x) = " << Fx << ", błąd = " << blad_punktowy << endl;
                file << x << ";" << fx << ";" << Fx << ";" << blad_punktowy << "\n";
            }
            file.close();
        }
        else {
            cout << "Nie można otworzyć pliku do zapisu: " << filename << endl;
        }
    }
}