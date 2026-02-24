# Ludo

## Overview

## Usage

- clone this repository
- run ./rolls.sh -n 100 > rolls.txt to generate random rolls, where -n argument passes the number of dice rolls in the game
- run ./Ludo rolls.txt > points.txt to compute the final locations of pawns after processing the number of rolls 
- run cat LudoBoard.jgr > Ludo.jgr to initialize a fresh new board 
- run cat points.txt >> Ludo.jgr to append point plotting code to the blank ludo board
- run < compile LudoBoard.jgr >
- run xdg-open Ludo.jpg to view board

