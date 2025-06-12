#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <locale>
#include "../include/interpolacja.h"

#include "../include/funkcje.h"
using namespace std;

//funkcja liczaca wartosc wielomianu interpolacyjnego
double interpolacja_lagrangea(vector<double> x, vector<double> y, double n) {
    double result = 0;
    cout<<endl;
    for (int i = 0; i < x.size(); i++) {
        double fxi = y[i];
        for (int j = 0; j < x.size(); j++) {
            if (j != i) {
                fxi = fxi * (n - x[j]) / (x[i] - x[j]);
            }
        }
        result += fxi;
    }
    return result;
}

struct lagrange {
    vector<double> xi;
    vector<double> fxi;
};

//generowanie rownomiernie rozlozonych punktow
vector<double> generatePoints(double a, double b, int n) {
    vector<double> points;
    double h = (b - a) / n;
    for (int i = 0; i <= n; i++) {
        points.push_back(a + i * h);
    }
    return points;
}

lagrange wczytaj_dane_lagrange(const string& nazwa_pliku, int lp) {
    lagrange data;
    string sciezka = "../data/" + nazwa_pliku;
    ifstream wczytaj(sciezka);
    if (!wczytaj.is_open())
    {
        cout << "Blad otwarcia pliku." << endl;
    }
    string szukany_wiersz = "l.p.: "+ to_string(lp);
    string linia;
    bool znaleziono = false;
    while (getline(wczytaj, linia))
    {
        if (linia.find(szukany_wiersz) != string::npos)
        {
            znaleziono = true;
            break;
        }
    }
    if (!znaleziono)
    {
        cout << "Nie znaleziono linii z l.p.: "+ to_string(lp) << endl;
    }
    getline(wczytaj, linia);
    stringstream ss_xi(linia.substr(linia.find(":") + 1));
    double tmp;
    while (ss_xi >> tmp)
        data.xi.push_back(tmp);
    getline(wczytaj, linia);
    stringstream ss_fxi(linia.substr(linia.find(":") + 1));
    double wartosc;
    while (ss_fxi >> wartosc)
    {
        data.fxi.push_back(wartosc);
    }
    wczytaj.close();
    return data;
}

void do_interpolacja_lagrangea(const string& nazwa_pliku, int lp, funkcja_t f) {
#ifdef _WIN32
    //Ustawienie strony kodowej konsoli na UTF-8 (65001)
    system("chcp 65001 > nul");
#endif
    lagrange dane = wczytaj_dane_lagrange(nazwa_pliku, lp);
    if (dane.xi.empty() || dane.fxi.empty()) {
        cout << "Brak danych do interpolacji." << endl;
        return;
    }

    //punkty wezlowe
    vector<double> xi_wezly, fxi_wezly;
    for (size_t i = 0; i < dane.xi.size(); i += 5) {
        xi_wezly.push_back(dane.xi[i]);
        fxi_wezly.push_back(dane.fxi[i]);
    }
    cout << endl;

    //zapisanie danych do pliku
    ofstream wyniki("wyniki.csv");
    wyniki<<"xi;f_interpolated\n";
    double punkt=0;
    for (int i=0; i<dane.xi.size(); i++) {
        punkt = dane.xi[i];
        double wynikI = interpolacja_lagrangea(dane.xi, dane.fxi, punkt);
        wyniki<<dane.xi[i]<<";"<<wynikI<<endl;
        //cout<<"xi = "<<xi[i]<<"\twynik = "<< wynikI<<"\n";
    }
    wyniki.close();

    //obliczenie bledu
    double blad = 0;
    int licznik=0;
    for (int i=0; i<dane.xi.size(); i++) {
        double wynikI = interpolacja_lagrangea(xi_wezly, fxi_wezly, dane.xi[i]);
        double error = dane.fxi[i] - wynikI;
        cout << "xi: " << dane.xi[i] << ", f(xi): " << dane.fxi[i]
             << ", Interpolowane: " << wynikI
             << ", Blad: " << error*error << endl;
        blad += error*error;
        licznik++;
    }
    blad /= licznik;
    cout<<"Sredni blad kwadratowy: "<<blad<<endl;

    //oblizenie bledu dla roznej ilosci punktow wezlowych
    cout<<endl<<endl;
    vector<double> xi_wezly2, fxi_wezly2;
    for (size_t i = 0; i < dane.xi.size(); i += 2) {
        xi_wezly2.push_back(dane.xi[i]);
        fxi_wezly2.push_back(dane.fxi[i]);
    }
    blad = 0;
    licznik=0;
    for (int i=0; i<dane.xi.size(); i++) {
        double wynikI = interpolacja_lagrangea(xi_wezly2, fxi_wezly2, dane.xi[i]);
        double error = dane.fxi[i] - wynikI;
        cout << "xi: " << dane.xi[i] << ", f(xi): " << dane.fxi[i]
             << ", Interpolowane: " << wynikI
             << ", Blad: " << error*error << endl;
        blad += error*error;
        licznik++;
    }
    blad /= licznik;
    cout<<"Sredni blad kwadratowy dla co 2 punktu: "<<blad<<endl;
    //zapisanie interpolowanych wartosci do pliku
    ofstream wynikiZBledem("wyniki_z_bledem.csv");
    wynikiZBledem<<"xi;f_interpolated\n";
    for (int i=0; i<dane.xi.size(); i++) {
        double wynikI = interpolacja_lagrangea(xi_wezly, fxi_wezly, dane.xi[i]);
        wynikiZBledem<<dane.xi[i]<<";"<<wynikI<<endl;
        //cout<<"xi = "<<xi[i]<<"\twynik = "<< wynikI<<"\n";
    }
    wynikiZBledem.close();
    //pobranie punktu od uzytkownika
    punkt=0;
    cout<<"Podaj punkt: ";
    cin>>punkt;
    cout<<"Wartosc wielomianu interpolacyjnego w podanym punkcie: "<<interpolacja_lagrangea(xi_wezly, fxi_wezly, punkt)<<endl;

    //zadanie dodatkowe
    vector<int> n = {5, 10, 15, 20, 25}; //ilosc wezlow interpolacji
    double a=-5, b=5; //przedzial interpolacji
    int test = 100;
    for (int i=0; i<n.size(); i++) {
        vector<double> x = generatePoints(a, b, n[i]);
        vector<double> y;
        for (double xi : x) {
            y.push_back(f(xi));
        }
        ofstream wynikiZadDod("wyniki_zad_dod_"+to_string(n[i])+".csv");
        wynikiZadDod<<"x;f_interpolated\n";
        double blad = 0;
        for (int j=0; j<=test; j++) {
            double punkt= a+j*(b-a)/test;
            double wartosc = f(punkt);
            double wynikI = interpolacja_lagrangea(x, y, punkt);
            blad +=pow(wartosc-wynikI, 2);
            wynikiZadDod<<punkt<<";"<<wynikI<<endl;
        }
        blad/=test;
        cout<<"Sredni blad kwadratowy dla "<<n[i]<<" wezlow: "<<blad<<endl;
        wynikiZadDod.close();
    }
}

//interpolacja 2

