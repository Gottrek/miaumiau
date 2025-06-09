#!/bin/bash

# Użycie: ./watchfile PLIK KOMENDA [ODSTĘP]
# Sprawdza co [ODSTĘP] sekund (domyślnie 1), czy plik się zmienił.
# Jeśli tak — wykonuje komendę i kończy działanie.

file=$1              # Ścieżka do pliku
cmd=$2               # Komenda do wykonania
interval=${3:-1}     # Odstęp czasowy (domyślnie 1 sekunda)

[ ! -r "$file" ] && echo "Błąd: nie można otworzyć pliku $file" && exit 1

prev=$(md5sum "$file")         # Zapamiętaj hash pliku

while true; do
  sleep "$interval"            # Odczekaj podany czas
  now=$(md5sum "$file") || exit 1
  [ "$prev" != "$now" ] && $cmd && break  # Jeśli zmieniono — wykonaj komendę
done