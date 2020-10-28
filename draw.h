#ifndef __DRAW_H_
#define __DRAW_H_

#define CONTEXT_MODE_DUMMY 0
#define CONTEXT_MODE_MMAP 1
#define CONTEXT_MODE_MALLOC 2

typedef struct {
  int* data;
  int width;
  int height;
} image_t;

typedef struct {
  int * data;
  int width;
  int height;
  unsigned char mode;
  char * fb_name;
  int fb_file_desc;
} context_t;

void invert_image(image_t * image);
void image_free(image_t * image);
void set_pixel(int x, int y, context_t * context, int color);
image_t * scale(image_t*image, int w, int h);
void draw_array(int x, int y, int w, int h, int* array, context_t* context);
void draw_image(int x, int y, image_t * image, context_t* context);
void draw_rect(int x, int y, int w, int h, context_t* context, int color);
void clear_context_color(context_t* context, int color);
void clear_context_gray(context_t* context, unsigned char gray);
int context_copy(context_t* from, context_t* to);
void clear_context(context_t* context);
void test_pattern(context_t* context);

void context_release(context_t * context);
context_t * context_create();
context_t * context_temporary (int width, int height);
context_t * context_get_dimensions();

#endif