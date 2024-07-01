#!/bin/bash

# Check if the C file is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <file.c> [args...]"
    exit 1
fi

# Extract the base name of the file (without the extension)
filename=$(basename -- "$1")
program="${filename%.*}"

# Compile the C file
gcc -o "$program" "$1"
if [ $? -ne 0 ]; then
    echo "Compilation failed"
    exit 1
fi

# Run the compiled program with Valgrind
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./"$program" "${@:2}"

# Remove the compiled program
rm -f "$program"
