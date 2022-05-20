#include "bot.h"

#include <stdio.h>
#include <assert.h>

#include "game.h"

wchar_t* bot_get_name(PlayerType playerType) {
	if (playerType == Human) {
		return L"Mensch";
	}
	else if (playerType == Minimax) {
		return L"Minimax";
	}
	else if (playerType == MisterR) {
		return L"Mister R.";
	}

	return L"Fehler";
}

int minimax_choice(Game* game) {
	return 1;
}

/**
 * @description Returns the cell index which is chosen by the legend himself, Mister R. (R stands for random)
 * @return the index of a randomly selected cell
 */
int misterr_choice(Game* game) {
	int count;
	int empty[9];
	game_get_empty_cells(game, empty, &count);

	int index = rand() % (count - 1) + 1;
	return empty[index];
}