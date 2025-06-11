#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <limits>
using namespace std;

//Gauss
struct Gauss {
    int N;
    vector<double> b;
    vector<vector<double>> A;
};

Gauss wczytaj_dane_Gauss(const string& filename, int lp) {
    ifstream file(filename);
    Gauss g;
    if (!file.is_open()) {
        cout << "Blad otwarcia pliku." << endl;
        exit(1);
    }
    string line;
    bool found = false;
    while (getline(file, line)) {
        if (line.find("l.p.: " + to_string(lp)) != string::npos) {
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Nie znaleziono linii z l.p.: " << lp << "." << endl;
        exit(1);
    }
    getline(file, line);
    stringstream ss(line.substr(line.find("=") + 1));
    ss >> g.N;
    getline(file, line);
    g.b.resize(g.N);
    for (int i = 0; i < g.N; ++i) {
        file >> g.b[i];
    }
    file.ignore(numeric_limits<streamsize>::max(), '\n'); //przejscie do kolejnej linii po b
    getline(file, line);
    g.A.resize(g.N, vector<double>(g.N));
    for (int i = 0; i < g.N; ++i) {
        for (int j = 0; j < g.N; ++j) {
            file >> g.A[i][j];
        }
    }
    file.close();
    return g;
}

void wypisz_macierz_rozszerzona(const vector<vector<double>>& A, const vector<double>& b, const int& n) {
    cout<< fixed<<setprecision(6)<<fixed;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            cout << setw(10) << A[i][j] << " ";
        cout << "| " << setw(10) << b[i] << endl;
    }
    cout << endl;
}

vector<double> eliminacja_gaussa(vector<vector<double>>& A, vector<double>& b, int n) {
    vector<double> x(n, 0.0); //wektor rozwiazan; poczatkowo wypelniony zerami
    cout<<"Poczatkowy uklad rownan:"<<endl;
    wypisz_macierz_rozszerzona(A, b, n);
    //eliminacja - przeksztalcenie macierzy do postaci trojkatnej gornej
    for (int k = 0; k < n; k++) {
        //czesciowy pivoting - szukanie wiersza z najwiekszym elementem w kolumnie k
        int maxRow = k;
        double maxVal = fabs(A[k][k]);

        for (int i = k + 1; i < n; i++) {
            if (fabs(A[i][k]) > maxVal) {
                maxVal = fabs(A[i][k]);
                maxRow = i;
            }
        }
        //sprawdzenie czy element na przekatnej jest zerowy (z dokladnoscia do epsilon)
        if (maxVal < 1e-10) {
            //sprawdzenie czy jest to przypadek rownan liniowo zaleznych
            bool isAllZero = true;
            for (int j = k; j < n; j++) {
                if (fabs(A[k][j]) > 1e-10) {
                    isAllZero = false;
                    break;
                }
            }
            if (isAllZero && fabs(b[k]) > 1e-10) {
                cout << "Uklad rownan jest sprzeczny - brak rozwiazania" << endl;
                return vector<double>(n, NAN); //zwracenie NaN jako oznaczenie braku rozwiazania
            } else if (isAllZero && fabs(b[k]) <= 1e-10) {
                cout << "Wiersz " << k << " jest liniowo zalezny od innych wierszy" << endl;
                //kontynuujemy - uklad moze miec nieskonczenie wiele rozwiazan
                continue;
            } else {
                cout << "Macierz osobliwa - determinant bliski zeru" << endl;
                return vector<double>(n, NAN);
            }
        }
        //zamiana wierszy, jesli znaleziono lepszy pivot
        if (maxRow != k) {
            cout << "Zamiana wierszy " << k << " i " << maxRow << " (czesciowy pivoting)" << endl;
            swap(A[k], A[maxRow]);
            swap(b[k], b[maxRow]);
            wypisz_macierz_rozszerzona(A, b, n);
        }
        //eliminacja zmiennych ponizej przekatnej
        for (int i = k + 1; i < n; i++) {
            double factor = A[i][k] / A[k][k];

            //jesli wspolczynnik jest bliski zeru, nie wykonujemy eliminacji
            if (fabs(factor) < 1e-10) continue;

            //odejmowanie odpowiednich wielokrotnosci wiersza k od wiersza i
            for (int j = k; j < n; j++) {
                A[i][j] -= factor * A[k][j];
            }
            b[i] -= factor * b[k];
        }
        cout << "Po eliminacji zmiennej x[" << k << "]:" << endl;
        wypisz_macierz_rozszerzona(A, b, n);
    }       //rozwiazanie ukladu trojkatnego (podstawienie wstecz)
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++) {
            sum += A[i][j] * x[j];
        }
        //sprawdzenie czy element na przekatnej jest zerowy
        if (fabs(A[i][i]) < 1e-10) {
            if (fabs(b[i] - sum) < 1e-10) {
                // 0 = 0, wiec mozna przyjac dowolna wartosc dla x[i]
                x[i] = 0.0; //przyjmujemy zero jako jedno z mozliwych rozwiazan
                cout << "Zmienna x[" << i << "] moze przyjmowac dowolna wartosc (przyjeto 0)" << endl;
            } else {
                // 0 != 0, uklad sprzeczny
                cout << "Uklad równan jest sprzeczny przy podstawieniu wstecz!" << endl;
                return vector<double>(n, NAN);
            }
        } else {
            //normalny przypadek - obliczenie x[i]
            x[i] = (b[i] - sum) / A[i][i];
        }
    }
    return x;
}
void weryfikacja(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& x, const int& n) {
    cout << "Weryfikacja rozwiazania:" << endl;
    double maxError = 0.0;
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < n; j++) {
            sum += A[i][j] * x[j];
        }

        double error = fabs(sum - b[i]);
        maxError = max(maxError, error);

        cout << "Rownanie " << i << ": ";
        for (int j = 0; j < n; j++) {
            cout << A[i][j] << " * " << x[j];
            if (j < n - 1) cout << " + ";
        }
        cout << " = " << sum << " (oczekiwano: " << b[i] << ", blad: " << error << ")" << endl;
    }

    cout << "Maksymalny blad: " << maxError << endl;
    if (maxError < 1e-10) {
        cout << "Rozwiazanie jest poprawne" << endl;
    } else if (maxError < 1e-5) {
        cout << "Rozwiazanie jest akceptowalne, ale wystepuja male bledy numeryczne." << endl;
    } else {
        cout << "Rozwiazanie zawiera znaczace bledy" << endl;
    }
    cout << endl;
}

