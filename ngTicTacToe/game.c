#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "game.h"
#include "utils.h"

Game game_create(int mode) {
	Game game = {
		{ // cells
			0, 0, 0,
			0, 0, 0,
			0, 0, 0
		},
		mode, // mode
		1, // currentPlayer
		0, // gamesTotal
		0, // gamesWon1
		0, // gamesLost1
		0, // gamesWon2
		0, // gamesLost2
	};

	return game;
}

void game_print_stats(Game* game) {
	if (game->mode == 1) {
		wprintf(L"Spiele gesamt: %d\tSpieler 1 gewonnen: %d\tSpieler 2 gewonnen: %d\n", game->gamesTotal, game->gamesWon1, game->gamesWon2);
	}
	else {
		wprintf(L"Spiele gesamt: %d\tSpieler 1 gewonnen: %d\tComputer gewonnen: %d\n", game->gamesTotal, game->gamesWon1, game->gamesWon2);
	}
	
}

bool check(int* cells, int offset, int stepsize) {
	return cells[offset] != 0
		&& cells[offset] == cells[offset + stepsize]
		&& cells[offset] == cells[offset + stepsize * 2];
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

int game_get_winner(Game* game) {
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
	if (check(game->cells, 3, 3)) {
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
		&& game->cells[0] == game->cells[1]
		&& game->cells[1] == game->cells[2]
		&& game->cells[2] == game->cells[3]
		&& game->cells[3] == game->cells[4]
		&& game->cells[4] == game->cells[5]
		&& game->cells[5] == game->cells[6]
		&& game->cells[6] == game->cells[7]
		&& game->cells[7] == game->cells[8]) {
		// Alle Felder sind befüllt
		// Unentschieden
		return -1;
	}

	return 0;
}