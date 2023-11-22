#include "universe.h"

#include <fcntl.h>
#include <inttypes.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS ":i:n:o:tsh"
#define DELAY   50000

// defining universe
struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

// swap function taken from asgn3 stats.h
void swap(Universe *A, Universe *B) {
    Universe t = *A;
    *A = *B;
    *B = t;
}

int main(int argc, char **argv) {
    // defining all variables
    int opt = 0;
    bool toro = false;
    bool ncur = true;
    uint32_t length = 100;
    FILE *in_file = stdin;
    ;
    FILE *out_file = stdout;
    // these exist for error message in regards to invalid file
    char *name_i;
    char *name_o;

    // reads the options selected by the user
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't': toro = true; break;
        case 's': ncur = false; break;
        case 'i':
            in_file = fopen(optarg, "r");
            name_i = optarg;
            break;
        case 'n': length = atoi(optarg); break;
        case 'o':
            out_file = fopen(optarg, "w");
            name_o = optarg;
            break;
        case 'h':
            printf("SYNOPSIS\n");
            printf("    Conway's Game of Life\n\n");
            printf("USAGE\n");
            printf("    ./life tsn:i:o:h\n\n");
            printf("OPTIONS\n");
            printf("    -t             Create your universe as a toroidal\n");
            printf("    -s             Silent - do not use animate the evolution\n");
            printf("    -n {number}    Number of generations [default: 100]\n");
            printf("    -i {file}      Input file [default: stdin]\n");
            printf("    -o {file}      Output file [default: stdout]\n");
            return 0;
        default:
            printf("SYNOPSIS\n");
            printf("    Conway's Game of Life\n\n");
            printf("USAGE\n");
            printf("    ./life tsn:i:o:h\n\n");
            printf("OPTIONS\n");
            printf("    -t             Create your universe as a toroidal\n");
            printf("    -s             Silent - do not use animate the evolution\n");
            printf("    -n {number}    Number of generations [default: 100]\n");
            printf("    -i {file}      Input file [default: stdin]\n");
            printf("    -o {file}      Output file [default: stdout]\n");
            return 0;
        }
    }

    // checks if the files are valid
    if (!in_file) {
        printf("Error in opening %s.\n", name_i);
        exit(0);
    }
    if (!out_file) {
        printf("Error in opening %s.\n", name_o);
        exit(0);
    }

    // get row and column from file
    uint32_t row = 0;
    uint32_t col = 0;
    fscanf(in_file, "%" PRIu32 " %" PRIu32 " ", &row, &col);

    // create two universes
    Universe *A = uv_create(row, col, toro);
    Universe *B = uv_create(row, col, toro);

    // populate universe A
    uv_populate(A, in_file);

    // initalize ncurses if not silent
    if (ncur) {
        initscr(); // Initialize the screen .
        curs_set(FALSE); // Hide the cursor .
    }
    // now create generations for universe a and store in universe b
    for (uint32_t gen = 0; gen < length; gen++) {
        // if ncurses isn't silenced
        if (ncur) {
            clear(); // Clear the window .
            for (uint32_t r = 0; r < uv_rows(A); r++) {
                for (uint32_t c = 0; c < uv_cols(A); c++) {
                    if (uv_get_cell(A, r, c) == true) {
                        printw("o");
                    } else {
                        printw(" ");
                    }
                }
                printw("\n");
            }
            refresh(); // Refresh the window .
            usleep(DELAY);
        }
        // chose which cells live or die based on census of each cell in a and save
        // it in b
        for (uint32_t r = 0; r < uv_rows(A); r++) {
            for (uint32_t c = 0; c < uv_cols(A); c++) {
                if (uv_get_cell(A, r, c) == true) {
                    if (uv_census(A, r, c) == 2 || uv_census(A, r, c) == 3) {
                        uv_live_cell(B, r, c);
                    } else {
                        uv_dead_cell(B, r, c);
                    }
                } else {
                    if (uv_census(A, r, c) == 3) {
                        uv_live_cell(B, r, c);
                    } else {
                        uv_dead_cell(B, r, c);
                    }
                }
            }
        }
        // swap universe a with b
        swap(A, B);
    }
    // closes ncurses if its true
    if (ncur) {
        endwin();
    }
    // prints the final generation
    uv_print(A, out_file);
    // close files ande clear space
    fclose(in_file);
    fclose(out_file);
    uv_delete(A);
    uv_delete(B);
    return 0;
}
