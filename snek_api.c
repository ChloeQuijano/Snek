/** snake API for C **/
// need to add the elongating of snek after nomming
#include "snek_api.h"
#include <string.h>
#include <time.h>

int CURR_FRAME = 0;
int SCORE = 0;
int MOOGLE_FLAG = 0;
int MOOGLES_EATEN = 0;
int TIME_OUT = ((BOARD_SIZE * 4) - 4) * CYCLE_ALLOWANCE;

GameBoard* init_board(){
	srand(time(0));
	GameBoard* gameBoard = (GameBoard*)(malloc(sizeof(GameBoard))); // board size 10x10 arrays

	// set every cell as empty on the game board
	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			gameBoard->cell_value[i][j] = 0;
			gameBoard->occupancy[i][j] = 0;
		}
	}
	gameBoard->occupancy[0][0] = 1; //snake initialized at the start (0,0)
	gameBoard->snek = init_snek(0, 0); // // add the snake body on the board
	return gameBoard; // return final gameboard with the snake on it
}

Snek* init_snek(int a, int b){
	Snek* snek = (Snek *)(malloc(sizeof(Snek))); // set structure for snake

	snek->head = (SnekBlock *)malloc(sizeof(SnekBlock)); // set structure for head
	// coordinates for the head of the snake
	snek->head->coord[x] = a;
	snek->head->coord[y] = b;

	snek->tail = (SnekBlock *)malloc(sizeof(SnekBlock));
	snek->tail->coord[x] = a;
	snek->tail->coord[y] = b;

	// add next to the length between the tail
	snek->tail->next = NULL; // adding nothing after the tail
	snek->head->next = snek->tail; // adding to the head through next
	
	// initiate length of the snake
	snek->length = 1;

	return snek; 
}

// ALGORITHM



char isSafe(GameBoard* board, Point path[], int position, GameBoard* checked, int y_coord, int x_coord) {
	printf("checking if safe called\n");
    if (board->occupancy[y_coord][x_coord] == 1 || checked->occupancy[y_coord][x_coord] == 1) {
        return 0;
    }
	for (int i = 0; i < position; i++ ) {
		if (path[i].y_coord == y_coord && path[i].x_coord == x_coord) {
			return 0;
		}
	}
    return 1;
}

// check if the snake hits the edge of the board
int hits_edge(int axis, int direction, GameBoard* gameBoard){
	// Replaced gameBoard->snek->head[y] with gameBoard->snek->head->coord[y]
	if (((axis == AXIS_Y) && ((direction == UP && gameBoard->snek->head->coord[y] + UP < 0) || (direction == DOWN && gameBoard->snek->head->coord[y] + DOWN > BOARD_SIZE - 1)))
	   || (axis == AXIS_X && ((direction == LEFT && gameBoard->snek->head->coord[x] + LEFT < 0) || (direction == RIGHT && gameBoard->snek->head->coord[x] + RIGHT > BOARD_SIZE-1))))
	{ // return if the coord of the head hits the edge
		return 1;
	} else { // if it doesnt hit the edge
		return 0;
	}

}

 // check if the snake hits itself
int hits_self(int axis, int direction, GameBoard *gameBoard){
	int new_x, new_y;
	if (axis == AXIS_X){
		new_x = gameBoard->snek->head->coord[x] + direction; // projected coord of direction + existing
		new_y = gameBoard->snek->head->coord[y];
	} else if (axis == AXIS_Y){
		new_x = gameBoard->snek->head->coord[x];
		new_y = gameBoard->snek->head->coord[y] + direction;
	}
	return gameBoard->occupancy[new_y][new_x]; //1 if occupied
}

// if moogle still exists 
int time_out(){
	return (MOOGLE_FLAG == 1 && CURR_FRAME > TIME_OUT);

}

// all the cases for the game to fail 
int is_failure_state(int axis, int direction, GameBoard *gameBoard){
	return (hits_self(axis, direction, gameBoard) || hits_edge(axis, direction, gameBoard) || time_out());
}

// adds a moogle on the board if there is no moogle anymore
void populate_moogles(GameBoard *gameBoard){
	if (MOOGLE_FLAG == 0){
		int r1 = rand() % BOARD_SIZE;
		int r2 = rand() % BOARD_SIZE;
		
		int r3 = rand() % (BOARD_SIZE * 10);
		if (r3 == 0){
			gameBoard->cell_value[r1][r2] = MOOGLE_POINT * HARRY_MULTIPLIER;
			MOOGLE_FLAG = 1;
		} else if (r3 < BOARD_SIZE){
			gameBoard->cell_value[r1][r2] = MOOGLE_POINT;
			MOOGLE_FLAG = 1;
		}
	}
}

// when the snake lands on the moogle based on the head of the snake
void eat_moogle(GameBoard* gameBoard, int head_x, int head_y) {
	SCORE = SCORE + gameBoard->cell_value[head_y][head_x]; // update the score based on the value on the board
	gameBoard->cell_value[head_y][head_x] = 0; // update the place the moogle is at by 0

	gameBoard->snek->length ++; // increase length by 1
	MOOGLES_EATEN ++;
	MOOGLE_FLAG = 0;
	CURR_FRAME = 0;
}

