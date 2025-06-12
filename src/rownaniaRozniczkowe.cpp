
#include "../include/rownaniaRozniczkowe.h"

#include <iostream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <string>
#include <locale>

using namespace std;

//Stałe używane w równaniu przewodnictwa cieplnego
const double BETA = 0.0;          //wartość beta
const double ALPHA = 7.0e-12;       //wartość alpha
const double T0 = 1967.0;         //temperatura początkowa
const double TIME_END = 1967.0;    //czas końcowy symulacji w sekundach

//Funkcja równania różniczkowego: dT/dt = -alpha * (T^4 - beta)
//Przyjmuje aktualny czas t i temperaturę T
//Zwraca wartość pochodnej dT/dt
double f(double t, double T) {
    return -ALPHA * (pow(T, 4) - BETA);
}

//Funkcja obliczająca dokładne rozwiązanie równania różniczkowego
double dokladne_rozwiazanie(double t) {
    return (19670000)/pow((1.0e12+159820459323.0*t), 1.0/3.0);
}

//Metoda Heuna (metoda trapezów)
//Metoda dwuetapowa: najpierw oblicza nachylenie na początku przedziału,
//potem używa go do znalezienia punktu końcowego,
//a następnie używa średniej nachyleń do obliczenia ostatecznej wartości
vector<data_point> metoda_heuna(double h, int steps) {
    vector<data_point> results;
    double t = 0.0;   //czas początkowy
    double y = T0;    //temperatura początkowa

    results.push_back({t, y});

    for (int i = 0; i < steps; i++) {
        //Krok 1: Oblicz nachylenie w punkcie początkowym
        double slope1 = f(t, y);

        //Krok 2: Zrób krok Eulera do znalezienia wartości tymczasowej
        double y_temp = y + h * slope1;

        //Krok 3: Oblicz nachylenie w punkcie końcowym
        double slope2 = f(t + h, y_temp);

        //Krok 4: Użyj średniej nachyleń do obliczenia ostatecznej wartości
        y = y + (h/2.0) * (slope1 + slope2);
        t = t + h;

        results.push_back({t, y});
    }

    return results;
}

//Metoda punktu środkowego
//Metoda dwuetapowa: oblicza nachylenie w środku przedziału całkowania
//i używa go do wykonania kroku całkowania
vector<data_point> metoda_punktu_srodkowego(double h, int steps) {
    vector<data_point> results;
    double t = 0.0;   //czas początkowy
    double y = T0;    //temperatura początkowa

    results.push_back({t, y});

    for (int i = 0; i < steps; i++) {
        //krok 1: Oblicz nachylenie w punkcie początkowym
        double k1 = f(t, y);

        //Krok 2: Znajdź punkt środkowy używając połowy kroku
        double t_mid = t + h/2.0;
        double y_mid = y + (h/2.0) * k1;

        //Krok 3: Oblicz nachylenie w punkcie środkowym
        double k_mid = f(t_mid, y_mid);

        //Krok 4: Użyj nachylenia w punkcie środkowym do wykonania pełnego kroku
        y = y + h * k_mid;
        t = t + h;

        results.push_back({t, y});
    }

    return results;
}

//Metoda Rungego-Kutty 4. rzędu
//Metoda czteroetapowa, najbardziej dokładna z prezentowanych
//Oblicza nachylenia w czterech punktach i waży je odpowiednio
vector<data_point> metoda_rk_4_rzedu(double h, int steps) {
    vector<data_point> results;
    double t = 0.0;   //czas początkowy
    double y = T0;    //temperatura początkowa

    results.push_back({t, y});

    for (int i = 0; i < steps; i++) {
        // Oblicz cztery nachylenia (k1, k2, k3, k4)

        //k1: nachylenie na początku przedziału
        double k1 = h * f(t, y);

        //k2: nachylenie w środku przedziału używając k1
        double k2 = h * f(t + h/2.0, y + k1/2.0);

        //k3: nachylenie w środku przedziału używając k2
        double k3 = h * f(t + h/2.0, y + k2/2.0);

        //k4: nachylenie na końcu przedziału używając k3
        double k4 = h * f(t + h, y + k3);

        //Oblicz przyrost używając ważonej średniej nachyleń
        //Wagi: k1=1/6, k2=2/6, k3=2/6, k4=1/6
        double dy = (k1 + 2*k2 + 2*k3 + k4) / 6.0;

        y = y + dy;
        t = t + h;

        results.push_back({t, y});
    }

    return results;
}

