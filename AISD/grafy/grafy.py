"""
Implementacja algorytmów sortowania topologicznego:
1. DFS na macierzy sąsiedztwa
2. DFS na macierzy grafu
3. Algorytm Kahna (usuwanie wierzchołków o zerowym stopniu wejściowym) na macierzy sąsiedztwa
4. Algorytm Kahna na macierzy grafu

Każdy algorytm wykrywa cykle w grafie wejściowym.
"""

from collections import deque


def wczytaj_graf_z_pliku(nazwa_pliku):
    """
    Wczytuje graf z pliku tekstowego.
    Format pliku:
    - Pierwsza linia: liczba wierzchołków i liczba krawędzi oddzielone spacją
    - Kolejne linie: pary wierzchołków połączonych łukiem oddzielone spacją
    
    Zwraca:
    - liczbę wierzchołków
    - macierz sąsiedztwa
    - macierz grafu
    """
    try:
        with open(nazwa_pliku, 'r') as plik:
            # Pierwsza linia zawiera liczbę wierzchołków i krawędzi
            pierwsza_linia = plik.readline().strip().split()
            liczba_wierzcholkow = int(pierwsza_linia[0])
            liczba_krawedzi = int(pierwsza_linia[1])
            
            # Inicjalizacja macierzy sąsiedztwa
            macierz_sasiedztwa = [[0 for _ in range(liczba_wierzcholkow)] for _ in range(liczba_wierzcholkow)]
            
            # Inicjalizacja macierzy grafu
            # Rozmiar macierzy grafu: |V| x (|V| + 4)
            macierz_grafu = [[-liczba_wierzcholkow - 1 for _ in range(liczba_wierzcholkow + 4)] for _ in range(liczba_wierzcholkow)]
            
            # Wczytywanie krawędzi
            for _ in range(liczba_krawedzi):
                linia = plik.readline().strip().split()
                v_i = int(linia[0]) - 1  # Odejmujemy 1, aby indeksy zaczynały się od 0
                v_j = int(linia[1]) - 1
                
                # Ustawienie krawędzi w macierzy sąsiedztwa
                macierz_sasiedztwa[v_i][v_j] = 1
            
            # Tworzenie list następników, poprzedników i wierzchołków nieincydentnych
            listy_nastepnikow = [[] for _ in range(liczba_wierzcholkow)]
            listy_poprzednikow = [[] for _ in range(liczba_wierzcholkow)]
            listy_nieincydentnych = [[] for _ in range(liczba_wierzcholkow)]
            
            for i in range(liczba_wierzcholkow):
                for j in range(liczba_wierzcholkow):
                    if macierz_sasiedztwa[i][j] == 1:
                        listy_nastepnikow[i].append(j)
                    if macierz_sasiedztwa[j][i] == 1:
                        listy_poprzednikow[i].append(j)
                    if i != j and macierz_sasiedztwa[i][j] == 0 and macierz_sasiedztwa[j][i] == 0:
                        listy_nieincydentnych[i].append(j)
            
            # Wypełnianie dodatkowych kolumn (|V|+1, |V|+2, |V|+3, |V|+4) w macierzy grafu
            for i in range(liczba_wierzcholkow):
                # Pierwszy następnik (kolumna |V|+1)
                if listy_nastepnikow[i]:
                    macierz_grafu[i][liczba_wierzcholkow] = listy_nastepnikow[i][0] + 1  # +1, bo indeksy od 1
                else:
                    macierz_grafu[i][liczba_wierzcholkow] = 0
                
                # Pierwszy poprzednik (kolumna |V|+2)
                if listy_poprzednikow[i]:
                    macierz_grafu[i][liczba_wierzcholkow + 1] = listy_poprzednikow[i][0] + 1  # +1, bo indeksy od 1
                else:
                    macierz_grafu[i][liczba_wierzcholkow + 1] = 0
                
                # Pierwszy nieincydentny wierzchołek (kolumna |V|+3)
                if listy_nieincydentnych[i]:
                    macierz_grafu[i][liczba_wierzcholkow + 2] = listy_nieincydentnych[i][0] + 1  # +1, bo indeksy od 1
                else:
                    macierz_grafu[i][liczba_wierzcholkow + 2] = 0
                
                # Pierwszy z cyklu (kolumna |V|+4) - dla prostego grafu ustawiamy 0
                macierz_grafu[i][liczba_wierzcholkow + 3] = 0
            
            # Teraz wykonujemy 3 kroki wypełniania głównej części macierzy zgodnie ze slajdami
            
            # Krok 1: Wypełnienie komórek odpowiednio do następników (kolumna 6)
            for i in range(liczba_wierzcholkow):
                j = macierz_grafu[i][liczba_wierzcholkow] - 1  # Indeks pierwszego następnika (odejmujemy 1)
                if j >= 0:  # Jeśli wierzchołek ma następnika
                    macierz_grafu[i][j] = macierz_grafu[i][liczba_wierzcholkow]  # Wstawiamy numer następnika
                    
                    # Teraz uzupełniamy listę następników w macierzy
                    for k in range(len(listy_nastepnikow[i])):
                        if k == 0:  # Pierwszy następnik już został wstawiony
                            continue
                        obecny = listy_nastepnikow[i][k-1]  # Poprzedni następnik
                        nastepny = listy_nastepnikow[i][k]  # Aktualny następnik
                        macierz_grafu[i][obecny] = nastepny + 1  # Wstawiamy numer kolejnego następnika
                    
                    # Ostatni następnik na liście wskazuje na pierwszy (cykl)
                    if listy_nastepnikow[i]:
                        ostatni = listy_nastepnikow[i][-1]
                        pierwszy = listy_nastepnikow[i][0]
                        macierz_grafu[i][ostatni] = pierwszy + 1
            
            # Krok 2: Wypełnienie komórek odpowiednio do poprzedników (kolumna 7)
            for i in range(liczba_wierzcholkow):
                j = macierz_grafu[i][liczba_wierzcholkow + 1] - 1  # Indeks pierwszego poprzednika
                if j >= 0:  # Jeśli wierzchołek ma poprzednika
                    macierz_grafu[i][j] = macierz_grafu[i][liczba_wierzcholkow + 1] + liczba_wierzcholkow
                    
                    # Teraz uzupełniamy listę poprzedników w macierzy
                    for k in range(len(listy_poprzednikow[i])):
                        if k == 0:  # Pierwszy poprzednik już został wstawiony
                            continue
                        obecny = listy_poprzednikow[i][k-1]  # Poprzedni w liście poprzedników
                        nastepny = listy_poprzednikow[i][k]  # Aktualny w liście poprzedników
                        macierz_grafu[i][obecny] = nastepny + 1 + liczba_wierzcholkow
                    
                    # Ostatni poprzednik na liście wskazuje na pierwszy (cykl)
                    if listy_poprzednikow[i]:
                        ostatni = listy_poprzednikow[i][-1]
                        pierwszy = listy_poprzednikow[i][0]
                        macierz_grafu[i][ostatni] = pierwszy + 1 + liczba_wierzcholkow
            
            # Krok 3: Wypełnienie komórek odpowiednio do wierzchołków nieincydentnych (kolumna 8)
            for i in range(liczba_wierzcholkow):
                j = macierz_grafu[i][liczba_wierzcholkow + 2] - 1  # Indeks pierwszego nieincydentnego
                if j >= 0:  # Jeśli istnieje wierzchołek nieincydentny
                    macierz_grafu[i][j] = -(macierz_grafu[i][liczba_wierzcholkow + 2])
                    
                    # Teraz uzupełniamy listę nieincydentnych w macierzy
                    for k in range(len(listy_nieincydentnych[i])):
                        if k == 0:  # Pierwszy nieincydentny już został wstawiony
                            continue
                        obecny = listy_nieincydentnych[i][k-1]  # Poprzedni w liście nieincydentnych
                        nastepny = listy_nieincydentnych[i][k]  # Aktualny w liście nieincydentnych
                        macierz_grafu[i][obecny] = -(nastepny + 1)
                    
                    # Ostatni nieincydentny na liście ma specjalną wartość
                    if listy_nieincydentnych[i]:
                        ostatni = listy_nieincydentnych[i][-1]
                        macierz_grafu[i][ostatni] = -liczba_wierzcholkow - 1
            
            return liczba_wierzcholkow, macierz_sasiedztwa, macierz_grafu
    
    except FileNotFoundError:
        print(f"Błąd: Plik {nazwa_pliku} nie istnieje.")
        return None, None, None
    except Exception as e:
        print(f"Błąd podczas wczytywania grafu: {e}")
        return None, None, None


