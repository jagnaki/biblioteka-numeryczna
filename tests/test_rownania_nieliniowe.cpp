#include <gtest/gtest.h>
#include "../include/rownaniaNieliniowe.h"
#include <cmath>
#include <vector>
#include <fstream>

const double EPSILON = 1e-10;
const double LOOSE_EPSILON = 1e-8;

extern double f1(double x);
extern double f2(double x);
extern double f3(double x);
extern double f1_analitycznie(double x);
extern double f2_analitycznie(double x);
extern double f3_analitycznie(double x);
extern double numeryczne_obliczanie_pochodnej(double (*func)(double), double x, double h);

struct pojedyncza_iteracja {
    int iteration;
    double x;
    double fx;
    double error;
};

struct wyniki {
    double root;
    std::vector<pojedyncza_iteracja> iterations;
    std::string method;
    bool converged;
    int function_number;
    std::string function_name;
    std::string function_description;
};

extern wyniki metoda_bisekcji(double (*func)(double), double a, double b, double epsilon, int max_iter);
extern wyniki metoda_newtona(double (*func)(double), double (*derivative)(double), double x0, double epsilon, int max_iter);
extern wyniki metoda_siecznych(double (*func)(double), double x0, double x1, double epsilon, int max_iter);

TEST(FunkcjeMatematyczneTest, FunkcjaF1) {
    EXPECT_NEAR(f1(0.0), log(1) - 1.0/3.0, EPSILON);
    EXPECT_NEAR(f1(-0.5), log(0.5) - 1.0/2.5, EPSILON);
    EXPECT_NO_THROW(f1(1.0));
    EXPECT_NO_THROW(f1(10.0));
    EXPECT_NO_THROW(f1(-0.99));
    EXPECT_NO_THROW(f1(-2.99));
}

TEST(FunkcjeMatematyczneTest, FunkcjaF2) {
    EXPECT_NEAR(f2(0.0), 0 + 30*cos(0) - 1, EPSILON);
    EXPECT_NEAR(f2(1.0), 1 + 30*cos(1) - 1, EPSILON);
    EXPECT_NO_THROW(f2(-5.0));
    EXPECT_NO_THROW(f2(5.0));
    EXPECT_NO_THROW(f2(M_PI));
}

TEST(FunkcjeMatematyczneTest, FunkcjaF3) {
    const double PI = 3.14159265359;
    EXPECT_NEAR(f3(0.0), sin(0)/2 + 1.0/4, EPSILON);
    EXPECT_NEAR(f3(1.0), sin(3*PI)/3 + 1.0/5, EPSILON);
    EXPECT_NO_THROW(f3(-1.0));
    EXPECT_NO_THROW(f3(2.0));
    EXPECT_NO_THROW(f3(-1.99));
    EXPECT_NO_THROW(f3(-3.99));
}

TEST(PochodneAnalityczneTest, PochodnaF1) {
    EXPECT_NEAR(f1_analitycznie(0.0), 1.0/1.0 + 1.0/9.0, EPSILON);
    EXPECT_NEAR(f1_analitycznie(1.0), 1.0/2.0 + 1.0/16.0, EPSILON);
    EXPECT_NO_THROW(f1_analitycznie(2.0));
    EXPECT_NO_THROW(f1_analitycznie(-0.5));
}

TEST(PochodneAnalityczneTest, PochodnaF2) {
    EXPECT_NEAR(f2_analitycznie(0.0), 0 - 30*sin(0), EPSILON);
    EXPECT_NEAR(f2_analitycznie(1.0), 3 - 30*sin(1), EPSILON);
    EXPECT_NEAR(f2_analitycznie(M_PI), 3*M_PI*M_PI - 30*sin(M_PI), EPSILON);
    EXPECT_NO_THROW(f2_analitycznie(-2.0));
    EXPECT_NO_THROW(f2_analitycznie(5.0));
}

TEST(PochodneAnalityczneTest, PochodnaF3) {
    EXPECT_NO_THROW(f3_analitycznie(0.0));
    EXPECT_NO_THROW(f3_analitycznie(1.0));
    EXPECT_NO_THROW(f3_analitycznie(-1.0));
    double pochodna = f3_analitycznie(0.5);
    EXPECT_TRUE(std::isfinite(pochodna));
}

TEST(PochodnaNumeryczynaTest, PorownaniеZAnalityczna) {
    double x_test = 1.0;
    double h = 1e-5;
    double num_derivative = numeryczne_obliczanie_pochodnej(f1, x_test, h);
    double anal_derivative = f1_analitycznie(x_test);
    EXPECT_NEAR(num_derivative, anal_derivative, 1e-6);
    x_test = 0.5;
    num_derivative = numeryczne_obliczanie_pochodnej(f2, x_test, h);
    anal_derivative = f2_analitycznie(x_test);
    EXPECT_NEAR(num_derivative, anal_derivative, 1e-6);
}

