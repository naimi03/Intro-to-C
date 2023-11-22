#include "universe.h"

#include <stdlib.h>

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

// create universe based on supplied 2d matrix initialization given in pdf
Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *uv = (Universe *) malloc(sizeof(Universe));
    uv->rows = rows;
    uv->cols = cols;
    uv->toroidal = toroidal;
    uv->grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t r = 0; r < rows; r++) {
        uv->grid[r] = (bool *) calloc(cols, sizeof(bool));
    }
    return uv;
}

// free up each column per row, followed by the rows, followed by the universe
void uv_delete(Universe *u) {
    if (u) {
        for (uint32_t r = 0; r < u->rows; r++) {
            free(u->grid[r]);
            u->grid[r] = NULL;
        }
        free(u->grid);
        u->grid = NULL;
        free(u);
        u = NULL;
    }
}

// return the number of rows
uint32_t uv_rows(Universe *u) {
    return u->rows;
}

// return the number of columns
uint32_t uv_cols(Universe *u) {
    return u->cols;
}

// if the cell is within boundaries of the universe, set that cell as alive
void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r < uv_rows(u) && c < uv_cols(u)) {
        u->grid[r][c] = true;
    }
}

// if the cell is within boundaries of the universe, set that cell as dead
void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r < uv_rows(u) && c < uv_cols(u)) {
        u->grid[r][c] = false;
    }
}

// if the cell is within boundaries of the universe, return the state of the
// cell
bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r < uv_rows(u) && c < uv_cols(u)) {
        return u->grid[r][c];
    }
    return false;
}

// return the number of live cells surrounding it.
uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t count = 0;
    uint32_t x = r - 1;
    uint32_t y = c - 1;
    // when its not toroidal
    if (!u->toroidal) {
        for (int a = 0; a < 3; a++) {
            for (int b = 0; b < 3; b++) {
                // this condition makes sure we aren't looking at the point itself
                if (!((x == r) && (y == c))) {
                    // this condition says that as long as x isn't out of range or y isn't
                    // out of range check the cell
                    if (!(x >= uv_rows(u)) || !(y >= uv_cols(u))) {
                        // if cell is alive, count increases
                        if (uv_get_cell(u, x, y)) {
                            count++;
                        }
                    }
                }
                // if cell is greater than number of cols, it means its -1 and thus
                // starts at 0. if we do ++, then it will still be out of bounds
                if (y > uv_cols(u)) {
                    y = 0;
                } else {
                    y += 1;
                }
            }
            y = c - 1;
            // same reason as y if block
            if (x > uv_rows(u)) {
                x = 0;
            } else {
                x += 1;
            }
        }

    } else {
        for (int a = 0; a < 3; a++) {
            for (int b = 0; b < 3; b++) {
                // if x is at -1, which means its value is greate than number of rows,
                // set x to end of grid
                if (x > uv_rows(u)) {
                    x = uv_rows(u) - 1;
                    // if x is moving past the end of grid, moe it back to the start.
                } else if (x == uv_rows(u)) {
                    x = 0;
                }
                // this y if-else block follows the same logic as above
                if (y > uv_cols(u)) {
                    y = uv_cols(u) - 1;
                } else if (y == uv_cols(u)) {
                    y = 0;
                }
                // make sure the cell isn't the cell we are checking for and increment
                // if cell is alive
                if (!((x == r) && (y == c))) {
                    if (uv_get_cell(u, x, y)) {
                        count++;
                    }
                }
                y++;
            }
            y = c - 1;
            x++;
        }
    }
    return count;
}

// reads from file which cells start as alive
bool uv_populate(Universe *u, FILE *infile) {
    uint32_t r;
    uint32_t c;
    // read while file isnt at end of file
    while (!feof(infile)) {
        // read the file and stroe values into r and c
        fscanf(infile, "%u %u", &r, &c);
        // if the cell is valid, plot it as alive
        if (r < uv_rows(u) && c < uv_cols(u)) {
            uv_live_cell(u, r, c);
        }
    }
    return false;
}

// if write out each element into outfile and moves to new line accordingly
void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t r = 0; r < uv_rows(u); r++) {
        for (uint32_t c = 0; c < uv_cols(u); c++) {
            if (uv_get_cell(u, r, c) == true) {
                fprintf(outfile, "%c", 'o');
            } else {
                fprintf(outfile, "%c", '.');
            }
        }
        fprintf(outfile, "\n");
    }
}

// below is part of my process for testing universe.c to check each function
// before using them in life.c. Also, my logic for the rules from below has
// changed in my implementation of it in life.c

/*
int main(){
    Universe *uv = uv_create(13,13,false);
    Universe *dup = uv_create(13,13,false);
    FILE *ouf;
    FILE *inf;
    ouf = fopen("outf.txt", "w");
    inf = fopen("inf.txt", "r");
    for(uint32_t r = 0; r < uv_rows(uv); r++){
        for(uint32_t c = 0; c < uv_cols(uv); c++){
            uv_dead_cell(uv, r, c);
        }
    }
    for(uint32_t r = 0; r < uv_rows(dup); r++){
        for(uint32_t c = 0; c < uv_cols(dup); c++){
            uv_dead_cell(dup, r, c);
        }
    }
    uv_populate(uv, inf);
    uv_print(uv, ouf);

    fprintf(ouf, "\n now \n\n");
    for(uint32_t r = 0; r < uv_rows(uv); r++){
        for(uint32_t c = 0; c < uv_cols(uv); c++){
            if(uv_get_cell(uv,r,c) == true){
                printf("cell at %u, %u = o with census = %u\n", r,
c,uv_census(uv,r,c)); }else{ printf("cell at %u, %u = . with census = %u\n", r,
c, uv_census(uv,r,c));
            }
            if(uv_census(uv,r,c) < 2){
                printf("dead\n");
                uv_dead_cell(dup,r,c);
            }else if(uv_census(uv,r,c) > 3){
                uv_dead_cell(dup,r,c);
                printf("dead\n");
            }else{
                if(uv_get_cell(uv,r,c) == false && uv_census(uv,r,c) == 3){
                    uv_live_cell(dup,r,c);
                    printf("live\n");
                }else if(uv_get_cell(uv,r,c) == true){
                    uv_live_cell(dup,r,c);
                    printf("live\n");
                }
            }
        }
    }

    uv_print(dup, ouf);
    fclose(inf);
    fclose(ouf);
    uv_delete(uv);
    return 0;
}*/
