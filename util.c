#include <stdio.h>
#include <jpeglib.h>
#include "util.h"
#include "pixbuf.h"

void read_jpeg(char *filename, pixbuf *buffer) {

  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  int i, row;
  FILE * infile;		/* source file */
  JSAMPARRAY output;    /* Output row buffer */
  int row_stride;		/* physical row width in output buffer */

  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return;
  }

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);

  (void) jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);

  row_stride = cinfo.output_width * cinfo.output_components;
  output = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  //rgbImage->height = cinfo.output_height;
  //rgbImage->width = cinfo.output_width;

  row = 0;
  while (cinfo.output_scanline < cinfo.output_height) {
    (void) jpeg_read_scanlines(&cinfo, output, 1);
	for (i = 0;i < row_stride;i += 3) {
		buffer->data[i/3][row].r = (float)(output[0][i] / 256.0);
		buffer->data[i/3][row].g = (float)(output[0][i+1] / 256.0);
		buffer->data[i/3][row].b = (float)(output[0][i+2] / 256.0);
	}
	row++;
  }

  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);
}
