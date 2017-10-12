#ifndef __FONT_H__
#define __FONT_H__

#include "draw.h"

typedef struct {
  int* data;
  int width;
  int height;
  int baseline_offset;
  int centerline_offset;
} glyph_t;

typedef struct {
  glyph_t * map;
  int size;
  int max_height;
  int max_width;
} fontmap_t;

void fontmap_free(fontmap_t* fontmap);
fontmap_t * fontmap_default();

void draw_string(int x, int y, char * strint, fontmap_t * fontmap, context_t * context);

#endif