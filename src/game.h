#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <stdbool.h>
#include "player.h"

typedef struct GameStruct {
	int cells[9];
	Player* player1;
	Player* player2;

	int currentPlayer; // will be 1 or 2 depending on which player's turn it is

	// Statistics
	int gamesDrawn;
	int gamesWon1;
	int gamesWon2;
} Game;

typedef struct StatsStruct {
	int gamesTotal;
	int gamesWon1;
	float gamesWon1Percentage;
	int gamesLost1;
	float gamesLost1Percentage;
	int gamesWon2;
	float gamesWon2Percentage;
	int gamesLost2;
	float gamesLost2Percentage;
	int gamesDrawn;
	float gamesDrawnPercentage;
} Stats;

extern Stats game_get_stats(Game*);

extern void game_print_extended_stats(Game*);
extern void game_print_simple_stats(Game*);

extern void game_reset(Game*);

extern Game game_create(Player*, Player*);

extern int game_get_winner(Game*);

extern void game_print(Game*);

extern void game_get_empty_cells(Game*, int*, int*);

extern bool game_is_cell_empty(Game*, int);

extern Game game_copy(Game*);

#endif
