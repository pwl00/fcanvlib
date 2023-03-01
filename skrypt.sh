#!/bin/bash
array=("A" "Ą" "B" "C" "Ć" "D" "E" "Ę" "F" "G" "H" "I" "J" "K" "L" "Ł" "M" "N" "Ń" "O" "Ó" "P" "Q" "R" "S" "Ś" "T" "U" "V" "W" "X" "Y" "Z" "Ź" "Ż" "0" "1" "2" "3" "4" "5" "6" "7" "8" "9" "." "," ":" "!" "µg/m³")

>alfabet.h
for str in ${array[@]}; do
  convert -font DejaVu-Sans -pointsize 25 label:$str tmp.xbm
  cat tmp.xbm >> alfabet.h
done


