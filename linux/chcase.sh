#!/bin/bash

case $1 in
  joined)
    shift
    echo $* | tr -d ' '
    ;;
  underline)
    shift
    echo $* | tr ' ' '_'
    ;;
  uppercase)
    shift
    echo $* | tr ' ' '_' | tr a-z A-Z
    ;;
  dashes)
    shift
    echo $* | tr ' ' '-'
    ;;
  *)
    echo "Dostępne formaty: joined underline uppercase dashes"
    ;;
esac