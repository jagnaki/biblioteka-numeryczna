#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>
#include "../include/rownaniaRozniczkowe.h"
using namespace std;

const double TOLERANCE = 1e-10;
const double LARGER_TOLERANCE = 1e-6;

TEST(RownaniaDifferentialTest, FunkcjaRownaniaDifferential) {
    double t = 0.0;
    double T = 1967.0;
    double expected = -7.0e-12 * (pow(1967.0, 4) - 0.0);
    double result = f(t, T);
    EXPECT_NEAR(result, expected, TOLERANCE);
}

TEST(RownaniaDifferentialTest, FunkcjaRownaniaDifferentialRozneWartosci) {
    double t = 100.0;
    double T = 1000.0;
    double expected = -7.0e-12 * (pow(1000.0, 4) - 0.0);
    double result = f(t, T);
    EXPECT_NEAR(result, expected, TOLERANCE);
}

TEST(RownaniaDifferentialTest, DokladneRozwiazanie) {
    double t = 0.0;
    double expected = 1967.0;
    double result = dokladne_rozwiazanie(t);
    EXPECT_NEAR(result, expected, LARGER_TOLERANCE);
}

TEST(RownaniaDifferentialTest, DokladneRozwiazanieRozneWartosci) {
    double t = 100.0;
    double result = dokladne_rozwiazanie(t);
    EXPECT_GT(result, 0.0);
    EXPECT_LT(result, 2000.0);
}

TEST(RownaniaDifferentialTest, MetodaEulera) {
    double t0 = 0.0;
    double h = 100.0;
    auto results = metoda_eulera(t0, h);
    EXPECT_FALSE(results.empty());
    EXPECT_NEAR(results[0].time, t0, TOLERANCE);
    EXPECT_NEAR(results[0].temperature, 1967.0, TOLERANCE);
    EXPECT_LT(results.back().temperature, results[0].temperature);
}

TEST(RownaniaDifferentialTest, MetodaHeuna) {
    double h = 100.0;
    int steps = 10;
    auto results = metoda_heuna(h, steps);
    EXPECT_EQ(results.size(), steps + 1);
    EXPECT_NEAR(results[0].time, 0.0, TOLERANCE);
    EXPECT_NEAR(results[0].temperature, 1967.0, TOLERANCE);
    EXPECT_LT(results.back().temperature, results[0].temperature);
}

TEST(RownaniaDifferentialTest, MetodaPunktuSrodkowego) {
    double h = 100.0;
    int steps = 10;
    auto results = metoda_punktu_srodkowego(h, steps);
    EXPECT_EQ(results.size(), steps + 1);
    EXPECT_NEAR(results[0].time, 0.0, TOLERANCE);
    EXPECT_NEAR(results[0].temperature, 1967.0, TOLERANCE);
    EXPECT_LT(results.back().temperature, results[0].temperature);
}

TEST(RownaniaDifferentialTest, MetodaRK4) {
    double h = 100.0;
    int steps = 10;
    auto results = metoda_rk_4_rzedu(h, steps);
    EXPECT_EQ(results.size(), steps + 1);
    EXPECT_NEAR(results[0].time, 0.0, TOLERANCE);
    EXPECT_NEAR(results[0].temperature, 1967.0, TOLERANCE);
    EXPECT_LT(results.back().temperature, results[0].temperature);
}

TEST(RownaniaDifferentialTest, PorownanieDokładnosci) {
    double h = 10.0;
    int steps = static_cast<int>(100.0/h);
    auto euler_results = metoda_eulera(0.0, h);
    auto heun_results = metoda_heuna(h, steps);
    auto rk4_results = metoda_rk_4_rzedu(h, steps);
    double t_final = 100.0;
    double analytical = dokladne_rozwiazanie(t_final);
    double euler_error = abs(euler_results.back().temperature - analytical);
    double heun_error = abs(heun_results.back().temperature - analytical);
    double rk4_error = abs(rk4_results.back().temperature - analytical);
    EXPECT_LT(euler_error, 2000.0);
    EXPECT_LT(heun_error, 2000.0);
    EXPECT_LT(rk4_error, 2000.0);
}

