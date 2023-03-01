#include "font.h"
#include <string.h>

#define cR(x) ((x>>16)&255)
#define cG(x) ((x>>8)&255)
#define cB(x) ((x)&255)

#define DEFCOL 0x00FFFFFF

struct lnode {
  lnode* next;
  int k, v;
};

struct fcanv {
  int* buffer;
  char* str;
  int w, h;
  lnode* colors;
};

static void list_add(lnode** l, int k, int v) {
  if(!l) {
    l[0] = (lnode*) malloc(sizeof(lnode));
    l[0]->next = 0;
    l[0]->k = k;
    l[0]->v = v;
  } else {
    lnode* prev = 0;
    lnode* c;
    for(c = *l;c;c=c->next) {
      if(c->k == k) {
        c->v = v;
        break;
      } else if(c->k > k) {
        printf("NOTK\n");
        lnode* n = (lnode*) malloc(sizeof(lnode));
        n->k = k;
        n->v = v;
        n->next = c;
        if(prev) prev->next = n;
        break;
      }

      prev = c;
    }
    if(!c) {
      printf("NOTC\n");
      lnode* tmp;
      if(!prev) {
        tmp = (lnode*) malloc(sizeof(lnode));
        *l = tmp;
      } else tmp = prev->next = (lnode*) malloc(sizeof(lnode));
      tmp->k = k;
      tmp->v = v;
      tmp->next = 0;
    }
  }
}

static void list_free(lnode** l) {
  lnode* prev = 0;
  for(lnode* c = *l;c;c=c->next) {
    free(prev);
    prev = c;
  }
  free(prev);
  *l = 0;
}

static void list_print(lnode* l) {
  for(lnode* c = l;c;c=c->next) {
    printf("%d %d\n", c->k, c->v);
  }
}

int mkcolor(unsigned char r, unsigned char g, unsigned char b) {
  int color = 0;
  color |= r;
  color <<= 8;
  color |= g;
  color <<= 8;
  color |= b;
  return color;
}

static void printcolor(int color) {
  printf("%d %d %d\n", cR(color), cG(color), cB(color));
}

static char nthbit(unsigned char* arr, int n) {
  int i = n/8;
  int p = n % 8;
  return (arr[i] >> p) & 1;
}

static char inrect(int w, int h, int px, int py) {
  return ((px < w && px >= 0) && (py < h && py >= 0));
}

fcanv fcanv_new(int* b, int w, int h) {
  fsetoffsets();
  fcanv c;
  c.buffer = b;
  memset(b, 0, w * h * sizeof(int));
  c.w = w; c.h = h;
  c.str = 0;
  c.colors = 0;
  return c;
}

void fcanv_setstring(fcanv* canvas, char* str) {
  canvas->str = str;
}

void fcanv_flush(fcanv* c) {
  memset(c->buffer, 0, c->w * c->h * sizeof(int));
}

int fcanv_textwidth(fcanv* k) {
  char* cstr = k->str;
  char c;
  int w = 0;
  while(c = *cstr++) {
    w += funmapw(c);
  }
  return w;
}

void fcanv_addcolor(fcanv* c, int pos, int color) {
  list_add(&c->colors, pos, color);
}

void fcanv_resetcolors(fcanv* c) {
  list_free(&c->colors);
}

void fcanv_draw(fcanv* c, int x, int y) {
  int _COLOR = DEFCOL;
  lnode* l_el = c->colors;
  if(!c->str) return;
  int cX = x;
  int cY = y;
  int (*buffer)[c->w] = (int(*)[c->w])c->buffer;
  char* cstr = c->str;
  int ind = 0;
  while(*cstr) {
    if(l_el)
      if(l_el->k == ind) {
        _COLOR = l_el->v;
        l_el = l_el->next;
      }
    int lW, lH;
    unsigned char* letter = fgetletter(funmap(*cstr++), &lW, &lH);
    int tW = ceildiv(lW, 8) * 8;
    if(cX < c->w) {
    for(int Y=0;Y<lH;Y++)
      for(int X=0;X<lW;X++) {
        if(nthbit(letter, Y * tW + X)) {
          int nX = X + cX;
          int nY = Y + cY;
          if(inrect(c->w, c->h, nX, nY))
            buffer[nY][nX] = _COLOR;
        }
      }
    }
    cX += lW;
    ind++;
  }
}
