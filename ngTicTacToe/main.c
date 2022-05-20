#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <locale.h>
#include <Windows.h>
#include <wchar.h>
#include <io.h>
#include <fcntl.h>
#include <assert.h>

#include "game.h"
#include "bot.h"
#include "utils.h"

int get_choice(Game* game) {
	assert(game->currentPlayer == 1 || game->currentPlayer == 2);
	assert(game->mode == 1 || game->mode == 2);
	if (game->currentPlayer == 1) {
		int choice = -1;
		while (choice < 1 || choice > 9 || game->cells[choice - 1] != 0) {
			wprintf(L"Auswahl Spieler 1: ");
			auto _ = scanf_s("%d", &choice);
		}
		return choice;
	}
	else if (game->currentPlayer == 2) {
		if (game->mode == 1) {
			int choice = -1;
			while (choice < 1 || choice > 9) {
				wprintf(L"Auswahl Spieler 2: ");
				scanf_s("%d", &choice);
			}
			return choice;
		}
		else if (game->mode == 2) {
			return bot_choice(game);
		}
	}

	return -1;
}


void play_game(Game* game) {
	while (game_get_winner(game) == 0) {
		system("CLS");
		game_print(game);
		wprintf(L"\n");

		game_print_stats(game);

		int choice = get_choice(game);
		game->cells[choice - 1] = game->currentPlayer;

		assert(game->currentPlayer == 1 || game->currentPlayer == 2);
		if (game->currentPlayer == 1) {
			game->currentPlayer = 2;
		}
		else if (game->currentPlayer == 2) {
			game->currentPlayer = 1;
		}
	}

	system("CLS");
	game_print(game);
	wprintf(L"\n");

	int winner = game_get_winner(game);

	game->gamesTotal++;

	assert(winner >= 0 && winner <= 2);

	if (winner == 0) {
		wprintf(L"Unentschieden!\n\n");
	}
	else if (winner == 1) {
		wprintf(L"Spieler 1 hat gewonnen!\n\n");
		game->gamesWon1++;
		game->gamesLost2++;
	}
	else if (winner == 2) {
		game->gamesLost1++;
		game->gamesWon2++;
		if (game->mode == 1) {
			wprintf(L"Spieler 2 hat gewonnen!\n\n");
		}
		else if (game->mode == 2) {
			wprintf(L"Der Computer hat gewonnen! Sorry :(\n\n");
		}
	}
}

int main() {
	// set stdout stream to UTF-16
	// we can now use wprintf but we can't use printf anymore
	auto _ = _setmode(_fileno(stdout), _O_U16TEXT);

	// Setup font family and font size
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 10;
	cfi.dwFontSize.Y = 20;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, 9, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &cfi);

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
	wprintf(L"Tic Tac Toe v1.0 - Niklas Gschaider\n");
	wprintf(L"Kompilierzeitpunkt: %ls, %ls\n", dateStr, timeStr);
	wprintf(L"\n");
	wprintf(L"Auswahl - Spielmodus\n");
	wprintf(L"[1] Spieler gegen Spieler\n");
	wprintf(L"[2] Spieler gegen Computer\n");
	wprintf(L"\n");

	// Ask for user input until the program receives a valid input (1 or 2)
	int choice = 0;
	while (choice < 1 || choice > 2) {
		wprintf(L"Deine Auswahl: ");
		fflush(stdin);
		auto _ = scanf_s("%d", &choice);
	}

	// create a game (this will be reused for folllowing games as it will keep track of stats)
	Game game = game_create(choice);

	char nextGameOrNot = 'j';
	while (nextGameOrNot == 'j') {
		play_game(&game);

		do {
			wprintf(L"Noch ein Spiel? <j>/<n> ");
			fflush(stdin);
			auto _ = scanf_s("%c", &nextGameOrNot, 1);
		} while (nextGameOrNot != 'j' && nextGameOrNot != 'n');
	}

	wprintf(L"Danke fürs Spielen. Auf Wiedersehen!");

	system("PAUSE");
	return EXIT_SUCCESS;
}


// Todo:
// Sound Effekte beim Setzen einer Zelle
// Melodie beim Gewinnen/Verlieren