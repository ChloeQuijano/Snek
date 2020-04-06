#include "snek_api.h"
#include <unistd.h>
#include "main.c"
#include "snek_api.c"

// make adjacency grid first based on BOARDSIZE and occupancy
int make_matrix(GameBoard *board) {
    int n = BOARD_SIZE*BOARD_SIZE;
    int final[n][n];
    // make matrix here

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE) {
            int i = y*BOARD_SIZE + x;
            if (x > 0) {
                final[i-1][i] = 1;
                final[i][i-1] = 1;
                // check if occupany is full here
                if (board->occupancy[x][y] == 1 || board->cell_value[x][y] != 0) {
                    final[i-1][i] = 0;
                    final[i][i-1] = 0;
                }
            }
            if (y > 0) {
                final[i-BOARD_SIZE][i] = 1;
                final[i][i-BOARD_SIZE] = 1;
                // check if occupancy is full
                if (board->occupancy[x][y] == 1 || board->cell_value[x][y] != 0) {
                    final[i-BOARD_SIZE][i] = 0;
                    final[i][i-BOARD_SIZE] = 0;
                }
            }
        }
    }
    return final;
}


// start algorithm here with board

int hamCycle(int graph[n][n], GameBoard* board) {
    int *path = new int[n];
    for (int i = 0; i < n; i++) {
        path[i] = -1;
    }

    // make point out of the current spot of the snake
    int start = (board->snek->head->coord[y] + 1) * (board->snek->head->coord[x] + 1);

    path[0] = start;
    if (hamCycleUtil(graph, path, 1) == 0) {
        // no solution
        return 0;
    }
    return path;
}

int hamCycleUtil(int graph[n][n], int path[], int pos) {
    if (pos == n) {
        // check if already included
        if ( graph[path[pos-1]][path[0]] == 1) {
            return 1;
        } else {
            return 0;
        }
    }

    for (int v = 1; v < n; v++) {
        if (isSafe(v, graph, path, pos)) {
            path[pos] = v;

            // construct rest of path
            if (hamCycleUtil (graph, path, pos+1) == 1) {
                return 1;
            }

            // if adding solution doesn't lead to a solution
            path[pos] = -1;
        }
    }
}

int isSafe(int v, int graph[n][n], int path[], int pos) {
    if (graph[path[pos-1]][n] == 0) {
        return 0;
    }
    for (int i = 0; i < pos; i++) {
        if (path[i] == v) {
            return 0;
        }
    }
    return 1;
}

// convert final path to coordinate points