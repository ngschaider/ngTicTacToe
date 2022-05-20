#include <stdlib.h>
#include "game.h"

#ifndef H_BOT
#define H_BOT

extern int minimax_choice(Game*);
extern int misterr_choice(Game*);

extern wchar_t* bot_get_name(PlayerType);

#endif