def dfs_macierz_sasiedztwa(macierz_sasiedztwa, liczba_wierzcholkow):
    """
    Sortowanie topologiczne grafu reprezentowanego przez macierz sąsiedztwa 
    przy użyciu algorytmu DFS.
    """
    # Stany odwiedzenia wierzchołków
    NIEODWIEDZONY = 0
    ODWIEDZANY = 1
    ODWIEDZONY = 2
    
    stany = [NIEODWIEDZONY] * liczba_wierzcholkow
    wynik = []
    
    def dfs_visit(v):
        if stany[v] == ODWIEDZANY:
            # Wykryto cykl
            return False
        
        if stany[v] == ODWIEDZONY:
            return True
        
        stany[v] = ODWIEDZANY
        
        # Przeglądanie wszystkich sąsiadów
        for u in range(liczba_wierzcholkow):
            if macierz_sasiedztwa[v][u] == 1:
                if not dfs_visit(u):
                    return False
        
        stany[v] = ODWIEDZONY
        wynik.append(v)
        return True
    
    # Wykonaj DFS dla każdego nieodwiedzonego wierzchołka
    for v in range(liczba_wierzcholkow):
        if stany[v] == NIEODWIEDZONY:
            if not dfs_visit(v):
                print("Graf zawiera cykl. Sortowanie niemożliwe.")
                return None
    
    # Odwróć wynik, aby otrzymać poprawne sortowanie topologiczne
    wynik.reverse()
    return [w + 1 for w in wynik]  # Dodajemy 1, aby wierzchołki były numerowane od 1


