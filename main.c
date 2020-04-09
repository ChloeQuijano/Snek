#include "snek_api.h"
#include <unistd.h>

#define INFINITY 9999
#define initial 1
#define waiting 2
#define visited_def 3

int NODES = BOARD_SIZE*BOARD_SIZE;
int MATRIX[BOARD_SIZE*BOARD_SIZE][BOARD_SIZE*BOARD_SIZE];
int PATH[BOARD_SIZE*BOARD_SIZE];
int moogle_x, moogle_y, target, start;
int axis_list[BOARD_SIZE*BOARD_SIZE];
int dir_list[BOARD_SIZE*BOARD_SIZE];
int queue[NODES];
int front = -1;
int rear = -1;
int state[NODES];


void play_game() {
	printf("starting\n");
	//board initialized, struct has pointer to snek
	GameBoard* board = init_board();
	show_board(board);

	FILE *file;
	file = fopen("snake_result.txt", "w+");

	// starts at (0,0)
	int axis = AXIS_INIT; // y axis first
	int direction = DIR_INIT; // goes down first
	
	int play_on = 1;
	int coord[2];
	int count = 0;
	
	while (play_on){
		// axis = AXIS_Y or axis_X
		// direction = UP, DOWN, LEFT, RIGHT
		if (count > 0) {
			axis = axis_list[count-1];
			direction = dir_list[count-1];
		}

		coord[x] = board->snek->head->coord[x];
		coord[y] = board->snek->head->coord[y];
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
		count++;
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
		
		if (MOOGLE_FLAG != 0) {
			call_algorithm(board);
		}
		// check if moogle appeared

		// check if the end of path and if so, you call the algorithm again and reset count
		usleep(500000);
	}
	fprintf(file, "$d\n", SCORE);
	fclose(file);
	end_game(&board);
}

void call_algorithm(GameBoard *board) {
	// start algorithm
	make_matrix(board); // get adjacency matrix
	moogle_location(board);
	target = (moogle_y*BOARD_SIZE) + (moogle_x+1);
	start = (board->snek->head->coord[y] * BOARD_SIZE) + (board->snek->head->coord[x]+1);
	hamCycle(board); // get path using Hamiltonian path
	dijkstra(start, target); // get path usin dijkstra's algorithm
	BF_traversal(); // get path using BFS 

	// change path to directions
	for (int i = 0; i < NODES-2; i++) {
		int curr = PATH[i];
		int next = PATH[i+1];
		if (curr == next-1) {
			// moves right
			axis_list[i] = AXIS_X;
			dir_list[i] = RIGHT;
		} else if (curr == next+1) {
			// moves left
			axis_list[i] = AXIS_X;
			dir_list[i] = LEFT;
		} else if (curr < next-1) {
			// moves down
			axis_list[i] = AXIS_Y;
			dir_list[i] = DOWN;
		} else if (curr > next+1) {
			// moves up
			axis_list[i] = AXIS_Y;
			dir_list[i] = UP;
		} else if (i > 0 && curr == 0) {
			// end of path
			break;
		}
	}
}

void make_matrix(GameBoard *board) {
    // make matrix here
    for (int y_inc = 0; y_inc < BOARD_SIZE; y_inc++) {
        for (int x_inc = 0; x_inc < BOARD_SIZE; x_inc++) {
            int i = y_inc*BOARD_SIZE + x_inc;
            if (x_inc > 0) {
                MATRIX[i-1][i] = 1;
                MATRIX[i][i-1] = 1;
                // check if occupany is full here
                if (board->occupancy[y_inc][x_inc] == 1) {
                    MATRIX[i-1][i] = 0;
                    MATRIX[i][i-1] = 0;
                }
            }
            if (y_inc > 0) {
                MATRIX[i-BOARD_SIZE][i] = 1;
                MATRIX[i][i-BOARD_SIZE] = 1;
                // check if occupancy is full
                if (board->occupancy[y_inc][x_inc] == 1) {
                    MATRIX[i-BOARD_SIZE][i] = 0;
                    MATRIX[i][i-BOARD_SIZE] = 0;
                }
            }
        }
    }
}

void moogle_location(GameBoard *board) {
	// loops through board to find the location of the current moogle
	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			if (board->cell_value[i][j] != 0) {
				moogle_x = j;
				moogle_y = i;
			}
		}
	}
}

// ==================== HAMILTONIAN CYCLE ALGORITHM ========================= //

int hamCycle(GameBoard* board) {
    int *path = new int[n];
    for (int i = 0; i < n; i++) {
        path[i] = -1;
    }

    // make point out of the current spot of the snake
    int start = (board->snek->head->coord[y] + 1) * (board->snek->head->coord[x] + 1);

    path[0] = start;
    if (hamCycleUtil(path, 1) == 0) {
        // no solution
        return 0;
    }
    return path;
}

int hamCycleUtil(int path[], int pos) {
    if (pos == n) {
        // check if already included
        if ( MATRIX[path[pos-1]][path[0]] == 1) {
            return 1;
        } else {
            return 0;
        }
    }

    for (int v = 1; v < n; v++) {
        if (isSafe(v, path, pos)) {
            path[pos] = v;

            // construct rest of path
            if (hamCycleUtil (path, pos+1) == 1) {
                return 1;
            }

            // if adding solution doesn't lead to a solution
            path[pos] = -1;
        }
    }
}

