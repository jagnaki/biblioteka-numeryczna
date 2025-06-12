#pragma once
#include <vector>
#include <string>

using namespace std;

double wielomian_hornera(const vector<double>& coefficients, const double& x);

double f1_calkowanie(double x);

double f2_calkowanie(double x);

double f3_calkowanie(double x);

double wybor_funkcji(int mode, double x, const vector<double>& coeffs);

double metoda_prostokatow(double a, double b, int n, int mode, const vector<double>& coefficients);

double metoda_trapezow(double a, double b, int n, int mode, const vector<double>& coefficients);

double metoda_simpsona(double a, double b, int n, int mode, const vector<double>& coefficients);

double oblicz_blad_bezwzgledny(double approximateValue, double exactValue);

double oblicz_blad_wzgledny(double approximateValue, double exactValue);

void do_calkowanie_czas(const string& nazwa_pliku, int lp);

double dokladna_wartosc1();

double dokladna_wartosc();

double metoda_gauss_legendre(double a, double b, int n, int mode, const vector<double>& coeffs);

double metoda_gauss_legendre_przedzialy(double a, double b, int n, int intervals, int mode, const vector<double>& coeffs);

void do_calkowanie_kwadratury(const string& nazwa_pliku, int lp);