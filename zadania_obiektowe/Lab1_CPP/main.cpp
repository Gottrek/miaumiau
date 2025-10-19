#include <iostream>
#include <string>

using namespace std;

class Car {
    private:
    string make;
    string model;
    int year;
    public:
    Car(string mak, string mod, int yea) {
        make = mak;
        model = mod;
        year = yea;
    }
    void displayInfo() {
        cout << "Marka: " << make << endl;
        cout << "Model: " << model << endl;
        cout << "Rok: " << year << endl;
    }
    int getCarAge(int curr) {
        return curr - year;
    }
    bool isSameCar(Car otherCar) {
        if (make == otherCar.make and model == otherCar.model and year == otherCar.year) {
            return true;
        }
        return false;
    }
};

int main() {
    int currentYear = 2025;
    Car panda = Car("fiat", "panda", 1991);
    Car opel = Car("opel", "astra", 2005);
    Car panda2 = Car("fiat", "panda", 1991);
    panda.displayInfo();
    cout << panda.getCarAge(currentYear);
    cout << panda.isSameCar(opel);
    return 0;
}