// the advance frame of the input direction by the user 
int advance_frame(int axis, int direction, GameBoard *gameBoard){
	// when the board fails based on the direction and existing gameboard
	if (is_failure_state(axis, direction, gameBoard)){
		return 0;

	// when it doesn't fail so it continues to move into the next frame
	} else {

		// update the occupancy grid and the snake coordinates
		int head_x, head_y;

		// figure out where the head should now be
		if (axis == AXIS_X) {
			head_x = gameBoard->snek->head->coord[x] + direction;
			head_y = gameBoard->snek->head->coord[y];
		} else if (axis == AXIS_Y){
			head_x = gameBoard->snek->head->coord[x];
			head_y = gameBoard->snek->head->coord[y] + direction;
		}
		
		int tail_x = gameBoard->snek->tail->coord[x];
		int tail_y = gameBoard->snek->tail->coord[y];

		// update the occupancy grid for the head
		gameBoard->occupancy[head_y][head_x] = 1;

		if (gameBoard->snek->length > 1) { //make new head
			SnekBlock *newBlock = (SnekBlock *)malloc(sizeof(SnekBlock));
			newBlock->coord[x] = gameBoard->snek->head->coord[x];
			newBlock->coord[y] = gameBoard->snek->head->coord[y];
			newBlock->next = gameBoard->snek->head->next;

			gameBoard->snek->head->coord[x] = head_x;
			gameBoard->snek->head->coord[y] = head_y;
			gameBoard->snek->head->next = newBlock;
	
			if (gameBoard->cell_value[head_y][head_x] > 0){  //eat something
				eat_moogle(gameBoard, head_x, head_y);
			} else { //did not eat
				//delete the tail
				gameBoard->occupancy[tail_y][tail_x] = 0;
				SnekBlock *currBlock = gameBoard->snek->head;
				while (currBlock->next != gameBoard->snek->tail){
					currBlock = currBlock->next;
				} //currBlock->next points to tail

				currBlock->next = NULL;
				free(gameBoard->snek->tail);
				gameBoard->snek->tail = currBlock;
			}

		} else if ((gameBoard->snek->length == 1) && gameBoard->cell_value[head_y][head_x] == 0){ // change both head and tail coords, head is tail
			gameBoard->occupancy[tail_y][tail_x] = 0;
			gameBoard->snek->head->coord[x] = head_x;
			gameBoard->snek->head->coord[y] = head_y;
			gameBoard->snek->tail->coord[x] = head_x;
			gameBoard->snek->tail->coord[y] = head_y;
			
		} else { //snake is length 1 and eats something
			eat_moogle(gameBoard, head_x, head_y);
			gameBoard->snek->head->coord[x] = head_x;
			gameBoard->snek->head->coord[y] = head_y;
		}

		// update the score and board
		SCORE = SCORE + LIFE_SCORE;
		if (MOOGLE_FLAG == 1){
			CURR_FRAME ++;
		}

		// populate moogles
		populate_moogles(gameBoard);
		return 1;
	} 
}

void show_board(GameBoard* gameBoard) {
	fprintf(stdout, "\033[2J"); // clear terminal ANSI code
	fprintf(stdout, "\033[0;0H"); // reset cursor position
	
	char blank = 	43;
	char snek = 	83;
	char moogle = 	88;

	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			if (gameBoard->occupancy[i][j] == 1){
				//snake is here
				fprintf(stdout, "%c", snek);
			} else if (gameBoard->cell_value[i][j] > 0) {
				//there be a moogle
				fprintf(stdout, "%c", moogle);
			} else {
				//nothing to see here
				fprintf(stdout, "%c", blank);
			}
		} //new line
		fprintf(stdout, "\n");
		
	}

	fprintf(stdout, "\n\n");
	
	if (MOOGLE_FLAG == 1){
		fprintf(stdout, "!..ALERT, MOOGLE IN VICINITY..!\n\n");
	}
	fprintf(stdout, "SCORE: %d\n", SCORE);
	fprintf(stdout, "YOU HAVE EATEN %d MOOGLES\n\n", MOOGLES_EATEN);

	fprintf(stdout, "SNEK HEAD\t(%d, %d)\n", gameBoard->snek->head->coord[x], gameBoard->snek->head->coord[y]);
	fprintf(stdout, "SNEK TAIL\t(%d, %d)\n", gameBoard->snek->tail->coord[x], gameBoard->snek->tail->coord[y]);
	fprintf(stdout, "LENGTH \t%d\n", gameBoard->snek->length);
	fprintf(stdout, "CURR FRAME %d vs TIME OUT %d\n", CURR_FRAME, TIME_OUT);


	fflush(stdout);
}


int get_score() {
	return SCORE;
}


void end_game(GameBoard **board){
	//fprintf(stdout, "\033[2J");
	//fprintf(stdout, "\033[0;0H"); 
	fprintf(stdout, "\n\n\n--!!---GAME OVER---!!--\n\nYour score: %d\n\n\n\n", SCORE);
	fflush(stdout);
	// need to free all allocated memory
	// first snek
	SnekBlock **snekHead = &((*board)->snek->head);
	SnekBlock *curr;
	SnekBlock *prev;
	while ((*snekHead)->next != NULL) {
		curr = *snekHead;
		while (curr->next != NULL){
			prev = curr;
			curr = curr->next;
		}
		prev->next = NULL;
		free(curr);
	}
	free(*snekHead);
	free((*board)->snek);
	free(*board);
}






