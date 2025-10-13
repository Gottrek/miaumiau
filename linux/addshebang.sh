#!/bin/bash

[ $# -eq 0 ] && echo "Użycie: $0 katalog1 [katalog2 ...]" && exit 1

for dir in "$@"; do
  [ -d "$dir" ] || continue

  find "$dir" -type f \( -name "*.sh" -o -name "*.py" \) | while read -r f; do
    head -n1 "$f" | grep -q '^#!' && echo "Pominięto: $f" && continue

    case "$f" in
      *.sh) s='#!/bin/bash' ;;
      *.py) s='#!/usr/bin/env python' ;;
      *) continue ;;
    esac

    echo -e "$s\n$(cat "$f")" > "$f"
    chmod +x "$f"
    echo "Dodano: $f"
  done
done