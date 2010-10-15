/*
 * Gameboard.cpp
 *
 *  Created on: 08-Dec-2008
 *      Author: 1parkk
 */

#include <iostream>
#include "Gameboard.h"

// if(x >= this->size || y >= this->size || x < 0 || y < 0){ return 0; }
// the line above will be used in every function to make the program
// idiot-proof

/*
 * Reversi()
 *  Constructor for the game board.
 *  Takes size and initializes game board.
 *
 */
Reversi::Reversi(int size){
	if(size < 3) return;

	this->size = size;
	this->score[0] = 0;
	this->score[1] = 0;
	this->score[2] = 0;
	this->status = 1;
	// allocate memory for gameboard matrix
	this->gameboard = new int*[size];
	for(int z = 0; z <= size; z++){
		this->gameboard[z] = new int[size];
	}

	// initialize board by setting all slots to 0
	for(int x = 0; x <= this->size; x++){ // loop for all rows
		for(int y = 0; y <= this->size; y++){ //loop for all columns
			this->set_slot(x,y,0); // set the slot to 0
		}
	}

	// set initial pieces
	this->set_slot(size/2 - 1, size/2 - 1, 1);
	this->set_slot(size/2, size/2, 1);
	this->set_slot(size/2 - 1, size/2, 2);
	this->set_slot(size/2, size/2 - 1, 2);
}

/*
 * get_slot_status()
 *  Takes coordinate and return the status of the slot
 *
 */
int Reversi::get_slot_status(int x, int y) const{
	if(x >= this->size || y >= this->size || x < 0 || y < 0){ return 0; }
	return this->gameboard[x][y];
}

/*
 * get_size()
 *  returns size of gameboard
 *
 */
int Reversi::get_size() const{
	return this->size;
}

/*
 * is_valid_move()
 *  returns whether a move is valid
 *
 */
bool Reversi::is_valid_move(int x, int y, int player) const{
	if(x >= this->size || y >= this->size || x < 0 || y < 0){ return 0; }
	// check if slot is already taken.. obviously
	if(get_slot_status(x,y) != 0){ return false; }
	// if the number of stones that will be flipped is not zero
	// the move is a valid move, thus return true if this is the case
	// and return false otherwise
	return (this->flip_count(x,y,player) > 0)?true:false;
}

/*
 * set_slot()
 *  Returns the staus/ownership of a slot
 *
 */
void Reversi::set_slot(int x, int y, int player){
	if(x >= this->size || y >= this->size || x < 0 || y < 0){ return; }
	this->gameboard[x][y] = player;
	this->change_score(1,player);
}

/*
 * flip_count()
 *  returns how many flips a move will make
 *
 */
int Reversi::flip_count(int x, int y, int player) const{
	if(x >= this->size || y >= this->size || x < 0 || y < 0){ return 0; }
	// dir_x and dir_y are each -1, 0, or 1 to change the direction of the search
	// this way, every possible move is considered.
	//
	// dist is for the length of the search
	// dist = 1 searches one slot away, dist = 2 searches 2 slots away
	//
	// cur_x and cur_y are temporary variables to store the temporary
	// positions being searched while looping through the possibilities
	int dir_x, dir_y, dist = 1, cur_x = x, cur_y = y, flip_count = 0;

	// these nested for loops consider -1, 0, and 1 for dir_x and dir_y
	for(dir_x = -1; dir_x < 2; dir_x++){
		for(dir_y = -1; dir_y < 2; dir_y++){
			// set initial dist to 1 and increment only if the slot tested before
			// is the opponent's stone
			dist = 1;
			while(true){
				// dir_x and dir_y only set the direction (-1, 0, or 1)
				cur_x = x + (dist * dir_x);
				cur_y = y + (dist * dir_y);
				// break current loop if it goes off board
				/*//DEBUG!
				std::cout << "cur_x : " << cur_x << std::endl
						  << "cur_y : " << cur_y << std::endl
						  << "dist : " << dist << std::endl
						  << "dir_x : " << dir_x << std::endl
						  << "dir_y : " << dir_y << std::endl;
				//DEBUG! */
				if(cur_x < 0 || cur_y < 0 || cur_x >= this->size || cur_y >= this->size){
					break;
				}
				// break current loop if the slot is blank
				// since no slots after will be able to be taken anyway
				if(gameboard[cur_x][cur_y] == 0){ break; }
				// if the stone is the current player's stone
				// increment flip count by the distance minus one
				// since the slots preceding the current slot would be flipped
				if(gameboard[cur_x][cur_y] == player){ flip_count += dist - 1; break; }
				// break if (dir_x,dir_y) = (0,0) since it'll go into an infinite loop
				if(dir_x == 0 && dir_y == 0){ break; }
				dist++;
			}
		}
	}
	return flip_count;
}

/*
 * flip_slots()
 *  Actually flips the slots of according to a move
 *
 */
int Reversi::flip_slots(int x, int y, int player){
	if(x >= this->size || y >= this->size || x < 0 || y < 0){ return 0; }
	// see flip_count for comments..
	// it's the same thing except this one actually flips
	int dir_x, dir_y, dist = 1, cur_x = x, cur_y = y, flip_count = 0;
	for(dir_x = -1; dir_x < 2; dir_x++){
		for(dir_y = -1; dir_y < 2; dir_y++){
			dist = 1;
			while(true){
				cur_x = x + (dist * dir_x);
				cur_y = y + (dist * dir_y);
				if(cur_x < 0 || cur_y < 0 || cur_x >= this->size || cur_y >= this->size){
					break;
				}
				if(gameboard[cur_x][cur_y] == 0){ break; }
				if(gameboard[cur_x][cur_y] == player){
					//flip_count += dist - 1; // count just for the sake of it
					// backtrace moves and flip opponent's slots
					// decrement dist and flip until dist is 0
					for(dist--; dist > 0; dist--){
						cur_x = x + (dist * dir_x);
						cur_y = y + (dist * dir_y);
						this->set_slot(cur_x, cur_y, player); // flip slot
						this->change_score(-1,(player == 1)?2:1);
					}
					break;
				}
				// break if (dir_x,dir_y) = (0,0) since it'll go into an infinite loop
				if(dir_x == 0 && dir_y == 0){ break; }
				dist++;
			}
		}
	}
	return flip_count;
}

