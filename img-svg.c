#include <librsvg/rsvg.h>
#include <cairo.h>
#include <cairo-svg.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include "draw.h"
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

image_t * read_svg_file(char* filename, int requested_width, int requested_height) {
  struct stat stat;
  
  int fd = open(filename, O_RDONLY);
  fstat(fd, &stat);
  unsigned char*file_contents = malloc(sizeof(char) * stat.st_size);

  // Read contents
  read(fd, file_contents, stat.st_size);

  // Load from SVG data
  GError *err = NULL;
  RsvgHandle *svg = rsvg_handle_new_from_data (file_contents, stat.st_size, &err);
  if(err != NULL) {
    printf("SVG Load Error: %s", err->message);
  }

  // Get sizing data
  RsvgDimensionData dimensions;
  rsvg_handle_get_dimensions(svg, &dimensions);

  //scale into the requested resolution
  double width;
  double height;
  double scale_width = 1;
  double scale_height = 1;

  // Some auto-scaling logic for the image.
  if (requested_width != 0 && requested_height != 0) {
    // If both width and height are nonzero, draw using given size
    width = requested_width;
    height = requested_height;
    scale_width = width / dimensions.width;
    scale_height = height / dimensions.height;
  } else if (requested_width == 0 && requested_height == 0) {
    // If both are zero, autosize.
    width = dimensions.width;
    height = dimensions.height;
    scale_width = 1;
    scale_height = 1;
  } else if (requested_width != 0) {
    // Auto-scale the height if 0
    width = requested_width;
    height = round(dimensions.height * scale_height);
    scale_width = width / dimensions.width;
    scale_height = scale_width;
  } else {
    // Auto-scale the width if 0
    width = round(dimensions.width * scale_width);
    height = requested_height;
    scale_height = height / dimensions.height;
    scale_width = scale_height;
  }
  
  // Allocate an output image
  image_t* out_image = malloc(sizeof(image_t));
  out_image->data = malloc(sizeof(int) * width * height);
  out_image->width = width;
  out_image->height = height;

  // Create a canvas
  cairo_surface_t *canvas = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cairo_t *cr = cairo_create(canvas);
  cairo_scale(cr, scale_width, scale_height);

  // Render
  if (!rsvg_handle_render_cairo(svg, cr)) {
    printf("SVG Render Error!\n");
  }

  // Extract the image stride
  int stride = cairo_image_surface_get_stride(canvas); //should be equal to width * channels
  int size = stride * height;

  // Flush + extract image
  cairo_surface_flush(canvas);
  memcpy(out_image->data, cairo_image_surface_get_data(canvas), size);

  // Cleanup
  g_object_unref(svg);
  cairo_surface_destroy(canvas);
  cairo_destroy(cr);

  return out_image;
}