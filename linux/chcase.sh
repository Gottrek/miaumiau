#!/bin/bash

# Skrypt przekształca słowa na różne formaty nazewnictwa
# Użycie: ./chcase FORMAT słowa...
# Dostępne formaty: joined, underline, uppercase, dashes

case $1 in
  joined)       # Bez spacji
    shift
    echo $* | tr -d ' '
    ;;
  underline)    # Zamiast spacji: podkreślenia
    shift
    echo $* | tr ' ' '_'
    ;;
  uppercase)    # Podkreślenia + wielkie litery
    shift
    echo $* | tr ' ' '_' | tr a-z A-Z
    ;;
  dashes)       # Zamiast spacji: myślniki
    shift
    echo $* | tr ' ' '-'
    ;;
  *)            # Nieznany format – pokaż dostępne
    echo "Dostępne formaty: joined underline uppercase dashes"
    ;;
esac