/*
 * make_move()
 *  makes the move based on a coordinate
 *
 */
bool Reversi::make_move(int x, int y, int player){
	if(x >= this->size || y >= this->size || x < 0 || y < 0){ return false; }
	if(this->is_valid_move(x,y,player) == true){
		this->set_slot(x,y,player);
		this->flip_slots(x,y,player);
		return true;
	}else{
		return false;
	}
}

/*
 * moves_exist()
 *  returns whether there are any possible moves for a player left
 *
 */
bool Reversi::moves_exist(int player) const{
	int x,y;
	for(x = 0; x < this->size; x++){
		for(y = 0; y < this->size; y++){
			if(get_slot_status(x,y) != 0){ continue; }
			else if(this->flip_count(x,y,player) > 0){
				return true;
			}
		}
	}
	return false;
}

/*
 * get_move()
 *  gets the user's input for a move
 *
 */
bool Reversi::get_move(int player){
	int x, y;
	while(true){
		printf("[>>] Player %d's turn: \n",player);
		printf("  [>>] Input x: ");

		// get x coordinate
		std::cin >> x;
		// sanitate input
		if(std::cin.fail()){
			std::cin.clear();
			std::string garbage;
			std::getline(std::cin, garbage);
			printf("    [!<<] Invalid input: x value must be an integer.\n");
		}else if(x > this->size || x < 1){
			std::cin.clear();
			printf("    [!<<] Invalid input: x value but be between 1 and %d.\n",this->size);
		}else{
			// get y coordinate
			printf("  [>>] Input y: ");
			std::cin >> y;
			// sanitate input
			if(std::cin.fail()){
				std::cin.clear();
				std::string garbage;
				std::getline(std::cin, garbage);
				printf("    [!<<] Invalid input: y value must be an integer.\n");
			}else if(y > this->size || y < 1){
				std::cin.clear();
				printf("    [!<<] Invalid input: y must be between 1 and %d.\n",this->size);
			}else{
				bool move = this->make_move(x-1,y-1,player);
				if(move == false){
					printf("    [!<<] Invalid move, try again.\n");
					continue;
				}else{
					return true;
				}
			}
		}
	}
}

/*
 * get_score()
 *  returns the score of a player
 *
 */
int Reversi::get_score(int player) const{
	return this->score[player];
}

/*
 * get_status()
 *  returns the current status of the game
 *
 */
bool Reversi::get_status() const{
	return this->status;
}


/*
 * change_status()
 *  toggles the status of the game
 *
 */
// only two possibilities
// therefore this function will only flip the current status of the game
// 0 for over, 1 for still in progress
void Reversi::change_status(void){
	this->status = (this->status == 0)?1:0;
}

/*
 * count_free_slots()
 *  returns the number of free slots on the game board
 *
 */
// counts the number of 0's on the board
int Reversi::count_free_slots() const{
	int x,y, count = 0;
	for(x = 0; x < this->size; x++){
		for(y = 0; y < this->size; y++){
			count += ((get_slot_status(x,y) == 0)?1:0);
		}
	}
	return count;
}

/*
 * count_player_slots()
 *  returns the number of slots owned by the player given
 *
 */
int Reversi::count_player_slots(int player) const{
	int x, y, count = 0;
	for(x = 0; x < this->size; x++){
		for(y = 0; y < this->size; y++){
			count += ((get_slot_status(x,y) == player)?1:0);
		}
	}
	return count;
}

/*
 * check_win()
 *  check whether a player has won
 *
 */
bool Reversi::check_win(int player){
	if(this->count_free_slots() == 0){
		this->change_status();
		return true;
	}else if(this->moves_exist((player == 1)?2:1) == 0){
		this->change_status();
		return true;
	}else{
		return false;
	}
}

/*
 * determine_winner()
 *  determine who has won the game.
 *
 */
int Reversi::determine_winner() const{
	int p1 = 0, p2 = 0;
	p1 = count_player_slots(1);
	p2 = count_player_slots(2);
	if(p1 == p2){ return 0; }
	else if(p1 > p2){ return 1; }
	else{ return 2; }
}

/*
 * change_score()
 *  change the score of a player
 *
 */
void Reversi::change_score(int change, int player){
	this->score[player] += change;
}

/*
 * print_board()
 *  dumps information about the current board
 *
 */
// this function prints the current board
void Reversi::print_board() const{
	printf("  [<<] Current board:\n");
	printf("\n\t");
	for(int z = 0; z < this->size; z++){
		printf("%d ",z+1);
	}
	printf("\n\n");
	for(int y = 0; y < this->size; y++){
		printf("   %d\t",y+1);
		for(int x = 0; x < this->size; x++){
			printf("%d ",get_slot_status(x,y));
		}
		printf("\n");
	}
	printf("\n");
}

/*
 * print_scores()
 *  prints scores
 *
 */
void Reversi::print_scores() const{
	printf("[<<] Current stats:\n");
	printf("  [<<] Player 1 score: %d\n",get_score(1));
	printf("  [<<] Player 2 score: %d\n\n",get_score(2));
}

Reversi::~Reversi() {
    delete [] this->gameboard;
}
