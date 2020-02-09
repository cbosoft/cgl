#include <stdlib.h>
#include <gtk/gtk.h>
#include "cgl.h"
#include <math.h>

static void activate (GtkApplication* app, gpointer data)
{

  GtkWidget *window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 700, 700);

  GtkWidget *plotter = gtk_drawing_area_new();
  gtk_widget_set_size_request(plotter, 600, 300);
  gtk_widget_set_hexpand(plotter, TRUE);
  gtk_widget_set_vexpand(plotter, TRUE);
  g_signal_connect(G_OBJECT(plotter), "draw",
                    G_CALLBACK(cgl_painter_cb), data);
  gtk_container_add(GTK_CONTAINER(window), plotter);

  gtk_widget_show_all (window);
}




int main(int argc, char *argv[])
{
  GtkApplication *app;
  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);

  cgl_uint n = 10000;
  cgl_float *x = calloc(n, sizeof(cgl_float));
  cgl_float *y = calloc(n, sizeof(cgl_float));

  for (cgl_uint i = 0; i < n; i++) {
    x[i] = i*0.01;
    y[i] = (sin(i*0.02*3.14) + 1.0)*0.5;
  }
  
  cgl_Figure *fig = cgl_init_figure();
  cgl_plot(fig, x, y, n);


  g_signal_connect(app, "activate", G_CALLBACK(activate), fig);
  g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

}