def dfs_macierz_grafu(macierz_sasiedztwa, liczba_wierzcholkow):
    """
    Sortowanie topologiczne grafu reprezentowanego przez macierz sąsiedztwa 
    przy użyciu algorytmu DFS.
    """
    # Stany odwiedzenia wierzchołków
    NIEODWIEDZONY = 0
    ODWIEDZANY = 1
    ODWIEDZONY = 2
    
    stany = [NIEODWIEDZONY] * liczba_wierzcholkow
    wynik = []
    
    def dfs_visit(v):
        if stany[v] == ODWIEDZANY:
            # Wykryto cykl
            return False
        
        if stany[v] == ODWIEDZONY:
            return True
        
        stany[v] = ODWIEDZANY
        
        # Przeglądanie wszystkich sąsiadów
        for u in range(liczba_wierzcholkow):
            if macierz_sasiedztwa[v][u] == 1:
                if not dfs_visit(u):
                    return False
        
        stany[v] = ODWIEDZONY
        wynik.append(v)
        return True
    
    # Wykonaj DFS dla każdego nieodwiedzonego wierzchołka
    for v in range(liczba_wierzcholkow):
        if stany[v] == NIEODWIEDZONY:
            if not dfs_visit(v):
                print("Graf zawiera cykl. Sortowanie niemożliwe.")
                return None
    
    # Odwróć wynik, aby otrzymać poprawne sortowanie topologiczne
    wynik.reverse()
    return [w + 1 for w in wynik]  # Dodajemy 1, aby wierzchołki były numerowane od 1


def kahn_macierz_sasiedztwa(macierz_sasiedztwa, liczba_wierzcholkow):
    """
    Sortowanie topologiczne grafu reprezentowanego przez macierz sąsiedztwa 
    przy użyciu algorytmu Kahna (usuwanie wierzchołków o zerowym stopniu wejściowym).
    """
    # Obliczanie stopni wejściowych wierzchołków
    stopnie_wejsciowe = [0] * liczba_wierzcholkow
    for i in range(liczba_wierzcholkow):
        for j in range(liczba_wierzcholkow):
            if macierz_sasiedztwa[j][i] == 1:
                stopnie_wejsciowe[i] += 1
    
    # Kolejka wierzchołków o zerowym stopniu wejściowym
    kolejka = deque()
    for i in range(liczba_wierzcholkow):
        if stopnie_wejsciowe[i] == 0:
            kolejka.append(i)
    
    # Wynik sortowania topologicznego
    wynik = []
    
    # Główna pętla algorytmu
    while kolejka:
        v = kolejka.popleft()
        wynik.append(v)
        
        # Usuń krawędzie wychodzące z v i zaktualizuj stopnie wejściowe
        for u in range(liczba_wierzcholkow):
            if macierz_sasiedztwa[v][u] == 1:
                stopnie_wejsciowe[u] -= 1
                if stopnie_wejsciowe[u] == 0:
                    kolejka.append(u)
    
    # Sprawdź, czy wszystkie wierzchołki zostały odwiedzone
    if len(wynik) != liczba_wierzcholkow:
        print("Graf zawiera cykl. Sortowanie niemożliwe.")
        return None
    
    return [w + 1 for w in wynik]  # Dodajemy 1, aby wierzchołki były numerowane od 1


