#include "snek_api.h"
#include <unistd.h>
#include <math.h>


void play_game() {
	printf("starting\n");

	//board initialized, struct has pointer to snek
	GameBoard* board = init_board(); // calls an empty board

	// start the adjacency matrix 
	int graph[n][n] = make_matrix(board);

	// start algorithm
	int *path = hamCycle(graph, board);

	// CHANGING THESE - THESE ARE JUST INITIALS
	int axis = AXIS_INIT; // AXIS_Y or AXIS_X
	int direction = DIR_INIT; // UP, DOWN, LEFT or RIGHT

	// convert path into coordinate points
	// y coord = floor(i / BOARDSIZE) - 1
	// x coord = i % BOARDSIZE - 1
	int coord_x[BOARD_SIZE*BOARD_SIZE];
	int coord_y[BOARD_SIZE*BOARD_SIZE];
	int size = sizeof(path)/sizeof(path[0]);
	for (int i = 0; i <= size; i++) {
		coord_x[i] = path[i] % BOARD_SIZE - 1;
		coord_y[i] = floor(path[i] / BOARD_SIZE) - 1;
	}

	int play_on = 1; // if true it will continue moving
	int count = 1;
	
	while (play_on){


		board->snek->head->coord[x] = coord_x[count];
		board->snek->head->coord[y] = coord_y[count];

		coord[x] = board->snek->head->coord[x]; // get new x cord
		coord[y] = board->snek->head->coord[y]; // get new y cord

		count++;

		// will return true when the input direction and axis is true
		unsigned short go_x = (axis == AXIS_Y && direction == 1 && coord[y] == (BOARD_SIZE - 1)) || (axis == AXIS_Y && direction == -1 && coord[y] == 0);

		unsigned short go_y = (axis == AXIS_X && direction == 1 && coord[x] == (BOARD_SIZE - 1)) || (axis == AXIS_X && direction == -1 && coord[x] == 0);

		if (go_x) {
			// going to hit the bottom or top
			// go right or left
			axis = AXIS_X;
			if (coord[x] < (int)(BOARD_SIZE / 2)){ 
				direction = RIGHT;
			} else {
				direction = LEFT;
			}

		} else if (go_y) {
			axis = 	AXIS_Y;
			if (coord[y] < (int)(BOARD_SIZE / 2)){ 
				direction = DOWN;
			} else {
				direction = UP;
			}
		}
		show_board(board);
		play_on = advance_frame(axis, direction, board);
		printf("Going ");

		if (axis == AXIS_X){
			if (direction == RIGHT){
				printf("RIGHT");
			} else {
				printf("LEFT");
			}
		} else {
			if (direction == UP){
				printf("UP");
			} else {
				printf("DOWN");
			}
		} printf("\n");
		usleep(555550);
	}
	end_game(&board);

}

int main(){
	play_game();
	return 0;
}
