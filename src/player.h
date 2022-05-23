#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <stdlib.h>

typedef struct GameStruct Game;

typedef enum {
	Human = 0,
	Bot = 1,
} PlayerType;

typedef struct PlayerStruct {
	int id;
	PlayerType type;
	wchar_t* name;
	int (*get_move)(Game*);
} Player;


extern void player_init(void);
extern Player* player_get(int);
extern int player_get_amount(void);

#endif