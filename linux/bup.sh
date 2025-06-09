#!/bin/bash

dir=$1; shift
ts=$(date +%y-%m-%d_%H:%M)

[ ! -e "$dir" ] && mkdir -p "$dir"
[ ! -d "$dir" ] && echo "$dir nie jest katalogiem!" && exit 1

missing=()

for f in "$@"; do
  [ ! -e "$f" ] && missing+=("$f") && continue
  name=$(basename "$f")
  cp -r "$f" "$dir/${ts}_$name"
done

[ ${#missing[@]} -gt 0 ] && echo "Nie znaleziono: ${missing[*]}"