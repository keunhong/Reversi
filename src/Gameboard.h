/*
 * Gameboard.h
 *
 *  Created on: 08-Dec-2008
 *      Author: 1parkk
 */

#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_

class Reversi {
	int ** gameboard;
	int size;
	int score[3];
	bool status;

	void change_status();
	void set_slot(int x, int y, int player);
	int flip_slots(int x, int y, int player);
	void change_score(int change, int player);

	public:
		//Gameboard();
		Reversi(int size = 8);
		bool make_move(int x, int y, int player);
		int get_slot_status(int x, int y) const;
		bool is_valid_move(int x, int y, int player) const;
		int flip_count(int x, int y, int player) const;
		int get_size() const;
		int get_score(int player) const;
		bool moves_exist(int player) const;
		bool get_move(int player);
		bool get_status() const;
		int determine_winner() const;
		bool check_win(int player);
		void print_board() const;
		void print_scores() const;
		int count_free_slots() const;
		int count_player_slots(int player) const;
		virtual ~Reversi();
};

#endif /* GAMEBOARD_H_ */
