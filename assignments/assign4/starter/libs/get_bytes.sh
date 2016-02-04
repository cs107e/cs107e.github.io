printf -v t '%08x' "$1"
l=${#t}
hex_le="\\x${t:l-2:2}\\x${t:l-4:2}\\x${t:l-6:2}\\x${t:l-8:2}"
printf "$hex_le"
