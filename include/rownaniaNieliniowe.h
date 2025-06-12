# pragma once

using namespace std;

// Deklaracje funkcji i ich pochodnych
double f1(double x);
double f1_analitycznie(double x);
double f2(double x);
double f2_analitycznie(double x);
double f3(double x);
double f3_analitycznie(double x);

// Deklaracja funkcji do numerycznego obliczania pochodnej
double numeryczne_obliczanie_pochodnej(double (*func)(double), double x, double h);

// Deklaracja głównej funkcji modułu
void do_rownania_nieliniowe(double start, double end, double h);
