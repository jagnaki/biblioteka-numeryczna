#pragma once
#include <vector>
using namespace std;

double interpolacja_lagrangea(vector<double> x, vector<double> y, double n);
vector<double> generowanie_punktow(double a, double b, int n);
void interpolacja_newtona();