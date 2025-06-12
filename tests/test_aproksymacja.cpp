#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "../include/aproksymacja.h"

TEST(AproksymacjaTest, FunkcjaBazowa) {
    EXPECT_DOUBLE_EQ(funkcja_bazowa(0, 2.0), 1.0);
    EXPECT_DOUBLE_EQ(funkcja_bazowa(1, 2.0), 2.0);
    EXPECT_DOUBLE_EQ(funkcja_bazowa(2, 2.0), 4.0);
    EXPECT_DOUBLE_EQ(funkcja_bazowa(2, -2.0), 4.0);
}

TEST(AproksymacjaTest, CalkiPomocnicze) {
    double wynik = calka_phi_i_phi_j(0, 0, 0.0, 1.0, 100);
    EXPECT_NEAR(wynik, 1.0, 1e-8);
    wynik = calka_phi_i_phi_j(1, 1, 0.0, 1.0, 100);
    EXPECT_NEAR(wynik, 1.0/3.0, 1e-8);
}

TEST(AproksymacjaTest, Funkcja) {
    EXPECT_DOUBLE_EQ(funkcja(0.0), 1.0);
    EXPECT_NEAR(funkcja(1.0), exp(1.0) * cos(5.0) - 1.0, 1e-10);
}

TEST(AproksymacjaTest, RozwiazUkladRownan) {
    vector<vector<double>> A = {
        {1.0, 0.0},
        {0.0, 1.0}
    };
    vector<double> b = {2.0, 3.0};
    vector<double> x = rozwiaz_uklad_rownan(A, b);
    ASSERT_EQ(x.size(), 2);
    EXPECT_NEAR(x[0], 2.0, 1e-8);
    EXPECT_NEAR(x[1], 3.0, 1e-8);
}

TEST(AproksymacjaTest, Aproksymacja) {
    vector<double> wspolczynniki = {1.0, 2.0, 3.0};
    double x = 1.0;
    double wynik = aproksymacja(wspolczynniki, x);
    EXPECT_NEAR(wynik, 6.0, 1e-10);
}

TEST(AproksymacjaTest, BladSrednioKwadratowy) {
    vector<double> wspolczynniki = {1.0};
    double blad = blad_srednio_kwadratowy(wspolczynniki, 0.0, 1.0, 10);
    EXPECT_GT(blad, 0.0);
}

TEST(AproksymacjaTest, DoAproksymacja) {
    ASSERT_NO_THROW(do_aproksymacja());
}