#include <iostream>

using namespace std;

int ile_cyfr_w_liczbie(long unsigned int liczba, int& cyfra) {
    int wystapienia = 0;
    while (liczba >= 1) {
        if (liczba % 10 == cyfra) {
            wystapienia++;
        }
        liczba /= 10;
    }
    return wystapienia;
}

int main() {
    long unsigned int liczba = 25497505;
    int cyfra;
    cout << "Podaj cyfre: "; cin >> cyfra;
    if (cyfra < 0 || cyfra > 9) {  // Validate input (digits 0-9 only)
        cout << "Blad: Wprowadzona wartosc nie jest cyfra.\n";
        return 1;
    }
    int ile_jej = ile_cyfr_w_liczbie(liczba, cyfra);
    cout << ile_jej;
}