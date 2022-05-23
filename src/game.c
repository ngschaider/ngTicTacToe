#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "game.h"
#include "bot.h"
#include "utils.h"


#ifndef DONOTDEFINE_Utilities

/**
 * @description Calculate an array containing the indices of empty cells
 * @param int* count The place to store the number of empty cells
 * @return int* out The place to store the cell indices (this gets dynamically allocated)
 */
void game_get_empty_cells(Game* game, int* out, int* count) {
	// fill the array with the indices
	int index = 0;
	for (int i = 0; i < 9; i++) {
		if (game_is_cell_empty(game, i)) {
			out[index] = i;
			index++;
		}
	}

	*count = index;
}

bool game_is_cell_empty(Game* game, int i) {
	return game->cells[i] == 0;
}

Game game_copy(Game* game) {
	Game copy = game_create(game->player1, game->player2);
	for (int i = 0; i < 9; i++) {
		copy.cells[i] = game->cells[i];
	}
	copy.currentPlayer = game->currentPlayer;
	copy.gamesTotal = game->gamesTotal;
	copy.gamesWon1 = game->gamesWon1;
	copy.gamesLost1 = game->gamesLost1;
	copy.gamesWon2 = game->gamesWon2;
	copy.gamesLost2 = game->gamesLost2;

	return copy;
}

#endif

#ifndef DONOTDEFINE_Lifecycle_Functions

Game game_create(Player* player1, Player* player2) {
	Game game = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0}, // int cells[9];
		player1, // Player player1;
		player2, // Player player2;

		1, // int currentPlayer;

		0, // int gamesTotal;
		0, // int gamesWon1;
		0, // int gamesLost1;
		0, // int gamesWon2;
		0, // int gamesLost2;
	};

	game_reset(&game);

	return game;
}

void game_reset(Game* game) {
	for (int i = 0; i < 9; i++) {
		game->cells[i] = 0;
	}

	// decide randomly who starts
	game->currentPlayer = (rand() % 2) + 1;
}

#endif

#ifndef DONOTDEFINE_Printing

void game_print_simple_stats(Game* game) {
	wprintf(L"Spiele gesamt: %d\t", game->gamesTotal);
	wprintf(L"%ls gewonnen: %d\t", game->player1->name, game->gamesWon1);
	wprintf(L"%ls gewonnen: %d\n", game->player2->name, game->gamesWon2);
}

void game_print_extended_stats(Game* game) {
	wprintf(L"                          Spieler 1    Spieler 2\n");
	wprintf(L"Typ                       %9ls    %9ls\n", game->player1->name, game->player2->name);
	wprintf(L"Spiele gewonnen           %9d    %9d\n", game->gamesWon1, game->gamesWon2);
	wprintf(L"Spiele gewonnen (%%)       %9.2f    %9.2f\n", (double)game->gamesWon1 / game->gamesTotal * 100, (double)game->gamesWon2 / game->gamesTotal * 100);
	wprintf(L"Spiele verloren           %9d    %9d\n", game->gamesLost1, game->gamesLost2);
	wprintf(L"Spiele verloren (%%)       %9.2f    %9.2f\n", (double)game->gamesLost1 / game->gamesTotal * 100, (double)game->gamesLost2 / game->gamesTotal * 100);
	wprintf(L"Spiele unentschieden      %9d\n", game->gamesTotal - game->gamesLost1 - game->gamesWon1);
	wprintf(L"Spiele unentschieden (%%)  %9.2f\n", ((double)game->gamesTotal - game->gamesLost1 - game->gamesWon1) / game->gamesTotal * 100);
	wprintf(L"\n");
}

void print_cell_part(int cell, int part, int index) {
	assert(part >= 0 && part <= 4);
	assert(cell >= 0 && cell <= 2);

	if (cell == 0) {
		if (part == 0 || part == 1 || part == 3 || part == 4) {
			wprintf(L"          ");
		}
		else if (part == 2) {
			wprintf(L"     %d    ", index + 1);
		}
	}
	else if (cell == 1) {
		if (part == 0 || part == 4) {
			wprintf(L"XX      XX");
		}
		else if (part == 1 || part == 3) {
			wprintf(L"  XX  XX  ");
		}
		else if (part == 2) {
			wprintf(L"    XX    ");
		}
	}
	else if (cell == 2) {
		if (part == 0 || part == 4) {
			wprintf(L"    OO    ");
		}
		else if (part == 1 || part == 2 || part == 3) {
			wprintf(L"OO      OO");
		}
	}
}

void print_row(int cellsInRow[3], int startIndex) {
	wprintf(L"██              ██              ██              ██\n");
	for (int part = 0; part < 5; part++) {
		wprintf(L"██  ");
		for (int i = 0; i < 3; i++) {
			print_cell_part(cellsInRow[i], part, i + startIndex);
			wprintf(L"  ██  ");
		}
		wprintf(L"\n");
	}
	wprintf(L"██              ██              ██              ██\n");
}

void game_print(Game* game) {
	wprintf(L"█████████████████████████");
	wprintf(L"█████████████████████████\n");
	for (int y = 0; y < 3; y++) {
		int cellsInRow[] = { game->cells[y * 3], game->cells[y * 3 + 1], game->cells[y * 3 + 2] };
		print_row(cellsInRow, y * 3);
		wprintf(L"█████████████████████████");
		wprintf(L"█████████████████████████\n");
	}
}

#endif


bool check(int* cells, int offset, int stepsize) {
	return cells[offset] != 0
		&& cells[offset] == cells[offset + stepsize]
		&& cells[offset] == cells[offset + stepsize * 2];
}

/**
 * @description Returns the winner of the given game by looking at the cell's states
 * @return int -1 if the game has not ended yet, 0 if the game is a draw, 1 if player1 won, 2 if player2 won
 */
int game_get_winner(Game* game) {
	// Todo: Don't even bother to check this when the game is not at least 5 moves old
	// A tic-tac-toe game is unwinnable before 5 moves have been made

	// Top Row
	if (check(game->cells, 0, 1)) {
		return game->cells[0];
	}

	// Middle row
	if (check(game->cells, 3, 1)) {
		return game->cells[3];
	}

	// Bottom row
	if (check(game->cells, 6, 1)) {
		return game->cells[6];
	}

	// Left column
	if (check(game->cells, 0, 3)) {
		return game->cells[0];
	}

	// Middle column
	if (check(game->cells, 1, 3)) {
		return game->cells[1];
	}

	// Right column
	if (check(game->cells, 2, 3)) {
		return game->cells[2];
	}

	// Diagonal: Top-Left <-> Bottom-Right
	if (check(game->cells, 0, 4)) {
		return game->cells[0];
	}

	// Diagonal: Top-Right <-> Bottom-Left
	if (check(game->cells, 2, 2)) {
		return game->cells[2];
	}

	if (game->cells[0] != 0
		&& game->cells[1] != 0
		&& game->cells[2] != 0
		&& game->cells[3] != 0
		&& game->cells[4] != 0
		&& game->cells[5] != 0
		&& game->cells[6] != 0
		&& game->cells[7] != 0
		&& game->cells[8] != 0) {
		// Alle Felder sind befüllt
		// Unentschieden
		return 0;
	}

	return -1;
}