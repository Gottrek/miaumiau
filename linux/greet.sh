#!/bin/bash

echo -n "Czy jest wieczór? (tak/nie): "
read odp

if [ "$odp" = "tak" ]; then
  echo "Dobry wieczór"
elif [ "$odp" = "nie" ]; then
  echo "Dzień dobry"
else
  echo "Nie rozpoznana odpowiedź: $odp"
fi