TEST(PochodnaNumeryczynaTest, RozneKroki) {
    double x = 1.0;
    double h1 = 1e-6;
    double h2 = 1e-8;
    double h3 = 1e-10;
    double deriv1 = numeryczne_obliczanie_pochodnej(f1, x, h1);
    double deriv2 = numeryczne_obliczanie_pochodnej(f1, x, h2);
    double deriv3 = numeryczne_obliczanie_pochodnej(f1, x, h3);
    double analityczna = f1_analitycznie(x);
    EXPECT_NEAR(deriv1, analityczna, 1e-4);
    EXPECT_NEAR(deriv2, analityczna, 1e-6);
    EXPECT_NEAR(deriv3, analityczna, 1e-7);
}

TEST(MetodaBisekcjiTest, ZnanyPrzyklad) {
    auto f_test = [](double x) { return x*x - 4.0; };
    wyniki result = metoda_bisekcji(f_test, 0.0, 5.0, 1e-10, 1000);
    EXPECT_TRUE(result.converged);
    EXPECT_NEAR(result.root, 2.0, 1e-9);
    EXPECT_GT(result.iterations.size(), 0);
    EXPECT_EQ(result.method, "Bisekcja");
}

TEST(MetodaBisekcjiTest, BrakZmianyZnaku) {
    auto f_test = [](double x) { return x*x + 1.0; };
    wyniki result = metoda_bisekcji(f_test, 0.0, 5.0, 1e-10, 1000);
    EXPECT_FALSE(result.converged);
}

TEST(MetodaBisekcjiTest, FunkcjaF1) {
    std::vector<std::pair<double, double>> test_intervals = {{-0.5, 0.5}, {0.0, 1.0}, {0.5, 1.5}, {1.0, 2.0}};
    bool found_valid_interval = false;
    for (auto interval : test_intervals) {
        double a = interval.first;
        double b = interval.second;
        if (f1(a) * f1(b) < 0) {
            wyniki result = metoda_bisekcji(f1, a, b, 1e-10, 1000);
            EXPECT_TRUE(result.converged);
            EXPECT_NEAR(f1(result.root), 0.0, 1e-9);
            EXPECT_GT(result.iterations.size(), 0);
            found_valid_interval = true;
            break;
        }
    }
    if (!found_valid_interval) {
        std::cout << "Uwaga: Nie znaleziono przedziału ze zmianą znaku dla f1 w testowanych zakresach" << std::endl;
    }
}

TEST(MetodaNewtaTest, ZnanyPrzyklad) {
    auto f_test = [](double x) { return x*x - 4.0; };
    auto f_prime = [](double x) { return 2.0*x; };
    wyniki result = metoda_newtona(f_test, f_prime, 3.0, 1e-10, 1000);
    EXPECT_TRUE(result.converged);
    EXPECT_NEAR(result.root, 2.0, 1e-9);
    EXPECT_GT(result.iterations.size(), 0);
    EXPECT_EQ(result.method, "Newton");
}

TEST(MetodaNewtaTest, PochodnaZero) {
    auto f_test = [](double x) { return x*x*x; };
    auto f_prime = [](double x) { return 3.0*x*x; };
    wyniki result = metoda_newtona(f_test, f_prime, 0.0, 1e-10, 1000);
    EXPECT_FALSE(result.converged);
}

TEST(MetodaNewtaTest, FunkcjaF1Analityczna) {
    wyniki result = metoda_newtona(f1, f1_analitycznie, 0.5, 1e-10, 1000);
    if (result.converged) {
        EXPECT_NEAR(f1(result.root), 0.0, 1e-9);
        EXPECT_GT(result.iterations.size(), 0);
    }
    wyniki result2 = metoda_newtona(f1, f1_analitycznie, 1.5, 1e-10, 1000);
    if (result2.converged) {
        EXPECT_NEAR(f1(result2.root), 0.0, 1e-9);
        EXPECT_GT(result2.iterations.size(), 0);
    }
}

TEST(MetodaSiecznychTest, ZnanyPrzyklad) {
    auto f_test = [](double x) { return x*x - 4.0; };
    wyniki result = metoda_siecznych(f_test, 0.0, 5.0, 1e-10, 1000);
    EXPECT_TRUE(result.converged);
    EXPECT_NEAR(result.root, 2.0, 1e-9);
    EXPECT_GT(result.iterations.size(), 0);
    EXPECT_EQ(result.method, "Sieczne");
}

TEST(MetodaSiecznychTest, FunkcjaF2) {
    wyniki result = metoda_siecznych(f2, 0.0, 1.0, 1e-10, 1000);
    if (result.converged) {
        EXPECT_NEAR(f2(result.root), 0.0, 1e-9);
        EXPECT_GT(result.iterations.size(), 0);
    }
}

TEST(MetodaSiecznychTest, BrakZbieznosci) {
    auto f_test = [](double x) { return x*x + 1.0; };
    wyniki result = metoda_siecznych(f_test, 0.0, 1.0, 1e-10, 1000);
    EXPECT_FALSE(result.converged);
}
