#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

extern void print_at(wchar_t*, int, int);

extern void color_red();
extern void color_white();
extern void color_cyan();

extern bool get_coords(int*, int*);

extern void set_coords(int, int);

extern void flush_stdin(void);

#endif
