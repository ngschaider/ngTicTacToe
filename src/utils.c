#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>

#include "utils.h"

void set_attr(int attr) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, attr);
}

void color_red() {
	set_attr(FOREGROUND_RED);
}

void color_white() {
	set_attr(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void color_cyan() {
	set_attr(FOREGROUND_BLUE | FOREGROUND_GREEN);
}

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