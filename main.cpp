#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "canvlib.h"

int buffer[32][64];

int main() {
  int x = 20;
  fcanv k = fcanv_new((int*)buffer, 64, 32);

  fcanv_setstring(&k, "\xa""A\1BC\2DE\3FGHIJKL\4MN\5O\6PRS\7TUVWXYZ\x8\x9 1234567890.,:!");
  fcanv_addcolor(&k, 0, mkcolor(255, 0, 0));
  fcanv_addcolor(&k, 3, mkcolor(255, 255, 0));
  fcanv_addcolor(&k, 4, mkcolor(0, 0, 0));
  fcanv_addcolor(&k, 5, mkcolor(0, 0, 255));
  for(int i=0;i<99999;i++) {
    system("clear");
    fcanv_flush(&k);
    fcanv_draw(&k, x, 3);
    for(int y = 0;y<32;y++) {
      for(int x=0;x<64;x++)
        if(buffer[y][x])
        putchar((cR(buffer[y][x]) + cG(buffer[y][x]) + cB(buffer[y][x])) % 52 + 'A');
        else putchar('.');
      putchar(10);
    }
    x--;
    printf("%d\n", fcanv_textwidth(&k));
    fcanv_resetcolors(&k);
    usleep(40000);
  }
  return 0;
}
