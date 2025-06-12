
#include "../include/rownaniaNieliniowe.h"

#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <locale>

using namespace std;

//Struktura przechowująca wyniki pojedynczej iteracji algorytmu
struct pojedyncza_iteracja {
    int iteration;
    double x;
    double fx;
    double error;
};

//Struktura przechowująca wszystkie wyniki dla jednego miejsca zerowego
struct wyniki {
    double root;
    vector<pojedyncza_iteracja> iterations;
    string method;
    bool converged;
    int function_number;
    string function_name;
    string function_description;
};

//=== DEFINICJE FUNKCJI I ICH POCHODNYCH ===

//Funkcja 1: ln(x+1) - 1/(x+3) = 0
double f1(double x) {
    return log(x + 1) - 1.0/(x + 3);
}

//Pochodna funkcji f1 obliczona analitycznie: f'(x) = 1/(x+1) + 1/(x+3)^2
double f1_analitycznie(double x) {
    return 1.0/(x + 1) + 1.0/((x + 3)*(x + 3));
}

//Funkcja 2: x^3 + 30*cos(x) - 1 = 0
double f2(double x) {
    return x*x*x + 30*cos(x) - 1;
}

//Pochodna funkcji f2 obliczona analitycznie: f'(x) = 3x^2 - 30*sin(x)
double f2_analitycznie(double x) {
    return 3*x*x - 30*sin(x);
}

//Funkcja 3: sin(3PIx)/(x+2) + 1/(x+4) = 0
double f3(double x) {
    const double PI = 3.14159265359;
    return sin(3*PI*x)/(x + 2) + 1.0/(x + 4);
}

//Pochodna funkcji f3 obliczona analitycznie
double f3_analitycznie(double x) {
    const double PI = 3.14159265359;
    double term1 = (3*PI*cos(3*PI*x)*(x + 2) - sin(3*PI*x)*1)/((x + 2)*(x + 2));
    double term2 = -1.0/((x + 4)*(x + 4));
    return term1 + term2;
}

//Funkcja numerycznego obliczania pochodnej metodą różnic skończonych
double numeryczne_obliczanie_pochodnej(double (*func)(double), double x, double h = 1e-8) {
    return (func(x + h) - func(x - h)) / (2.0 * h);  //Centralna różnica skończona
}

//=== IMPLEMENTACJA METODY BISEKCJI ===
wyniki metoda_bisekcji(double (*func)(double), double a, double b, double epsilon = 1e-10, int max_iter = 1000) {
    wyniki result;
    result.method = "Bisekcja";
    result.converged = false;
    result.root = 0.0;
    result.function_number = 0;
    result.function_name = "";
    result.function_description = "";

    double fa = func(a);          //Obliczenie f(a) - wartość funkcji w lewym końcu przedziału
    double fb = func(b);          //Obliczenie f(b) - wartość funkcji w prawym końcu przedziału

    //Sprawdzenie czy na końcach przedziału funkcja ma różne znaki (warunek konieczny)
    if (fa * fb > 0) {
        cout << "Błąd: funkcja ma ten sam znak na końcach przedziału [" << a << ", " << b << "]" << endl;
        return result;  //Zwracamy pustą strukturę w przypadku błędu
    }

    double c, fc;  //c to środek przedziału, fc to wartość funkcji w środku

    //Główna pętla algorytmu bisekcji
    for (int i = 0; i < max_iter; i++) {
        c = (a + b) / 2.0;
        fc = func(c);

        //Tworzenie struktury z wynikami bieżącej iteracji
        pojedyncza_iteracja iter;
        iter.iteration = i;
        iter.x = c;              //Bieżące przybliżenie pierwiastka
        iter.fx = fc;            //Wartość funkcji w tym punkcie
        //Błąd to połowa szerokości aktualnego przedziału
        iter.error = (b - a) / 2.0;
        result.iterations.push_back(iter);

        //Sprawdzenie warunków zakończenia
        if (abs(fc) < epsilon || (b - a) < epsilon) {
            result.root = c;          //Znalezione miejsce zerowe
            result.converged = true;  //Metoda się zbiegła
            return result;
        }

        //Wybór nowego przedziału na podstawie znaku funkcji
        if (fa * fc < 0) {
            //Jeśli f(a) i f(c) mają różne znaki, pierwiastek jest w [a, c]
            b = c;
            fb = fc;
        } else {
            //W przeciwnym przypadku pierwiastek jest w [c, b]
            a = c;
            fa = fc;
        }
    }

    //Jeśli osiągnięto maksymalną liczbę iteracji bez zbieżności
    result.root = c;              //Ostatnie przybliżenie
    result.converged = false;     //Brak zbieżności
    return result;
}

