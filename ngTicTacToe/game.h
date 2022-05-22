#include <stdbool.h>

#ifndef H_GRID
#define H_GRID

#define PLAYERTYPE_LENGTH 5

typedef enum {
	Human = 1,
	Minimax = 2,
	MisterR = 3,
	OneLayer = 4,
	TwoLayer = 5,
} PlayerType;

typedef struct {
	int cells[9];
	PlayerType player1Type;
	PlayerType player2Type;

	int currentPlayer; // will be 1 or 2 depending on which player's turn it is

	// Statistics
	int gamesTotal;
	int gamesWon1;
	int gamesLost1;
	int gamesWon2;
	int gamesLost2;
} Game;

extern void game_print_extended_stats(Game*);
extern void game_print_simple_stats(Game*);

extern void game_reset(Game*);

extern Game game_create(PlayerType, PlayerType);

extern int game_get_winner(Game*);

extern void game_print(Game*);

extern void game_get_empty_cells(Game*, int*, int*);

extern bool game_is_cell_empty(Game*, int);

extern Game game_copy(Game*);

#endif