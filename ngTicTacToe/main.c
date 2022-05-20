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

void flushStdin(void) {
	while (getchar() != '\n');
}

int get_choice(Game* game) {
	assert(game->currentPlayer == 1 || game->currentPlayer == 2);
	PlayerType playerType = game->currentPlayer == 1 ? game->player1Type : game->player2Type;

	if (playerType == Human) {
		system("CLS");
		game_print(game);
		wprintf(L"\n");
		game_print_stats(game);
		wprintf(L"\n");

		int choice = 0;
		while (choice < 1 || choice > 9) {
			wprintf(L"Ihre Auswahl: ");
			auto _ = scanf("%d", &choice);
			flushStdin();
		}
		return choice;
	}
	else if (playerType == MisterR) {
		return misterr_choice(game);
	}
	else if (playerType == Minimax) {
		return minimax_choice(game);
	}

	return -1;
}


void play_game(Game* game) {
	game_reset(game);
	while (game_get_winner(game) == 0) {
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
	game_print_stats(game);
	wprintf(L"\n");

	int winner = game_get_winner(game);

	game->gamesTotal++;

	assert(winner >= 0 && winner <= 2);

	if (winner == 0) {
		wprintf(L"Unentschieden!\n\n");
	}
	else if (winner == 1) {
		const wchar_t* player1Name = bot_get_name(game->player1Type);
		wprintf(L"Gewinner: %ls\n", player1Name);
		game->gamesWon1++;
		game->gamesLost2++;
	}
	else if (winner == 2) {
		const wchar_t* player2Name = bot_get_name(game->player2Type);
		wprintf(L"Gewinner: %ls\n", player2Name);
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
	wprintf(L"Tic Tac Toe v1.0 - Niklas Gschaider\n");
	wprintf(L"Kompilierzeitpunkt: %ls, %ls\n", dateStr, timeStr);
	wprintf(L"\n");
}

PlayerType prompt_PlayerType(int player_number) {
	wprintf(L"--- Typ für Spieler %d ---\n", player_number);
	for (int i = 0; i < PLAYERTYPE_LENGTH; i++) {
		wchar_t* prefix = i == 0 ? L"" : L"BOT - ";
		wchar_t* name = bot_get_name(i + 1);
		wprintf(L"[%d] %ls%ls\n", i + 1, prefix, name);
	}

	int choice = 0;
	while (choice < 1 || choice > PLAYERTYPE_LENGTH) {
		wprintf(L"Ihre Auswahl: ");
		auto _ = scanf_s("%d", &choice);
		flushStdin();
	}

	wprintf(L"\n");

	assert(choice >= 1 && choice <= PLAYERTYPE_LENGTH);
	return (PlayerType)choice;
}

bool prompt_play_again(void) {
	char choice = '_';
	while (choice != 'j' && choice != 'n') {
		wprintf(L"Noch ein Spiel? [j/n] ");
		auto _ = scanf("%c", &choice);
		flushStdin();
	}

	return choice == 'j';
}

int main() {
	// seed the random number generator with the system time (only once!)
	srand((unsigned int)time(NULL));

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

	system("CLS");
	print_header();
	PlayerType playerType1 = prompt_PlayerType(1);
	PlayerType playerType2 = prompt_PlayerType(2);

	// create a game (this will be reused for folllowing games as it will also keep track of stats)
	Game game = game_create(playerType1, playerType2);

	bool play_again = true;
	while (play_again) {
		play_game(&game);
		play_again = prompt_play_again();
	}

	wprintf(L"\n");
	wprintf(L"Danke fürs Spielen. Auf Wiedersehen!\n");

	system("PAUSE");
	return EXIT_SUCCESS;
}


// Todo:
// Sound Effekte beim Setzen einer Zelle
// Melodie beim Gewinnen/Verlieren