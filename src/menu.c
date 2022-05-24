#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <assert.h>

#include "menu.h"
#include "utils.h"



Menu MENU_create(wchar_t** entries, int size) {
	Menu menu = {
		entries,
		size,
		0
	};

	return menu;
}

bool MENU_select(Menu* menu, int index) {
	if (index >= menu->size) {
		return false;
	}
	menu->selected = index;
	return true;
}

void MENU_print(Menu* menu) {
	for (int i = 0; i < menu->size; i++) {
		if (i == menu->selected) {
			color_cyan();
			wprintf(L"→");
		}
		else {
			wprintf(L" ");
		}
		wprintf(L"[%d] ", i + 1);
		wchar_t* entry = menu->entries[i];
		wprintf(L"%ls", entry);
		wprintf(L"\n");

		color_white();
	}
}

int MENU_prompt(Menu* menu) {
	int original_x;
	int original_y;
	if (!get_coords(&original_x, &original_y)) {
		assert(false);
		return -1;
	}
	
	while (true) {
		MENU_print(menu);

		int input = getch();
		if (input == 0xE0) {
			int which = getch();
			if (which == 72) {
				// UP
				menu->selected--;
				if (menu->selected < 0) {
					menu->selected = menu->size - 1;
				}
			}
			else if (which == 80) {
				// Down
				menu->selected++;
				if (menu->selected >= menu->size) {
					menu->selected = 0;
				}
			}
			else if (which == 75) {
				// Left
			}
			else if (which == 77) {
				// Right
			}
		}
		else if (input == 0x0D) {
			// Return
			return menu->selected + 1;
		}
		set_coords(original_x, original_y);
	}
}