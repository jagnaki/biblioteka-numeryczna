#pragma once

double f1(double x);
typedef double (*funkcja_t)(double); // definicja typu dla wskaźnika na funkcję
funkcja_t pobierz_funkcje(); // deklaracja nowej funkcji