//=== IMPLEMENTACJA METODY NEWTONA ===
wyniki metoda_newtona(double (*func)(double), double (*derivative)(double), double x0, double epsilon = 1e-10, int max_iter = 1000) {
    wyniki result;
    result.method = "Newton";
    result.converged = false;
    result.root = 0.0;
    result.function_number = 0;
    result.function_name = "";
    result.function_description = "";

    double x = x0;                        //Punkt startowy
    double fx, fpx;                       //fx = f(x), fpx = f'(x)

    //Główna pętla metody Newtona
    for (int i = 0; i < max_iter; i++) {
        fx = func(x);                     //Obliczenie wartości funkcji
        fpx = derivative(x);              //Obliczenie wartości pochodnej

        //Sprawdzenie czy pochodna nie jest bliska zeru (unikanie dzielenia przez zero)
        if (abs(fpx) < 1e-15) {
            cout << "Błąd: pochodna bliska zeru w punkcie x = " << x << endl;
            return result;  //Zwracamy wynik bez zbieżności
        }

        //Tworzenie struktury z wynikami bieżącej iteracji
        pojedyncza_iteracja iter;
        iter.iteration = i;
        iter.x = x;                      //Bieżące przybliżenie
        iter.fx = fx;                    //Wartość funkcji
        iter.error = (i > 0) ? abs(x - result.iterations[i-1].x) : abs(fx);  //Błąd względem poprzedniej iteracji
        result.iterations.push_back(iter);

        //Sprawdzenie warunku zakończenia
        if (abs(fx) < epsilon) {
            result.root = x;              //Znalezione miejsce zerowe
            result.converged = true;      //Metoda się zbiegła
            return result;
        }

        //Obliczenie nowego przybliżenia według wzoru Newtona: x_{n+1} = x_n - f(x_n)/f'(x_n)
        double x_new = x - fx / fpx;

        //Sprawdzenie zbieżności przez porównanie kolejnych przybliżeń
        if (abs(x_new - x) < epsilon) {
            result.root = x_new;          //Znalezione miejsce zerowe
            result.converged = true;      //Metoda się zbiegła
            return result;
        }

        x = x_new;  //Aktualizacja punktu dla następnej iteracji
    }

    //Jeśli osiągnięto maksymalną liczbę iteracji
    result.root = x;
    result.converged = false;
    return result;
}

//=== IMPLEMENTACJA METODY SIECZNYCH ===
wyniki metoda_siecznych(double (*func)(double), double x0, double x1, double epsilon = 1e-10, int max_iter = 1000) {
    wyniki result;
    result.method = "Sieczne";
    result.converged = false;
    result.root = 0.0;
    result.function_number = 0;
    result.function_name = "";
    result.function_description = "";

    double fx0 = func(x0);               //f(x0) - wartość funkcji w pierwszym punkcie
    double fx1 = func(x1);               //f(x1) - wartość funkcji w drugim punkcie

    //Główna pętla metody siecznych
    for (int i = 0; i < max_iter; i++) {
        //Sprawdzenie czy różnica wartości funkcji nie jest bliska zeru
        if (abs(fx1 - fx0) < 1e-15) {
            cout << "Błąd: różnica f(x1) - f(x0) bliska zeru" << endl;
            return result;
        }

        //Obliczenie nowego punktu według wzoru siecznych: x2 = x1 - f(x1) * (x1-x0)/(f(x1)-f(x0))
        double x2 = x1 - fx1 * (x1 - x0) / (fx1 - fx0);
        double fx2 = func(x2);           //Obliczenie wartości funkcji w nowym punkcie

        //Tworzenie struktury z wynikami bieżącej iteracji
        pojedyncza_iteracja iter;
        iter.iteration = i;
        iter.x = x2;                     //Nowe przybliżenie
        iter.fx = fx2;                   //Wartość funkcji
        iter.error = abs(x2 - x1);       //Błąd jako różnica między kolejnymi przybliżeniami
        result.iterations.push_back(iter);

        //Sprawdzenie warunków zakończenia
        if (abs(fx2) < epsilon || abs(x2 - x1) < epsilon) {
            result.root = x2;             //Znalezione miejsce zerowe
            result.converged = true;      //Metoda się zbiegła
            return result;
        }

        //Aktualizacja punktów dla następnej iteracji (przesunięcie okna)
        x0 = x1;
        x1 = x2;
        fx0 = fx1;
        fx1 = fx2;
    }

    //Jeśli osiągnięto maksymalną liczbę iteracji
    result.root = x1;
    result.converged = false;
    return result;
}

