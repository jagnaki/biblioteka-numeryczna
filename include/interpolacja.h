#pragma once
#include <vector>

#include "funkcje.h"
using namespace std;

double interpolacja_lagrangea(vector<double> x, vector<double> y, double n);
vector<double> generowanie_punktow(double a, double b, int n);
void interpolacja_newtona();
void do_interpolacja_lagrangea(const string& nazwa_pliku, int lp, funkcja_t f);