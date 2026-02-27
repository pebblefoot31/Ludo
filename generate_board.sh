#!/bin/bash

# Ensure your variables are set
CPP_FILE="Ludo.cpp"
EXE_FILE="Ludo"
ROLLS_FILE="rolls.txt"
POINTS_FILE="points.txt"
BOARD_FILE="LudoBoard.jgr"
OUTPUT_JGR="Ludo.jgr"

echo "Step 2: Compiling $CPP_FILE..."
g++ -std=c++11 "$CPP_FILE" -o "$EXE_FILE"

if [ $? -eq 0 ]; then
    echo "Step 3: Running simulation..."
    ./"$EXE_FILE" "$ROLLS_FILE" > "$POINTS_FILE"
    
    echo "Step 4: Creating Jgraph file ($OUTPUT_JGR)..."
    cat "$BOARD_FILE" > "$OUTPUT_JGR"
    cat "$POINTS_FILE" >> "$OUTPUT_JGR"
else
    echo "Error: Compilation failed."
fi
