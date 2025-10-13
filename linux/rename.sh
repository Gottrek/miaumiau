#!/bin/bash

# Sprawdź liczbę argumentów
if [ $# -ne 2 ]; then
  echo "Użycie: $0 SZUKANY_WZORZEC NOWA_NAZWA"
  exit 1
fi

find . -depth -name "*$1*" | while read old; do
  new=$(echo "$old" | sed "s/$1/$2/g")
  [ "$old" != "$new" ] && mv "$old" "$new"
done
