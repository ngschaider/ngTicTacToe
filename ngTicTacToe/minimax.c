#include <assert.h>

#include "minimax.h"
#include "game.h"

// In order to understand minimax see the following article series:
// https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-1-introduction/
// https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-2-evaluation-function/
// https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-3-tic-tac-toe-ai-finding-optimal-move/


int helper(Game* game, int depth, bool isMaximizerPlaying) {
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

			int value = helper(game, depth + 1, false);
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

			int value = helper(game, depth + 1, true);
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

/**
 * @description Returns the cell index chosen by the legend itself, Minimax (named after the algorithm it uses)
 * @return the index chosen by minimax
 */
int minimax_choice(Game* game) {
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

		int value = helper(&copy, 0, !isMaximizer);
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
