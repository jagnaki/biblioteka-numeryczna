#include <iostream>
#include <cmath>
#include "../include/funkcje.h"

using namespace std;

//funkcja f(x)=1/(1+x^2)
double f(double x) {
    return 1/(1+x*x);
}

//funkcja f(x)=x^2
double f2(double x) {
    return x*x;
}

//funkcja f(x)=sin(x)
double f3(double x) {
    return sin(x);
}

funkcja_t pobierz_funkcje() {
    int wybor;
    cout << "Dostepne funkcje:\n";
    cout << "1. f(x) = 1/(1+x^2)\n";
    cout << "2. f(x) = x^2\n";
    cout << "3. f(x) = sin(x)\n";
    cout << "Wybierz numer funkcji: ";
    cin >> wybor;

    switch(wybor) {
        case 1: return f;
        case 2: return f2;
        case 3: return f3;
        default:
            cout << "Nieprawidlowy wybor. Uzywam domyslnej funkcji f(x) = 1/(1+x^2)\n";
            return f;
    }
}