//=== FUNKCJA EKSPORTU DANYCH DO CSV ===
//Funkcja do eksportu z jawnym podziałem na funkcje
void zapisz_csv_wszystkie_wyniki(const vector<wyniki>& all_results, const vector<string>& func_names) {
    ofstream main_file("wszystkie_wyniki.csv");
    if (main_file.is_open()) {
        main_file << "Funkcja;Opis_funkcji;Metoda;Miejsce_zerowe;Zbieznosc;Iteracja;X;F(X);Blad" << endl;

        for (size_t i = 0; i < all_results.size(); i++) {
            const wyniki& result = all_results[i];

            string func_name = result.method.find("f1") != string::npos ? "f1" :
                              result.method.find("f2") != string::npos ? "f2" :
                              result.method.find("f3") != string::npos ? "f3" : "unknown";

            for (const pojedyncza_iteracja& iter : result.iterations) {
                main_file << func_name << ";"
                         << "\"" << func_names[0] << "\";"  // Opis w cudzysłowach aby obsłużyć przecinki
                         << result.method << ","
                         << fixed << setprecision(12) << result.root << ";"
                         << (result.converged ? "TAK" : "NIE") << ";"
                         << iter.iteration << ";"
                         << fixed << setprecision(12) << iter.x << ";"
                         << scientific << setprecision(8) << iter.fx << ";"
                         << scientific << setprecision(8) << iter.error
                         << endl;
            }
        }
        main_file.close();
    }
}

void zapisz_csv(const vector<wyniki>& results, const vector<string>& func_names) {
    //Mapa do grupowania wyników według funkcji
    map<string, vector<wyniki>> results_by_function;

    //Grupowanie wyników według funkcji
    int func_index = 0;
    int results_per_function = 0;

    //Zliczamy ile wyników jest na funkcję (powinna być stała liczba)
    for (const wyniki& result : results) {
        if (results_per_function == 0) {
            //Szukanie pierwszego powtórzenia wzorca metod aby określić liczbę wyników na funkcję
            //Zakładany porządek: Bisekcja, Newton_analityczna, Newton_numeryczna, Sieczne
            //dla każdego znalezionego miejsca zerowego
        }
    }

    //Grupowanie na podstawie śledzenia indeksu funkcji
    string current_function = "";
    int result_counter = 0;

    for (size_t i = 0; i < results.size(); i++) {
        //Określanie nazwy funkcji na podstawie kolejności w programie głównym
        //Każda funkcja ma swoje przedziały, w każdym przedziale testujemy 4 metody

        //Szacujemy którą funkcję obecnie przetwarzamy
        string func_name;
        if (i < results.size() / 3) {
            func_name = "f1";
        } else if (i < (2 * results.size()) / 3) {
            func_name = "f2";
        } else {
            func_name = "f3";
        }

        results_by_function[func_name].push_back(results[i]);
    }

    //Eksport z jawnym przekazaniem nazwy funkcji
    zapisz_csv_wszystkie_wyniki(results, func_names);
}

