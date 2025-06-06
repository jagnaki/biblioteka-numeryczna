#include <iostream>
#include <locale>

using namespace std;

int main() {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    return 0;
}
