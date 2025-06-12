#pragma once
#include <vector>

using namespace std;

void weryfikacja(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& x, const int& n);
void wypiszMacierzR(const vector<vector<double>>& A, const vector<double>& b, const int& n);
void dekompozycja_LU(vector<vector<double>>& A, vector<vector<double>>& L, vector<vector<double>>& U, vector<int>& P, const int& N);
vector<double> podstawienie_w_przod(const vector<vector<double>>& L, const vector<double>& b, const vector<int>& P, const int& N);
vector<double> podstawienie_wstecz(const vector<vector<double>>& U, const vector<double>& z, const int& N);
void sprawdz_rozwiazanie(const vector<vector<double>>& A, const vector<double>& x, const vector<double>& b, const int& n);
void do_LU(const string& filename, int lp);
void do_gauss(const string& filename, int lp);