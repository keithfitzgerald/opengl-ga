#include <jpeglib.h>
#include <stdlib.h>

typedef struct {
  struct jpeg_source_mgr pub;   // public fields

  JOCTET * buffer;              // start of buffer
  boolean start_of_file;        // have we gotten any data yet?
} my_source_mgr;

typedef my_source_mgr * my_src_ptr;

static void jpg_memInitSource(j_decompress_ptr cinfo)
{
  my_src_ptr src = (my_src_ptr) cinfo->src;
  src->start_of_file = true;
}

static boolean jpg_memFillInputBuffer(j_decompress_ptr cinfo)
{
  my_src_ptr src = (my_src_ptr) cinfo->src;
  src->start_of_file = FALSE;
  return TRUE;
}

static void jpg_memSkipInputData(j_decompress_ptr cinfo, long num_bytes)
{
  my_src_ptr src = (my_src_ptr) cinfo->src;
  if (num_bytes > 0) {
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}

static void jpg_memTermSource(j_decompress_ptr cinfo)
{
  // no work necessary here
}

void decompressJpeg(uint8 const *compressed, size_t len)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr         jerr;
  my_src_ptr src;

  // ToDo: change error handling to work with our return codes
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);

  cinfo.src = (struct jpeg_source_mgr *)
    (*cinfo.mem->alloc_small) ((j_common_ptr) &cinfo, JPOOL_PERMANENT,
    sizeof(my_source_mgr));
  src = (my_src_ptr) cinfo.src;
  src->buffer = (JOCTET *)compressed;

  src->pub.init_source = jpg_memInitSource;
  src->pub.fill_input_buffer = jpg_memFillInputBuffer;
  src->pub.skip_input_data = jpg_memSkipInputData;
  src->pub.resync_to_restart = jpeg_resync_to_restart;
  src->pub.term_source = jpg_memTermSource;
  src->pub.bytes_in_buffer = len;
  src->pub.next_input_byte = compressed;

  jpeg_read_header(&cinfo, true);
  jpeg_start_decompress(&cinfo);

  // check: cinfo.out_color_space == JCS_RGB

  currImage.setSize(
    (int)cinfo.output_width, (int)cinfo.output_height));

  size_t w = currImage.getWidth() * 3;
  uint8 *p = (uint8*)currImage.getPixels();
  while (cinfo.output_scanline < cinfo.output_height) {
    jpeg_read_scanlines(&cinfo, &p, 1);
    p += w;
  }

  jpeg_finish_decompress(&cinfo); // Do this even under error conditions
  jpeg_destroy_decompress(&cinfo); // Do this even under error conditions
}