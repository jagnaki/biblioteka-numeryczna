#pragma once
#include <vector>

using namespace std;

#include <vector>
#include <string>

using namespace std;

typedef double (*funkcja_t)(double);

double f1_interpolacja(double x);
double f2_interpolacja(double x);
double f3_interpolacja(double x);

funkcja_t pobierz_funkcje();

double interpolacja_lagrangea(vector<double> x, vector<double> y, double n);

vector<double> generatePoints(double a, double b, int n);
void do_interpolacja_lagrangea(const string& nazwa_pliku, int lp, funkcja_t f);

vector<double> dividedDifferences(const vector<double>& xi, const vector<double>& fxi);
double newtonInterpolation(const vector<double>& xi, const vector<double>& coef, double x);
double MSE(const vector<double>& xi, const vector<double>& fxi, const vector<double>& xi_wezly, const vector<double>& coef);
void do_interpolacja_newtona(const string& nazwa_pliku, int lp, funkcja_t f);