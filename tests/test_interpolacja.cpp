#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <cmath>
#include "../include/interpolacja.h"

TEST(InterpolacjaTest, FunkcjeTestowe) {
    EXPECT_DOUBLE_EQ(f1_interpolacja(0.0), 1.0);
    EXPECT_NEAR(f2_interpolacja(2.0), 4.0, 1e-10);
    EXPECT_NEAR(f3_interpolacja(M_PI/2), 1.0, 1e-10);
}

TEST(InterpolacjaTest, GenerowanieWezlow) {
    auto punkty = generatePoints(0.0, 1.0, 2);
    ASSERT_EQ(punkty.size(), 3);
    EXPECT_NEAR(punkty[0], 0.0, 1e-10);
    EXPECT_NEAR(punkty[1], 0.5, 1e-10);
    EXPECT_NEAR(punkty[2], 1.0, 1e-10);
}

TEST(InterpolacjaTest, InterpolacjaLagrangea) {
    std::vector<double> x = {0, 1, 2};
    std::vector<double> y = {0, 1, 4};

    double wynik = interpolacja_lagrangea(x, y, 1.5);
    EXPECT_NEAR(wynik, 2.25, 1e-10);
}

TEST(InterpolacjaTest, InterpolacjaNewtona) {
    std::vector<double> x = {0, 1, 2};
    std::vector<double> y = {0, 1, 4};

    auto coef = dividedDifferences(x, y);
    ASSERT_EQ(coef.size(), 3);

    double wynik = newtonInterpolation(x, coef, 1.5);
    EXPECT_NEAR(wynik, 2.25, 1e-10);
}

TEST(InterpolacjaTest, IlorazyRoznicowe) {
    std::vector<double> x = {0, 1, 2};
    std::vector<double> y = {0, 1, 4};

    auto coef = dividedDifferences(x, y);
    ASSERT_EQ(coef.size(), 3);

    EXPECT_NEAR(coef[2], 1.0, 1e-10);
}

TEST(InterpolacjaTest, PrzypadkiBrzegowe) {
    std::vector<double> x_single = {1.0};
    std::vector<double> y_single = {2.0};

    EXPECT_NEAR(interpolacja_lagrangea(x_single, y_single, 1.0), 2.0, 1e-10);

    auto coef_single = dividedDifferences(x_single, y_single);
    EXPECT_NEAR(newtonInterpolation(x_single, coef_single, 1.0), 2.0, 1e-10);

    std::vector<double> x_empty, y_empty;
    ASSERT_THROW(interpolacja_lagrangea(x_empty, y_empty, 0.0), std::invalid_argument);
    ASSERT_THROW(dividedDifferences(x_empty, y_empty), std::invalid_argument);
}

TEST(InterpolacjaTest, DoInterpolacjaLagrangea) {
    std::string nazwa_pliku = "test_lagrange.txt";
    std::ofstream outFile(nazwa_pliku);
    outFile << "1 \n";
    outFile << "-1 1 \n";
    outFile << "5 \n";
    outFile << "10 \n";
    outFile.close();

    ASSERT_NO_THROW(do_interpolacja_lagrangea(nazwa_pliku, 1, f1_interpolacja));

    std::ifstream inFile(nazwa_pliku);
    ASSERT_TRUE(inFile.good());

    std::string line;
    bool hasContent = false;
    if (std::getline(inFile, line)) {
        hasContent = true;
    }
    EXPECT_TRUE(hasContent);

    inFile.close();
    std::remove(nazwa_pliku.c_str());
}

TEST(InterpolacjaTest, DoInterpolacjaNewtona) {
    std::string nazwa_pliku = "test_newton.txt";
    std::ofstream outFile(nazwa_pliku);
    outFile << "1 \n";
    outFile << "-1 1 \n";
    outFile << "5 \n";
    outFile << "10 \n";
    outFile.close();

    ASSERT_NO_THROW(do_interpolacja_newtona(nazwa_pliku, 1, f1_interpolacja));

    std::ifstream inFile(nazwa_pliku);
    ASSERT_TRUE(inFile.good());

    std::string line;
    bool hasContent = false;
    if (std::getline(inFile, line)) {
        hasContent = true;
    }
    EXPECT_TRUE(hasContent);

    inFile.close();
    std::remove(nazwa_pliku.c_str());
}