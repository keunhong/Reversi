/*
 * AIPlayer.cpp
 *
 *  Created on: 10-Dec-2008
 *      Author: 1parkk
 */

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include "Gameboard.h"
#include "AIPlayer.h"

/*
 * AIPlayer()
 *  Constructor for AIPlayer
 */
AIPlayer::AIPlayer(Reversi &game, int player, bool dump_info) {
	this->game = &game;
	this->corner_value = 3;
	this->player = player;
	return;
}

/*
 * rate_move()
 *  rates how good a move is
 *
 */
int AIPlayer::rate_move(int x, int y) const{
	int eval = 0;
	// return if coordinates are off the board
	if(x >= this->game->get_size() || y >= this->game->get_size() || x < 0 || y < 0){ return 0; }
	// return if slot is not empty
	if(this->game->get_slot_status(x,y) != 0){ return 0; }
	if(this->game->is_valid_move(x,y,this->player) == 0){ return 0; };

	int temp = this->game->flip_count(x,y,this->player);
	if((x == 0) || (x == (this->game->get_size() - 1))){
		temp += this->corner_value;
	}
	if((y == 0) || (y == (this->game->get_size() - 1))){
		temp += this->corner_value;
	}
	// if the move gives the opponent the corners
	// it is a bad move, decrement
	if((x == 1) || (x == (this->game->get_size() - 2))){
		temp -= this->corner_value;
	}
	if((y == 1) || (y == (this->game->get_size() - 2))){
		temp -= this->corner_value;
	}
	if(temp > eval){
		eval = temp;
	}

	if(eval < 1){ eval = 1; }

	// DEBUG: printf("EVAL = %d\n",eval);

	return eval;
}

/*
 * play()
 *  starts playing
 *
 */
void AIPlayer::play(){
	int x = 0,y = 0, cur_max = 0, best_x = 0, best_y = 0, temp = 0, rand_n = 0;
	srand(time(NULL));
	if(this->dump_info == true){
		printf("[<<] AI (Player %d) is thinking...\n",this->player);
	}
	for(x = 0; x < this->game->get_size(); x++){
		for(y = 0; y < this->game->get_size(); y++){
			temp = rate_move(x,y);
			if(temp > cur_max){
				cur_max = temp;
				best_x = x;
				best_y = y;
			}else if(temp == cur_max){
				rand_n = rand() % 2;
				// DEBUG: printf("\n\nRAND %d\n\n",rand_n);
				if(rand_n  == 1){
					best_x = x;
					best_y = y;
				}
			}
		}
	}
	if(this->dump_info == true){
		printf("  [<<] AI (Player %d) is making its move at (%d,%d)...\n\n",this->player,best_x + 1,best_y + 1);
	}
	this->game->make_move(best_x,best_y,this->player);
}

/*
 * set_player()
 *  sets player
 *
 */
void AIPlayer::set_player(int player){ this->player = player; }

AIPlayer::~AIPlayer() {
	// TODO Auto-generated destructor stub
}
