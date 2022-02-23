#ifndef _GUI_H
#define _GUI_H


#include "draw_api.h"
#include "stdint.h"
#include "common.h"

#define LINE(n)              ((n-1)*_FontSize*8)


void gui_printf_chosestring(int x, int y,char *str);
void gui_print_s(int x, int y,char *str);

void gui_print_f(unsigned char x,unsigned char y, float num);
void gui_print_d(unsigned char x,unsigned char y,unsigned int num,unsigned char len);
#endif /*_GUI_H*/
