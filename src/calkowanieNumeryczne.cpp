#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>
#include <sstream>
#include <chrono>
#include <locale>

using namespace std;

struct kwadratury {
    int N;
    vector<double> ai;
    double a;
    double b;
};

kwadratury wczytaj_dane_kwadratury(const string& nazwa_pliku, int lp) {
    kwadratury data;
    string sciezka = "../data/" + nazwa_pliku;
    ifstream wczytaj(sciezka);
    if (!wczytaj.is_open()) {
        cout << "Blad otwarcia pliku." << endl;
        exit(1);
    }

    string szukany_wiersz = "l.p.: "+to_string(lp);
    string line;
    bool znaleziono = false;
    while (getline(wczytaj, line))
    {
        if (line.find(szukany_wiersz) != string::npos)
        {
            znaleziono = true;
            break;
        }
    }
    if (!znaleziono)
    {
        cerr << "Nie znaleziono linii z l.p.: 12." << endl;
        return 1;
    }

    //wczytujemy stopień wielomianu
    getline(wczytaj, line);
    size_t pos = line.find("=") + 1;
    data.N = stoi(line.substr(pos));

    //wczytujemy linię "ai:"
    getline(wczytaj, line);

    //wczytujemy współczynniki wielomianu
    getline(wczytaj, line);
    istringstream iss(line);
    data.ai.resize(data.N + 1);
    for (int i = 0; i <= data.N; i++) {
        iss >> data.ai[i];
    }

    //wczytujemy przedział całkowania
    getline(wczytaj, line);
    size_t pos_a = line.find("[a,b]:") + 6;
    size_t pos_b = line.find('\t', pos_a);
    data.a = stod(line.substr(pos_a, pos_b - pos_a));;
    data.b = std::stod(line.substr(pos_b));
    wczytaj.close();
    cout<<"N = "<<data.N<<endl;
    for (int i = 0; i <= data.N; i++) {
        cout<<"coef["<<i<<"] = "<<data.ai[i]<<endl;
    }
    cout<<"a = "<<data.a<<endl;
    cout<<"b = "<<data.b<<endl;

    wczytaj.close();
    return data;
}

//funkcja obliczająca wartość wielomianu w punkcie x za pomocą schematu Hornera
//przyjmuje wektor współczynników wielomianu oraz wartość x
double wielomian_hornera(const vector<double>& coefficients, const double& x)
{
    double result = 0.0;
    int n = coefficients.size() - 1; //stopień wielomianu

    //implementacja schematu Hornera - od najwyższej potęgi
    for (int i = n; i >= 0; i--) {
        //mnożymy dotychczasowy wynik przez x i dodajemy kolejny współczynnik
        result = result * x + coefficients[i];
    }
    return result;
}

//funkcja obliczająca funkcję na potrzeby całkowania
double f1(double x) {
    return x * pow(cos(x), 3);
}

/**
 * funkcja obliczająca wartość funkcji w zależności od wybranego trybu
 * mode 0: funckja podcałkowa x²·sin³(x)
 * mode 1: funkcja podcałkowa exp(x²)·(1-x)
 * mode 2: funkcja podcałkowa x·cos³(x)
 * mode 3: funkcja podcałkowa wielomian
 * @param mode - tryb funkcji
 * @param x - punkt, w którym obliczamy wartość funkcji
 * @param coeffs - wektor współczynników wielomianu (używany tylko dla mode=3)
 * @return - wartość funkcji w punkcie x
 */
double wybor_funkcji(int mode, double x, const vector<double>& coeffs = vector<double>()) {
    switch (mode) {
        case 0:
            return f2(x);
        case 1:
            return f3(x);
        case 2:
            return f1(x);
        case 3:
            return wielomian_hornera(coeffs, x);
        default:
            cerr << "Nieobsługiwany tryb funkcji: " << mode << endl;
            return 0.0;
    }
}

