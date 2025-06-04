def convert_adj_matrix_to_successor_list(input_filename, output_filename):
    """
    Konwertuje plik z macierzą sąsiedztwa na plik z listą następników.

    Args:
        input_filename (str): ścieżka do pliku z macierzą sąsiedztwa
        output_filename (str): ścieżka do pliku wynikowego z listą następników
    """
    with open(input_filename, "r") as f:
        lines = f.readlines()

    n = len(lines)  # liczba wierszy = liczba wierzchołków
    edges = []

    for i, line in enumerate(lines):
        row = list(map(int, line.strip().split()))
        for j, val in enumerate(row):
            if val == 1:
                # Dodaj krawędź (numerujemy od 1)
                edges.append((i + 1, j + 1))

    with open(output_filename, "w") as f:
        f.write(f"{n} {len(edges)}\n")
        for u, v in edges:
            f.write(f"{u} {v}\n")

convert_adj_matrix_to_successor_list("dag_n1500_sat50_2258.txt", "g1500.txt")