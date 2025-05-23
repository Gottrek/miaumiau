from collections import defaultdict

def read_graph_from_file(filename):
    try:
        with open(filename, 'r') as file:
            lines = file.readlines()
            
        n, m = map(int, lines[0].strip().split())
        
        edges = []
        for i in range(1, m + 1):
            u, v = map(int, lines[i].strip().split())
            edges.append((u, v))
            
        return n, edges
    except FileNotFoundError:
        print(f"Plik {filename} nie został znaleziony")
        return None, None
    except Exception as e:
        print(f"Błąd podczas wczytywania pliku: {e}")
        return None, None

def create_adjacency_matrix(n, edges, directed=False):
    matrix = [[0] * n for _ in range(n)]
    
    for u, v in edges:
        matrix[u][v] = 1
        if not directed:
            matrix[v][u] = 1
            
    return matrix

def create_adjacency_list(n, edges, directed=False):
    adj_list = defaultdict(list)
    
    for u, v in edges:
        adj_list[u].append(v)
        if not directed:
            adj_list[v].append(u)
            
    return adj_list

def hamilton_cycle_undirected_matrix(matrix):
    n = len(matrix)
    if n == 0:
        return None
        
    path = [0]
    visited = [False] * n
    visited[0] = True
    
    def backtrack():
        if len(path) == n:
            if matrix[path[-1]][path[0]] == 1:
                return True
            return False
        
        current = path[-1]
        for next_vertex in range(n):
            if matrix[current][next_vertex] == 1 and not visited[next_vertex]:
                visited[next_vertex] = True
                path.append(next_vertex)
                
                if backtrack():
                    return True
                
                path.pop()
                visited[next_vertex] = False
        
        return False
    
    if backtrack():
        return path + [path[0]]                             
    return None

def hamilton_cycle_directed_list(adj_list, n):
    if n == 0:
        return None
        
    path = [0]
    visited = [False] * n
    visited[0] = True
    
    def backtrack():
        if len(path) == n:
            # Sprawdź czy można wrócić do punktu startowego
            if path[0] in adj_list[path[-1]]:
                return True
            return False
        
        current = path[-1]
        for next_vertex in adj_list[current]:
            if not visited[next_vertex]:
                visited[next_vertex] = True
                path.append(next_vertex)
                
                if backtrack():
                    return True
                
                path.pop()
                visited[next_vertex] = False
        
        return False
    
    if backtrack():
        return path + [path[0]]
    return None

def euler_cycle_undirected_matrix(matrix):
    n = len(matrix)
    if n == 0:
        return None
    
    # Sprawdź warunek istnienia cyklu Eulera - wszystkie stopnie parzyste
    degrees = []
    for i in range(n):
        degree = sum(matrix[i])
        if degree > 0:
            degrees.append(degree)
            if degree % 2 != 0:
                return None
    
    if not degrees:
        return None
    
    # Algorytm Hierholzera
    temp_matrix = [row[:] for row in matrix]
    
    start = 0
    for i in range(n):
        if sum(temp_matrix[i]) > 0:
            start = i
            break
    
    def find_cycle(start_vertex):
        cycle = [start_vertex]
        current = start_vertex
        
        while True:
            found_edge = False
            for next_vertex in range(n):
                if temp_matrix[current][next_vertex] > 0:
                    temp_matrix[current][next_vertex] -= 1
                    temp_matrix[next_vertex][current] -= 1
                    
                    cycle.append(next_vertex)
                    current = next_vertex
                    found_edge = True
                    break
            
            if not found_edge or current == start_vertex:
                break
        
        return cycle
    
    euler_cycle = find_cycle(start)
    
    # Znajdź i włącz sub-cykle
    total_edges = sum(sum(row) for row in temp_matrix)
    while total_edges > 0:
        insertion_point = -1
        for i, vertex in enumerate(euler_cycle):
            if sum(temp_matrix[vertex]) > 0:
                insertion_point = i
                break
        
        if insertion_point == -1:
            break
        
        sub_cycle = find_cycle(euler_cycle[insertion_point])
        
        euler_cycle = (euler_cycle[:insertion_point] + 
                      sub_cycle[:-1] + 
                      euler_cycle[insertion_point:])
        
        total_edges = sum(sum(row) for row in temp_matrix)
    
    if sum(sum(row) for row in temp_matrix) == 0:
        return euler_cycle
    return None

