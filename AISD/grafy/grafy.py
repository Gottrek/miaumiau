"""
Implementacja algorytmów sortowania topologicznego:
1. DFS na macierzy sąsiedztwa
2. DFS na macierzy grafu
3. Algorytm Kahna (usuwanie wierzchołków o zerowym stopniu wejściowym) na macierzy sąsiedztwa
4. Algorytm Kahna na macierzy grafu

Każdy algorytm wykrywa cykle w grafie wejściowym.
"""

from collections import deque
import random
import time

def convert_to_adjacency_list(graph_data):
    """
    Converts graph data from a list of edges to an adjacency list.
    
    The input format is expected to be:
    - First line: "V E" where V is the number of vertices and E is the number of edges
    - Subsequent E lines: "u v" representing a directed edge from vertex u to vertex v
    
    Parameters:
        graph_data (str): Multi-line string with graph data in the specified format
        
    Returns:
        dict: Adjacency list representation {vertex: [neighbors]}
    """
    # Split input data into lines and remove any empty lines
    lines = [line.strip() for line in graph_data.strip().split('\n') if line.strip()]
    
    # Parse first line to get number of vertices and edges
    first_line = lines[0].split()
    num_vertices = int(first_line[0])
    num_edges = int(first_line[1])
    
    # Initialize empty adjacency list for all vertices
    adj_list = {i: [] for i in range(1, num_vertices + 1)}
    
    # Process each edge
    for i in range(1, num_edges + 1):
        if i < len(lines):
            edge = lines[i].split()
            if len(edge) >= 2:
                u = int(edge[0])
                v = int(edge[1])
                
                # Add v to the adjacency list of u (directed edge u->v)
                adj_list[u].append(v)
    
    return adj_list

def zapisz_czas_do_pliku(nazwa_funkcji, czas_ms, nazwa_pliku="czasy_wykonania.txt"):
    """
    Zapisuje czas wykonania funkcji do pliku tekstowego.
    
    Args:
        nazwa_funkcji (str): nazwa operacji
        czas_ms (float): czas w milisekundach
        nazwa_pliku (str): nazwa pliku do zapisu
    """
    try:
        with open(nazwa_pliku, "a", encoding="utf-8") as f:
            f.write(f"{nazwa_funkcji}: {czas_ms:.3f} ms\n")
    except Exception as e:
        print(f"Błąd zapisu do pliku: {e}")