void wypisz_rozwiazanie(const vector<double>& x) {
    cout << "Rozwiazanie ukladu rownan:" << endl;
    for (int i = 0; i < x.size(); i++) {
        cout << "x[" << i << "] = " << fixed << setprecision(6) << x[i] << endl;
    }
    cout << endl;
}

void do_gauss(const string& nazwa_pliku, int lp) {
    Gauss dane = wczytaj_dane_Gauss(nazwa_pliku, lp);
    if (dane.A.empty() || dane.b.empty()) {
        cout << "Brak danych do rozwiazania ukladu rownan." << endl;
        return;
    }

    cout << "Wczytano uklad rownan z pliku: " << nazwa_pliku << endl;
    wypisz_macierz_rozszerzona(dane.A, dane.b, dane.N);

    vector<double> rozwiazanie = eliminacja_gaussa(dane.A, dane.b, dane.N);
    if (rozwiazanie.empty() || isnan(rozwiazanie[0])) {
        cout << "Nie mozna znalezc rozwiazania." << endl;
        return;
    }

    wypisz_rozwiazanie(rozwiazanie);
    weryfikacja(dane.A, dane.b, rozwiazanie, dane.N);
}

//LU
struct Macierz {
    int N;
    vector<double> b;
    vector<vector<double>> A;
    Macierz(int n, vector<double> b, vector<vector<double>> A ) {
        this->N=n;
        this->b=b;
        this->A=A;
    }
    Macierz() {
        N = 0;
    }
};

