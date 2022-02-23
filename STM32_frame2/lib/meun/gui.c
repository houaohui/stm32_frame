#include "gui.h"
#include "draw_api.h"

void gui_printf_chosechar(int x, int y, unsigned char c)
{
	int i,j;
  unsigned char a,b,size;
	size=GetFontSize();
  if ((x >= SCREEN_COLUMN) ||         // Clip right
    (y >= SCREEN_ROW) ||        // Clip bottom
    ((x + 5 * size - 1) < 0) ||   // Clip left
    ((y + 8 * size - 1) < 0)    // Clip top
  )
  {
    return;
  }

  for ( i=0; i<6; i++ )
  {
    int line;
    if (i == 5)
    {
      line = 0x0;
    }
    else
    {
      line = pgm_read_byte(font5x7+(c*5)+i);
    }

    for ( j = 0; j<8; j++)
    {
      unsigned char draw_color = (line & 0x1) ? 0 : 1;

		//去掉draw_background可以不改变背景
      if (draw_color || 1) {
        for ( a = 0; a < size; a++ ) {
          for ( b = 0; b < size; b++ ) {
            SetPointBuffer(x + (i * size) + a, y + (j * size) + b, draw_color);
          }
        }
      }
      line >>= 1;
    }
  }
}


void gui_printf_chosestring(int x, int y,char *str)
{
	unsigned char j=0,tempx=x,tempy=y;
	unsigned char size=GetFontSize();
	while (str[j]!='\0')
	{		
		//if(str[j]=='\n'||tempx+size*6>127)
		if(str[j]=='\n')
		{
			tempy+=8*size;
			tempx=x;
			j++;
			continue;
		}
		gui_printf_chosechar(tempx,tempy,str[j]);
		tempx+=size*6;
		j++;
	}
}

void gui_print_s(int x, int y,char *str)
{
	unsigned char j=0,tempx=x,tempy=y;
	unsigned char size=GetFontSize();
	while (str[j]!='\0')
	{		
		//if(str[j]=='\n'||tempx+size*6>127)
		if(str[j]=='\n')
		{
			tempy+=8*size;
			tempx=x;
			j++;
			continue;
		}
		DrawChar(tempx,tempy,str[j]);   
		tempx+=size*6;
		j++;
	}
}


void gui_print_d(unsigned char x,unsigned char y,unsigned int num,unsigned char len)
{
	DrawNum(x,y,num,len);
}
#include "string.h"
void gui_print_f(unsigned char x,unsigned char y, float num)
{
	char array[20];
	sprintf(array, "%.1f",num);
	gui_print_s(x,y,array);
}
