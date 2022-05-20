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

int misterr_choice(Game* game) {
	return 1;
}