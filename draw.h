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

// Commonly used mask values
#define MASK_RED 0xFF0000
#define MASK_GREEN 0x00FF00
#define MASK_BLUE 0x0000FF
#define MASK_YELLOW 0xFFFF00
#define MASK_CYAN 0x00FFFF
#define MASK_MAGENTA 0xFF00FF
#define MASK_WHITE 0xFFFFFF
#define MASK_NONE 0x000000

// Hue threshold of 20 is the default
#define HUE_THRESHOLD_DEFAULT 20

void grayscale_image(image_t * image);
void invert_image(image_t * image);
void hueify_image(image_t * image, unsigned int mask, int min_level);
void image_free(image_t * image);
void set_pixel(int x, int y, context_t * context, int color);
image_t * scale(image_t*image, int w, int h);
void draw_array(int x, int y, int w, int h, int* array, context_t* context);
void draw_image(int x, int y, image_t * image, context_t* context);
void draw_rect(int x, int y, int w, int h, context_t* context, int color);
void clear_context_color(context_t* context, int color);
void clear_context_gray(context_t* context, unsigned char gray);
void clear_context(context_t* context);
void test_pattern(context_t* context);

void context_release(context_t * context);
context_t * context_create();
context_t * context_get_dimensions();

#endif