int isSafe(int v, int graph[n][n], int path[], int pos) {
    if (MATRIX[path[pos-1]][n] == 0) {
        return 0;
    }
    for (int i = 0; i < pos; i++) {
        if (path[i] == v) {
            return 0;
        }
    }
    return 1;
}

// ==================== DIJKSTRA's ALGORITHM ========================= //

void dijkstra(int start, int target) {
	int cost[NODES][NODES], distance[NODES], pred[NODES];
	int visited[NODES], count, min_distance, nextnode = 0;

	// create cost matrix from adjacency, setting cost as infinity so not to choose them
	for (int i = 0; i < NODES; i++) {
		for (int j = 0; j < NODES; j++) {
			if (MATRIX[i][j] == 0) {
				// setting all as infinity
				cost[i][j] = INFINITY;
			} else {
				cost[i][j] = MATRIX[i][j];
			}
		}
	}

	// initialize pred to start, distances and visited as empty
	for (int i = 0; i < NODES; i++) {
		distance[i] = cost[start][i];
		pred[i] = start;
		visited[i] = 0;
	}

	// we know the shortest distance so far is the start so we mark as visited and set distance to itself as 0
	distance[start] = 0;
	visited[start] = 1;
	count = 1;

	// 
	while(count < NODES-1) {
		min_distance = INFINITY;

		for (int i = 0; i < NODES; i++) {
			if (distance[i] < min_distance && !visited[i]) {
				min_distance = distance[i];
				nextnode = i;
			}

			// setting next as already visited
			visited[nextnode] = 1;

			// choosing next node by checking the one with minimum distance to source
			for(int j= 0; j < NODES; j++) {
				if (!visited[j]) {
					if (min_distance + cost[nextnode][i] < distance[i]) {
						distance[i] = min_distance + cost[nextnode][i];
						pred[i] = nextnode;
					}
				}
			}
			count++;
		}
	}
	PATH[0] = target;
	int size = 1;

	// filling path with predecessors
	for (int k = target; k != start; size++, k = pred[k]) {
		PATH[size] = pred[k];
	}

	// reverting pred path to make path from source to target
	for (int temp, end = size-1, i = 0; i< NODES/2; i++) {
		temp = PATH[i];
		PATH[i] = PATH[end];
		PATH[end] = temp;
		end--;
	}
}

// ==================== BREADTH FIRST SEARCH ALGORITHM ========================= //

void BF_traversal() {
	int v;
	
	for (v = 0; v < NODES; v++) {
		state[v]= INFINITY;
	}

	v = start;
	BFS(v);
}

void BFS(int v) {
	int i;

	insert_queue(v);
	state[v] = waiting;

	// checks if previously made queue is empty, while false it will remove it
	while (!isEmpty_queue()) {
		v = delete_queue();
		state[v] = visited_def;
	}

	// checks if there is an available node to enter
	for (i = 0; i < NODES; i++) {
		// looks if the node has connection and is marked as distance of INFINITY
		if (MATRIX[v][i] == 1 && state[i] == INFINITY) {
			// insert this free node
			insert_queue(i);
			state[i] = waiting;
		}
	}
}

void insert_queue(int vertex) {
	if (rear == NODES-1) {
		printf("queue overflown");
	} else {
		if (front == -1) {
			front = 0;
		}
		rear = rear + 1;
		queue[rear] = vertex;
	}
}

int isEmpty_queue() {
	if (front == -1 || front > rear) {
		return 1;
	} else {
		return 0;
	}
}

int delete_queue() {
	int delete_item;
	if (front = -1 || front > rear) {
		printf("queue overflown");
	}
	delete_item = queue[front];
	front = front+1;
	return delete_item;
}

// ==================== LONGEST PATH ALGORITHM ========================= //

int longest_path(GameBoard* board) {
   // longest path from the start to the moogle
   int distance[NODES];
   for (int i = 0; i < NODES; i++) {
	   distance[i] = INFINITY;
   }

	// initialize all starting 
	int queue[NODES];
   	queue[0] = start;
   	distance[0] = 0;

	// checks if there is an available node to enter
	for (i = 0; i < NODES; i++) {
		// looks if the node has connection and is marked as distance of INFINITY
		if (MATRIX[v][i] == 1 && distance[i] == INFINITY) {
			// insert this free node
			insert_queue(i);
			distance[i] = distance[0] + 1;
		}
	}

	int maxDistance = 0;
	// get farthest node distance to its target
	for (int i = 0; i < NODES; i++) {
		if (distance[i] > maxDistance) {
			maxDistance = distance[i]
		}
		if (i == target) {
			PATH[NODES] = distance[NODES];
			break;
		}
	}
}

void insert_queue(int vertex) {
	if (rear == NODES-1) {
		printf("queue overflown");
	} else {
		if (front == -1) {
			front = 0;
		}
		rear = rear + 1;
		queue[rear] = vertex;
	}
}

int isEmpty_queue() {
	if (front == -1 || front > rear) {
		return 1;
	} else {
		return 0;
	}
}

int delete_queue() {
	int delete_item;
	if (front = -1 || front > rear) {
		printf("queue overflown");
	}
	delete_item = queue[front];
	front = front+1;
	return delete_item;
}

int main(){
	play_game();
	return 0;
}
