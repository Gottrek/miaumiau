#include <iostream>
#include <memory>
#include <string>

using namespace std;

struct Stos
{
int elem;
shared_ptr<Stos> nast;
};
class Obsluga_stosu
{
private:
shared_ptr<Stos> wi_stos;
shared_ptr<Stos> wi_pom_el;
public:
 Obsluga_stosu(shared_ptr<Stos> st) : wi_stos(st)
 {}
shared_ptr<Stos> Podaj_stos()
 {
 return wi_stos;
 }
void dodaj(int elem) {
 Stos s = { elem, wi_stos };
 wi_pom_el = make_shared<Stos>(s);
 wi_stos = wi_pom_el;
 }
void usun() {
 if (wi_stos != nullptr)
 wi_stos = wi_stos.get()->nast;
 }
};
int Fun_wyj1(int z1)
//funkcja realizuje obliczenia numeryczne
{
if (z1 < 7) return -1;
else
 if (z1 == 7) { throw 0; }
 else
 if (z1 > 9000) { throw z1; }
 else return (300 * (z1 - 2) * 300 * (z1 - 2)) % (z1 - 7);
};

void Fun_wyj2(shared_ptr<Stos>& wi_podst_s, int licz, shared_ptr<Stos> wi_pom_s)
//funkcja zdejmuje licz>0 elementow ze stosu wi_podst_s,
//po czym umieszcza na nim wszystkie elementy ze stosu wi_pom_s
{
shared_ptr<Stos> wi_st;
Obsluga_stosu *wsk_Obs_stosu;
shared_ptr<Stos> wi_pom_el;
 wsk_Obs_stosu = new Obsluga_stosu(wi_podst_s);
try
 {
 if (wsk_Obs_stosu->Podaj_stos() == nullptr)
 {
 throw wsk_Obs_stosu;
 }
 else
 {
 for (int i = 1; i < licz; i++)
 {
 wi_st = wsk_Obs_stosu->Podaj_stos();
 if (wi_st.get()->nast == nullptr)
 {
 throw wi_st.get()->elem;
 }
 else { wsk_Obs_stosu->usun(); }
 //usuniecie kolejnego, i-tego elementu z wewn.
 //stosu obiektu wsk_Obs_stosu
 }
 wsk_Obs_stosu->usun();
 //usuniecie licz-tego elementu z wewn. stosu
 //obiektu wsk_Obs_stosu,
 while (wi_pom_s != nullptr) {
 wsk_Obs_stosu->dodaj(wi_pom_s.get()->elem);
 wi_pom_s = wi_pom_s.get()->nast;
 }
 //po czym wprowadzenie tam elementow z wi_pom_s
 }
 }
catch (int)
 {
 cout << "nie da sie usunac nakazanej liczby elementow ";
 cout << "z podanego stosu podstawowego" << endl;

 //kontynuacja dzialan – obliczenie alternatywne
 }
wi_podst_s = wsk_Obs_stosu->Podaj_stos();
}

int przyk_Fun(int par)
{ //poczatkowa czesc definicji funkcji przyk_Fun
//...
try
 {
 switch (Fun_wyj1(par))
 {
 case -1:
 cout << "ujemny mianownik " << endl;
 break;
 case 0:
 cout << "podzielnosc " << endl;
 break;
 default:
 cout << "brak podzielnosci " << endl;
 break;
 }
 //...
 return 0;
 }
catch (int arg)
 {
 if (!arg) cout << "dzielenie przez zero!!!" << endl;
 else cout << "przekroczenie zakresu!!! " << endl;
 }
catch (...)
 {
 cout << "blad!!!" << endl;
 }
//koncowa czesc definicji funkcji przyk_Fun
return 1;
} //koniec definicji funkcji przyk_Fun

int main()
{ //dotyczy Fun_wyj1
 cout << przyk_Fun(0) << endl;
 cout << przyk_Fun(7) << endl;
 cout << przyk_Fun(1000) << endl;
 cout << przyk_Fun(10000) << endl;
//dotyczy Fun_wyj2
//utworzenie stosu wi_podst_s
shared_ptr<Stos> wi_podst_s = nullptr;
shared_ptr<Stos> wi_pom_el;
for (int i = 1; i <= 20; i++)
 {
 Stos s = { i, wi_podst_s };
 wi_pom_el = make_shared<Stos>(s);
 wi_podst_s = wi_pom_el;
 }
//utworzenie stosu wi_pom_s
string tab;
 cout << "Wprowadz ciag znakow o dlugosci nie wiekszej niz 20;"
 << endl
 << "ciag ma zawierac przynajmniej jedna litere 'a',"
 << endl
 << " a na pozycji 0 ma byc znak rozny od 'a' "
 << endl;
 cin >> tab; //np. "Konstantynopolitan"
shared_ptr<Stos> wi_pom_s = nullptr;
char c;
int i = 0;
 c = tab.at(i);
while (c != 'a')
 {
 Stos s = { int(c), wi_pom_s };
 wi_pom_el = make_shared<Stos>(s);
 wi_pom_s = wi_pom_el;
 i++;
 c = tab.at(i);
 }
try
 {
 Fun_wyj2(wi_podst_s, i, wi_pom_s);
 }
catch (Obsluga_stosu *wsk_obiektu)
 {
 cout << "stos globalny jest pusty" << endl
 << "nie da sie wykonac nakazanych operacji" << endl;

 //przerwanie dzialan
 return 1;
 }
while (wi_podst_s != nullptr) {
 cout << wi_podst_s.get()->elem << endl;
 wi_podst_s = wi_podst_s.get()->nast;
 }
return 0;
}//koniec definicji funkcji main