def generate_graph_matrix(adjacency_list):
    """
    Converts an adjacency list to a graph matrix, handling multigraphs and loops
    
    Parameters:
        adjacency_list: Dictionary mapping vertices to their neighbors {vertex: [neighbors]}
    
    Returns:
        A graph matrix according to lecture specifications
    """
    # Determine the vertex count
    vertex_count = len(adjacency_list)
    
    # Initialize data structures with deep copies to preserve original data
    next_vertices = {v+1: [] for v in range(vertex_count)}     # Successor list
    prev_vertices = {v+1: [] for v in range(vertex_count)}     # Predecessor list
    disconnected = {v+1: [] for v in range(vertex_count)}      # Non-incident list
    loop_cycles = {v+1: [] for v in range(vertex_count)}       # Cycle list
    
    # Populate successors list from adjacency list
    # (keeping duplicates for multi-edges)
    for vertex, neighbors in adjacency_list.items():
        next_vertices[vertex] = neighbors.copy()
    
    # Build predecessor lists, preserving edge multiplicity
    for vertex, neighbors in adjacency_list.items():
        for neighbor in neighbors:
            prev_vertices[neighbor].append(vertex)
    
    # Identify cycles of length 0 (self-loops) and length 1
    cycles_registry = {}  # Dictionary to store identified cycles
    
    # First pass: identify all cycles
    for vertex, neighbors in adjacency_list.items():
        # Self-loops (cycles of length 0)
        if vertex in neighbors:
            # Count occurrences of vertex in its own neighborhood
            loop_count = neighbors.count(vertex)
            # Add the appropriate number of loops to cycles
            for _ in range(loop_count):
                if vertex not in loop_cycles[vertex]:
                    loop_cycles[vertex].append(vertex)
            
            # Store cycle information for later processing
            if vertex not in cycles_registry:
                cycles_registry[vertex] = {}
            cycles_registry[vertex][vertex] = loop_count
        
        # Cycles of length 1 (bidirectional edges)
        for neighbor in neighbors:
            if vertex in adjacency_list.get(neighbor, []) and vertex != neighbor:
                # Count bidirectional connections
                forward_count = neighbors.count(neighbor)
                backward_count = adjacency_list[neighbor].count(vertex)
                cycle_count = min(forward_count, backward_count)
                
                # Add appropriate number of cycles
                for _ in range(cycle_count):
                    if neighbor not in loop_cycles[vertex]:
                        loop_cycles[vertex].append(neighbor)
                
                # Store cycle information for later processing
                if vertex not in cycles_registry:
                    cycles_registry[vertex] = {}
                cycles_registry[vertex][neighbor] = cycle_count
    
    # Process identified cycles - remove cycles from successor and predecessor lists
    for v1, neighbors in cycles_registry.items():
        for v2, count in neighbors.items():
            # Remove specified number of edges from successor and predecessor lists
            for _ in range(count):
                if v2 in next_vertices[v1]:
                    next_vertices[v1].remove(v2)
                if v1 in prev_vertices[v2]:
                    prev_vertices[v2].remove(v1)
                
                # For length-1 cycles, remove reverse edges too
                if v1 != v2:
                    if v1 in next_vertices[v2]:
                        next_vertices[v2].remove(v1)
                    if v2 in prev_vertices[v1]:
                        prev_vertices[v1].remove(v2)
    
    # Build non-incident vertex lists (vertices with no connecting edges)
    for v1 in range(1, vertex_count + 1):
        for v2 in range(1, vertex_count + 1):
            if v2 not in next_vertices[v1] and v1 not in next_vertices[v2] and v2 not in loop_cycles[v1]:
                disconnected[v1].append(v2)
    
    # Create graph matrix with dimensions |V| x (|V| + 4)
    result_matrix = [[0 for _ in range(vertex_count + 4)] for _ in range(vertex_count)]
    
    # Phase 0: Fill matrix with vertex relationships
    for i in range(vertex_count):
        for j in range(vertex_count):
            v_i = i + 1
            v_j = j + 1
            
            # Check connection type
            is_next = v_j in next_vertices[v_i]
            is_prev = v_i in next_vertices[v_j]
            has_cycle = v_j in loop_cycles[v_i]
            
            if has_cycle:
                # Vertices are in a cycle (will handle in phase 4)
                pass
            elif is_next and is_prev:
                # v_j is both successor and predecessor of v_i
                result_matrix[i][j] = 2
            elif is_next:
                # v_j is a successor of v_i
                result_matrix[i][j] = 5
            elif is_prev:
                # v_j is a predecessor of v_i
                result_matrix[i][j] = 3
            else:
                # v_i and v_j have no connecting edge
                result_matrix[i][j] = -1
    
    # Phase 1: Fill successor column (|V|+1)
    for i in range(vertex_count):
        v_i = i + 1
        if next_vertices[v_i]:
            result_matrix[i][vertex_count] = next_vertices[v_i][0]  # First successor
            
            for j in range(vertex_count):
                v_j = j + 1
                if v_j in next_vertices[v_i]:
                    # Find successor's index in successor list
                    idx = next_vertices[v_i].index(v_j)
                    # Get next successor if exists, otherwise the last one
                    if idx + 1 < len(next_vertices[v_i]):
                        result_matrix[i][j] = next_vertices[v_i][idx + 1]
                    else:
                        result_matrix[i][j] = next_vertices[v_i][-1]
    
    # Phase 2: Fill predecessor column (|V|+2)
    for i in range(vertex_count):
        v_i = i + 1
        if prev_vertices[v_i]:
            result_matrix[i][vertex_count + 1] = prev_vertices[v_i][0]  # First predecessor
            
            for j in range(vertex_count):
                v_j = j + 1
                if v_j in prev_vertices[v_i]:
                    # Find predecessor's index in predecessor list
                    idx = prev_vertices[v_i].index(v_j)
                    # Get next predecessor if exists, otherwise the last one
                    if idx + 1 < len(prev_vertices[v_i]):
                        next_pred = prev_vertices[v_i][idx + 1]
                    else:
                        next_pred = prev_vertices[v_i][-1]
                    # Add vertex count
                    result_matrix[i][j] = next_pred + vertex_count
    
    # Phase 3: Fill non-incident column (|V|+3)
    for i in range(vertex_count):
        v_i = i + 1
        if disconnected[v_i]:
            result_matrix[i][vertex_count + 2] = disconnected[v_i][0]  # First non-incident
            
            for j in range(vertex_count):
                v_j = j + 1
                if v_j in disconnected[v_i]:
                    # Find non-incident vertex's index in non-incident list
                    idx = disconnected[v_i].index(v_j)
                    # Get next non-incident if exists, otherwise the last one
                    if idx + 1 < len(disconnected[v_i]):
                        next_non = disconnected[v_i][idx + 1]
                    else:
                        next_non = disconnected[v_i][-1]
                    # Add negative sign
                    result_matrix[i][j] = -next_non
    
    # Phase 4: Fill cycle column (|V|+4)
    for i in range(vertex_count):
        v_i = i + 1
        if loop_cycles[v_i]:
            result_matrix[i][vertex_count + 3] = loop_cycles[v_i][0]  # First cycle vertex
            
            for j in range(vertex_count):
                v_j = j + 1
                if v_j in loop_cycles[v_i]:
                    # Find cycle vertex's index in cycle list
                    idx = loop_cycles[v_i].index(v_j)
                    # Get next cycle vertex if exists, otherwise the last one
                    if idx + 1 < len(loop_cycles[v_i]):
                        next_cycle = loop_cycles[v_i][idx + 1]
                    else:
                        next_cycle = loop_cycles[v_i][-1]
                    # Add 2 * vertex count
                    result_matrix[i][j] = next_cycle + 2 * vertex_count
    
    return result_matrix

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
            zawartosc = plik.read()
            
            # Wczytaj listę sąsiedztwa
            adj_list = convert_to_adjacency_list(zawartosc)
            
            # Określ liczbę wierzchołków
            liczba_wierzcholkow = len(adj_list)
            
            # Utwórz macierz sąsiedztwa na podstawie listy sąsiedztwa
            macierz_sasiedztwa = [[0 for _ in range(liczba_wierzcholkow)] for _ in range(liczba_wierzcholkow)]
            for u, sasiedzi in adj_list.items():
                for v in sasiedzi:
                    macierz_sasiedztwa[u-1][v-1] = 1
            
            # Utwórz macierz grafu
            macierz_grafu = generate_graph_matrix(adj_list)
            
            return liczba_wierzcholkow, macierz_sasiedztwa, macierz_grafu
    
    except FileNotFoundError:
        print(f"Błąd: Plik {nazwa_pliku} nie istnieje.")
        return None, None, None
    except Exception as e:
        print(f"Błąd podczas wczytywania grafu: {e}")
        return None, None, None


