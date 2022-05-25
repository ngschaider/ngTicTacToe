#ifndef BOT_H_INCLUDED
#define BOT_H_INCLUDED

#include <stdlib.h>
#include "game.h"

extern int misterr_get_move(Game*);
extern int simpler_get_move(Game*);
extern int minimax_get_move(Game*);
extern int onelayer_get_move(Game*);
extern int twolayer_get_move(Game*);

#endif