//funkcja implementująca metodę prostokątów
//przyjmuje funkcję, przedział całkowania [a, b], liczbę przedziałów n i tryb funkcji
double metoda_prostokatow(double a, double b, int n, int mode,const vector<double>& coefficients = vector<double>()) {
    //obliczamy krok całkowania
    double h = (b - a) / n;

    double sum = 0.0;

    //obliczamy sumę prostokątów (punkty środkowe)
    for (int i = 0; i < n; i++) {
        //obliczamy środek i-tego przedziału
        double x = a + (i + 0.5) * h;
        //dodajemy wartość funkcji w środku przedziału
        sum += wybor_funkcji(mode, x,coefficients);
    }

    //mnożymy przez h, zgodnie ze wzorem metody prostokątów
    return sum * h;
}

//funkcja implementująca metodę trapezów
//przyjmuje funkcję, przedział całkowania [a, b], liczbę przedziałów n i tryb funkcji
double metoda_trapezow(double a, double b, int n, int mode, const vector<double>& coefficients = vector<double>()) {
    //obliczamy krok całkowania
    double h = (b - a) / n;

    //inicjalizujemy sumę wartością funkcji na końcach przedziału
    double sum = wybor_funkcji(mode,a, coefficients ) + wybor_funkcji(mode, b,coefficients);

    //dodajemy wartości funkcji w punktach wewnętrznych
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        //mnożymy przez 2, zgodnie ze wzorem metody trapezów
        sum += 2 * wybor_funkcji(mode,x, coefficients );
    }

    //mnożymy przez h/2, zgodnie ze wzorem metody trapezów
    return sum * h / 2;
}

//funkcja implementująca metodę Simpsona
//przyjmuje funkcję, przedział całkowania [a, b], liczbę przedziałów n i tryb funkcji
//liczba przedziałów n musi być parzysta
double metoda_simpsona(double a, double b, int n, int mode,const vector<double>& coefficients = vector<double>()) {
    //sprawdzamy, czy liczba podziałów jest parzysta
    if (n % 2 != 0) {
        cerr << "Liczba podzialow dla metody Simpsona musi byc parzysta" << endl; //cerr bo strumień błędów
        n = n + 1; //korygujemy liczbę przedziałów na parzystą
    }
    //obliczamy krok całkowania
    double h = (b - a) / n;
    //inicjalizujemy sumę wartością funkcji na końcach przedziału
    double sum = wybor_funkcji(mode,a, coefficients ) + wybor_funkcji(mode, b, coefficients);
    //dodajemy wartości funkcji w punktach wewnętrznych
    for (int i = 1; i < n; i++) {
        double x = a + i * h;

        //dla punktów o indeksie nieparzystym mnożymy przez 4, dla parzystych przez 2
        if (i % 2 == 1) {
            sum += 4 * wybor_funkcji(mode, x, coefficients);
        } else {
            sum += 2 * wybor_funkcji(mode, x, coefficients);
        }
    }
    //mnożymy przez h/3, zgodnie ze wzorem metody Simpsona
    return sum * h / 3;
}

//funkcja obliczająca błąd bezwzględny
double oblicz_blad_bezwzgledny(double approximateValue, double exactValue) {
    return abs(approximateValue - exactValue);
}

double oblicz_blad_wzgledny(double approximateValue, double exactValue) {
    return (abs(approximateValue - exactValue)) / abs(exactValue)*100;
}

//to jest to samo co całkowanie 2 tylko, że tu jest mierzony czas

