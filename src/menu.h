#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

typedef struct MenuStruct {
	wchar_t** entries;
	int size;
	int selected;
} Menu;


extern Menu MENU_create(wchar_t**, int);
extern int MENU_prompt(Menu*);

#endif // MENU_H_INCLUDED