#!/bin/bash
a=( *.h *.cpp )
for file in "${a[@]}"; do
    tmp="tmp"
    clang-format -style=file "$file" > "$file$tmp"
    mv "$file$tmp" "$file"
done