def euler_cycle_directed_list(adj_list, n):
    if n == 0:
        return None
    
    # Sprawdź warunek: stopień_wchodzący == stopień_wychodzący
    in_degree = [0] * n
    out_degree = [0] * n
    
    for vertex in adj_list:
        out_degree[vertex] = len(adj_list[vertex])
        for neighbor in adj_list[vertex]:
            in_degree[neighbor] += 1
    
    for i in range(n):
        if in_degree[i] != out_degree[i]:
            return None
    
    if all(out_degree[i] == 0 for i in range(n)):
        return None
    
    temp_adj_list = defaultdict(list)
    for vertex in adj_list:
        temp_adj_list[vertex] = adj_list[vertex][:]
    
    start = 0
    for i in range(n):
        if len(temp_adj_list[i]) > 0:
            start = i
            break
    
    def find_cycle(start_vertex):
        cycle = [start_vertex]
        current = start_vertex
        
        while temp_adj_list[current]:
            next_vertex = temp_adj_list[current].pop(0)
            cycle.append(next_vertex)
            current = next_vertex
        
        return cycle
    
    euler_cycle = find_cycle(start)
    
    total_edges = sum(len(temp_adj_list[vertex]) for vertex in temp_adj_list)
    
    while total_edges > 0:
        insertion_point = -1
        for i, vertex in enumerate(euler_cycle):
            if len(temp_adj_list[vertex]) > 0:
                insertion_point = i
                break
        
        if insertion_point == -1:
            break
        
        sub_cycle = find_cycle(euler_cycle[insertion_point])
        
        euler_cycle = (euler_cycle[:insertion_point] + 
                      sub_cycle[:-1] + 
                      euler_cycle[insertion_point:])
        
        total_edges = sum(len(temp_adj_list[vertex]) for vertex in temp_adj_list)
    
    if sum(len(temp_adj_list[vertex]) for vertex in temp_adj_list) == 0:
        return euler_cycle
    return None

def print_cycle(cycle, cycle_type):
    if cycle:
        print(f"Znaleziony cykl {cycle_type}: {' -> '.join(map(str, cycle))}")
    else:
        print("Graf wejściowy nie zawiera cyklu.")

def run_interactive():
    print("=== ZNAJDOWANIE CYKLI W GRAFACH ===")
    print("1. Graf nieskierowany - cykl Hamiltona (macierz sąsiedztwa)")
    print("2. Graf skierowany - cykl Hamiltona (lista następników)")
    print("3. Graf nieskierowany - cykl Eulera (macierz sąsiedztwa)")
    print("4. Graf skierowany - cykl Eulera (lista następników)")
    
    choice = input("\nWybierz opcję (1-4): ")
    filename = input("Podaj nazwę pliku z grafem: ")
    
    n, edges = read_graph_from_file(filename)
    if n is None:
        return
    
    print(f"\nWczytano graf o {n} wierzchołkach i {len(edges)} krawędziach")
    
    if choice == '1':
        matrix = create_adjacency_matrix(n, edges, directed=False)
        cycle = hamilton_cycle_undirected_matrix(matrix)
        print_cycle(cycle, "Hamiltona")
        
    elif choice == '2':
        adj_list = create_adjacency_list(n, edges, directed=True)
        cycle = hamilton_cycle_directed_list(adj_list, n)
        print_cycle(cycle, "Hamiltona")
        
    elif choice == '3':
        matrix = create_adjacency_matrix(n, edges, directed=False)
        cycle = euler_cycle_undirected_matrix(matrix)
        print_cycle(cycle, "Eulera")
        
    elif choice == '4':
        adj_list = create_adjacency_list(n, edges, directed=True)
        cycle = euler_cycle_directed_list(adj_list, n)
        print_cycle(cycle, "Eulera")
        
    else:
        print("Nieprawidłowy wybór!")

def main():
    # Przykłady testowe
    print("=== PRZYKŁADY TESTOWE ===\n")
    
    # Test 1: Graf nieskierowany z cyklem Hamiltona
    print("Test 1: Graf nieskierowany - cykl Hamiltona")
    n = 4
    edges = [(0, 1), (1, 2), (2, 3), (3, 0), (0, 2)]
    matrix = create_adjacency_matrix(n, edges, directed=False)
    cycle = hamilton_cycle_undirected_matrix(matrix)
    print_cycle(cycle, "Hamiltona")
    
    # Test 2: Graf skierowany z cyklem Hamiltona
    print("\nTest 2: Graf skierowany - cykl Hamiltona")
    adj_list = create_adjacency_list(n, edges, directed=True)
    cycle = hamilton_cycle_directed_list(adj_list, n)
    print_cycle(cycle, "Hamiltona")
    
    # Test 3: Graf nieskierowany z cyklem Eulera
    print("\nTest 3: Graf nieskierowany - cykl Eulera")
    n = 4
    edges = [(0, 1), (1, 2), (2, 3), (3, 0)]
    matrix = create_adjacency_matrix(n, edges, directed=False)
    cycle = euler_cycle_undirected_matrix(matrix)
    print_cycle(cycle, "Eulera")
    
    # Test 4: Graf skierowany z cyklem Eulera
    print("\nTest 4: Graf skierowany - cykl Eulera")
    adj_list = create_adjacency_list(n, edges, directed=True)
    cycle = euler_cycle_directed_list(adj_list, n)
    print_cycle(cycle, "Eulera")
    
    print("\n" + "="*50)
    
    try:
        run_interactive()
    except KeyboardInterrupt:
        print("\nProgram zakończony przez użytkownika.")

if __name__ == "__main__":
    main()