#include "snek_api.h"
#include <unistd.h>

void play_game() {
	printf("starting\n");

	//board initialized, struct has pointer to snek
	GameBoard* board = init_board(); // calls an empty board
	show_board(board); // builds the board into the terminal

	// CHANGING THESE - THESE ARE JUSt INITIALS
	int axis = AXIS_INIT;
	int direction = DIR_INIT;
	
	int play_on = 1; // if true it will continue moving
	int coord[2];
	
	while (play_on){

		coord[x] = board->snek->head->coord[x]; // get new x cord
		coord[y] = board->snek->head->coord[y]; // get new y cord

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
