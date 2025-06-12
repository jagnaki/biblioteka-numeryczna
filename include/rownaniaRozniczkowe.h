#pragma once
#include <vector>
#include <string>

struct data_point {
    double time;
    double temperature;
};
double f(double t, double T);
double dokladne_rozwiazanie(double t);
std::vector<data_point> metoda_heuna(double h, int steps);
std::vector<data_point> metoda_punktu_srodkowego(double h, int steps);
std::vector<data_point> metoda_rk_4_rzedu(double h, int steps);
std::vector<data_point> metoda_eulera(double t0, double h);
double oblicz_MSE(const std::vector<data_point>& numerical);
double oblicz_max_blad(const std::vector<data_point>& numerical);
void zapisz_do_csv(const std::string& filename,
                   const std::vector<data_point>& heun,
                   const std::vector<data_point>& midpoint,
                   const std::vector<data_point>& rk4,
                   const std::vector<data_point>& euler,
                   double h);
void zapisz_do_csv_blad(const std::string& filename,
                        const std::vector<double>& step_sizes,
                        const std::vector<std::vector<double>>& rmse_errors,
                        const std::vector<std::vector<double>>& max_errors);
void do_rownania_rozniczkowe();