/*void do_calkowanie1(const string& nazwa_pliku, int lp) {
    kwadratury data = wczytaj_dane_kwadratury(nazwa_pliku, lp);
    //liczba przedziałów
    int n = 2500;
    //czas liczenia wartości całki dla wielomianu Hornera
    auto startRH = chrono::high_resolution_clock::now();
    //obliczamy wartość całki przy użyciu metody prostokątów
    double resultRectangle = metoda_prostokatow(data.a, data.b, n, 0, data.ai);
    auto endRH = chrono::high_resolution_clock::now();
    auto startTH = chrono::high_resolution_clock::now();
    //obliczamy wartość całki przy użyciu metody trapezów
    double resultTrapezoidal = metoda_trapezow(data.a, data.b, n, 0, data.ai);
    auto endTH = chrono::high_resolution_clock::now();
    auto startSH = chrono::high_resolution_clock::now();
    //obliczamy wartość całki przy użyciu metody Simpsona
    double resultSimpson = metoda_simpsona(data.a, data.b, n, 0, data.ai);
    auto endSH = chrono::high_resolution_clock::now();

    //czas liczenia wartości całki dla specjalnej funkcji
    double aF = 3.5;
    double bF = 6.52968912439344;
    auto startRF = chrono::high_resolution_clock::now();
    //obliczenie wartość całki przy użyciu metody prostokątów
    double resultRectangleF = metoda_prostokatow(data.a, data.b, n, 1, data.ai);
    auto endRF = chrono::high_resolution_clock::now();
    auto startTF = chrono::high_resolution_clock::now();
    //obliczenie wartość całki przy użyciu metody trapezów
    double resultTrapezoidalF = metoda_trapezow(data.a, data.b, n, 1, data.ai);
    auto endTF = chrono::high_resolution_clock::now();
    auto startSF = chrono::high_resolution_clock::now();
    //obliczenie wartość całki przy użyciu metody Simpsona
    double resultSimpsonF = metoda_simpsona(data.a, data.b, n, 1, data.ai);
    auto endSF = chrono::high_resolution_clock::now();
    //obliczenie czas wykonania dla każdej metody
    auto timeRectangleH = chrono::duration_cast<chrono::microseconds>(endRH - startRH).count();
    auto timeTrapezoidalH = chrono::duration_cast<chrono::microseconds>(endTH - startTH).count();
    auto timeSimpsonH = chrono::duration_cast<chrono::microseconds>(endSH - startSH).count();
    auto timeRectangleF = chrono::duration_cast<chrono::microseconds>(endRF - startRF).count();
    auto timeTrapezoidalF = chrono::duration_cast<chrono::microseconds>(endTF - startTF).count();
    auto timeSimpsonF = chrono::duration_cast<chrono::microseconds>(endSF - startSF).count();
    //wypisanie wyników
    cout << fixed << std::setprecision(12);
    cout << "Wyniki dla wielomianu Hornera:" << endl;
    cout << "Metoda prostokatow: " << resultRectangle << " (czas: " << timeRectangleH << " mikrosekund)" << endl;
    cout << "Metoda trapezow: " << resultTrapezoidal << " (czas: " << timeTrapezoidalH << " mikrosekund)" << endl;
    cout << "Metoda Simpsona: " << resultSimpson << " (czas: " << timeSimpsonH << " mikrosekund)" << endl;
    cout << "Wyniki dla funkcji specjalnej:" << endl;
    cout << "Metoda prostokatow: " << resultRectangleF << " (czas: " << timeRectangleF << " mikrosekund)" << endl;
    cout << "Metoda trapezow: " << resultTrapezoidalF << " (czas: " << timeTrapezoidalF << " mikrosekund)" << endl;
    cout << "Metoda Simpsona: " << resultSimpsonF << " (czas: " << timeSimpsonF << " mikrosekund)" << endl;
    //obliczamy błąd bezwzględny
    double exactValueH = (2141792/21); //wartość dokładna całki !!! TO JEST WARTOSC DLA LP 12
    double exactValueF = 4.20250409000001; //wartość dokładna całki dla funkcji specjalnej
    double errorRectangleH = oblicz_blad_bezwzgledny(resultRectangle, exactValueH);
    double errorTrapezoidalH = oblicz_blad_bezwzgledny(resultTrapezoidal, exactValueH);
    double errorSimpsonH = oblicz_blad_bezwzgledny(resultSimpson, exactValueH);
    double errorRectangleF = oblicz_blad_bezwzgledny(resultRectangleF, exactValueF);
    double errorTrapezoidalF = oblicz_blad_bezwzgledny(resultTrapezoidalF, exactValueF);
    double errorSimpsonF = oblicz_blad_bezwzgledny(resultSimpsonF, exactValueF);
    cout << "Blad bezwzgledny dla wielomianu Hornera:" << endl;
    cout << "Metoda prostokatow: " << errorRectangleH << endl;
    cout << "Metoda trapezow: " << errorTrapezoidalH << endl;
    cout << "Metoda Simpsona: " << errorSimpsonH << endl;
    cout << "Blad bezwzgledny dla funkcji specjalnej:" << endl;
    cout << "Metoda prostokatow: " << errorRectangleF << endl;
    cout << "Metoda trapezow: " << errorTrapezoidalF << endl;
    cout << "Metoda Simpsona: " << errorSimpsonF << endl;
    //obliczamy błąd względny
    exactValueH = (2141792/21); // wartość dokładna całki
    exactValueF = 4.20250409000001; // wartość dokładna całki dla funkcji specjalnej
    errorRectangleH = oblicz_blad_wzgledny(resultRectangle, exactValueH);
    errorTrapezoidalH = oblicz_blad_wzgledny(resultTrapezoidal, exactValueH);
    errorSimpsonH = oblicz_blad_wzgledny(resultSimpson, exactValueH);
    errorRectangleF = oblicz_blad_wzgledny(resultRectangleF, exactValueF);
    errorTrapezoidalF = oblicz_blad_wzgledny(resultTrapezoidalF, exactValueF);
    errorSimpsonF = oblicz_blad_wzgledny(resultSimpsonF, exactValueF);
    //wypisujemy błędy
    cout << "Blad wzgledny dla wielomianu Hornera:" << endl;
    cout << "Metoda prostokatow: " << errorRectangleH << " %" << endl;
    cout << "Metoda trapezow: " << errorTrapezoidalH << " %" << endl;
    cout << "Metoda Simpsona: " << errorSimpsonH << " %" << endl;
    cout << "Blad bezwzgledny dla funkcji specjalnej:" << endl;
    cout << "Metoda prostokatow: " << errorRectangleF << " %" << endl;
    cout << "Metoda trapezow: " << errorTrapezoidalF << " %" << endl;
    cout << "Metoda Simpsona: " << errorSimpsonF << " %" << endl;
}*/

