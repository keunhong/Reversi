//============================================================================
// Name        : lesson_11.cpp
// Author      : Keunhong Park
// Version     : 0.0.0.1b
// Description : Reversi in C++
/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//============================================================================

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "Gameboard.h"
#include "AIPlayer.h"
using namespace std;


//#define max(x,y) ( ((x)>(y))?x:y )
//#define max4(p,q,r,s) ( max(( max(p,q)>max(r,s) ))  )

/*
 * Function prototypes
 *
 */
int get_board_size();
int select_player();
int select_play_mode();
void play_against(int size);
void play_ai(int size);
void play_ai_to_ai(int size);

/*
 * Main function
 *
 */
int main(int argc, char * argv[]) {
	std::cout << "   ______________________   _________________________  _________.___" << std::endl
			  << "\\______   \\_   _____/\\   \\ /   /\\_   _____/\\______   \\/   _____/|   |" << std::endl
			  << " |       _/|    __)_  \\       /  |    __)_  |       _/\\_____  \\ |   |" << std::endl
			  << " |    |   \\|        \\  \\     /   |        \\ |    |   \\/        \\|   |" << std::endl
			  << " |____|_  /_______  /   \\___/   /_______  / |____|_  /_______  /|___|" << std::endl
			  << "        \\/        \\/                    \\/         \\/        \\/     " << std::endl
			  << "[<<] K Park, 2008 (k@y.flixey.com)" << std::endl
			  << "[<<] Visit http://flixey.com for more." << std::endl
			  << std::endl
			  << "[<<] GNU General Public License:" << std::endl
			  << "   This program is free software: you can redistribute it and/or modify" << std::endl
			  << "   it under the terms of the GNU General Public License as published by" << std::endl
			  << "   the Free Software Foundation, either version 3 of the License, or" << std::endl
			  << "   (at your option) any later version." << std::endl << std::endl
			  << "   This program is distributed in the hope that it will be useful," << std::endl
			  << "   but WITHOUT ANY WARRANTY; without even the implied warranty of" << std::endl
			  << "   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << std::endl
			  << "   GNU General Public License for more details." << std::endl
			  << "   You should have received a copy of the GNU General Public License" << std::endl
			  << "   along with this program.  If not, see <http://www.gnu.org/licenses/>." << std::endl
			  << std::endl;



	// if argument was passed through command line
	// then initialize board using that as the size
	// if it isn't valid, just make the user input it again
	int size, arg = 0;
	if(argv[1]){ arg = strtol(argv[1],NULL,10); }
	if(arg > 3 && arg % 2 == 0){
		size = arg;
	}else{
		// if no argument or argument is not valid
		// prompt for board size
		size = get_board_size();
	}

	printf("  [<<] Board size is: %d x %d\n", size, size);

	// prompt for play mode
	// 1 => against another player
	// 2 => against computer
	int mode = select_play_mode();
	if(mode == 2){
		play_ai(size);
	}else if(mode == 3){
		play_ai_to_ai(size);
	}else{
		play_against(size);
	}

	// to prevent the window from closing after program terminates
	cin.sync();
	cin.get();
	return 0;
}

/*
 * Input/output functions
 *
 */
int get_board_size(){
	while(true){
		printf("[>>] Input size of board: ");
		int value;
		cin >> value;
		if(cin.fail()){
			cin.clear();
			string garbage;
			std::getline(std::cin, garbage);
			printf("  [!<<] Invalid input: board size must be an integer.\n");
		}else if(value % 2 != 0 || value == 0){
			cin.clear();
			printf("  [!<<] Invalid input: board size must be even.\n");
		}else{
			return value;
		}
	}
}

