#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>
#include "../include/ukladyRownanLiniowych.h"

TEST(UkladyRownanTest, EliminacjaGaussaProsty) {
    vector<vector<double>> A = {{2.0, 1.0}, {1.0, 1.0}};
    vector<double> b = {3.0, 2.0};
    int n = 2;
    vector<double> x = eliminacja_gaussa(A, b, n);
    ASSERT_FALSE(x.empty());
    EXPECT_NEAR(x[0], 1.0, 1e-10);
    EXPECT_NEAR(x[1], 1.0, 1e-10);
}

TEST(UkladyRownanTest, DekompozycjaLUProsty) {
    vector<vector<double>> A = {{4.0, 3.0}, {6.0, 3.0}};
    vector<vector<double>> L, U;
    vector<int> P;
    int N = 2;
    dekompozycja_LU(A, L, U, P, N);
    EXPECT_EQ(L.size(), 2);
    EXPECT_EQ(U.size(), 2);
    EXPECT_EQ(P.size(), 2);
    EXPECT_DOUBLE_EQ(L[0][0], 1.0);
    EXPECT_DOUBLE_EQ(L[1][1], 1.0);
}

TEST(UkladyRownanTest, PodstawienieWPrzod) {
    vector<vector<double>> L = {{1.0, 0.0}, {2.0, 1.0}};
    vector<double> b = {3.0, 8.0};
    vector<int> P = {0, 1};
    int N = 2;
    vector<double> z = podstawienie_w_przod(L, b, P, N);
    ASSERT_EQ(z.size(), 2);
    EXPECT_NEAR(z[0], 3.0, 1e-10);
    EXPECT_NEAR(z[1], 2.0, 1e-10);
}

TEST(UkladyRownanTest, PodstawienieWstecz) {
    vector<vector<double>> U = {{2.0, 1.0}, {0.0, 1.0}};
    vector<double> z = {3.0, 2.0};
    int N = 2;
    vector<double> x = podstawienie_wstecz(U, z, N);
    ASSERT_EQ(x.size(), 2);
    EXPECT_NEAR(x[0], 0.5, 1e-10);
    EXPECT_NEAR(x[1], 2.0, 1e-10);
}

TEST(UkladyRownanTest, KompletneRozwiazanieLU) {
    vector<vector<double>> A_orig = {{4.0, 3.0}, {6.0, 3.0}};
    vector<double> b = {7.0, 9.0};
    int N = 2;
    vector<vector<double>> A = A_orig;
    vector<vector<double>> L, U;
    vector<int> P;
    dekompozycja_LU(A, L, U, P, N);
    vector<double> z = podstawienie_w_przod(L, b, P, N);
    vector<double> x = podstawienie_wstecz(U, z, N);
    ASSERT_EQ(x.size(), 2);
    ASSERT_FALSE(isnan(x[0]));
    ASSERT_FALSE(isnan(x[1]));
    double result1 = A_orig[0][0] * x[0] + A_orig[0][1] * x[1];
    double result2 = A_orig[1][0] * x[0] + A_orig[1][1] * x[1];
    EXPECT_NEAR(result1, b[0], 1e-10);
    EXPECT_NEAR(result2, b[1], 1e-10);
}

TEST(UkladyRownanTest, UkladSprzeczny) {
    vector<vector<double>> A = {{1.0, 2.0}, {2.0, 4.0}};
    vector<double> b = {3.0, 7.0};
    int n = 2;
    vector<double> x = eliminacja_gaussa(A, b, n);
    EXPECT_TRUE(isnan(x[0]));
}

TEST(UkladyRownanTest, MacierzJednostkowa) {
    vector<vector<double>> A = {{1.0, 0.0}, {0.0, 1.0}};
    vector<double> b = {5.0, 3.0};
    int n = 2;
    vector<double> x = eliminacja_gaussa(A, b, n);
    ASSERT_FALSE(x.empty());
    EXPECT_NEAR(x[0], 5.0, 1e-10);
    EXPECT_NEAR(x[1], 3.0, 1e-10);
}

TEST(UkladyRownanTest, TestPivotingu) {
    vector<vector<double>> A = {{0.0, 1.0}, {1.0, 1.0}};
    vector<double> b = {2.0, 3.0};
    int n = 2;
    vector<double> x = eliminacja_gaussa(A, b, n);
    ASSERT_FALSE(x.empty());
    EXPECT_NEAR(x[0], 1.0, 1e-10);
    EXPECT_NEAR(x[1], 2.0, 1e-10);
}