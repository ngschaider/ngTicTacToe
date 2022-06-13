
#include <stdlib.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "player.h"
#include "game.h"
#include "utils.h"
#include "bot.h"


int used = 0;
Player players[PLAYER_MAX_SIZE];




int human_get_move(Game* game) {
	int choice = 0;
	do {
        system("CLS");
        GAME_print(game);
        wprintf(L"\n");
        GAME_print_simple_stats(game);
        wprintf(L"\n");

		wprintf(L"Ihre Auswahl: ");
		scanf("%d", &choice);
		flush_stdin();
	}while (choice < 1 || choice > 9 || !GAME_is_cell_empty(game, choice - 1));

	return choice - 1;
}


void PLAYER_add(Player player) {
	assert(used < PLAYER_MAX_SIZE);

	player.id = used;
	players[used] = player;
	used++;
}


void PLAYER_init(void) {
	Player human;
	human.name = L"Mensch";
	human.type = Human;
	human.get_move = &human_get_move;
	PLAYER_add(human);

	Player misterr;
	misterr.name = L"Mister R.";
	misterr.type = Bot;
	misterr.get_move = &misterr_get_move;
	PLAYER_add(misterr);

	Player simpler;
	simpler.name = L"Simpler";
	simpler.type = Bot;
	simpler.get_move = &simpler_get_move;
	PLAYER_add(simpler);

	Player onelayer;
	onelayer.name = L"OneLayer";
	onelayer.type = Bot;
	onelayer.get_move = &onelayer_get_move;
	PLAYER_add(onelayer);

	Player twolayer;
	twolayer.name = L"TwoLayer";
	twolayer.type = Bot;
	twolayer.get_move = &twolayer_get_move;
	PLAYER_add(twolayer);

	Player minimax;
	minimax.name = L"Minimax";
	minimax.type = Bot;
	minimax.get_move = &minimax_get_move;
	PLAYER_add(minimax);
}

Player* PLAYER_get(int id) {
	assert(used > id);

	return &players[id];
}

int PLAYER_get_amount(void) {
	return used;
}
