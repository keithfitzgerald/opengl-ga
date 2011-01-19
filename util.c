#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include "util.h"
#include "display.h"

byte *ga_read_jpeg(char *filename) {

  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  int i, row;
  FILE * infile;		/* source file */
  JSAMPARRAY output;    /* Output row buffer */
  int row_stride;		/* physical row width in output buffer */

  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return NULL;
  }

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);

  (void) jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);

  row_stride = cinfo.output_width * cinfo.output_components;
  output = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  //R_HEIGHT = cinfo.output_height;
  //R_WIDTH = cinfo.output_width;

  printf("width = %d, height = %d\n",cinfo.output_width,cinfo.output_height);
 
  int bufsz = sizeof(byte) * 3 * R_HEIGHT * R_WIDTH;
  byte *buffer = (byte*)malloc(bufsz);

  row = 0;
  while (cinfo.output_scanline < cinfo.output_height) {
    jpeg_read_scanlines(&cinfo, output, 1);
	for (i = 0;i < row_stride;i += 3) {
        int idx = (row * row_stride) + i;
		buffer[idx] = output[0][i];
		buffer[idx+1] = output[0][i+1];
		buffer[idx+2] = output[0][i+2];
	}
	row++;
  }

  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);

  printf("read %d bytes\n",bufsz);
  
  return buffer;
}
