#include <stdio.h>

#include <gtk/gtk.h>

#include "cgl.h"


cgl_Figure *cgl_init_figure()
{
  cgl_Figure *fig = calloc(1, sizeof(cgl_Figure));
  fig->axes = cgl_create_axes();
  return fig;
}

cgl_Axes *cgl_create_axes()
{
  cgl_Axes *ax = calloc(1, sizeof(cgl_Axes));
  ax->x_label = "x";
  ax->y_label = "y";
  ax->x_lim[0] = 0.0;
  ax->x_lim[1] = 1.0;
  ax->y_lim[0] = 0.0;
  ax->y_lim[1] = 1.0;
  return ax;
}

cgl_float cgl_axes_get_span_x(cgl_Axes *ax)
{
  cgl_float max = ax->x_lim[1];
  cgl_float min = ax->x_lim[0];
  return max - min;
}

cgl_float cgl_axes_get_span_y(cgl_Axes *ax)
{
  cgl_float max = ax->y_lim[1];
  cgl_float min = ax->y_lim[0];
  return max - min;
}


cgl_LineStyle *cgl_create_style()
{
  cgl_LineStyle *style = calloc(1, sizeof(cgl_LineStyle));
  style->r = 0.0;
  style->g = 0.0;
  style->b = 1.0;
  style->a = 1.0;
  return style;
}

cgl_Line *cgl_create_line()
{
  cgl_Line *line = calloc(1, sizeof(cgl_Line));
  line->style = cgl_create_style();
  return line;
}



void line_to(cairo_t *cr, guint x1, guint y1, guint x2, guint y2)
{
  cairo_move_to(cr, x1, y1);
  cairo_line_to(cr, x2, y2);
}


void cgl_plot (cgl_Figure *figure, cgl_float *x, cgl_float *y, cgl_uint npoints)
{
  (void) figure;
  
  cgl_Line *line = cgl_create_line();
  line->points = calloc(npoints, sizeof(cgl_Point*));
  // TODO error checking on alloc
  line->npoints = npoints;
  
  for (cgl_uint point_i = 0; point_i < npoints; point_i++) {
    line->points[point_i] = calloc(1, sizeof(cgl_Point));
    line->points[point_i]->x = x[point_i];
    line->points[point_i]->y = y[point_i];
    fprintf(stderr, "Added point (%f,%f) to line\n", x[point_i], y[point_i]);
  }

  figure->lines = realloc(figure->lines, (++figure->nlines)*sizeof(cgl_Line));
  figure->lines[figure->nlines-1] = line;
  fprintf(stderr, "Added line\n");
}

gboolean cgl_painter_cb (GtkWidget *widget, cairo_t *cr, gpointer data)
{
  (void) widget;
  cgl_Figure *fig = (cgl_Figure *)data;

  if (fig == NULL)
    return FALSE;

  guint width = gtk_widget_get_allocated_width(widget);
  guint height = gtk_widget_get_allocated_height(widget);

  // Background
  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
  cairo_paint(cr);

  cgl_uint margin_left = (guint)(0.1*width);
  cgl_uint margin_right = (guint)(0.1*width);
  cgl_uint margin_top = (guint)(0.1*height);
  cgl_uint margin_bottom = (guint)(0.1*height);

  // Axes
  cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
  line_to(cr, margin_left, height-margin_bottom, width-margin_right, height-margin_top);
  line_to(cr, margin_left, height-margin_bottom, margin_left, margin_top);
  cairo_stroke(cr);

  // Labels
  cairo_select_font_face(cr, "Courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, 12);

  cairo_text_extents_t extents;
  cairo_text_extents(cr, "X-LABEL", &extents);
  cairo_move_to(cr, width/2 - extents.width/2, height-(margin_bottom/2));
  cairo_show_text(cr, "X-LABEL");
  //cairo_save(cr); 
  //cairo_rotate(cr, 90);
  cairo_text_extents(cr, "Y-LABEL", &extents);
  cairo_move_to(cr, margin_left/2, height/2-extents.height/2);
  //cairo_move_to(cr, width/2 - extents.width/2, height-(margin_bottom/2));
  cairo_show_text(cr, "Y-LABEL");
  //cairo_restore(cr);

  // Data
  cgl_float px_per_data_x = ((cgl_float)(width-margin_left-margin_right)) / cgl_axes_get_span_x(fig->axes);
  cgl_float px_per_data_y = ((cgl_float)(width-margin_top-margin_bottom)) / cgl_axes_get_span_y(fig->axes);
#define CGL_SCALE_X(F) (cgl_uint)(px_per_data_x * F)
#define CGL_SCALE_Y(F) (cgl_uint)(px_per_data_y * F)
  for (cgl_uint line_i = 0; line_i < fig->nlines; line_i++) {
    fprintf(stderr, "Plotting line\n");
    cgl_Line *line = fig->lines[line_i];
    if (!line->npoints)
      continue;

    cgl_Point *point = line->points[0];
    cairo_move_to(cr, margin_left + CGL_SCALE_X(point->x), height-(margin_bottom+CGL_SCALE_Y(point->y)));
    // TODO draw point if style requires it

    cairo_set_source_rgba(cr, line->style->r, line->style->g, line->style->b, line->style->a);

    for (cgl_uint point_i = 1; point_i < line->npoints; point_i++) {
      point = line->points[point_i];
      cairo_line_to(cr, margin_left + CGL_SCALE_X(point->x), height-(margin_bottom+CGL_SCALE_Y(point->y)));
    }
  }
#undef CGL_SCALE_Y
#undef CGL_SCALE_X
  cairo_stroke(cr);

  return FALSE;
}