//Implementacja metody Eulera do rozwiązywania równań różniczkowych
vector<data_point> metoda_eulera(double t0, double h) {
    vector<data_point> results;
    results.push_back({t0, T0});

    double t = t0;
    double T = T0;

    //Użycie warunku i sprawdzenie precyzjy, aby nie przekroczyć tMax
    while (t < TIME_END - 1e-10) {  //Dodanie małej tolerancji do porównania
        double derivative = f(t, T);
        T = T + h * derivative;
        t = t + h;

        //Upewnienie się, że nie przekroczono tMax
        if (t > TIME_END) {
            t = TIME_END;
            //Oblicenie ostatniej wartości T używając mniejszego kroku
            T = results.back().temperature + (TIME_END - results.back().time) * f(results.back().time, results.back().temperature);
        }

        results.push_back({t, T});
    }

    return results;
}

//Funkcja obliczająca średni błąd kwadratowy (MSE) względem dokładnego rozwiązania
double oblicz_MSE(const vector<data_point>& numerical) {
    double sum_squared_errors = 0.0;
    int n = numerical.size();

    //Sumuj kwadraty błędów dla każdego punktu
    for (int i = 0; i < n; i++) {
        double analytical_temp = dokladne_rozwiazanie(numerical[i].time);
        double error = numerical[i].temperature - analytical_temp;
        sum_squared_errors += error * error;
    }

    return (sum_squared_errors / n);
}




//Funkcja obliczająca błąd punktowy (maksymalny błąd bezwzględny)
//Znajduje największą różnicę między rozwiązaniem numerycznym a analitycznym
double oblicz_max_blad(const vector<data_point>& numerical) {
    double max_error = 0.0;

    //Znajdź maksymalny błąd bezwzględny
    for (const auto& point : numerical) {
        double analytical_temp = dokladne_rozwiazanie(point.time);
        double error = abs(point.temperature - analytical_temp);
        if (error > max_error) {
            max_error = error;
        }
    }

    return max_error;
}

//Funkcja eksportująca wyniki do pliku CSV
//Format: czas, temperatura_metoda1, temperatura_metoda2, temperatura_metoda3, temperatura_metoda4, temperatura_analityczna
void zapisz_do_csv(const string& filename,
                const vector<data_point>& heun,
                const vector<data_point>& midpoint,
                const vector<data_point>& rk4,
                const vector<data_point>& euler,
                double h) {
    ofstream file(filename);

    //Nagłówek pliku CSV
    file << "Czas[s];Temp_Heun[K];Temp_Srodek[K];Temp_RK4[K];Temp_Euler[K];Temp_Analityczna[K]\n";

    //Zakładamy, że wszystkie metody mają taką samą liczbę punktów
    int n = heun.size();

    //Zapisz dane dla każdego punktu czasowego
    for (int i = 0; i < n; i++) {
        file << std::fixed << setprecision(6);
        file << heun[i].time << ";";
        file << heun[i].temperature << ";";
        file << midpoint[i].temperature << ";";
        file << rk4[i].temperature << ";";
        file << euler[i].temperature << ";";
        file << dokladne_rozwiazanie(heun[i].time) << "\n";
    }

    file.close();
}

//Funkcja eksportująca podsumowanie błędów do pliku CSV
void zapisz_do_csv_blad(const string& filename,
                       const vector<double>& step_sizes,
                       const vector<vector<double>>& rmse_errors,
                       const vector<vector<double>>& max_errors) {
    ofstream file(filename);

    file << "Krok_h[s];RMSE_Heun;RMSE_Srodek;RMSE_RK4;RSME_Euler;MaxError_Heun;MaxError_Srodek;MaxError_RK4;MaxError_Euler\n";

    for (size_t i = 0; i < step_sizes.size(); i++) {
        file << fixed << setprecision(6);
        file << step_sizes[i] << ";";
        file << rmse_errors[0][i] << ";";
        file << rmse_errors[1][i] << ";";
        file << rmse_errors[2][i] << ";";
        file << rmse_errors[3][i] << ";";
        file << max_errors[0][i] << ";";
        file << max_errors[1][i] << ";";
        file << max_errors[2][i] << ";";
        file << max_errors[3][i] << "\n";
    }

    file.close();
}


