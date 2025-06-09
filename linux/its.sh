#!/bin/bash

hour=$(date +%H)
case $1 in
  early)     [ "$hour" -ge 4 ]  && [ "$hour" -lt 7 ] ;;
  morning)   [ "$hour" -ge 7 ]  && [ "$hour" -lt 11 ] ;;
  lunchtime) [ "$hour" -ge 11 ] && [ "$hour" -lt 14 ] ;;
  day)       [ "$hour" -ge 7 ]  && [ "$hour" -lt 18 ] ;;
  evening)   [ "$hour" -ge 18 ] && [ "$hour" -lt 21 ] ;;
  night)     [ "$hour" -ge 21 ] || [ "$hour" -lt 4 ] ;;
  late)      [ "$hour" -ge 23 ] || [ "$hour" -lt 2 ] ;;
  *) echo "Nieznana pora: $1" >&2; exit 1 ;;
esac