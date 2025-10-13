#!/bin/bash

[ $# -eq 0 ] && { echo "Użycie: $0 <katalog>"; exit 1; }
[ ! -d "$1" ] && { echo "Błąd: '$1' nie jest katalogiem"; exit 1; }

tar -czf "${1}_$(date +%Y-%m-%d).tar.gz" "$1"