//całkowanie numeryczne 2
/**
 * Struktura przechowująca węzły i wagi dla kwadratur Gaussa-Legendre'a
 * Węzły (points) to punkty na przedziale [-1, 1], w których będziemy obliczać wartość funkcji
 * Wagi (weights) określają współczynniki, przez które mnożymy wartości funkcji w węzłach
 */
struct gauss_legendre_punkty {
    vector<double> points;  //Węzły kwadratury
    vector<double> weights; //Wagi kwadratury
};

/**
 * Funkcja inicjalizująca węzły i wagi dla różnych stopni kwadratur Gaussa-Legendre'a
 *
 * @param n - liczba punktów (węzłów) kwadratury
 * @return - struktura zawierająca węzły i wagi
 */
gauss_legendre_punkty inicjalizacja_gauss_legendre(int n) {
    gauss_legendre_punkty result;

    //W zależności od liczby punktów kwadratury, inicjalizujemy węzły i wagi
    switch (n) {
        case 1:
            //Dla jednopunktowej kwadratury G-L
            result.points = {0.0}; //Jeden węzeł w punkcie 0
            result.weights = {2.0}; //Waga równa 2
            break;
        case 2:
            //Dla dwupunktowej kwadratury G-L
            result.points = {-1.0/sqrt(3.0), 1.0/sqrt(3.0)}; //Węzły w punktach +-1sqrt(3)
            result.weights = {1.0, 1.0}; //Wagi równe 1
            break;
        case 3:
            //Dla trzypunktowej kwadratury G-L
            result.points = {-sqrt(3.0/5.0), 0.0, sqrt(3.0/5.0)}; //Węzły w punktach -sqrt(3/5), 0, sqrt(3/5)
            result.weights = {5.0/9.0, 8.0/9.0, 5.0/9.0}; //Wagi: 5/9, 8/9, 5/9
            break;
        case 4:
            //Dla czteropunktowej kwadratury G-L
        {
            //Obliczamy wartości węzłów
            double p1 = sqrt((3.0 - 2.0 * sqrt(6.0/5.0)) / 7.0);
            double p2 = sqrt((3.0 + 2.0 * sqrt(6.0/5.0)) / 7.0);

            //Obliczamy wartości wag
            double w1 = (18.0 + sqrt(30.0)) / 36.0;
            double w2 = (18.0 - sqrt(30.0)) / 36.0;

            //Zapisujemy węzły i wagi do struktury
            result.points = {-p2, -p1, p1, p2};
            result.weights = {w2, w1, w1, w2};
        }
            break;
        case 5:
        {
            double p1 = (1.0/3.0)*sqrt(5.0-2.0*sqrt(10.0/7.0));
            double p2 = (1.0/3.0)*sqrt(5.0+2.0*sqrt(10.0/7.0));
            double p3 = 0.0;
            double w1 = (322.0+13.0*sqrt(70.0))/900.0;
            double w2 = (322.0-13.0*sqrt(70.0))/900.0;
            double w3 = 128.0/225.0;
            result.points = {-p2, -p1, p3, p1, p2};
            result.weights = {w2, w1, w3, w1, w2};
            break;
        }
        default:
            //Dla innych wartości wyświetlamy błąd
            cerr << "Nieobsługiwana liczba punktów kwadratury: " << n << endl;
            break;
    }

    return result;
}

