#!/bin/bash

# Sprawdzenie, czy podano login jako parametr
[ -z "$1" ] && echo "Użycie: $0 login_użytkownika" && exit 1

login="$1"

# Czekaj, aż użytkownik się zaloguje
until who | grep -q "^$login "; do
  echo "Użytkownik $login nie jest zalogowany. Czekam..."
  sleep 5
done

echo "Użytkownik $login jest już zalogowany!"
