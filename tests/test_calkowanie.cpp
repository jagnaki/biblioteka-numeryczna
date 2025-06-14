#include <gtest/gtest.h>
#include "calkowanieNumeryczne.h"
#include <cmath>
#include <vector>

const double EPSILON = 1e-10;

TEST(CalkowanieNumeryczneTest, WielomianHornera) {
    std::vector<double> coefficients = {1, 2, 1};
    EXPECT_NEAR(wielomian_hornera(coefficients, 2.0), 9.0, EPSILON);
    EXPECT_NEAR(wielomian_hornera(coefficients, 0.0), 1.0, EPSILON);
    EXPECT_NEAR(wielomian_hornera(coefficients, -1.0), 0.0, EPSILON);
}

TEST(CalkowanieNumeryczneTest, FunkcjeTestowe) {
    double wynik_f1 = f1_calkowanie(0.0);
    double wynik_f2 = f2_calkowanie(M_PI/2);

    std::cout << "f1_calkowanie(0.0) = " << wynik_f1 << std::endl;
    std::cout << "f2_calkowanie(PI/2) = " << wynik_f2 << std::endl;
}

TEST(CalkowanieNumeryczneTest, MetodaProstokatow) {
    std::vector<double> coefficients = {1};
    double wynik = metoda_prostokatow(0, 2, 100, 0, coefficients);
    std::cout << "Metoda prostokątów: " << wynik << std::endl;
}

TEST(CalkowanieNumeryczneTest, MetodaTrapezow) {
    std::vector<double> coefficients = {1};
    double wynik = metoda_trapezow(0, 2, 100, 0, coefficients);
    std::cout << "Metoda trapezów: " << wynik << std::endl;
}

TEST(CalkowanieNumeryczneTest, MetodaSimpsona) {
    std::vector<double> coefficients = {1};
    double wynik = metoda_simpsona(0, 2, 100, 0, coefficients);
    std::cout << "Metoda Simpsona: " << wynik << std::endl;
}

TEST(CalkowanieNumeryczneTest, MetodaGaussLegendre) {
    std::vector<double> coefficients = {1};
    double wynik = metoda_gauss_legendre(0, 2, 5, 0, coefficients);
    std::cout << "Metoda Gauss-Legendre: " << wynik << std::endl;
}

TEST(CalkowanieNumeryczneTest, ObliczanieBledow) {
    double wartoscDokladna = 2.0;
    double wartoscPrzyblizona = 1.99;

    double blad_bezwzgledny = oblicz_blad_bezwzgledny(wartoscPrzyblizona, wartoscDokladna);
    double blad_wzgledny = oblicz_blad_wzgledny(wartoscPrzyblizona, wartoscDokladna);

    std::cout << "Błąd bezwzględny: " << blad_bezwzgledny << std::endl;
    std::cout << "Błąd względny: " << blad_wzgledny << std::endl;

    EXPECT_NEAR(blad_bezwzgledny, 0.01, EPSILON);
    EXPECT_NEAR(blad_wzgledny, 0.005, EPSILON);
}

TEST(CalkowanieNumeryczneTest, TestDiagnostyczny) {
    std::vector<double> coefficients = {};

    std::cout << "\n=== TEST DIAGNOSTYCZNY ===" << std::endl;

    for(int func_type = 0; func_type < 3; func_type++) {
        std::cout << "Funkcja typu " << func_type << ":" << std::endl;

        double wynik_prost = metoda_prostokatow(0, 1, 10, func_type, coefficients);
        double wynik_trap = metoda_trapezow(0, 1, 10, func_type, coefficients);

        std::cout << "  Prostokąty: " << wynik_prost << std::endl;
        std::cout << "  Trapezy: " << wynik_trap << std::endl;
    }
}