#include <stdlib.h>
#include "game.h"

#ifndef BOT_H_INCLUDED
#define BOT_H_INCLUDED

extern int bot_choice(Game*, PlayerType);

extern wchar_t* bot_get_name(PlayerType);

#endif