int select_play_mode(){
	printf("[<<] Select play mode (1 = multiplayer, 2 = singleplayer, 3 = AI only ): ");
	while(true){
		int value;
		cin >> value;
		if(cin.fail()){
			cin.clear();
			string garbage;
			std::getline(std::cin, garbage);
			printf("  [!<<] Invalid input: play mode must be an integer.\n");
		}else if(value != 1 && value != 2 && value != 3){
			cin.clear();
			printf("  [!<<] Invalid input: play mode must be 1, 2 or 3.\n");
		}else{
			return value;
		}
	}
}

void play_against(int size){
	printf("[<<] Creating game object...\n");
	// create game object
	Reversi game(size);
	printf("  [<<] Game object successfully created.\n\n");
	printf("[<<] Player 1's turn:\n");
	game.print_board();
	while(game.get_status() != 0){
		game.get_move(1);
		game.print_board();
		game.print_scores();
		if(game.check_win(1) == true){ break; }
		game.get_move(2);
		game.print_board();
		game.print_scores();
		if(game.check_win(2) == true){ break; }
	}
	if(game.get_status() == 0){
		printf("[<<] Game over !\n");
		printf("  [<<] Player 1 score: %d\n",game.get_score(1));
		printf("  [<<] Player 2 score: %d\n",game.get_score(2));
		if(game.determine_winner() == 0){
			printf("[!!!] Player 1 and Player 2 have TIED.\n");
		}else{
			printf("[!!!] Player %d WINS!\n",game.determine_winner());
		}
	}
}

void play_ai(int size){
	printf("[<<] Creating game object...\n");
	// create game object
	Reversi game(size);
	printf("  [<<] Game object successfully created.\n");
	// create AI object
	printf("  [<<] Creating AI object...\n");
	AIPlayer ai(game,2);
	printf("  [<<] AI object successfully created.\n\n");
	printf("[<<] Player 1's turn:\n");
	game.print_board();
	while(game.get_status() != 0){
		game.get_move(1);
		game.print_board();
		game.print_scores();
		if(game.check_win(1) == true){ break; }
		ai.play();
		game.print_board();
		game.print_scores();
		if(game.check_win(2) == true){ break; }
	}
	if(game.get_status() == 0){
		printf("[<<] Game over !\n");
		printf("  [<<] Player 1 score: %d\n",game.get_score(1));
		printf("  [<<] Player 2 (AI) score: %d\n",game.get_score(2));
		int winner = game.determine_winner();
		if(winner == 0){
			printf("[!!!] Player 1 and Player 2 have TIED.\n");
		}else{
			printf("[!!!] Player %d WINS!\n",winner);
		}
	}
}

void play_ai_to_ai(int size){
	printf("[<<] Creating game object...\n");
	// create game object
	Reversi game(size);
	printf("  [<<] Game object successfully created.\n");
	// create AI object
	printf("  [<<] Creating first AI object...\n");
	AIPlayer ai1(game,1,true);
	printf("  [<<] First AI object successfully created.\n\n");
	printf("  [<<] Creating second AI object...\n");
	AIPlayer ai2(game,2,true);
	printf("  [<<] Second AI object successfully created.\n\n");
	printf("[<<] Player 1's turn:\n");
	game.print_board();
	while(game.get_status() != 0){
		ai1.play();
		game.print_board();
		game.print_scores();
		if(game.check_win(1) == true){ break; }
		ai2.play();
		game.print_board();
		game.print_scores();
		if(game.check_win(2) == true){ break; }
	}
	if(game.get_status() == 0){
		game.print_board();
		game.print_scores();
		printf("[<<] Game over !\n");
		printf("  [<<] Player 1 score: %d\n",game.get_score(1));
		printf("  [<<] Player 2 score: %d\n",game.get_score(2));
		if(game.determine_winner() == 0){
			printf("[!!!] Player 1 and Player 2 have TIED.\n");
		}else{
			printf("[!!!] Player %d WINS by %d points!\n",game.determine_winner(),game.get_score((game.determine_winner() == 1)?1:2) - game.get_score((game.determine_winner() == 1)?2:1));
		}
	}
}
