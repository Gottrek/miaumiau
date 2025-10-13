#!/bin/bash

[ "$1" = "-f" ] && find . -maxdepth 1 -type f |
while read f; do n=$(basename "$f"); mv "$n" "${n,,}"; done

[ "$1" = "-d" ] && find . -maxdepth 1 -type d ! -name . |
while read d; do n=$(basename "$d"); mv "$n" "${n,,}"; done
