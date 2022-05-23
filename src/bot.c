#include <stdio.h>
#include <assert.h>

#include "bot.h"
#include "game.h"

int misterr_get_move(Game* game) {
	int count;
	int empty[9];

	game_get_empty_cells(game, empty, &count);
	assert(count >= 1 && count <= 9);

	if (count == 1) {
		return empty[0];
	}

	int index = rand() % (count - 1) + 1;
	return empty[index];
}

int onelayer_get_move(Game* game) {
	int count;
	int empty[9];

	game_get_empty_cells(game, empty, &count);
	assert(count >= 1 && count <= 9);

	if (count == 1) {
		return empty[0];
	}

	// loop through every possible play, copy the board, play the game 
	// in the copied board and see if it would be the winning move
	for (int i = 0; i < count; i++) {
		int index = empty[i];
		Game copy = game_copy(game);
		copy.cells[index] = game->currentPlayer;

		if (game_get_winner(&copy) == game->currentPlayer) {
			return index;
		}
	}

	int index = rand() % (count - 1) + 1;
	return empty[index];
}

int twolayer_get_move(Game* game) {
	int count1;
	int empty1[9];

	game_get_empty_cells(game, empty1, &count1);
	assert(count1 >= 1 && count1 <= 9);

	if (count1 == 1) {
		return empty1[0];
	}

	// loop through every possible play, copy the board, play the game 
	// in the copied board and see if it would be the winning move
	for (int i = 0; i < count1; i++) {
		int index1 = empty1[i];
		Game copy1 = game_copy(game);
		copy1.cells[index1] = game->currentPlayer;

		if (game_get_winner(&copy1) == game->currentPlayer) {
			return index1;
		}

		int count2;
		int empty2[9];

		game_get_empty_cells(game, empty2, &count2);
		assert(count2 >= 1 && count2 <= 9);

		for (int j = 0; j < count2; j++) {
			int index2 = empty2[j];
			Game copy2 = game_copy(game);
			copy2.cells[index2] = game->currentPlayer;

			if (game_get_winner(&copy2) == game->currentPlayer) {
				return index2;
			}
		}
	}

	int index = rand() % (count1 - 1) + 1;
	return empty1[index];
}



#ifndef DONOTDEFINE_Minimax


// In order to understand minimax see the following article series:
// https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-1-introduction/
// https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-2-evaluation-function/
// https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-3-tic-tac-toe-ai-finding-optimal-move/


int minimax_helper(Game* game, int depth, bool isMaximizerPlaying) {
	int winner = game_get_winner(game);
	if (winner == 0) {
		// Draw
		return 0;
	}
	else if (winner == 1) {
		return 10;
	}
	else if (winner == 2) {
		return -10;
	}

	int empty[9];
	int count;
	game_get_empty_cells(game, empty, &count);

	if (isMaximizerPlaying) {
		int best = -100;
		for (int i = 0; i < count; i++) {
			int index = empty[i];
			game->cells[index] = 1;

			int value = minimax_helper(game, depth + 1, false);
			if (value > best) {
				best = value;
			}

			// Undo move
			game->cells[index] = 0;
		}

		return best;
	}
	else {
		// Minimizer is playing
		int best = 100;
		for (int i = 0; i < count; i++) {
			int index = empty[i];
			game->cells[index] = 2;

			int value = minimax_helper(game, depth + 1, true);
			if (value < best) {
				best = value;
			}

			// Undo move
			game->cells[index] = 0;
		}

		return best;
	}

	// we should never get here
	assert(false);
	return -1;
}

int minimax_get_move(Game* game) {
	Game copy = game_copy(game);

	bool isMaximizer = copy.currentPlayer == 1;

	int best_choice = -1;
	int best_value = isMaximizer ? -100 : 100;

	int empty[9];
	int count;
	game_get_empty_cells(game, empty, &count);
	for (int i = 0; i < count; i++) {
		int index = empty[i];
		copy.cells[index] = copy.currentPlayer;

		int value = minimax_helper(&copy, 0, !isMaximizer);
		if ((isMaximizer && value > best_value) || (!isMaximizer && value < best_value)) {
			best_value = value;
			best_choice = index;
		}

		// Undo move
		copy.cells[index] = 0;
	}

	assert(best_choice != -1);
	return best_choice;
}

#endif