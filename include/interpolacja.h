#pragma once
#include <vector>

using namespace std;

typedef double (*funkcja_t)(double);
funkcja_t pobierz_funkcje();
vector<double> generowanie_punktow(double a, double b, int n);
void do_interpolacja_lagrangea(const string& nazwa_pliku, int lp, funkcja_t f);
void do_interpolacja_newtona(const string& nazwa_pliku, int lp, funkcja_t f);