//=== FUNKCJA GŁÓWNA PROGRAMU ===
void do_rownania_nieliniowe(double start = -3.0, double end = 4.0, double h = 0.1) {
    //Ustawienie kodowania UTF-8 w konsoli Windows
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    cout << "=== METODY NUMERYCZNE - ROZWIĄZYWANIE RÓWNAŃ NIELINIOWYCH ===" << endl;

    vector<wyniki> all_results;

    //Tablica wskaźników na funkcje
    double (*functions[])(double) = {f1, f2, f3};
    //Tablica wskaźników na pochodne analityczne
    double (*derivatives[])(double) = {f1_analitycznie, f2_analitycznie, f3_analitycznie};
    //Nazwy funkcji do wyświetlania
    string func_names[] = {"ln(x+1) - 1/(x+3)", "x^3 + 30*cos(x) - 1", "sin(3PIx)/(x+2) + 1/(x+4)"};

    //Pętla przez wszystkie trzy funkcje
    for (int f = 0; f < 3; f++) {
        cout << "\n=== FUNKCJA " << (f+1) << ": " << func_names[f] << " ===" << endl;

        double search_start = start;
        double search_end = end;
        double step = h;  //Krok do wyszukiwania przedziałów z miejscami zerowymi

        vector<pair<double, double>> intervals;  //Wektor par (początek, koniec) przedziału

        //Iteracja przez cały przedział poszukiwań z małym krokiem
        for (double x = search_start; x < search_end - step; x += step) {
            double f_x = functions[f](x);      //Wartość funkcji w punkcie x
            double f_x_next = functions[f](x + step);  //Wartość funkcji w punkcie x+krok

            //Jeśli funkcja zmienia znak, to w tym przedziale jest miejsce zerowe
            if (f_x * f_x_next < 0) {
                intervals.push_back(make_pair(x, x + step));
            }
        }

        cout << "Znaleziono " << intervals.size() << " przedziałów z miejscami zerowymi:" << endl;

        //Iteracja przez wszystkie znalezione przedziały
        for (size_t i = 0; i < intervals.size(); i++) {
            double a = intervals[i].first;   //Lewy koniec przedziału
            double b = intervals[i].second;  //Prawy koniec przedziału

            cout << "\nPrzedział [" << a << ", " << b << "]:" << endl;

            //=== METODA BISEKCJI ===
            cout << "  Metoda bisekcji: ";
            wyniki bisection_result = metoda_bisekcji(functions[f], a, b);

            //Ustawienie informacji o funkcji
            bisection_result.function_number = f + 1;
            bisection_result.function_name = "f" + to_string(f + 1);
            bisection_result.function_description = func_names[f];

            if (bisection_result.converged) {
                cout << "pierwiastek = " << fixed << setprecision(10) << bisection_result.root
                     << " (iteracji: " << bisection_result.iterations.size() << ")" << endl;
            } else {
                cout << "brak zbieżności" << endl;
            }
            all_results.push_back(bisection_result);  // Dodanie wyniku do głównego wektora

            //=== METODA NEWTONA Z POCHODNĄ ANALITYCZNĄ ===
            cout << "  Metoda Newtona (analityczna): ";
            double x0_newton = (a + b) / 2.0;  //Punkt startowy w środku przedziału
            wyniki newton_result = metoda_newtona(functions[f], derivatives[f], x0_newton);

            //Ustawienie informacji o funkcji
            newton_result.function_number = f + 1;
            newton_result.function_name = "f" + to_string(f + 1);
            newton_result.function_description = func_names[f];
            newton_result.method = "Newton_analityczna";

            if (newton_result.converged) {
                cout << "pierwiastek = " << fixed << setprecision(10) << newton_result.root
                     << " (iteracji: " << newton_result.iterations.size() << ")" << endl;
            } else {
                cout << "brak zbieżności" << endl;
            }
            all_results.push_back(newton_result);

            //=== METODA NEWTONA Z POCHODNĄ NUMERYCZNĄ ===
            cout << "  Metoda Newtona (numeryczna): ";
            //Funkcja pomocnicza do obliczania pochodnej numerycznej dla konkretnej funkcji
            double (*num_derivative)(double);
            if (f == 0) {
                num_derivative = [](double x) { return numeryczne_obliczanie_pochodnej(f1, x); };
            } else if (f == 1) {
                num_derivative = [](double x) { return numeryczne_obliczanie_pochodnej(f2, x); };
            } else {
                num_derivative = [](double x) { return numeryczne_obliczanie_pochodnej(f3, x); };
            }

            wyniki newton_num_result;
            newton_num_result.method = "Newton_numeryczna";
            newton_num_result.converged = false;
            newton_num_result.root = 0.0;
            newton_num_result.function_number = f + 1;
            newton_num_result.function_name = "f" + to_string(f + 1);
            newton_num_result.function_description = func_names[f];

            double x = x0_newton;  //Punkt startowy
            double fx, fpx;

            //Główna pętla metody Newtona z pochodną numeryczną
            for (int iter = 0; iter < 1000; iter++) {
                fx = functions[f](x);                           //Obliczenie wartości funkcji
                fpx = numeryczne_obliczanie_pochodnej(functions[f], x);    //Obliczenie pochodnej numerycznej

                //Sprawdzenie czy pochodna nie jest bliska zeru
                if (abs(fpx) < 1e-15) {
                    cout << "brak zbieżności (pochodna bliska zeru)" << endl;
                    break;
                }

                //Tworzenie struktury z wynikami bieżącej iteracji
                pojedyncza_iteracja iter_result;
                iter_result.iteration = iter;
                iter_result.x = x;
                iter_result.fx = fx;
                iter_result.error = (iter > 0) ? abs(x - newton_num_result.iterations[iter-1].x) : abs(fx);
                newton_num_result.iterations.push_back(iter_result);

                //Sprawdzenie warunku zakończenia
                if (abs(fx) < 1e-10) {
                    newton_num_result.root = x;
                    newton_num_result.converged = true;
                    break;
                }

                //Obliczenie nowego przybliżenia
                double x_new = x - fx / fpx;

                //Sprawdzenie zbieżności
                if (abs(x_new - x) < 1e-10) {
                    newton_num_result.root = x_new;
                    newton_num_result.converged = true;
                    break;
                }

                x = x_new;
            }

            if (newton_num_result.converged) {
                cout << "pierwiastek = " << fixed << setprecision(10) << newton_num_result.root
                     << " (iteracji: " << newton_num_result.iterations.size() << ")" << endl;
            } else {
                cout << "brak zbieżności" << endl;
            }
            all_results.push_back(newton_num_result);

            //=== METODA SIECZNYCH ===
            cout << "  Metoda siecznych: ";
            wyniki secant_result = metoda_siecznych(functions[f], a, b);  // Dwa punkty startowe to końce przedziału

            secant_result.function_number = f + 1;
            secant_result.function_name = "f" + to_string(f + 1);
            secant_result.function_description = func_names[f];

            if (secant_result.converged) {
                cout << "pierwiastek = " << fixed << setprecision(10) << secant_result.root
                     << " (iteracji: " << secant_result.iterations.size() << ")" << endl;
            } else {
                cout << "brak zbieżności" << endl;
            }
            all_results.push_back(secant_result);
        }
    }

    //=== EKSPORT WYNIKÓW DO PLIKÓW CSV ===
    cout << "\n=== EKSPORT DANYCH ===" << endl;

    //Główny plik ze wszystkimi wynikami
    ofstream main_file("wszystkie_wyniki.csv");
    if (main_file.is_open()) {
        // Nagłówek z informacjami
        main_file << "# WYNIKI METOD NUMERYCZNYCH ZNAJDOWANIA MIEJSC ZEROWYCH" << endl;
        main_file << "# Przedzial poszukiwan: ["<<to_string(start)<<", "<<to_string(end)<<"]" << endl;
        main_file << "# Dokladnosc: 1e-10" << endl;
        main_file << "# Funkcje:" << endl;
        main_file << "#   f1: " << func_names[0] << endl;
        main_file << "#   f2: " << func_names[1] << endl;
        main_file << "#   f3: " << func_names[2] << endl;
        main_file << "#" << endl;
        main_file << "Funkcja;Opis_funkcji;Metoda;Miejsce_zerowe;Zbieznosc;Liczba_iteracji;Koncowy_blad;Iteracja;X;F_X;Blad_iteracji" << endl;

        for (const wyniki& result : all_results) {
            double final_error = result.iterations.empty() ? 0.0 : result.iterations.back().error;

            for (const pojedyncza_iteracja& iter : result.iterations) {
                main_file << result.function_name << ";"
                         << "\"" << result.function_description << "\";"
                         << result.method << ";"
                         << fixed << setprecision(12) << result.root << ";"
                         << (result.converged ? "TAK" : "NIE") << ";"
                         << result.iterations.size() << ";"
                         << scientific << setprecision(8) << final_error << ";"
                         << iter.iteration << ";"
                         << fixed << setprecision(12) << iter.x << ";"
                         << scientific << setprecision(8) << iter.fx << ";"
                         << scientific << setprecision(8) << iter.error
                         << endl;
            }
        }
        main_file.close();
    }

    //Podsumowanie - tylko końcowe wyniki każdej metody
    ofstream summary_file("podsumowanie_wynikow.csv");
    if (summary_file.is_open()) {
        summary_file << "Funkcja;Opis;Metoda;Miejsce_zerowe;Zbieznosc;Liczba_iteracji;Koncowy_blad" << endl;

        for (const wyniki& result : all_results) {
            double final_error = result.iterations.empty() ? 0.0 : result.iterations.back().error;

            summary_file << result.function_name << ";"
                        << "\"" << result.function_description << "\";"
                        << result.method << ";"
                        << fixed << setprecision(12) << result.root << ";"
                        << (result.converged ? "TAK" : "NIE") << ";"
                        << result.iterations.size() << ";"
                        << scientific << setprecision(8) << final_error
                        << endl;
        }
        summary_file.close();
    }

    //Osobne pliki dla każdej funkcji
    map<string, vector<wyniki>> results_by_function;
    for (const wyniki& result : all_results) {
        string func_key = result.function_name;
        if (func_key.empty()) {
            cout << "WARNING: Znaleziono wynik bez nazwy funkcji, metoda: " << result.method << endl;
            func_key = "UNKNOWN";
        }
        results_by_function[func_key].push_back(result);
    }

    cout << "\nGrupowanie wyników według funkcji:" << endl;
    for (const auto& func_pair : results_by_function) {
        cout << "Funkcja " << func_pair.first << ": " << func_pair.second.size() << " wyników" << endl;
        for (const wyniki& result : func_pair.second) {
            cout << "  - " << result.method << " (root: " << result.root << ")" << endl;
        }
    }

    for (const auto& func_pair : results_by_function) {
        string func_name = func_pair.first;
        const vector<wyniki>& func_results = func_pair.second;

        if (func_results.empty()) continue;

        string filename = "wyniki_" + func_name + ".csv";
        ofstream func_file(filename);

        if (func_file.is_open()) {
            func_file << "# Wyniki dla funkcji " << func_name << ": " << func_results[0].function_description << endl;
            func_file << "# Znaleziono " << func_results.size() << " wynikow (wszystkie metody dla wszystkich miejsc zerowych)" << endl;
            func_file << "#" << endl;
            func_file << "Metoda;Miejsce_zerowe;Zbieznosc;Liczba_iteracji;Koncowy_blad;Iteracja;X;F_X;Blad_iteracji" << endl;

            for (const wyniki& result : func_results) {
                double final_error = result.iterations.empty() ? 0.0 : result.iterations.back().error;

                for (const pojedyncza_iteracja& iter : result.iterations) {
                    func_file << result.method << ";"
                             << fixed << setprecision(12) << result.root << ";"
                             << (result.converged ? "TAK" : "NIE") << ";"
                             << result.iterations.size() << ";"
                             << scientific << setprecision(8) << final_error << ";"
                             << iter.iteration << ";"
                             << fixed << setprecision(12) << iter.x << ";"
                             << scientific << setprecision(8) << iter.fx << ";"
                             << scientific << setprecision(8) << iter.error
                             << endl;
                }
            }
            func_file.close();
        }
    }

    //=== PODSUMOWANIE WYNIKÓW ===
    cout << "\n=== PODSUMOWANIE ===" << endl;
    cout << "Łączna liczba znalezionych miejsc zerowych: " << all_results.size() << endl;

    //Zliczanie zbieżnych wyników dla każdej metody
    int bisection_count = 0, newton_anal_count = 0, newton_num_count = 0, secant_count = 0;

    for (const wyniki& result : all_results) {  // Iteracja przez wszystkie wyniki
        if (result.converged) {  // Jeśli metoda się zbiegła
            if (result.method == "Bisekcja") bisection_count++;
            else if (result.method == "Newton_analityczna") newton_anal_count++;
            else if (result.method == "Newton_numeryczna") newton_num_count++;
            else if (result.method == "Sieczne") secant_count++;
        }
    }

    //Wyświetlenie statystyk zbieżności
    cout << "Zbieżne wyniki:" << endl;
    cout << "  - Metoda bisekcji: " << bisection_count << endl;
    cout << "  - Metoda Newtona (analityczna): " << newton_anal_count << endl;
    cout << "  - Metoda Newtona (numeryczna): " << newton_num_count << endl;
    cout << "  - Metoda siecznych: " << secant_count << endl;
}