#include <stdio.h>

#include <gtk/gtk.h>

#include "cgl.h"


void line_to(cairo_t *cr, guint x1, guint y1, guint x2, guint y2)
{
  cairo_move_to(cr, x1, y1);
  cairo_line_to(cr, x2, y2);
}


gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{
  (void) widget;
  cgl_Line *line_data = (cgl_Line *)data;

  if (line_data == NULL)
    return FALSE;

  guint width = gtk_widget_get_allocated_width(widget);
  guint height = gtk_widget_get_allocated_height(widget);

  // Background
  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
  cairo_paint(cr);

  // Axes
  cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
  line_to(cr, (guint)(0.1*width), (guint)(0.9*height), (guint)(0.9*width), (guint)(0.9*height));
  line_to(cr, (guint)(0.1*width), (guint)(0.9*height), (guint)(0.1*width), (guint)(0.1*height));
  cairo_stroke(cr);

  return FALSE;
}



static void activate (GtkApplication* app, gpointer data)
{

  GtkWidget *window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 700, 700);

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(window), box);

  GtkWidget *title_label = gtk_label_new("TITLE");
  gtk_container_add(GTK_CONTAINER(box), title_label);

  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_container_add(GTK_CONTAINER(box), hbox);

  GtkWidget *y_label = gtk_label_new("Y LABEL");
  gtk_label_set_angle(GTK_LABEL(y_label), 90);
  gtk_container_add(GTK_CONTAINER(hbox), y_label);

  GtkWidget *plotter = gtk_drawing_area_new();
  gtk_widget_set_size_request(plotter, 600, 300);
  gtk_widget_set_hexpand(plotter, TRUE);
  gtk_widget_set_vexpand(plotter, TRUE);
  g_signal_connect(G_OBJECT(plotter), "draw",
                    G_CALLBACK(draw_callback), data);
  gtk_container_add(GTK_CONTAINER(hbox), plotter);

  GtkWidget *x_label = gtk_label_new("X LABEL");
  gtk_container_add(GTK_CONTAINER(box), x_label);

  gtk_widget_show_all (window);
}

void cgl_plot (cgl_Line *line)
{
  int argc = 1;
  char *argv[1] = {"test"};
  GtkApplication *app;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), line);
  g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

}
