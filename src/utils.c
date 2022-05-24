#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>

#include "utils.h"

void print_at(wchar_t* str, int x, int y) {
	set_coords(x, y);
	wprintf(str);
}

bool get_coords(int* x, int* y) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	bool ret = GetConsoleScreenBufferInfo(handle, &csbi);
	if (ret) {
		*x = csbi.dwCursorPosition.X;
		*y = csbi.dwCursorPosition.Y;
	}
	return ret;
}

void set_coords(int x, int y) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, (COORD) { x, y });
}

void flush_stdin(void) {
	while (getchar() != '\n');
}