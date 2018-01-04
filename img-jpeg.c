/*
 * example.c
 *
 * This file illustrates how to use the IJG code as a subroutine library
 * to read or write JPEG image files.  You should look at this code in
 * conjunction with the documentation file libjpeg.doc.
 *
 * This code will not do anything useful as-is, but it may be helpful as a
 * skeleton for constructing routines that call the JPEG library.  
 *
 * We present these routines in the same coding style used in the JPEG code
 * (ANSI function definitions, etc); but you are of course free to code your
 * routines in a different style if you prefer.
 */

 #include <stdlib.h>
 #include <stdio.h> 
 #include <string.h>
 #include <setjmp.h>
 #include "jpeglib.h"
 #include "img-jpeg.h"
 
// Adapted libjpeg demo to write to our image_t struct.

 struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

void  my_error_exit (j_common_ptr cinfo)
{
  printf("Error!\n");
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

image_t* read_jpeg_file (char * filename) {
  printf("Read JPEG %s\n", filename);
   /* This struct contains the JPEG decompression parameters and pointers to
    * working space (which is allocated as needed by the JPEG library).
    */
   struct jpeg_decompress_struct cinfo;
   /* We use our private extension JPEG error handler.
    * Note that this struct must live as long as the main JPEG parameter
    * struct, to avoid dangling-pointer problems.
    */
   struct my_error_mgr jerr;
   /* More stuff */
   FILE * infile;		/* source file */
   JSAMPARRAY buffer;		/* Output row buffer */
   int row_stride;		/* physical row width in output buffer */
 
   /* In this example we want to open the input file before doing anything else,
    * so that the setjmp() error recovery below can assume the file is open.
    * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
    * requires it in order to read binary files.
    */
 
   if ((infile = fopen(filename, "rb")) == NULL) {
     fprintf(stderr, "can't open %s\n", filename);
     return 0;
   }
   
 
   /* Step 1: allocate and initialize JPEG decompression object */
 
   /* We set up the normal JPEG error routines, then override error_exit. */
   cinfo.err = jpeg_std_error(&jerr.pub);
   jerr.pub.error_exit = my_error_exit;
   /* Establish the setjmp return context for my_error_exit to use. */
   if (setjmp(jerr.setjmp_buffer)) {
     /* If we get here, the JPEG code has signaled an error.
      * We need to clean up the JPEG object, close the input file, and return.
      */
     jpeg_destroy_decompress(&cinfo);
     fclose(infile);
     return 0;
   }

   
   jpeg_create_decompress(&cinfo);
   jpeg_stdio_src(&cinfo, infile);
   (void) jpeg_read_header(&cinfo, TRUE);
   cinfo.out_color_space = JCS_EXT_BGRX;
   (void) jpeg_start_decompress(&cinfo);
   
   row_stride = cinfo.output_width * cinfo.output_components;
   /* Make a one-row-high sample array that will go away when done with image */
   buffer = (*cinfo.mem->alloc_sarray)
     ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  image_t * image = malloc(sizeof(image_t));
  image->data = malloc(sizeof(int) * cinfo.output_width * cinfo.output_height);
  image->width = cinfo.output_width;
  image->height = cinfo.output_height;
  
#if DEBUG
  printf("JPEG %dx%d\n", image->width, image->height);
  printf("output components: %d, stride: %d\n", cinfo.output_components, row_stride);
#endif
  int row = 0;

   while (cinfo.output_scanline < cinfo.output_height) {
     /* jpeg_read_scanlines expects an array of pointers to scanlines.
      * Here the array is only one element long, but you could ask for
      * more than one scanline at a time if that's more convenient.
      */
     (void) jpeg_read_scanlines(&cinfo, buffer, 1);

      // for(int i = 0; i < row_stride; i+=3) {
      //   int hexval = ((buffer[0][i] << 16) & 0xFF0000) | ((buffer[0][i+1] << 8) & 0x00FF00) | (buffer[0][i+2] & 0x0000FF);
      //   image->data[row * image->width + (i / cinfo.output_components)] = hexval;
      // }
     memcpy(&image->data[row * image->width], buffer[0], row_stride);
     row += 1;
   }
 
   /* Step 7: Finish decompression */
 
   (void) jpeg_finish_decompress(&cinfo);
   /* We can ignore the return value since suspension is not possible
    * with the stdio data source.
    */
 
   /* Step 8: Release JPEG decompression object */
 
   /* This is an important step since it will release a good deal of memory. */
   jpeg_destroy_decompress(&cinfo);
 
   /* After finish_decompress, we can close the input file.
    * Here we postpone it until after no more JPEG errors are possible,
    * so as to simplify the setjmp error logic above.  (Actually, I don't
    * think that jpeg_destroy can do an error exit, but why assume anything...)
    */
   fclose(infile);
 
   /* At this point you may want to check to see whether any corrupt-data
    * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
    */
 
#if DEBUG
   printf("JPEG done.\n");
#endif
   /* And we're done! */
   return image;
 }
 
 
 /*
  * SOME FINE POINTS:
  *
  * In the above code, we ignored the return value of jpeg_read_scanlines,
  * which is the number of scanlines actually read.  We could get away with
  * this because we asked for only one line at a time and we weren't using
  * a suspending data source.  See libjpeg.doc for more info.
  *
  * We cheated a bit by calling alloc_sarray() after jpeg_start_decompress();
  * we should have done it beforehand to ensure that the space would be
  * counted against the JPEG max_memory setting.  In some systems the above
  * code would risk an out-of-memory error.  However, in general we don't
  * know the output image dimensions before jpeg_start_decompress(), unless we
  * call jpeg_calc_output_dimensions().  See libjpeg.doc for more about this.
  *
  * Scanlines are returned in the same order as they appear in the JPEG file,
  * which is standardly top-to-bottom.  If you must emit data bottom-to-top,
  * you can use one of the virtual arrays provided by the JPEG memory manager
  * to invert the data.  See wrbmp.c for an example.
  *
  * As with compression, some operating modes may require temporary files.
  * On some systems you may need to set up a signal handler to ensure that
  * temporary files are deleted if the program is interrupted.  See libjpeg.doc.
  */
