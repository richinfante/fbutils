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

image_t * read_svg_file(char* filename, int r_width, int r_height) {
  struct stat stat;
  
  int fd = open(filename, O_RDONLY);
  fstat(fd, &stat);
  unsigned char*file_contents = malloc(sizeof(char) * stat.st_size);

  read(fd, file_contents, stat.st_size);

  GError *err = NULL;
  RsvgHandle *svg = rsvg_handle_new_from_data (file_contents, stat.st_size, &err);
  if(err != NULL) {
    printf("Failed to parse svg: %s", err->message);
  }

  RsvgDimensionData dimensions;
  rsvg_handle_get_dimensions(svg, &dimensions);

  //scale into the requested resolution
  double width;
  double height;
  double sx;
  double sy;

  // Some auto-scaling logic for the image.
  if (r_width == 0 && r_height == 0){
    width = dimensions.width;
    height = dimensions.height;
    sx = sy = 1;
  } else if(r_width != 0 && r_height != 0){
    width = r_width;
    height = r_height;
    sx = width / dimensions.width;
    sy = height / dimensions.height;
  } else if(r_width != 0){
    width = r_width;
    sx = sy = width / dimensions.width;
    height = round(dimensions.height * sy);
  } else {
    height = r_height;
    sx = sy = height / dimensions.height;
    width = round(dimensions.width * sx);
  }

  image_t* out_image = malloc(sizeof(image_t));
  out_image->data = malloc(sizeof(int) * width * height);
  out_image->width = width;
  out_image->height = height;

  cairo_surface_t *canvas = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cairo_t *cr = cairo_create(canvas);
  cairo_scale(cr, sx, sy);

  if (!rsvg_handle_render_cairo(svg, cr)) {
    printf("Cairo failed to render svg\n");
  }

  int stride = cairo_image_surface_get_stride(canvas); //should be equal to width * channels
  int size = stride * height;

  cairo_surface_flush(canvas);
  memcpy(out_image->data, cairo_image_surface_get_data(canvas), size);

  g_object_unref(svg);
  cairo_surface_destroy(canvas);
  cairo_destroy(cr);

  return out_image;
}