//funkcja do odczytu układu równań z pliku
Macierz wczytaj_dane_LU(const string& filename, int lp) {
    ifstream file(filename);
    Macierz g;
    if (!file.is_open()) {
        cout << "Blad otwarcia pliku." << endl;
        exit(1);
    }
    string line;
    bool found = false;
    //szukamy linii z podanym l.p.
    while (getline(file, line)) {
        if (line.find("l.p.: " + to_string(lp)) != string::npos) {
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Nie znaleziono linii z l.p.: " << lp << "." << endl;
        exit(1);
    }
    //odczytujemy rozmiar macierzy N
    getline(file, line);
    stringstream ss(line.substr(line.find("=") + 1));
    ss >> g.N;
    //odczytujemy wektor wyrazów wolnych b
    getline(file, line);
    g.b.resize(g.N);
    for (int i = 0; i < g.N; ++i) {
        file >> g.b[i];
    }
    file.ignore(numeric_limits<streamsize>::max(), '\n'); //przejscie do kolejnej linii po b
    //odczytujemy macierz współczynników A
    getline(file, line);
    g.A.resize(g.N, vector<double>(g.N));
    for (int i = 0; i < g.N; ++i) {
        for (int j = 0; j < g.N; ++j) {
            file >> g.A[i][j];
        }
    }
    file.close();
    return g;
}

//funkcja wypisująca macierz
void wypisz_macierz(const vector<vector<double>>& A, const int& N, const string& nazwa) {
    cout<<"Macierz "<<nazwa<<":"<<endl;
    for (int i =0; i<N; i++) {
       for (int j=0; j<N; j++) {
           cout << setw(10) << fixed << setprecision(4) << A[i][j] << " ";
       }
        cout<< endl;
    }
    cout << endl;
}

//funkcja wypisująca wektor
void wypisz_wektor(const vector<double>& b, const int& N, const string& napis) {
    cout<<"Wektor "<<napis<<":"<<endl;
    for (int i = 0; i < N; i++) {
        cout << setw(10) << fixed << setprecision(4) << b[i] << " ";
    }
    cout << endl;
}

//funkcja do dekompozycji LU macierzy A z częściowym pivotingiem
void dekompozycja_LU(vector<vector<double>>& A, vector<vector<double>>& L, vector<vector<double>>& U, vector<int>& P, const int& N) {
    //inicjalizacja macierzy L i U
    L.resize(N, vector<double>(N, 0.0));
    U.resize(N, vector<double>(N, 0.0));
    //inicjalizacja wektora permutacji P
    P.resize(N);
    for (int i = 0; i < N; i++) {
        P[i] = i;  //początkowo brak permutacji
    }
    //inicjalizacja macierzy L jako macierzy jednostkowej (na przekątnej są jedynki)
    for (int i = 0; i < N; i++) {
        L[i][i] = 1.0;  //ustawienie elementu na przekątnej macierzy L na 1
    }
    //tworzenie kopii macierzy A do operacji eliminacji
    vector<vector<double>> PA = A;
    //algorytm dekompozycji LU z częściowym pivotingiem
    for (int k = 0; k < N; k++) {
        //częściowy pivoting - szukanie wiersza z największym elementem w kolumnie k
        double maxVal = fabs(PA[k][k]);
        int maxRow = k;
        for (int i = k + 1; i < N; i++) {
            if (fabs(PA[i][k]) > maxVal) {
                maxVal = fabs(PA[i][k]);
                maxRow = i;
            }
        }
        //sprawdzenie czy macierz jest osobliwa (element główny bliski zeru)
        if (maxVal < 1e-10) {
            cout << "Macierz osobliwa lub bliska osobliwosci. Element główny U[" << k << "][" << k << "] jest zbyt bliski zeru." << endl;
            cout << "Wartosc: " << maxVal << endl;
            //sprawdzenie, czy jest to przypadek wierszy liniowo zależnych
            bool isAllZero = true;
            for (int j = k; j < N; j++) {
                if (fabs(PA[k][j]) > 1e-10) {
                    isAllZero = false;
                    break;
                }
            }
            if (isAllZero) {
                cout << "Wiersz " << k << " jest liniowo zalezny od innych wierszy." << endl;
            }
            //kontynuujemy, ale ostrzegamy użytkownika
            cout << "Proba kontynuowania obliczania, ale wyniki mogą być niedokladne." << endl;
        }
        //jeśli znaleziono lepszy pivot, zamiana wierszy
        if (maxRow != k) {
            cout << "Zamiana wierszy " << k << " i " << maxRow << " (czesciowy pivoting)" << endl;
            //zamiana wierszy w macierzy PA
            for (int j = 0; j < N; j++) {
                swap(PA[k][j], PA[maxRow][j]);
            }
            //aktualizacja wektora permutacji
            swap(P[k], P[maxRow]);
            //jeśli nie jesteśmy w pierwszym kroku, musimy również zaktualizować macierz L
            if (k > 0) {
                for (int j = 0; j < k; j++) {
                    swap(L[k][j], L[maxRow][j]);
                }
            }
        }
        //obliczanie elementu k-tego wiersza macierzy U
        for (int j = k; j < N; j++) {
            double suma = 0.0;
            for (int s = 0; s < k; s++) {
                suma += L[k][s] * U[s][j];
            }
            U[k][j] = PA[k][j] - suma;
        }
        //obliczanie elementu k-tej kolumny macierzy L
        for (int i = k + 1; i < N; i++) {
            double suma = 0.0;
            for (int s = 0; s < k; s++) {
                suma += L[i][s] * U[s][k];
            }
            //sprawdzanie czy element na przekątnej jest bliski zeru
            if (fabs(U[k][k]) < 1e-10) {
                cout << "Ostrzezenie: Element U[" << k << "][" << k << "] jest bliski zeru. Przyjeta wartosc 0 dla L[" << i << "][" << k << "]." << endl;
                L[i][k] = 0.0;
            } else {
                L[i][k] = (PA[i][k] - suma) / U[k][k];
            }
        }

        //wypisujanie macierzy L i U w każdej iteracji
        cout << "Iteracja " << k + 1 << ":" << endl;
        wypisz_macierz(L, N, "L");
        wypisz_macierz(U, N, "U");
    }
}

//funkcja rozwiązująca układ Lz = Pb (podstawianie w przód)
vector<double> podstawienie_w_przod(const vector<vector<double>>& L, const vector<double>& b, const vector<int>& P, const int& N) {
    vector<double> z(N, 0.0);
    //najpierw tworzymy permutowany wektor b
    vector<double> Pb(N);
    for (int i = 0; i < N; i++) {
        Pb[i] = b[P[i]];
    }
    //podstawianie w przód na permutowanym wektorze
    for (int i = 0; i < N; i++) {
        double suma = 0.0;
        for (int j = 0; j < i; j++) {
            suma += L[i][j] * z[j];
        }
        //sprawdzamy czy element na przekątnej jest zerowy
        if (fabs(L[i][i]) < 1e-10) {
            cout << "Ostrzezenie: Element L[" << i << "][" << i << "] jest bliski zeru." << endl;
            //sprawdzamy czy mamy równanie postaci 0 = 0
            if (fabs(Pb[i] - suma) < 1e-10) {
                cout << "Rownanie jest tozsamosciowe. Przyjeto z[" << i << "] = 0." << endl;
                z[i] = 0.0;
            } else {
                cout << "Rownanie jest sprzeczne" << endl;
                z[i] = NAN;  //oznaczamy brak rozwiązania
                return z;    //zwracamy aktualny wektor z
            }
        } else {
            z[i] = (Pb[i] - suma) / L[i][i];
        }
    }
    return z;
}

//funkcja rozwiązująca układ Ux = z (podstawianie wstecz)
vector<double> podstawienie_wstecz(const vector<vector<double>>& U, const vector<double>& z, const int& N) {
    vector<double> x(N, 0.0);
    for (int i = N - 1; i >= 0; i--) {
        //jeśli z[i] jest NaN, to brak rozwiązania
        if (isnan(z[i])) {
            x[i] = NAN;
            continue;
        }
        double suma = 0.0;
        for (int j = i + 1; j < N; j++) {
            suma += U[i][j] * x[j];
        }
        //sprawdzamy czy element na przekątnej jest bliski zeru
        if (fabs(U[i][i]) < 1e-10) {
            cout << "Ostrzezenie: Element U[" << i << "][" << i << "] jest bliski zeru." << endl;
            //sprawdzamy czy mamy równanie postaci 0 = 0
            if (fabs(z[i] - suma) < 1e-10) {
                cout << "Rownanie jest tozsamosciowe. Przyjeto x[" << i << "] = 0." << endl;
                x[i] = 0.0;
            } else {
                cout << "Rownanie jest sprzeczne" << endl;
                x[i] = NAN;  //oznaczamy brak rozwiązania
                return x;    //zwracamy aktualny wektor x
            }
        } else {
            x[i] = (z[i] - suma) / U[i][i];
        }
    }
    return x;
}

//funkcja sprawdzająca poprawność rozwiązania
void sprawdz_rozwiazanie(const vector<vector<double>>& A, const vector<double>& x, const vector<double>& b, const int& N) {
    cout << "Sprawdzenie poprawnosci rozwiazania:" << endl;
    //sprawdzenie czy wektor x zawiera NaN (brak rozwiązania)
    bool hasSolution = true;
    for (int i = 0; i < N; i++) {
        if (isnan(x[i])) {
            hasSolution = false;
            break;
        }
    }
    if (!hasSolution) {
        cout << "Brak rozwiazania - uklad sprzeczny lub nieoznaczony." << endl;
        return;
    }
    cout << "A * x = b" << endl;
    //obliczenie wynikowego wektora A*x
    vector<double> wynik(N, 0.0);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            wynik[i] += A[i][j] * x[j];
        }
    }
    //wypisanie szczegółowego porównania równań
    cout << "Szczegolowa weryfikacja dla kazdego rownania:" << endl;
    double maxError = 0.0;
    for (int i = 0; i < N; i++) {
        cout << "Rownanie " << i << ": ";
        //wypisz lewą stronę równania
        for (int j = 0; j < N; j++) {
            if (j > 0) cout << " + ";
            cout << "(" << fixed << setprecision(2) << A[i][j] << " * " << x[j] << ")";
        }
        double error = fabs(wynik[i] - b[i]);
        maxError = max(maxError, error);
        cout << " = " << fixed << setprecision(4) << wynik[i]
             << " (oczekiwano: " << b[i]
             << ", blad: " << scientific << setprecision(6) << error << ")" << endl;
    }
    cout << fixed << setprecision(4) << endl;
    cout << "A * x = " << endl;
    for (int i = 0; i < N; i++) {
        cout << setw(10) << wynik[i] << " ";
    }
    cout << endl;
    cout << "b = " << endl;
    for (int i = 0; i < N; i++) {
        cout << setw(10) << b[i] << " ";
    }
    cout << endl;
    //obliczenie i wypisanie błędu
    double bladEuklides = 0.0;
    for (int i = 0; i < N; i++) {
        bladEuklides += (wynik[i] - b[i]) * (wynik[i] - b[i]);
    }
    bladEuklides = sqrt(bladEuklides);

    cout << endl;
    cout << "Maksymalny blad: " << scientific << setprecision(6) << maxError << endl;
    cout << "Blad (norma euklidesowa): " << scientific << setprecision(6) << bladEuklides << endl;

    //ocena jakości rozwiązania
    if (bladEuklides < 1e-10) {
        cout << "Rozwiazanie jest bardzo dokladne." << endl;
    } else if (bladEuklides < 1e-5) {
        cout << "Rozwiazanie jest akceptowalne, ale wystepuja male bledy numeryczne." << endl;
    } else if (bladEuklides < 1e-2) {
        cout << "Rozwiazanie zawiera znaczące bledy numeryczne, ale moze byc akceptowalne dla niektorych zastosowan." << endl;
    } else {
        cout << "Rozwiazanie zawiera duze bledy i prawdopodobnie jest niepoprawne." << endl;
    }
    cout << endl;
}

