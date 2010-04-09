#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <errno.h>
#include <cairo.h>
#include <svg-cairo.h>
#include "arg.h"


cairo_status_t write_callback (void *closure, const unsigned char *data, unsigned int length);
svg_cairo_status_t write_surface_to_png_file (cairo_surface_t *surface, FILE *file);
svg_cairo_status_t render_to_png (FILE *svg_file, FILE *png_file, double scale, int width, int height);
