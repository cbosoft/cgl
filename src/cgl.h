#pragma once

typedef double cgl_float;
typedef unsigned int cgl_uint;

typedef struct {
  cgl_float *x;
  cgl_float *y;
  cgl_uint len;
  char *style;
} cgl_Line;

void cgl_plot();
