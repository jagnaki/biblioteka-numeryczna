#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "../include/aproksymacja.h"

// Test funkcji bazowej
TEST(AproksymacjaTest, FunkcjaBazowa) {
    // Test dla x^0 = 1
    EXPECT_DOUBLE_EQ(funkcja_bazowa(0, 2.0), 1.0);
    
    // Test dla x^1 = x
    EXPECT_DOUBLE_EQ(funkcja_bazowa(1, 2.0), 2.0);
    
    // Test dla x^2
    EXPECT_DOUBLE_EQ(funkcja_bazowa(2, 2.0), 4.0);
    
    // Test dla wartości ujemnej
    EXPECT_DOUBLE_EQ(funkcja_bazowa(2, -2.0), 4.0);
}

// Test całek pomocniczych
TEST(AproksymacjaTest, CalkiPomocnicze) {
    // Test całki z x^0 * x^0 na przedziale [0,1]
    double wynik = calka_phi_i_phi_j(0, 0, 0.0, 1.0, 100);
    EXPECT_NEAR(wynik, 1.0, 1e-8);
    
    // Test całki z x^1 * x^1 na przedziale [0,1]
    wynik = calka_phi_i_phi_j(1, 1, 0.0, 1.0, 100);
    EXPECT_NEAR(wynik, 1.0/3.0, 1e-8);
}

// Test funkcji aproksymowanej
TEST(AproksymacjaTest, Funkcja) {
    // Test dla x = 0
    EXPECT_DOUBLE_EQ(funkcja(0.0), 1.0); // e^0 * cos(0) - 0^3 = 1
    
    // Test dla innych wartości
    EXPECT_NEAR(funkcja(1.0), exp(1.0) * cos(5.0) - 1.0, 1e-10);
}

// Test rozwiązywania układu równań
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

// Test funkcji aproksymującej
TEST(AproksymacjaTest, Aproksymacja) {
    vector<double> wspolczynniki = {1.0, 2.0, 3.0}; // a0 + a1*x + a2*x^2
    double x = 1.0;
    
    double wynik = aproksymacja(wspolczynniki, x);
    EXPECT_NEAR(wynik, 6.0, 1e-10); // 1 + 2*1 + 3*1 = 6
}

// Test błędu średniokwadratowego
TEST(AproksymacjaTest, BladSrednioKwadratowy) {
    vector<double> wspolczynniki = {1.0}; // Stała funkcja = 1
    double blad = blad_srednio_kwadratowy(wspolczynniki, 0.0, 1.0, 10);
    
    EXPECT_GT(blad, 0.0);
}

// Test całej procedury aproksymacji
TEST(AproksymacjaTest, DoAproksymacja) {
    ASSERT_NO_THROW(do_aproksymacja());
}