def kahn_macierz_grafu(macierz_sasiedztwa, liczba_wierzcholkow):
    """
    Sortowanie topologiczne grafu reprezentowanego przez macierz sąsiedztwa 
    przy użyciu algorytmu Kahna (usuwanie wierzchołków o zerowym stopniu wejściowym).
    """
    # Obliczanie stopni wejściowych wierzchołków
    stopnie_wejsciowe = [0] * liczba_wierzcholkow
    for i in range(liczba_wierzcholkow):
        for j in range(liczba_wierzcholkow):
            if macierz_sasiedztwa[j][i] == 1:
                stopnie_wejsciowe[i] += 1
    
    # Kolejka wierzchołków o zerowym stopniu wejściowym
    kolejka = deque()
    for i in range(liczba_wierzcholkow):
        if stopnie_wejsciowe[i] == 0:
            kolejka.append(i)
    
    # Wynik sortowania topologicznego
    wynik = []
    
    # Główna pętla algorytmu
    while kolejka:
        v = kolejka.popleft()
        wynik.append(v)
        
        # Usuń krawędzie wychodzące z v i zaktualizuj stopnie wejściowe
        for u in range(liczba_wierzcholkow):
            if macierz_sasiedztwa[v][u] == 1:
                stopnie_wejsciowe[u] -= 1
                if stopnie_wejsciowe[u] == 0:
                    kolejka.append(u)
    
    # Sprawdź, czy wszystkie wierzchołki zostały odwiedzone
    if len(wynik) != liczba_wierzcholkow:
        print("Graf zawiera cykl. Sortowanie niemożliwe.")
        return None
    
    return [w + 1 for w in wynik]  # Dodajemy 1, aby wierzchołki były numerowane od 1

def wyswietl_macierz(macierz, nazwa="Macierz"):
    """Wyświetla macierz w czytelnym formacie."""
    print(f"\n{nazwa}:")
    for wiersz in macierz:
        print(" ".join(str(elem).rjust(3) for elem in wiersz))


def main():
    print("Program do sortowania topologicznego grafów skierowanych")
    print("=" * 60)
    
    nazwa_pliku = input("Podaj nazwę pliku z grafem: ")
    liczba_wierzcholkow, macierz_sasiedztwa, macierz_grafu = wczytaj_graf_z_pliku(nazwa_pliku)
    
    if macierz_sasiedztwa is None:
        return
    
    print(f"\nWczytano graf o {liczba_wierzcholkow} wierzchołkach.")
    
    wybor = True
    while wybor:
        print("\nWybierz algorytm sortowania topologicznego:")
        print("1. DFS na macierzy sąsiedztwa")
        print("2. DFS na macierzy grafu")
        print("3. Algorytm Kahna na macierzy sąsiedztwa")
        print("4. Algorytm Kahna na macierzy grafu")
        print("5. Wyświetl macierz sąsiedztwa")
        print("6. Wyświetl macierz grafu")
        print("0. Zakończ")
        
        opcja = input("\nTwój wybór: ")
        
        if opcja == "1":
            print("\nSortowanie topologiczne DFS na macierzy sąsiedztwa:")
            wynik = dfs_macierz_sasiedztwa(macierz_sasiedztwa, liczba_wierzcholkow)
            if wynik:
                print("Wynik sortowania:", " -> ".join(map(str, wynik)))
        
        elif opcja == "2":
            print("\nSortowanie topologiczne DFS na macierzy grafu:")
            wynik = dfs_macierz_grafu(macierz_sasiedztwa, liczba_wierzcholkow)
            if wynik:
                print("Wynik sortowania:", " -> ".join(map(str, wynik)))
        
        elif opcja == "3":
            print("\nSortowanie topologiczne algorytmem Kahna na macierzy sąsiedztwa:")
            wynik = kahn_macierz_sasiedztwa(macierz_sasiedztwa, liczba_wierzcholkow)
            if wynik:
                print("Wynik sortowania:", " -> ".join(map(str, wynik)))
        
        elif opcja == "4":
            print("\nSortowanie topologiczne algorytmem Kahna na macierzy grafu:")
            wynik = kahn_macierz_grafu(macierz_sasiedztwa, liczba_wierzcholkow)
            if wynik:
                print("Wynik sortowania:", " -> ".join(map(str, wynik)))
        
        elif opcja == "5":
            wyswietl_macierz(macierz_sasiedztwa, "Macierz sąsiedztwa")
        
        elif opcja == "6":
            wyswietl_macierz(macierz_grafu, "Macierz grafu")
        
        elif opcja == "0":
            wybor = False
        
        else:
            print("Nieprawidłowy wybór. Spróbuj ponownie.")


if __name__ == "__main__":
    main()