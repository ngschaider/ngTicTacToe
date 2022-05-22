#include <stdio.h>
#include <assert.h>

#include "bot.h"
#include "game.h"
#include "minimax.h"

wchar_t* bot_get_name(PlayerType playerType) {
	assert(playerType >= 1 && playerType <= PLAYERTYPE_LENGTH);

	if (playerType == Human) {
		return L"Mensch";
	}
	else if (playerType == Minimax) {
		return L"Minimax";
	}
	else if (playerType == MisterR) {
		return L"Mister R.";
	}
	else if (playerType == OneLayer) {
		return L"OneLayer";
	}
	else if (playerType == TwoLayer) {
		return L"TwoLayer";
	}

	return L"Fehler";
}



/**
 * @description Returns the cell index chosen by the legend itself, Mister R. (R stands for random)
 * @return the index of a randomly selected cell
 */
int misterr_choice(Game* game) {
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


/**
 * @description Returns the cell index chosen by the legend itself, OneLayer (because he thinks one move ahead)
 * @return the index of a randomly selected cell
 */
int onelayer_choice(Game* game) {
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

/**
 * @description Returns the cell index chosen by the legend itself, TwoLayer (because he thinks two moves ahead)
 * @return the index of a randomly selected cell
 */
int twolayer_choice(Game* game) {
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

int bot_choice(Game* game, PlayerType playerType) {
	assert(playerType > 1 && playerType <= PLAYERTYPE_LENGTH);
	if (playerType == MisterR) {
		return misterr_choice(game);
	}
	else if (playerType == Minimax) {
		return minimax_choice(game);
	}
	else if (playerType == OneLayer) {
		return onelayer_choice(game);
	}
	else if (playerType == TwoLayer) {
		return twolayer_choice(game);
	}

	assert(false);
	return -1;
}