#include <stdlib.h>
#include "game.h"

#ifndef H_BOT
#define H_BOT

extern int bot_choice(Game*, PlayerType);

extern wchar_t* bot_get_name(PlayerType);

#endif