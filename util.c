#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include "util.h"

float getRandomPointX() {
	return (float)(rand() % R_HEIGHT);
}

float getRandomPointY() {
	return (float)(rand() % R_WIDTH);
}

float getRandomColor() {
	return (float)(rand()/(RAND_MAX * 1.0)); 
}

polygon* gen_random_image() {
	int i,j;
	polygon *image = (polygon*)malloc(sizeof(polygon) * MAX_POLYGONS); 
	for (i = 0; i < MAX_POLYGONS;i++) {
		polygon *p = &image[i];
		p->num_vertices = 0;

		set_color(p,getRandomColor(),getRandomColor(),getRandomColor(),getRandomColor());

		for (j = 0; j < 3;j++) {
			add_vertex(p,getRandomPointX(),getRandomPointY());
		}
	}

	return image;
}

void free_image(polygon *image) {
	free(image);
}

int readJPEG(char *filename, bitmap *rgbImage) {

  struct jpeg_decompress_struct cinfo;
  
  struct jpeg_error_mgr jerr;

  int i, row;
  FILE * infile;		/* source file */
  JSAMPARRAY buffer;    /* Output row buffer */
  int row_stride;		/* physical row width in output buffer */

  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return 0;
  }

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);

  (void) jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);

  row_stride = cinfo.output_width * cinfo.output_components;
  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  printf("image dimension: %d x %d\n",cinfo.output_width,cinfo.output_height);
  rgbImage->height = cinfo.output_height;
  rgbImage->width = cinfo.output_width;

  /* Step 6: while (scan lines remain to be read) */
  /*           jpeg_read_scanlines(...); */

  /* Here we use the library's state variable cinfo.output_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   */

  row = 0;
  while (cinfo.output_scanline < cinfo.output_height) {
    /* jpeg_read_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could ask for
     * more than one scanline at a time if that's more convenient.
     */
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    /* Assume put_scanline_someplace wants a pointer and sample count. */
    //put_scanline_someplace(buffer[0], row_stride);

	//printf("row stride: %d\n", row_stride);
	
	for (i = 0;i < row_stride;i = i+3) {
		rgbImage->data[i/3][row].r = (float)(buffer[0][i] / 256.0);
		rgbImage->data[i/3][row].g = (float)(buffer[0][i+1] / 256.0);
		rgbImage->data[i/3][row].b = (float)(buffer[0][i+2] / 256.0);
	}
	row++;
  }

  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);

  /* At this point you may want to check to see whether any corrupt-data
   * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
   */
  return 1;
}
