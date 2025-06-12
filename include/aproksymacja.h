# pragma once
#include <vector>
using namespace std;

void do_aproksymacja();
extern int i_global, j_global;
double funkcja(double x);
double funkcja_bazowa(int stopien, double x);
template<typename F>
double calka_simpsona_template(F func, double a, double b, int n);
double calka_phi_i_phi_j(int i, int j, double a, double b, int n);
double calka_f_phi_i(int i, double a, double b, int n);
vector<double> rozwiaz_uklad_rownan(const std::vector<std::vector<double>>& A, const std::vector<double>& b);
double aproksymacja(const std::vector<double>& a, double x);
double blad_srednio_kwadratowy(const std::vector<double>& a, double a_przedzial, double b_przedzial, int n_punktow);