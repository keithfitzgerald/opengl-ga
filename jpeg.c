#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include "jpeg.h"

ga_jpeg *ga_read_jpeg(char *filename) {

  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * infile;		/* source file */

  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return NULL;
  }

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);

  (void) jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);

  int row_stride = cinfo.output_width * cinfo.output_components;
  JSAMPARRAY output = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  ga_jpeg *jpeg = (ga_jpeg*)malloc(sizeof(ga_jpeg));
  
  jpeg->height = cinfo.output_height;
  jpeg->width = cinfo.output_width;

  printf("jpeg width = %d, height = %d\n",cinfo.output_width,cinfo.output_height);
 
  int bufsz = sizeof(uint8_t) * 3 * jpeg->height * jpeg->width;
  jpeg->buffer = (byte*)malloc(bufsz);

  int row = 0;
  while (cinfo.output_scanline < cinfo.output_height) {
    jpeg_read_scanlines(&cinfo, output, 1);
	for (int i = 0;i < row_stride;i += 3) {
        int idx = (row * row_stride) + i;
		jpeg->buffer[idx] = output[0][i];
		jpeg->buffer[idx+1] = output[0][i+1];
		jpeg->buffer[idx+2] = output[0][i+2];
	}
	row++;
  }

  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);

  printf("read %d bytes\n",bufsz);
  
  return jpeg;
}

void ga_free_jpeg(ga_jpeg *jpeg) {
    free(jpeg->buffer);
    free(jpeg);
}