/**
 * Funkcja podcałkowa dla pierwszego zadania: x^2*sin^3(x)
 *
 * @param x - zmienna niezależna
 * @return - wartość funkcji w punkcie x
 */
double f2(double x) {
    return x * x * pow(sin(x), 3);
}

/**
 * Funkcja podcałkowa dla drugiego zadania: exp(x^2)*(1-x)
 *
 * @param x - zmienna niezależna
 * @return - wartość funkcji w punkcie x
 */
double f3(double x) {
    return exp(x * x) * (1.0 - x);
}

/**
 * Funkcja obliczająca dokładną wartość pierwszej całki (analitycznie)
 *
 * @return - dokładna wartość całki pierwszej
 */
double dokladna_wartosc1() {
    return -10.1010101105917;
}

/**
 * Funkcja obliczająca dokładną wartość drugiej całki (analitycznie)
 *
 * @return - dokładna wartość całki drugiej
 */
double dokladna_wartosc() {
    return -9876.543210075460;
}

/**
 * Funkcja opakowująca metodę Gaussa-Legendre'a, aby miała podobny interfejs jak inne metody
 * @param a - dolna granica całkowania
 * @param b - górna granica całkowania
 * @param n - liczba punktów kwadratury (nie liczba przedziałów jak w innych metodach)
 * @param mode - tryb funkcji (0-3)
 * @param coeffs - wektor współczynników wielomianu (używany tylko dla mode=3)
 * @return - przybliżona wartość całki
 */
double metoda_gauss_legendre(double a, double b, int n, int mode, const vector<double>& coeffs = vector<double>()) {
    //Inicjalizujemy węzły i wagi dla kwadratury Gaussa-Legendre'a
    gauss_legendre_punkty gl = inicjalizacja_gauss_legendre(n);

    //Współczynniki przekształcenia liniowego z przedziału [-1, 1] na [a, b]
    double halfLength = (b - a) / 2.0; //(b-a)/2 - połowa długości przedziału
    double midPoint = (a + b) / 2.0;   //(a+b)/2 - środek przedziału

    //Zmienna przechowująca sumę wartości całki
    double result = 0.0;

    //Obliczanie sumy dla kwadratury
    for (int i = 0; i < n; ++i) {
        //Przekształcenie liniowe punktu z [-1, 1] na [a, b]
        double x = halfLength * gl.points[i] + midPoint;

        //Dodajemy do sumy wartość f(x) * wi
        result += gl.weights[i] * wybor_funkcji(mode, x, coeffs);
    }

    //Mnożymy sumę przez współczynnik (b-a)/2 z przekształcenia zmiennych
    result *= halfLength;

    return result;
}

/**
 * Funkcja implementująca metodę Gaussa-Legendre'a z podziałem na przedziały
 * @param a - dolna granica całkowania
 * @param b - górna granica całkowania
 * @param n - liczba punktów kwadratury w każdym przedziale
 * @param intervals - liczba przedziałów, na które dzielimy zakres całkowania
 * @param mode - tryb funkcji (0-3)
 * @param coeffs - wektor współczynników wielomianu (używany tylko dla mode=3)
 * @return - przybliżona wartość całki
 */
