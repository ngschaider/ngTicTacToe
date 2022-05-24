#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <locale.h>
#include <Windows.h>
#include <wchar.h>
#include <io.h>
#include <fcntl.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#include "game.h"
#include "bot.h"
#include "utils.h"
#include "player.h"

int get_choice(Game* game) {
	assert(game->currentPlayer == 1 || game->currentPlayer == 2);
	Player* player = game->currentPlayer == 1 ? game->player1 : game->player2;

	return player->get_move(game);
}


void play_game(Game* game, bool verbose) {
	GAME_reset(game);
	while (GAME_get_winner(game) == -1) {
		int choice = get_choice(game);
		assert(choice >= 0 && choice <= 8);
		game->cells[choice] = game->currentPlayer;

		assert(game->currentPlayer == 1 || game->currentPlayer == 2);
		if (game->currentPlayer == 1) {
			game->currentPlayer = 2;
		}
		else if (game->currentPlayer == 2) {
			game->currentPlayer = 1;
		}
	}

	int winner = GAME_get_winner(game);

	assert(winner >= 0 && winner <= 2);

	if (winner == 0) {
		game->gamesDrawn++;
	}
	if (winner == 1) {
		game->gamesWon1++;
	}
	else if (winner == 2) {
		game->gamesWon2++;
	}

	if (verbose) {
		system("CLS");
		GAME_print(game);
		wprintf(L"\n");
		GAME_print_simple_stats(game);
		wprintf(L"\n");

		if (winner == 0) {
			wprintf(L"Unentschieden!\n");
		}
		else if (winner == 1) {
			wprintf(L"Gewinner: %ls (Spieler 1 | X)\n", game->player1->name);
		}
		else if (winner == 2) {
			wprintf(L"Gewinner: %ls (Spieler 2 | O)\n", game->player1->name);
		}
	}
}

void print_header(void) {
	// copy date and time from __DATE__ and __TIME__ macros to a wide string in order
	// to be used with wprintf (remember, we cant use the normal printf anymore);
	size_t charsConverted = 0;
	wchar_t dateStr[100];

	mbstowcs_s(&charsConverted, dateStr, 100, __DATE__, _TRUNCATE);
	wchar_t timeStr[100];
	mbstowcs_s(&charsConverted, timeStr, 100, __TIME__, _TRUNCATE);

	// Clear the screen and output some information about the program
	// as well as the game mode selection menu
	system("CLS");
	color_cyan();
	wprintf(L"Tic Tac Toe v1.0 - Niklas Gschaider\n");
	wprintf(L"Kompilierzeitpunkt: %ls, %ls\n", dateStr, timeStr);
	color_white();
	wprintf(L"\n");
}

#ifndef DONOTDEFINE_Prompts

Player* prompt_player(int player_number) {
	color_cyan();
	wprintf(L"--- Typ für Spieler %d ---\n", player_number);
	color_white();
	for (int i = 0; i < PLAYER_get_amount(); i++) {
		Player* player = PLAYER_get(i);
		wchar_t* prefix = player->type == Human ? L"" : L"BOT - ";;
		wprintf(L"[%d] %ls%ls\n", i + 1, prefix, player->name);
	}

	int choice = 0;
	while (choice < 1 || choice > PLAYER_get_amount()) {
		wprintf(L"Ihre Auswahl: ");
		scanf("%d", &choice);
		flush_stdin();
	}

	wprintf(L"\n");

	int player_id = choice - 1;
	assert(player_id >= 0 && player_id < PLAYER_get_amount());
	return PLAYER_get(player_id);
}

int prompt_amount_of_rounds() {
	wprintf(L"--- Rundenanzahl ---\n");
	int choice = 0;
	while (choice < 1) {
		wprintf(L"Wieviele Runden sollen gespielt werden: ");
		scanf("%d", &choice);
		flush_stdin();
	}

	wprintf(L"\n");

	assert(choice >= 1);
	return choice;
}

