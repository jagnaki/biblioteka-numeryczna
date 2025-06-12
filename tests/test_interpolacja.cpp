#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <cmath>
#include "../include/interpolacja.h"

// Testy dla funkcji pomocniczych
TEST(InterpolacjaTest, FunkcjeTestowe) {
    EXPECT_DOUBLE_EQ(f1_interpolacja(0.0), 1.0);
    EXPECT_NEAR(f2_interpolacja(2.0), 4.0, 1e-10);
    EXPECT_NEAR(f3_interpolacja(M_PI/2), 1.0, 1e-10);
}

TEST(InterpolacjaTest, GenerowanieWezlow) {
    auto punkty = generatePoints(0.0, 1.0, 2);
    ASSERT_EQ(punkty.size(), 3); // n+1 punktów
    EXPECT_NEAR(punkty[0], 0.0, 1e-10);
    EXPECT_NEAR(punkty[1], 0.5, 1e-10);
    EXPECT_NEAR(punkty[2], 1.0, 1e-10);
}

TEST(InterpolacjaTest, InterpolacjaLagrangea) {
    std::vector<double> x = {0, 1, 2};
    std::vector<double> y = {0, 1, 4}; // y = x^2

    double wynik = interpolacja_lagrangea(x, y, 1.5);
    EXPECT_NEAR(wynik, 2.25, 1e-10); // dla x = 1.5 spodziewamy się y = 2.25
}

TEST(InterpolacjaTest, InterpolacjaNewtona) {
    std::vector<double> x = {0, 1, 2};
    std::vector<double> y = {0, 1, 4}; // y = x^2

    auto coef = dividedDifferences(x, y);
    ASSERT_EQ(coef.size(), 3);

    double wynik = newtonInterpolation(x, coef, 1.5);
    EXPECT_NEAR(wynik, 2.25, 1e-10);
}

// Test sprawdzający poprawność ilorazu różnicowego
TEST(InterpolacjaTest, IlorazyRoznicowe) {
    std::vector<double> x = {0, 1, 2};
    std::vector<double> y = {0, 1, 4}; // y = x^2

    auto coef = dividedDifferences(x, y);
    ASSERT_EQ(coef.size(), 3);

    // Dla funkcji kwadratowej, drugi iloraz różnicowy powinien być stały
    EXPECT_NEAR(coef[2], 1.0, 1e-10);
}

// Test sprawdzający przypadki brzegowe
TEST(InterpolacjaTest, PrzypadkiBrzegowe) {
    // Test dla pojedynczego punktu
    std::vector<double> x_single = {1.0};
    std::vector<double> y_single = {2.0};

    EXPECT_NEAR(interpolacja_lagrangea(x_single, y_single, 1.0), 2.0, 1e-10);

    auto coef_single = dividedDifferences(x_single, y_single);
    EXPECT_NEAR(newtonInterpolation(x_single, coef_single, 1.0), 2.0, 1e-10);

    // Test dla pustych wektorów
    std::vector<double> x_empty, y_empty;
    ASSERT_THROW(interpolacja_lagrangea(x_empty, y_empty, 0.0), std::invalid_argument);
    ASSERT_THROW(dividedDifferences(x_empty, y_empty), std::invalid_argument);
}