//funkcja wypisująca macierz rozszerzoną
void wypiszMacierzR(const vector<vector<double>>& A, const vector<double>& b, const int& n) {
    cout<< fixed<<setprecision(2)<<fixed;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            cout << setw(10) << A[i][j] << " ";
        cout << "| " << setw(10) << b[i] << endl;
    }
    cout << endl;
}

void test() {
    //testowy układ równań
    Macierz matrix;
    matrix.N=5;
    matrix.b = {1, 2, 3, 4, 5};
    matrix.A={
        {17, 24, 1, 8, 15},
        {23, 5, 7, 14, 16},
        {4, 6, 13, 20, 22},
        {10, 12, 19, 21, 3},
        {11, 18, 25, 2, 9}
    };
    cout<<"Układ równań Ax = b:" << endl;
    wypiszMacierzR(matrix.A, matrix.b, matrix.N);
    //tworzymy kopie macierzy A i wektora b, które mogą być modyfikowane
    vector<vector<double>> A = matrix.A;
    vector<double> b = matrix.b;
    //wykonanie dekompozycji LU z pivotingiem
    vector<vector<double>> L, U;
    vector<int> P;
    dekompozycja_LU(A, L, U, P, matrix.N);
    //wypisanie wektora permutacji
    cout << "Wektor permutacji P:" << endl;
    for (int i = 0; i < matrix.N; i++) {
        cout << P[i] << " ";
    }
    cout << endl << endl;
    //rozwiązanie układu Lz = Pb
    vector<double> z = podstawienie_w_przod(L, matrix.b, P, matrix.N);
    wypisz_wektor(z, matrix.N, "z (Lz = Pb)");
    //sprawdzenie czy układ ma rozwiązanie
    bool ma_rozwiazanie = true;
    for (int i = 0; i < matrix.N; i++) {
        if (isnan(z[i])) {
            ma_rozwiazanie = false;
            break;
        }
    }
    if (ma_rozwiazanie) {
        //rozwiązanie układu Ux = z
        vector<double> x = podstawienie_wstecz(U, z, matrix.N);
        wypisz_wektor(x, matrix.N, "x (Ux = z)");
        //sprawdzenie poprawności rozwiązania
        sprawdz_rozwiazanie(matrix.A, x, matrix.b, matrix.N);
    } else {
        cout << "Nie mozna rozwiazac ukladu - uklad sprzeczny lub nieoznaczony." << endl;
    }
}

