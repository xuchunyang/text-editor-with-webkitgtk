#include <gtk/gtk.h>

void 
on_button_clicked (GtkWidget *button,
                 gpointer userdata)
{
  g_print ( gtk_button_get_label (GTK_BUTTON (button)) );
}

void
on_delete_event (GtkWidget *window,
                 gpointer userdata)
{
  g_print ("emit delete-event signal\n");
}

void
print_hi (GtkWidget *widget,
          gpointer userdata)
{
  g_print ("print_hi: Hi!\n");
}

/* build UI with GtkBuilder */
GtkWidget *
create_window ()
{
  GtkBuilder *builder;
  GObject *window;
  GObject *button;
  GObject *label;
  GObject *menuitem;

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "ui/main.ui", NULL);

  window = gtk_builder_get_object (builder, "window1");
  gtk_window_set_title (GTK_WINDOW (window), "New Window from builder");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  // TODO How to get signal callback function in builder?
  button = gtk_builder_get_object (builder, "button1");
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_main_quit), window);

  // TODO How to use signal with Action flag?
  menuitem = gtk_builder_get_object (builder, "imagemenuitem1");
  // g_signal_connect_swapped (menuitem, "select", G_CALLBACK (gtk_main_quit), window);
  // g_signal_connect_swapped (menuitem, "deselect", G_CALLBACK (gtk_main_quit), window);
  g_signal_connect (menuitem, "select", G_CALLBACK (print_hi), window);
  g_signal_connect (menuitem, "deselect", G_CALLBACK (print_hi), window);

  /*
  button = gtk_builder_get_object (builder, "button1");
  g_signal_connect (button, "clicked", G_CALLBACK (on_click_button), NULL);

  // set label value
  label = gtk_builder_get_object (builder, "label1");

  button = gtk_builder_get_object (builder, "button2");
  g_signal_connect (button, "clicked", G_CALLBACK (on_button_clicked), label);
  
  //use g_signal_connect_swapped
  button = gtk_builder_get_object (builder, "quit");
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_main_quit), window);
  */

  return window;
}

int
main (int argc, char **argv)
{
  GtkWidget *window;

  gtk_init (&argc, &argv);

  window = create_window ();
  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}
