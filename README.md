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

This gameplay is slightly modified as there is no capturing of other players pawns or skipped turns upon rolling three 6's in a row.
The final board image depicts the position of each player's 4 pawns on the ludo gameboard.

## Building and Running

1. Clone this repository
2. Run `./rolls.sh -n <num>` to generate a file of random rolls, where -n is a flag that allows you to pass the number of dice rolls into the game. The default is 20 rolls. This produces a file called rolls.txt, which contains all the randomly generated dice rolls
3. Run `./Ludo rolls.txt > points.txt` to compute the final locations of pawns after processing the number of rolls 
4. Run `cat LudoBoard.jgr > Ludo.jgr` to initialize a fresh new board 
5. Run `cat points.txt >> Ludo.jgr` to append point plotting code to the blank ludo board
6. Run `./jgraph -P Ludo.jgr | ps2pdf - | convert -density 300 - -quality 100 Ludo.jpg`
 7. Run `xdg-open Ludo.jpg` to view board