def konwertuj_na_macierz_sasiedztwa(macierz_grafu):
    n = len(macierz_grafu)
    adj = [[0] * n for _ in range(n)]
    
    for i in range(n):
        for j in range(n):
            val = macierz_grafu[i][j]
            if val < 1 or val > n:
                adj[i][j] = 0
            else:
                adj[i][j] = 1
    return adj


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


def dfs_macierz_grafu(macierz_grafu, liczba_wierzcholkow):
    """
    Sortowanie topologiczne grafu reprezentowanego przez macierz sąsiedztwa 
    przy użyciu algorytmu DFS.
    """
    # Konwersja
    macierz_sasiedztwa = konwertuj_na_macierz_sasiedztwa(macierz_grafu)

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


def kahn_macierz_grafu(macierz_grafu, liczba_wierzcholkow):
    """
    Sortowanie topologiczne grafu reprezentowanego przez macierz sąsiedztwa 
    przy użyciu algorytmu Kahna (usuwanie wierzchołków o zerowym stopniu wejściowym).
    """
    # Konwersja
    macierz_sasiedztwa = konwertuj_na_macierz_sasiedztwa(macierz_grafu)

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
        start_total = time.perf_counter()

        if opcja == "1":
            print("\nSortowanie topologiczne DFS na macierzy sąsiedztwa:")
            wynik = dfs_macierz_sasiedztwa(macierz_sasiedztwa, liczba_wierzcholkow)
            if wynik:
                print("Wynik sortowania:", " -> ".join(map(str, wynik)))
        
        elif opcja == "2":
            print("\nSortowanie topologiczne DFS na macierzy grafu:")
            wynik = dfs_macierz_grafu(macierz_grafu, liczba_wierzcholkow)
            if wynik:
                print("Wynik sortowania:", " -> ".join(map(str, wynik)))
        
        elif opcja == "3":
            print("\nSortowanie topologiczne algorytmem Kahna na macierzy sąsiedztwa:")
            wynik = kahn_macierz_sasiedztwa(macierz_sasiedztwa, liczba_wierzcholkow)
            if wynik:
                print("Wynik sortowania:", " -> ".join(map(str, wynik)))
        
        elif opcja == "4":
            print("\nSortowanie topologiczne algorytmem Kahna na macierzy grafu:")
            wynik = kahn_macierz_grafu(macierz_grafu, liczba_wierzcholkow)
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
        
        end_total = time.perf_counter()
        total_time_ms = (end_total - start_total) * 1000
        zapisz_czas_do_pliku(opcja, total_time_ms, "pomiary.txt")


if __name__ == "__main__":
    main()