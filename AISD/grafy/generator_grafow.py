from collections import deque
import random
import time

def generate_dag(n, saturation_percent=50):
    """
    Generuje graf skierowany acykliczny (DAG) o n wierzchołkach i zapisuje do pliku .txt.

    Args:
        n (int): liczba wierzchołków
        saturation_percent (float): procent nasycenia krawędziami (domyślnie 50%)

    Returns:
        list[list[int]]: macierz sąsiedztwa n×n grafu DAG
    """
    # Oblicz maksymalną i docelową liczbę krawędzi
    max_edges = n * (n - 1) // 2
    target_edges = int(max_edges * saturation_percent / 100)

    # Inicjalizuj pustą macierz n x n wypełnioną zerami
    adj_matrix = [[0 for _ in range(n)] for _ in range(n)]

    # Lista możliwych krawędzi (i, j), gdzie i < j (gwarantuje acykliczność)
    possible_edges = [(i, j) for i in range(n) for j in range(i + 1, n)]

    # Losowy wybór krawędzi
    if target_edges <= len(possible_edges):
        selected_edges = random.sample(possible_edges, target_edges)
        for i, j in selected_edges:
            adj_matrix[i][j] = 1

    # Tworzenie unikalnej nazwy pliku
    random_id = random.randint(1000, 9999)
    filename = f"dag_n{n}_sat{int(saturation_percent)}_{random_id}.txt"

    # Zapis macierzy do pliku
    with open(filename, "w") as f:
        for row in adj_matrix:
            f.write(" ".join(str(val) for val in row) + "\n")

    return adj_matrix

generate_dag(15)