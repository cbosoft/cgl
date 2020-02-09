#pragma once
#include <gtk/gtk.h>

typedef double cgl_float;
typedef unsigned int cgl_uint;

typedef struct {
  cgl_float x;
  cgl_float y;
} cgl_Point;

typedef struct {
  cgl_float r;
  cgl_float g;
  cgl_float b;
  cgl_float a;
} cgl_LineStyle;

typedef struct {
  cgl_Point **points;
  cgl_uint npoints;
  cgl_LineStyle *style;
} cgl_Line;

typedef struct {
  char *x_label;
  char *y_label;
  cgl_float x_lim[2];
  cgl_float y_lim[2];
} cgl_Axes;

typedef struct {
  cgl_Axes *axes;
  cgl_Line **lines;
  cgl_uint nlines;
  /*char *title*/
} cgl_Figure;

cgl_Figure *cgl_init_figure();
cgl_Axes *cgl_create_axes();
cgl_Line *cgl_create_line();
cgl_LineStyle *cgl_create_style();

char *cgl_axes_get_ylabel(cgl_Axes *ax);
char *cgl_axes_get_xlabel(cgl_Axes *ax);
void cgl_axes_set_xlabel(cgl_Axes *ax, char *lbl);
void cgl_axes_set_xlabel(cgl_Axes *ax, char *lbl);

void cgl_axes_get_xlim(cgl_Axes *ax, cgl_float *left, cgl_float *right);
void cgl_axes_get_ylim(cgl_Axes *ax, cgl_float *bottom, cgl_float *top);
void cgl_axes_set_xlim(cgl_Axes *ax, cgl_float left, cgl_float right);
void cgl_axes_set_ylim(cgl_Axes *ax, cgl_float bottom, cgl_float top);

void cgl_plot (cgl_Figure *figure, cgl_float *x, cgl_float *y, cgl_uint npoints);
gboolean cgl_painter_cb (GtkWidget *widget, cairo_t *cr, gpointer data);

// vim: ft=c
