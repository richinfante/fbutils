#ifndef __DRAW_H_
#define __DRAW_H_

typedef struct {
  int* data;
  int width;
  int height;
} image_t;

typedef struct {
  int * data;
  int width;
  int height;
  char * fb_name;
  int fb_file_desc;
} context_t;

void image_free(image_t * image);
void set_pixel(int x, int y, context_t * context, int color);
image_t * scale(image_t*image, int w, int h);
void draw_array(int x, int y, int w, int h, int* array, context_t* context);
void draw_image(int x, int y, image_t * image, context_t* context);
void draw_rect(int x, int y, int w, int h, context_t* context, int color);
void clear_context_color(context_t* context, int color);
void clear_context(context_t* context);
void test_pattern(context_t* context);

void context_release(context_t * context);
context_t * context_create();

#endif