double metoda_gauss_legendre_przedzialy(double a, double b, int n, int intervals, int mode, const vector<double>& coeffs = vector<double>()) {
    //Obliczamy długość każdego przedziału
    double intervalLength = (b - a) / intervals;

    //Zmienna przechowująca sumę wartości całki
    double result = 0.0;

    //Dla każdego przedziału obliczamy całkę używając kwadratury Gaussa-Legendre'a
    for (int i = 0; i < intervals; ++i) {
        //Obliczamy granice bieżącego przedziału
        double subIntervalStart = a + i * intervalLength;
        double subIntervalEnd = a + (i + 1) * intervalLength;

        //Obliczamy całkę na bieżącym przedziale i dodajemy do wyniku
        result += metoda_gauss_legendre(subIntervalStart, subIntervalEnd, n, mode, coeffs);
    }

    return result;
}

/**
 * Funkcja główna programu
 */
void do_calkowanie2(const string& nazwa_pliku, int lp) {
#ifdef _WIN32
    //Ustawienie strony kodowej konsoli na UTF-8 (65001)
    system("chcp 65001 > nul");
#endif

    //Zadanie 1: Obliczenie całki x^2*sin^3(x) w przedziale [1, 4.764798248]
    double a1 = 1.0;
    double b1 = 4.764798248;
    //Wartość dokładna całki (do obliczenia błędu)
    double exact1 = dokladna_wartosc1();

    //Zadanie 2: Obliczenie całki exp(x^2)*(1-x) w przedziale [-2, 3.20870913294]
    double a2 = -2.0;
    double b2 = 3.20870913294;
    //Wartość dokładna całki (do obliczenia błędu)
    double exact2 = dokladna_wartosc();

    //Całka 3: Obliczenie całki x*cos^3(x) w przedziale [3.5, 6.52968912439344]
    double a3 = 3.5;
    double b3 = 6.52968912439344;
    double exact3 = 4.20250409000001;

    //Całka 4: Obliczanie całki z wielomianu
    kwadratury data = wczytaj_dane_kwadratury(nazwa_pliku, lp);
    double exact4 = (2141792.0/21.0); //!!! TO JEST WARTOSC DLA LP 12

    //Definicja przedziałów do testów
    vector<pair<double, double>> intervals = {
        {a1, b1}, {a2, b2}, {a3, b3}, {data.a, data.b}
    };

    //Definicja dokładnych wartości
    vector<double> exactValues = {exact1, exact2, exact3, exact4};

    //Nazwy funkcji dla raportów
    vector<string> functionNames = {
        "x^2*sin^3(x)",
        "exp(x^2)*(1-x)",
        "x*cos^3(x)",
        "wielomian (stopien 6)"
    };

    //Liczba punktów kwadratury dla metody Gaussa-Legendre'a
    vector<int> glPoints = {2, 3, 4, 5};

    //Liczba przedziałów dla klasycznych metod
    int numIntervals = 1000;

    //Liczba przedziałów do testowania dla metody Gaussa-Legendre'a z podziałem
    vector<int> glIntervals = {10, 50, 100};

    cout << fixed << setprecision(12);

    //Otwarcie pliku dla porównania wszystkich metod
    ofstream compareAllFile("compare_all_methods.csv");
    compareAllFile << "Funkcja;Metoda;Punkty;Przedzialy;Wartosc;Blad\n";

    //Porównanie wszystkich metod dla wszystkich funkcji
    for (int funcIndex = 0; funcIndex < 4; ++funcIndex) {
        double a = intervals[funcIndex].first;
        double b = intervals[funcIndex].second;
        double exactValue = exactValues[funcIndex];
        string funcName = functionNames[funcIndex];

        cout << "Funkcja " << funcIndex+1 << ": " << funcName << " w przedziale ["
             << a << ", " << b << "]" << endl;
        cout << "Wartość dokładna: " << exactValue << endl;

        //Metoda prostokątów
        double rectResult;
        if (funcIndex == 3) {
            rectResult = metoda_prostokatow(a, b, numIntervals, funcIndex, data.ai);
        } else {
            rectResult = metoda_prostokatow(a, b, numIntervals, funcIndex);
        }
        double rectError = abs(rectResult - exactValue);

        cout << "  Metoda prostokątów (" << numIntervals << " przedziałów): "
             << rectResult << " (błąd: " << rectError << ")" << endl;
        compareAllFile << funcName << ";Metoda prostokatow;" << numIntervals << ";-;"
                     << rectResult << ";" << rectError << "\n";

        //Metoda trapezów
        double trapResult;
        if (funcIndex == 3) {
            trapResult = metoda_trapezow(a, b, numIntervals, funcIndex, data.ai);
        } else {
            trapResult = metoda_trapezow(a, b, numIntervals, funcIndex);
        }
        double trapError = abs(trapResult - exactValue);

        cout << "  Metoda trapezów (" << numIntervals << " przedziałów): "
             << trapResult << " (błąd: " << trapError << ")" << endl;
        compareAllFile << funcName << ";Metoda trapezow;" << numIntervals << ";-;"
                     << trapResult << ";" << trapError << "\n";

        //Metoda Simpsona
        double simpResult;
        if (funcIndex == 3) {
            simpResult = metoda_simpsona(a, b, numIntervals, funcIndex, data.ai);
        } else {
            simpResult = metoda_simpsona(a, b, numIntervals, funcIndex);
        }
        double simpError = abs(simpResult - exactValue);

        cout << "  Metoda Simpsona (" << numIntervals << " przedziałów): "
             << simpResult << " (błąd: " << simpError << ")" << endl;
        compareAllFile << funcName << ";Metoda Simpsona;" << numIntervals << ";-;"
                     << simpResult << ";" << simpError << "\n";

        //Metoda Gaussa-Legendre'a dla różnych liczb punktów
        for (int points : glPoints) {
            double glResult;
            if (funcIndex == 3) {
                glResult = metoda_gauss_legendre(a, b, points, funcIndex, data.ai);
            } else {
                glResult = metoda_gauss_legendre(a, b, points, funcIndex);
            }
            double glError = abs(glResult - exactValue);

            cout << "  Metoda Gaussa-Legendre'a (" << points << " punkty): "
                 << glResult << " (błąd: " << glError << ")" << endl;
            compareAllFile << funcName << ";Metoda G-L;" << points << ";1;"
                         << glResult << ";" << glError << "\n";

            //Metoda Gaussa-Legendre'a z podziałem na przedziały
            for (int subintervals : glIntervals) {
                double glWithIntervalsResult;
                if (funcIndex == 3) {
                    glWithIntervalsResult = metoda_gauss_legendre_przedzialy(a, b, points, subintervals, funcIndex, data.ai);
                } else {
                    glWithIntervalsResult = metoda_gauss_legendre_przedzialy(a, b, points, subintervals, funcIndex);
                }
                double glWithIntervalsError = abs(glWithIntervalsResult - exactValue);

                cout << "  Metoda Gaussa-Legendre'a (" << points << " punkty, " << subintervals << " przedziały): "
                     << glWithIntervalsResult << " (błąd: " << glWithIntervalsError << ")" << endl;
                compareAllFile << funcName << ";Metoda G-L z podzialem;" << points << ";" << subintervals << ";"
                             << glWithIntervalsResult << ";" << glWithIntervalsError << "\n";
            }
        }

        cout << endl;
    }

    compareAllFile.close();

    //Dodatkowe testy dla G-L z przedziałami - badanie zbieżności
    cout << "Testy zbieżności dla kwadratury Gaussa-Legendre'a z podziałem na przedziały" << endl;

    ofstream convergenceFileGLWithIntervals("convergence_gl_with_intervals.csv");
    convergenceFileGLWithIntervals << "Funkcja;Punkty;Przedzialy;Wartosc;Blad\n";

    //Testy dla różnej liczby przedziałów przy stałej liczbie punktów kwadratury
    vector<int> testIntervals = {1, 2, 5, 10, 20, 50, 100};
    int fixedPoints = 3; // Stała liczba punktów kwadratury

    for (int funcIndex = 0; funcIndex < 4; ++funcIndex) {
        double a = intervals[funcIndex].first;
        double b = intervals[funcIndex].second;
        double exactValue = exactValues[funcIndex];
        string funcName = functionNames[funcIndex];

        cout << "Funkcja " << funcIndex+1 << ": " << funcName << endl;

        for (int subintervals : testIntervals) {
            double result;
            if (funcIndex == 3) {
                result = metoda_gauss_legendre_przedzialy(a, b, fixedPoints, subintervals, funcIndex, data.ai);
            } else {
                result = metoda_gauss_legendre_przedzialy(a, b, fixedPoints, subintervals, funcIndex);
            }
            double error = abs(result - exactValue);

            cout << "  " << fixedPoints << " punkty, " << subintervals << " przedziały: "
                 << result << " (błąd: " << error << ")" << endl;
            convergenceFileGLWithIntervals << funcName << ";" << fixedPoints << ";" << subintervals << ";"
                                         << result << ";" << error << "\n";
        }

        cout << endl;
    }

    convergenceFileGLWithIntervals.close();

    //Dodatkowe porównanie - ile przedziałów metody G-L potrzeba, aby uzyskać podobną dokładność jak metody klasyczne
    cout << "Porównanie liczby ewaluacji funkcji potrzebnej do osiągnięcia podobnej dokładności" << endl;

    ofstream evaluationsCompareFile("evaluations_comparison.csv");
    evaluationsCompareFile << "Funkcja;Metoda;Liczba_ewaluacji;Dokladnosc\n";

    //Liczba ewaluacji dla metod klasycznych (dla n przedziałów)
    //Prostokąty: n ewaluacji
    //Trapezy: n+1 ewaluacji
    //Simpson: 2n+1 ewaluacji

    //Liczba ewaluacji dla G-L: n punktów * m przedziałów

    vector<int> smallerIntervals = {10, 20, 50, 100, 200, 500, 1000};

    for (int funcIndex = 0; funcIndex < 4; ++funcIndex) {
        double a = intervals[funcIndex].first;
        double b = intervals[funcIndex].second;
        double exactValue = exactValues[funcIndex];
        string funcName = functionNames[funcIndex];

        cout << "Funkcja " << funcIndex+1 << ": " << funcName << endl;

        //Zbadajmy dokładność metod klasycznych
        for (int n : smallerIntervals) {
            double rectResult, trapResult, simpResult;

            if (funcIndex == 3) {
                rectResult = metoda_prostokatow(a, b, n, funcIndex, data.ai);
                trapResult = metoda_trapezow(a, b, n, funcIndex, data.ai);
                simpResult = metoda_simpsona(a, b, n, funcIndex, data.ai);
            } else {
                rectResult = metoda_prostokatow(a, b, n, funcIndex);
                trapResult = metoda_trapezow(a, b, n, funcIndex);
                simpResult = metoda_simpsona(a, b, n, funcIndex);
            }

            double rectError = abs(rectResult - exactValue);
            double trapError = abs(trapResult - exactValue);
            double simpError = abs(simpResult - exactValue);

            evaluationsCompareFile << funcName << ";Prostokaty;" << n << ";" << rectError << "\n";
            evaluationsCompareFile << funcName << ";Trapezy;" << n+1 << ";" << trapError << "\n";
            evaluationsCompareFile << funcName << ";Simpson;" << 2*n+1 << ";" << simpError << "\n";
        }

        //Porównajmy z G-L z różnymi kombinacjami punktów i przedziałów
        for (int points : {2, 3, 4, 5}) {
            for (int intervals : {1, 2, 5, 10, 20, 50, 100}) {
                double glResult;

                if (funcIndex == 3) {
                    glResult = metoda_gauss_legendre_przedzialy(a, b, points, intervals, funcIndex, polynomialCoeffs);
                } else {
                    glResult = metoda_gauss_legendre_przedzialy(a, b, points, intervals, funcIndex);
                }

                double glError = abs(glResult - exactValue);
                int evaluations = points * intervals;

                evaluationsCompareFile << funcName << ";G-L " << points << " punkty;" << evaluations << ";" << glError << "\n";
            }
        }
    }

    evaluationsCompareFile.close();
}
