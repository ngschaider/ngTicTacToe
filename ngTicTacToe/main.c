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
		game_print_simple_stats(game);
		wprintf(L"\n");

		int choice = 0;
		while (choice < 1 || choice > 9) {
			wprintf(L"Ihre Auswahl: ");
			auto _ = scanf("%d", &choice);
			flushStdin();
		}
		return choice - 1;
	}
	else {
		return bot_choice(game, playerType);
	}


	assert(false);

	return -1;
}


void play_game(Game* game, bool verbose) {
	game_reset(game);
	while (game_get_winner(game) == -1) {
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

	int winner = game_get_winner(game);

	game->gamesTotal++;

	assert(winner >= 0 && winner <= 2);

	if (winner == 1) {
		game->gamesWon1++;
		game->gamesLost2++;
	}
	else if (winner == 2) {
		game->gamesWon2++;
		game->gamesLost1++;
	}

	if (verbose) {
		system("CLS");
		game_print(game);
		wprintf(L"\n");
		game_print_simple_stats(game);
		wprintf(L"\n");

		if (winner == 0) {
			wprintf(L"Unentschieden!\n");
		}
		else if (winner == 1) {
			const wchar_t* player1Name = bot_get_name(game->player1Type);
			wprintf(L"Gewinner: %ls (Spieler 1 | X)\n", player1Name);
		}
		else if (winner == 2) {
			const wchar_t* player2Name = bot_get_name(game->player2Type);
			wprintf(L"Gewinner: %ls (Spieler 2 | O)\n", player2Name);
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
	wprintf(L"Tic Tac Toe v1.0 - Niklas Gschaider\n");
	wprintf(L"Kompilierzeitpunkt: %ls, %ls\n", dateStr, timeStr);
	wprintf(L"\n");
}

#ifndef DONOTDEFINE_Prompts

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

int prompt_amount_of_rounds() {
	wprintf(L"--- Rundenanzahl ---\n");
	int choice = 0;
	while (choice < 1) {
		wprintf(L"Wieviele Runden sollen gespielt werden: ");
		auto _ = scanf_s("%d", &choice);
		flushStdin();
	}

	wprintf(L"\n");

	assert(choice >= 1);
	return choice;
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

#endif

int main() {
	// seed the random number generator with the system time (only once!)
	srand((unsigned int)time(NULL));

	// set stdout stream to UTF-16
	// we can now use wprintf but we can't use printf anymore
	auto _ = _setmode(_fileno(stdout), _O_U16TEXT);

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

		wprintf(L"--- Hauptmenü ---\n");
		wprintf(L"[1] Spielen\n");
		wprintf(L"[2] Bot Benchmark\n");
		wprintf(L"[3] Beenden\n");
		wprintf(L"\n");

		int choice = -1;
		while (choice < 1 || choice > 3) {
			wprintf(L"Ihre Auswahl: ");
			auto _ = scanf_s("%d", &choice);
			flushStdin();
		}

		if (choice == 1) {
			PlayerType playerType1 = prompt_PlayerType(1);
			PlayerType playerType2 = prompt_PlayerType(2);

			// create a game (this will be reused for folllowing games as it will also keep track of stats)
			Game game = game_create(playerType1, playerType2);

			if (game.player1Type == Human || game.player2Type == Human) {
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
			game_print_extended_stats(&game);
			system("PAUSE");
		}
		else if (choice == 2) {
			wprintf(L"\n");
			int amount_of_rounds = prompt_amount_of_rounds();
			system("CLS");
			wprintf(L"                               Spieler 1 gew.     Spieler 2 gew.     Unentschieden\n");
			for (int bot1 = 2; bot1 <= PLAYERTYPE_LENGTH; bot1++) {
				for (int bot2 = 2; bot2 <= PLAYERTYPE_LENGTH; bot2++) {
					Game game = game_create(bot1, bot2);

					for (int i = 0; i < amount_of_rounds; i++) {
						play_game(&game, false);
					}

					double won1 = (double)game.gamesWon1 / game.gamesTotal * 100;
					double won2 = (double)game.gamesWon2 / game.gamesTotal * 100;
					double draw = ((double)game.gamesTotal - game.gamesWon1 - game.gamesWon2) / game.gamesTotal * 100;
					wprintf(L"%-9ls  gegen  %-9ls:  %14.2f%%    %14.2f%%    %13.2f%%\n", bot_get_name(bot1), bot_get_name(bot2), won1, won2, draw);
				}
			}
			wprintf(L"\n");
			system("PAUSE");
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