void do_LU(const string& filename, int lp) {
    cout<<"Plik: "<<filename<<endl;
    //odczyt układu równań z pliku
    Macierz matrix = wczytaj_dane_LU(filename, lp);
    cout << "Uklad rownan Ax = b:" << endl;
    wypiszMacierzR(matrix.A, matrix.b, matrix.N);
    //tworzymy kopie macierzy A i wektora b, które mogą być modyfikowane
    vector<vector<double>> A = matrix.A;
    vector<double> b = matrix.b;
    //wykonanie dekompozycji LU z pivotingiem
    vector<vector<double>> L, U;
    vector<int> P;
    dekompozycja_LU(A, L, U, P, matrix.N);
    //wypisanie wektora permutacji
    cout << "Wektor permutacji P:" << endl;
    for (int i = 0; i < matrix.N; i++) {
        cout << P[i] << " ";
    }
    cout << endl << endl;
    //rozwiązanie układu Lz = Pb
    vector<double> z = podstawienie_w_przod(L, matrix.b, P, matrix.N);
    wypisz_wektor(z, matrix.N, "z (Lz = Pb)");
    //sprawdzenie czy układ ma rozwiązanie
    bool ma_rozwiazanie = true;
    for (int i = 0; i < matrix.N; i++) {
        if (isnan(z[i])) {
            ma_rozwiazanie = false;
            break;
        }
    }
    if (ma_rozwiazanie) {
        //rozwiązanie układu Ux = z
        vector<double> x = podstawienie_wstecz(U, z, matrix.N);
        wypisz_wektor(x, matrix.N, "x (Ux = z)");
        //sprawdzenie poprawności rozwiązania
        sprawdz_rozwiazanie(matrix.A, x, matrix.b, matrix.N);
    } else {
        cout << "Nie mozna rozwiazac ukladu - uklad sprzeczny lub nieoznaczony." << endl;
    }
}