TEST(RownaniaDifferentialTest, ObliczMSE) {
    vector<data_point> test_data = {
        {0.0, 1967.0},
        {100.0, 1900.0},
        {200.0, 1800.0}
    };
    double mse = oblicz_MSE(test_data);
    EXPECT_GT(mse, 0.0);
    EXPECT_LT(mse, 1e10);
}

TEST(RownaniaDifferentialTest, ObliczMaxBlad) {
    std::vector<data_point> test_data = {
        {0.0, 1967.0},
        {100.0, 1900.0},
        {200.0, 1800.0}
    };
    double max_error = oblicz_max_blad(test_data);
    EXPECT_GT(max_error, 0.0);
    EXPECT_LT(max_error, 2000.0);
}

TEST(RownaniaDifferentialTest, StabilnoscNumeryczna) {
    double h = 1.0;
    int steps = 100;
    auto results = metoda_rk_4_rzedu(h, steps);
    for (const auto& point : results) {
        EXPECT_TRUE(std::isfinite(point.time));
        EXPECT_TRUE(std::isfinite(point.temperature));
        EXPECT_GT(point.temperature, 0.0);
    }
}

TEST(RownaniaDifferentialTest, MalyKrokCzasowy) {
    double h = 0.1;
    int steps = 10;
    auto results = metoda_heuna(h, steps);
    EXPECT_EQ(results.size(), steps + 1);
    EXPECT_NEAR(results[0].temperature, 1967.0, TOLERANCE);
    for (size_t i = 1; i < results.size(); i++) {
        EXPECT_LT(results[i].temperature, results[i-1].temperature);
    }
}

TEST(RownaniaDifferentialTest, DuzyKrokCzasowy) {
    double h = 500.0;
    int steps = 3;
    auto results = metoda_punktu_srodkowego(h, steps);
    EXPECT_EQ(results.size(), steps + 1);
    EXPECT_NEAR(results[0].temperature, 1967.0, TOLERANCE);
    EXPECT_TRUE(std::isfinite(results.back().temperature) == false || results.back().temperature > 0.0);
}

TEST(RownaniaDifferentialTest, ZapisDoCSV) {
    double h = 100.0;
    int steps = 5;
    auto heun_results = metoda_heuna(h, steps);
    auto midpoint_results = metoda_punktu_srodkowego(h, steps);
    auto rk4_results = metoda_rk_4_rzedu(h, steps);
    auto euler_results = metoda_eulera(0.0, h);
    EXPECT_NO_THROW(
        zapisz_do_csv("test_output.csv", heun_results, midpoint_results,
                     rk4_results, euler_results, h)
    );
    std::ifstream file("test_output.csv");
    EXPECT_TRUE(file.good());
    file.close();
    std::remove("test_output.csv");
}

TEST(RownaniaDifferentialTest, ZapisDoCSVBledow) {
    vector<double> step_sizes = {1.0, 0.5};
    vector<vector<double>> rmse_errors = {{0.1, 0.05}, {0.2, 0.1}, {0.05, 0.02}, {0.3, 0.15}};
    vector<vector<double>> max_errors = {{0.5, 0.25}, {0.8, 0.4}, {0.3, 0.15}, {1.0, 0.5}};
    EXPECT_NO_THROW(
        zapisz_do_csv_blad("test_errors.csv", step_sizes, rmse_errors, max_errors)
    );
    std::ifstream file("test_errors.csv");
    EXPECT_TRUE(file.good());
    file.close();
    std::remove("test_errors.csv");
}

TEST(RownaniaDifferentialTest, BardzoPojMalyCzas) {
    double t = 1e-6;
    double analytical = dokladne_rozwiazanie(t);
    EXPECT_NEAR(analytical, 1967.0, 1.0);
}

TEST(RownaniaDifferentialTest, KonsystencjaMetod) {
    double h = 1.0;
    int steps = 10;
    double t_test = 10.0;
    auto heun_results = metoda_heuna(h, steps);
    auto midpoint_results = metoda_punktu_srodkowego(h, steps);
    auto rk4_results = metoda_rk_4_rzedu(h, steps);
    double heun_final = heun_results.back().temperature;
    double midpoint_final = midpoint_results.back().temperature;
    double rk4_final = rk4_results.back().temperature;
    EXPECT_NEAR(heun_final, rk4_final, 0.1 * rk4_final);
    EXPECT_NEAR(midpoint_final, rk4_final, 0.1 * rk4_final);
}