# Ludo

---

## Overview

This program outputs the positions of Ludo pieces after processing inputted dice rolls. The rules of Ludo are simple and listed below:

Goal: Be the first player to bring all 4 of your pawns to 'home', which is your triangle in the center of the board

Gameplay:

1. Each player takes turns rolling a single die

In a turn:

- If you roll a 6 and have inactive pieces in your starting square, you 'activate' a piece, and roll again to move the piece that was activated
- Otherwise, if all pieces are active and on the gameboard, move the piece the number of spaces that you rolled
- In scenario outlined in step 3, roll again after rolling a 6 and move the pawn the number of spaces that is the sum of both rolls

This gameplay is slightly modified as there are no skipped turns upon rolling three 6's in a row.
The final board image depicts the position of each player's 4 pawns on the ludo gameboard.

## Building and Running

1. Clone this repository
2. Make sure to be one level up from the untarred /jgraph directory 
3. Run `make` to generate images corresponding to each of the data files in `input`
4. Run `xdg-open rolls1.jpg` to view the generated image of the board

## Notes
- There is a shell script in this repo called `gen_num.sh`. This script generates random numbers that simulate dice rolls. The output is in a file called rolls.txt
- Some of the file inputs may look unusual, and have numbers far larger than those on a 6-sided die. These kind of input files are just to create extreme boards for demonstration purposes.

---