void do_rownania_rozniczkowe() {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    double t0 = 0.0;             //Czas początkowy [s]
    //Różne kroki całkowania do przetestowania
    vector<double> step_size = {10.0, 5.0, 1.0, 0.5, 0.1};

    //Wektory do przechpwywania błędów dla każdej metody
    vector<vector<double>> mse_errors(4);  //4 metody
    vector<vector<double>> max_errors(4);

    //Przetwórz każdy krok całkowania
    for (const double& h: step_size) {
        cout<< "\n=== Krok czasowy h = " << h << " s ===" << endl;

        int steps = static_cast<int>(TIME_END/h);

        auto heun_results = metoda_heuna(h, steps);

        auto midpoint_results = metoda_punktu_srodkowego(h, steps);

        auto rk4_results = metoda_rk_4_rzedu(h, steps);

        auto euler_results = metoda_eulera(t0, h);

        //Oblicz błędy dla każdej metody
        double rmse_heun = oblicz_MSE(heun_results);
        double rmse_midpoint = oblicz_MSE(midpoint_results);
        double rmse_rk4 = oblicz_MSE(rk4_results);
        double rmse_euler = oblicz_MSE(euler_results);

        double max_heun = oblicz_max_blad(heun_results);
        double max_midpoint = oblicz_max_blad(midpoint_results);
        double max_rk4 = oblicz_max_blad(rk4_results);
        double max_euler = oblicz_max_blad(euler_results);

        //Zapisz błędy
        mse_errors[0].push_back(rmse_heun);
        mse_errors[1].push_back(rmse_midpoint);
        mse_errors[2].push_back(rmse_rk4);
        mse_errors[3].push_back(rmse_euler);

        max_errors[0].push_back(max_heun);
        max_errors[1].push_back(max_midpoint);
        max_errors[2].push_back(max_rk4);
        max_errors[3].push_back(max_euler);

        //Wyświetl wyniki
        cout << "\nWyniki dla h = " << h << " s:" << std::endl;
        cout << "Temperatura końcowa (t = " << TIME_END << " s):" << endl;
        cout << "  Metoda Heuna:           " << heun_results.back().temperature << " K" << endl;
        cout << "  Metoda punktu środkowego: " << midpoint_results.back().temperature << " K" << endl;
        cout << "  Metoda RK4:             " << rk4_results.back().temperature << " K" << endl;
        cout << "  Metoda Eulera:          " << euler_results.back().temperature << " K" << endl;
        cout << "  Rozwiązanie analityczne: " << dokladne_rozwiazanie(TIME_END) << " K" << endl;

        cout << "\nBłąd średniokwadratowy (RMSE):" << endl;
        cout << "  Metoda Heuna:           " << rmse_heun << " K" << endl;
        cout << "  Metoda punktu środkowego: " << rmse_midpoint << " K" << endl;
        cout << "  Metoda RK4:             " << rmse_rk4 << " K" << endl;
        cout << "  Metoda Eulera:          " << rmse_euler << " K" << endl;

        cout << "\nMaksymalny błąd bezwzględny:" << endl;
        cout << "  Metoda Heuna:           " << max_heun << " K" << endl;
        cout << "  Metoda punktu środkowego: " << max_midpoint << " K" << endl;
        cout << "  Metoda RK4:             " << max_rk4 << " K" << endl;
        cout << "  Metoda Eulera:          " << max_euler << " K" << endl;

        string filename = "wyniki_h_" + to_string(h).substr(0, 4) + ".csv";
        zapisz_do_csv(filename, heun_results, midpoint_results, rk4_results, euler_results, h);
    }

    //Eksportuj podsumowanie błędów do pliku csv
    zapisz_do_csv_blad("podsumowanie_bledow.csv", step_size, mse_errors, max_errors);
}