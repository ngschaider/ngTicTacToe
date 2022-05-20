#ifndef H_GRID
#define H_GRID

typedef struct {
	int cells[9];
	int mode;
	int currentPlayer;

	// Statistics
	int gamesTotal;
	int gamesWon1;
	int gamesLost1;
	int gamesWon2;
	int gamesLost2;
} Game;

extern void game_print_stats(Game*);

extern Game game_create(int);

extern int game_get_winner(Game*);

extern void game_print(Game*);

#endif