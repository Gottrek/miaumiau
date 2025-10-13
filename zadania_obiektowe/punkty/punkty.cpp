#include <string>
#include <iostream>
#include <memory>
using namespace std;
class Punkt
{
private:
int x, y;
protected:
string odl_srodek;
public:
 Punkt(int, int);
 Punkt(const Punkt &p) {
 cout << "konstruktor kopiujacy " << endl;
 }
 Punkt(Punkt&& p) {
 cout << "konstruktor przenoszacy " << endl;
 return;
 }
 ~Punkt();
void Oszacuj_odl_srodek(string osr) {
 odl_srodek = osr;
 }
void Rysuj(void);
Punkt operator +(const Punkt& p) {
 Punkt temp = Punkt(x + p.x, y + p.y);
 return temp;
 }
Punkt& operator = (const Punkt& p) {
 cout << "kopiujący operator przypisania " << endl;
 return *this;
 }
Punkt& operator = (Punkt&& p) {
 cout << "przenoszący operator przypisania " << endl;
 return *this;
 }
};
Punkt::Punkt(int wsp_x, int wsp_y) {
 cout << "konstruktor klasyczny" << endl;
 x = wsp_x;
 y = wsp_y;
}

//definicja metody destruktora
Punkt::~Punkt() {
 cout << "destruktor" << endl;
}
//definicja metody Rysuj
void Punkt::Rysuj(void) {
//...
}
int main() {
Punkt pkt1(-8, 2);
Punkt pkt2 = Punkt(-4, -1);
Punkt* pkt3 = &pkt1;
Punkt& pkt4 = pkt2;
Punkt pkt5 = { 2, 0 };
Punkt* pkt6 = new Punkt(5, 8);
unique_ptr<Punkt> pkt7 = make_unique<Punkt>(-1, 3);
Punkt pkt8(*pkt6);
Punkt pkt9 = pkt8;
Punkt pkt10 = *pkt6;
Punkt p11(Punkt(1, 1) + Punkt(2, 2));
 pkt1 = Punkt(1, 1);
 pkt1 = pkt10;
return 0; // brak delete pkt6;
} 