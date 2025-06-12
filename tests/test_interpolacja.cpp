#include <gtest/gtest.h>
#include "../src/interpolacja.cpp"  // Upewnij się, że ścieżka jest poprawna

// Test funkcji f1(x) = 1 / (1 + x^2)
TEST(FunkcjaTest, F1Test) {
    EXPECT_DOUBLE_EQ(f1_interpolacja(0), 1.0);
    EXPECT_NEAR(f1_interpolacja(1), 0.5, 1e-6);
}

// Test funkcji f2(x) = x^2
TEST(FunkcjaTest, F2Test) {
    EXPECT_DOUBLE_EQ(f2_interpolacja(2), 4.0);
    EXPECT_DOUBLE_EQ(f2_interpolacja(-3), 9.0);
}

// Test funkcji f3(x) = sin(x)
TEST(FunkcjaTest, F3Test) {
    EXPECT_NEAR(f3_interpolacja(0), 0.0, 1e-6);
    EXPECT_NEAR(f3_interpolacja(M_PI/2), 1.0, 1e-6);
}

// Test generowania punktów
TEST(GeneratePointsTest, GenerateCorrectly) {
    std::vector<double> expected = {0, 0.5, 1.0};
    std::vector<double> result = generatePoints(0, 1, 2);
    ASSERT_EQ(result.size(), expected.size());
    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_NEAR(result[i], expected[i], 1e-6);
    }
}

// Test interpolacji Lagrange’a dla funkcji f(x) = x^2
TEST(InterpolacjaTest, LagrangeKwadratowa) {
    std::vector<double> x = {0, 1, 2};
    std::vector<double> y = {0, 1, 4}; // y = x^2

    double result = interpolacja_lagrangea(x, y, 1.5);
    EXPECT_NEAR(result, 2.25, 1e-6);  // 1.5^2 = 2.25
}

// Test współczynników Newtona (dividedDifferences)
TEST(InterpolacjaTest, NewtonCoefficients) {
    std::vector<double> x = {1, 2, 3};
    std::vector<double> y = {1, 4, 9}; // f(x) = x^2
    std::vector<double> coef = dividedDifferences(x, y);

    EXPECT_NEAR(coef[0], 1.0, 1e-6);
    EXPECT_NEAR(coef[1], 3.0, 1e-6);
    EXPECT_NEAR(coef[2], 1.0, 1e-6);
}

// Test interpolacji Newtona dla f(x) = x^2
TEST(InterpolacjaTest, NewtonKwadratowa) {
    std::vector<double> x = {0, 1, 2};
    std::vector<double> y = {0, 1, 4};
    std::vector<double> coef = dividedDifferences(x, y);

    double result = newtonInterpolation(x, coef, 1.5);
    EXPECT_NEAR(result, 2.25, 1e-6);
}

// Test błędu średniokwadratowego (MSE)
TEST(InterpolacjaTest, MSETest) {
    std::vector<double> x = {0, 1, 2};
    std::vector<double> y = {0, 1, 4};
    std::vector<double> coef = dividedDifferences(x, y);

    double error = MSE(x, y, x, coef);
    EXPECT_NEAR(error, 0.0, 1e-6);  // Idealna interpolacja
}
