#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
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

//funkcja f(x)=1/(1+x^2)
double f(double x) {
    return 1/(1+x*x);
}

//generowanie rownomiernie rozlozonych punktow
vector<double> generowanie_punktow(double a, double b, int n) {
    vector<double> points;
    double h = (b - a) / n;
    for (int i = 0; i <= n; i++) {
        points.push_back(a + i * h);
    }
    return points;
}
