/*
 * AIPlayer.h
 *
 *  Created on: 10-Dec-2008
 *      Author: 1parkk
 */

#ifndef AIPLAYER_H_
#define AIPLAYER_H_

class AIPlayer {
	int player;
	int corner_value;
	bool dump_info;
	Reversi * game;

	public:
		AIPlayer(Reversi &game, int player = 1, bool dump_info = true);
		int rate_move(int x, int y) const;
		void set_player(int player);
		void play();
		virtual ~AIPlayer();
};

#endif /* AIPLAYER_H_ */
