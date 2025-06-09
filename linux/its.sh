#!/bin/bash

# Skrypt-predykat: zwraca 0 (true) jeśli aktualna pora pasuje do tej podanej jako argument.
# Obsługiwane pory: early, late, day, night, morning, lunchtime, evening

hour=$(date +%H)  # Pobiera aktualną godzinę (00–23)
case $1 in
  early)     [ "$hour" -ge 4 ]  && [ "$hour" -lt 7 ] ;;   # 04:00–06:59
  morning)   [ "$hour" -ge 7 ]  && [ "$hour" -lt 11 ] ;;  # 07:00–10:59
  lunchtime) [ "$hour" -ge 11 ] && [ "$hour" -lt 14 ] ;;  # 11:00–13:59
  day)       [ "$hour" -ge 7 ]  && [ "$hour" -lt 18 ] ;;  # 07:00–17:59
  evening)   [ "$hour" -ge 18 ] && [ "$hour" -lt 21 ] ;;  # 18:00–20:59
  night)     [ "$hour" -ge 21 ] || [ "$hour" -lt 4 ] ;;   # 21:00–03:59
  late)      [ "$hour" -ge 23 ] || [ "$hour" -lt 2 ] ;;   # 23:00–01:59
  *) echo "Nieznana pora dnia: $1" >&2; exit 1 ;;         # Błąd przy nieznanej porze
esac
