/* Jgraph Chessboard Generation Program */
/* Author: Evan Ezell */

#include <stdio.h>
#include <stdlib.h>

int is_valid(char c) {
    switch (c) {
        case 'p':
            return 1;
        case 'n':
            return 1;
        case 'b':
            return 1;
        case 'r':
            return 1;
        case 'q':
            return 1;
        case 'k':
            return 1;
        case 'P':
            return 1;
        case 'N':
            return 1;
        case 'B':
            return 1;
        case 'R':
            return 1;
        case 'Q':
            return 1;
        case 'K':
            return 1;
        case '.':
            return 1;
        default:
            return 0;
    }
}

void print_board(char board[8][8]) {
    int rank, file;

    printf("    ----------------\n");
    for (rank = 7; rank >= 0; rank--) {
        printf("%3d|", rank + 1);
        for (file = 0; file <= 7; file++) {
            printf("%2c",board[rank][file]);
        }
        printf("\n");
    }
    printf("    ----------------\n");
    printf("     A B C D E F G H\n");
}

void print_jgraph_board() {
    printf("(* chess board *)\n");
    printf("newgraph\n\n");

    printf("xaxis min 0 max 80 hash 10 mhash 0\n");
    printf("grid_lines no_draw_hash_marks no_auto_hash_labels\n");
    printf("    hash_label at 5  : A\n");
    printf("    hash_label at 15 : B\n");
    printf("    hash_label at 25 : C\n");
    printf("    hash_label at 35 : D\n");
    printf("    hash_label at 45 : E\n");
    printf("    hash_label at 55 : F\n");
    printf("    hash_label at 65 : G\n");
    printf("    hash_label at 75 : H\n");
    printf("\n");
    printf("yaxis min 0 max 80 hash 10 mhash 0 \n");
    printf("grid_lines no_draw_hash_marks no_auto_hash_labels\n");
    printf("    hash_label at 5  : 1\n");
    printf("    hash_label at 15 : 2\n");
    printf("    hash_label at 25 : 3\n");
    printf("    hash_label at 35 : 4\n");
    printf("    hash_label at 45 : 5\n");
    printf("    hash_label at 55 : 6\n");
    printf("    hash_label at 65 : 7\n");
    printf("    hash_label at 75 : 8\n");
    printf("\n");
    printf("(* print dark square background *)\n");
    printf("shell : echo \"\" | awk '{ \\\n");
    printf("    for (rank = 0; rank < 8; rank++) { \\\n");
    printf("        if (rank %% 2 == 0) file = 0; \\\n");
    printf("        else file = 1; \\\n");
    printf("        for (; file < 8; file += 2) { \\\n");
    printf("            printf(\"newline poly ppattern estripe 45 pfill .8 pts \");\\\n");
    printf("            printf(\"%%d %%d %%d %%d %%d %%d %%d %%d\\n\",\\\n");
    printf("                file * 10, rank * 10, \\\n");
    printf("                file * 10 + 10, rank * 10, \\\n");
    printf("                file * 10 + 10, rank * 10 + 10, \\\n");
    printf("                file * 10, rank * 10 + 10); \\\n");
    printf("        } \\\n");
    printf("    } \\\n");
    printf("}'\n\n");
}

void print_jgraph_pieces(char board[8][8]) {
    int rank, file;

    for (rank = 7; rank >= 0; rank--)
        for (file = 0; file <= 7; file++)
            if (board[rank][file] != '.') {
                printf("newcurve eps ps_pieces/");
                printf("%c.ps marksize 8 8 pts %d %d\n",
                        board[rank][file],
                        file * 10 + 5,
                        rank * 10 + 5);
            }
}

int main () {
    char c, board[8][8];
    int rank, file;

    for (rank = 7; rank >= 0; rank--) {
        for (file = 0; file <= 7; file++) {
            c = getchar();
            if(is_valid(c)) {
                board[rank][file] = c;
            } else {
                fprintf(stderr, "Invalid piece.\n");
                exit(1);
            }
        }
        c = getchar();
        if (c != '\n') {
            fprintf(stderr, "Too many pieces on rank.\n");
            exit(1);
        }
    }

    print_jgraph_board();
    print_jgraph_pieces(board);

    return 0;
}
