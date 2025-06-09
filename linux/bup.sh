#!/bin/bash

# Użycie: ./bup KATALOG_DOCELOWY PLIK1 [PLIK2 ...]
# Tworzy kopie podanych plików/katalogów w formacie:
# KATALOG/YY-MM-DD_HH:MM_nazwa

dir=$1; shift                          # Pierwszy argument to katalog docelowy
ts=$(date +%y-%m-%d_%H:%M)            # Znacznik czasu

[ ! -e "$dir" ] && mkdir -p "$dir"    # Jeśli katalog nie istnieje — utwórz
[ ! -d "$dir" ] && echo "$dir nie jest katalogiem!" && exit 1  # Jeśli nie katalog — błąd

missing=()                            # Lista brakujących plików

for f in "$@"; do
  [ ! -e "$f" ] && missing+=("$f") && continue      # Jeśli nie istnieje — zapisz i pomiń
  name=$(basename "$f")                             # Wyciągnij nazwę pliku
  cp -r "$f" "$dir/${ts}_$name"                     # Skopiuj z nową nazwą
done

[ ${#missing[@]} -gt 0 ] && echo "Nie znaleziono: ${missing[*]}"  # Ostrzeż o brakach