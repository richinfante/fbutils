#include "draw.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <linux/input.h>
#include <linux/fb.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <termios.h>
#include <linux/vt.h>
#include <signal.h>
#include <unistd.h>



void image_free(image_t * image) {
    free(image->data);
    image->width = 0;
    image->height = 0;
    image->data = NULL;
    free(image);
}

// Set an individual pixel. This is SLOW for bulk operations.
// Do as little as possible, and memcpy the result.
void set_pixel(int x, int y, context_t * context, int color) {
    int write_index = x+y*context->width;
    if (write_index < context->width * context->height) {
        context->data[x+y*context->width] = color;
    } else {
        printf("Attempted to set color #%x at x=%d, y=%d). (out of bounds)\n", color, x, y);
        exit(1);
    }
}

// We scale and crop the image to this new rect.
image_t * scale(image_t*image, int w, int h) {
    int sfx = w / image->width;
    int sfy = h / image->height;
    int crop_x_w = image->width;
    int crop_y_h = image->height;
    int crop_x = 0;
    int crop_y = 0;

    if (sfx < sfy) {
        crop_x_w = image->height * w / h;
        crop_x = (image->width - crop_x_w)  / 2;
    } else if(sfx > sfy) {
        crop_y_h = image->width * h / w;
        crop_y = (image->height - crop_y_h)  / 2;
    }

    image_t * new_image = malloc(sizeof(image_t));
    new_image->data = malloc(sizeof(int) * w * h);
    new_image->width = w;
    new_image->height = h;
    for(int x = 0; x < w; x++) {
        for(int y = 0; y < h; y++) {
            int tr_x = ((float) crop_x_w / (float) w) * x + crop_x;
            int tr_y = ((float) crop_y_h / (float) h) * y + crop_y;
            new_image->data[y * w + x] = image->data[tr_y * image->width + tr_x];
        }
    }

    return new_image;
    /*

    for(int x = 0; x < w; x++) {
        for(int y = 0; y < h; y++) {
            int tr_x = ((float) image->width / (float) w) * (float) x;
            int tr_y = ((float) image->height / (float) h) * (float) y;
            new_image->data[y * w + x] = image->data[tr_y * image->width + tr_x];
        }
    }
*/
}

// !! This operation is potentially unsafe. Use drawImage. It's harder to mess up.
// X and w are the size of the array.
void draw_array(int x, int y, int w, int h, int* array, context_t* context) {
  // Ignore draws out of bounds
  if (x > context->width || y > context->height) {
    return;
  }

  // Ignore draws out of bounds
  if (x + w < 0 || y + h < 0) {
    return;
  }

  // Column and row correction for partial onscreen images
  int cy = 0;
  int cx = 0; 

  // if y is less than 0, trim that many lines off the render.
  if (y < 0) {
    cy = -y;
  }

  // If x is less than 0, trim that many pixels off the render line.
  if (x < 0) {
    cx = -x;
  }

  // Number of items in a line
  int line_width = (w - cx);

  // Number of lines total.
  // We don't subtract cy because the loop starts with cy already advanced.
  int line_count = h;

  // If the end of the line goes offscreen, trim that many pixels off the
  // row.
  if (x + w > context->width) {
    line_width -= ((x + w) - context->width);
  }

  // If the number of rows is more than the height of the context, trim
  // them off.
  if (y + h > context->height) {
    line_count -= ((y + h) - context->height);
  }

  for (cy; cy < line_count; cy++) {
    // Draw each graphics line.
    memcpy(
        &context->data[context->width * y + context->width * cy + x + cx], 
        &array[cy * w] + cx, 
        sizeof(int) * line_width
    );
  }
}

void draw_image(int x, int y, image_t * image, context_t* context) {
    draw_array(x, y, image->width, image->height, image->data, context);
}

void draw_rect(int x, int y, int w, int h, context_t* context, int color) {
    // Ignore draws out of bounds
    if (x > context->width || y > context->height) {
        return;
    }

    // Ignore draws out of bounds
    if(x + w < 0 || y + h < 0) {
        return;
    }
    // Trim offscreen pixels
    if (x < 0) {
        w += x;
        x = 0;
    }

    // Trim offscreen lines
    if (y < 0) {
        h += y;
        y = 0;
    }

    // Trim offscreen pixels
    if (x + w > context->width) {
        w -= ((x + w) - context->width);
    }

    // Trim offscreen lines.
    if (y + h > context->height) {
       h -= ((y + h) - context->height);
    }

    // Set the first line.
    for (int rx = x; rx < x+w; rx++) {
        set_pixel(rx, y, context, color);
    }

    // Repeat the first line.
    for (int ry = 1; ry < h; ry++) {
        memcpy(
            &context->data[context->width * y + context->width * ry + x], &
            context->data[context->width * y + x], 
            w*sizeof(int)
        );
    }
}

void clear_context_color(context_t* context, int color) {
    draw_rect(0, 0, context->width, context->height, context, color);
}

void clear_context(context_t* context) {
    memset(context->data, 0, context->width * context->height * sizeof(int));  
}

void test_pattern(context_t* context) {
  const unsigned int pattern[8] =
  {
    0xFFFFFF,
    0xFFFF00,
    0x00FFFF,
    0x00FF00,
    0xFF00FF,
    0xFF0000,
    0x0000FF,
    0x000000
  };

    unsigned columnWidth = context->width / 8;
    for(int rx = 0; rx < context->width; rx++) {
        set_pixel(rx, 0, context, pattern[rx / columnWidth]);
    }

    // make it faster: memcpy the first row.
    for(int y = 1; y < context->height; y++) {
        memcpy(&context[context->width * y], context, context->width*sizeof(int));
    }
}

void context_release(context_t * context) {
    munmap(context->data, context->width * context->height);
    close(context->fb_file_desc);
    context->data = NULL;
    context->fb_file_desc = 0;
    free(context);
}

context_t * context_create() {
    char *FB_NAME = "/dev/fb0";
    void* mapped_ptr = NULL;
    struct fb_fix_screeninfo fb_fixinfo;
    struct fb_var_screeninfo fb_varinfo;
    int fb_file_desc;
    int fb_size = 0;

    // Open the framebuffer device in read write
    fb_file_desc = open(FB_NAME, O_RDWR);
    if (fb_file_desc < 0) {
        printf("Unable to open %s.\n", FB_NAME);
        return NULL;
    }
    //Do Ioctl. Retrieve fixed screen info.
    if (ioctl(fb_file_desc, FBIOGET_FSCREENINFO, &fb_fixinfo) < 0) {
        printf("get fixed screen info failed: %s\n",
               strerror(errno));
        close(fb_file_desc);
        return NULL;
    }
    // Do Ioctl. Get the variable screen info.
    if (ioctl(fb_file_desc, FBIOGET_VSCREENINFO, &fb_varinfo) < 0) {
        printf("Unable to retrieve variable screen info: %s\n",
               strerror(errno));
        close(fb_file_desc);
        return NULL;
    }

    // Calculate the size to mmap
    fb_size = fb_fixinfo.line_length * fb_varinfo.yres;
    
    // Now mmap the framebuffer.
    mapped_ptr = mmap(NULL, fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_file_desc,0);

    if (mapped_ptr == NULL) {
        printf("mmap failed:\n");
        close(fb_file_desc);
        return NULL;
    }

    context_t * context = malloc(sizeof(context_t));
    context->data = (int *) mapped_ptr;
    context->width = fb_fixinfo.line_length / 4;
    context->height = fb_varinfo.yres;
    context->fb_file_desc = fb_file_desc;
    context->fb_name = FB_NAME;
    return context;
}



