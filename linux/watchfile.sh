#!/bin/bash

file=$1
cmd=$2
interval=${3:-1}

[ ! -r "$file" ] && echo "Błąd: nie można otworzyć pliku $file" && exit 1

prev=$(md5sum "$file")

while true; do
  sleep "$interval"
  now=$(md5sum "$file") || exit 1
  [ "$prev" != "$now" ] && $cmd && break
done