bool prompt_play_again(void) {
	char choice = '_';
	while (choice != 'j' && choice != 'n') {
		wprintf(L"Noch ein Spiel? [j/n] ");
		scanf(" %c", &choice);
		flush_stdin();
	}

	return choice == 'j';
}

#endif

void bot_benchmark() {
	wprintf(L"\n");
	int amount_of_rounds = prompt_amount_of_rounds();
	system("CLS");
	print_header();
	wprintf(L"------------------------------ BOT BENCHMARK (%d ROUNDS) ------------------------------\n", amount_of_rounds);
	wprintf(L"                               Spieler 1 gew.     Spieler 2 gew.     Unentschieden\n");
	for (int index1 = 0; index1 < PLAYER_get_amount(); index1++) {
		Player* player1 = PLAYER_get(index1);
		if (player1->type == Human) {
			continue;
		}
		for (int index2 = 0; index2 < PLAYER_get_amount(); index2++) {
			Player* player2 = PLAYER_get(index2);
			if (player2->type == Human) {
				continue;
			}

			Game game = GAME_create(player1, player2);

			for (int i = 0; i < amount_of_rounds; i++) {
				play_game(&game, false);
			}

			Stats stats = GAME_get_stats(&game);
			wprintf(L"%-9ls  gegen  %-9ls:  %14.2f%%    %14.2f%%    %13.2f%%\n", player1->name, player2->name, stats.gamesWon1Percentage, stats.gamesWon2Percentage, stats.gamesDrawnPercentage);
		}
	}
	wprintf(L"\n");
	system("PAUSE");
}

int main() {
	// seed the random number generator with the system time (only once!)
	srand((unsigned int)time(NULL));

	// set stdout stream to UTF-16
	// we can now use wprintf but we can't use printf anymore
	_setmode(_fileno(stdout), _O_U16TEXT);

	// setup bots and human player interface
	PLAYER_init();

	// Setup font family and font size
	CONSOLE_FONT_INFOEX cfi = {
		sizeof cfi, // cbSize
		0, // nFont
		{10, 20}, // dwFontSize.X, dwFontSize.Y
		FF_DONTCARE, // FontFamily
		FW_NORMAL, // FontWeight
	};
	wcscpy_s(cfi.FaceName, 9, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &cfi);

	while (true) {
		system("CLS");
		print_header();

		color_cyan();
		wprintf(L"--- Hauptmenü ---\n");
		color_white();
		wprintf(L"[1] Spielen\n");
		wprintf(L"[2] Bot Benchmark\n");
		wprintf(L"[3] Beenden\n");
		wprintf(L"\n");

		int choice = -1;
		while (choice < 1 || choice > 3) {
			wprintf(L"Ihre Auswahl: ");
			scanf("%d", &choice);
			flush_stdin();
		}

		wprintf(L"\n");

		if (choice == 1) {
			Player* player1 = prompt_player(1);
			Player* player2 = prompt_player(2);

			// create a game (this will be reused for folllowing games as it will also keep track of stats)
			Game game = GAME_create(player1, player2);

			if (game.player1->type == Human || game.player2->type == Human) {
				bool play_again = true;
				while (play_again) {
					play_game(&game, true);
					play_again = prompt_play_again();
				}
			}
			else {
				int amount_of_rounds = prompt_amount_of_rounds();
				for (int i = 0; i < amount_of_rounds; i++) {
					play_game(&game, false);
				}
			}

			system("CLS");
			print_header();
			GAME_print_extended_stats(&game);
			system("PAUSE");
		}
		else if (choice == 2) {
			bot_benchmark();
		}
		else if (choice == 3) {
			break;
		}
	}

	wprintf(L"\n");
	wprintf(L"Danke fürs Spielen. Auf Wiedersehen!\n");

	system("PAUSE");
	return EXIT_SUCCESS;
}