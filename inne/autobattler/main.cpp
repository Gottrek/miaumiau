#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

void display(int &HP, int &DMG, int &AS, int &HP2, int &DMG2, int &AS2) {
    cout << "Delikwent 1     Delikwent 2\n";
    cout << "HP:   " << HP << "           " << HP2 << endl;
    cout << "DMG:  " << DMG << "           " << DMG2 << endl;
    cout << "AS:   " << AS << "           " << AS2 << endl;
}

int main() {
    int HP = 100, DMG = 4, AS = 2;
    int HP2 = 30, DMG2 = 3, AS2 = 1;
    std::chrono::milliseconds timespan(1000); // or whatever
    for (int i = 0; i < 5; i++) {
        HP2 -= DMG;
        display(HP, DMG, AS, HP2, DMG2, AS2);
        std::this_thread::sleep_for(timespan);
    }
    return 0;
}