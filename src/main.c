#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

#include <glib.h>
#include <glib/gstdio.h>

#include <glib/gi18n.h>

static void
insertImage_cb (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  // TODO: Port to Webkit2
  g_print ("insertImage_cb\n");
}

static void
bold_cb (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  g_print ("bold_cb\n");
  /*
  GtkWindow *window = user_data;
  // TODO: Port to Webkit2
  WebKitWebView *view = g_object_get_data ((GObject*)window, "webkit-view");
  webkit_web_view_execute_script (view,"document.execCommand(\"bold\", false, false)");
  */
}

static void
underline_cb (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
     g_print ("underline_cb\n");
  /*
  // TODO: Port to Webkit2
  GtkWindow *window = user_data;
  WebKitWebView *view = g_object_get_data ((GObject*)window, "webkit-view");
  webkit_web_view_execute_script (view, "document.execCommand(\"underline\", false, false)");
  */
}


static void
font_cb (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  // TODO: Port to Webkit2
  g_print ("font_cb\n");
}

static void
color_cb (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  // TODO: Port to Webkit2
  g_print ("olor_cb\n");
}

static GActionEntry win_entries[] = {
  { "insertImage", insertImage_cb, NULL, NULL, NULL },
  { "bold", bold_cb, NULL, NULL, NULL },
  { "underline", underline_cb, NULL, NULL, NULL },
  { "font", font_cb, NULL, NULL, NULL },
  { "color", color_cb, NULL, NULL, NULL }
};

static void
new_window (GApplication *app,
            GFile *file)
{
  GtkWidget *window, *box, *view;

  GSimpleAction *about_action;
  GSimpleAction *fullscreen_action;
  GSimpleAction *copy_action;
  GSimpleAction *paste_action;

  gchar *html = NULL;

  window = gtk_application_window_new (G_APPLICATION (app));
  gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);
  gtk_window_set_title (GTK_WINDOW (window), "Webkit2Gtk+ Demo");
  g_signal_connect_swapped (window, "delete-event",
                            G_CALLBACK (g_application_quit), app);

  g_action_map_add_action_entries (G_ACTION_MAP (window), win_entries,
                                   G_N_ELEMENTS (win_entries), window);

  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
  gtk_container_set_border_width (GTK_CONTAINER (box), 6);
  gtk_container_add (GTK_CONTAINER (window), box);

  // TODO: Set HTML editable
  view = webkit_web_view_new ();
  g_object_set_data ((GObject*)window, "webkit-view", view);
  gtk_box_pack_start (GTK_BOX (box), view, TRUE, TRUE, 0);

  webkit_web_view_load_html (view, 
                             "<html><body>Hello, world!</body></html>",
                             NULL);
  gtk_widget_show_all (window);
}

static void
activate (GApplication *application)
{
  new_window (application, NULL);
}

typedef GtkApplication WebKitEditor;
typedef GtkApplicationClass WebKitEditorClass;

G_DEFINE_TYPE (WebKitEditor, webkit_editor, GTK_TYPE_APPLICATION)

static void
new_cb (GSimpleAction *simple,
         GVariant      *parameter,
         gpointer       user_data)
{
  g_print ("new_cb\n");
}


static void
about_cb (GSimpleAction *about_action, 
          GVariant *parameter,
          gpointer userdata)
{
  g_print ("about_cb\n");
}

static void
quit_cb (GSimpleAction *simple,
         GVariant      *parameter,
         gpointer       user_data)
{
  GApplication *application = user_data;

  g_application_quit (application);
}

static GActionEntry app_entries[] = {
  { "new", new_cb, NULL, NULL, NULL },
  { "about", about_cb, NULL, NULL, NULL },
  { "quit", quit_cb, NULL, NULL, NULL },
};

static void
startup (GApplication *app,
         gpointer      user_data)
{
  GtkBuilder *builder;
  GError *error = NULL;

  G_APPLICATION_CLASS (webkit_editor_parent_class)->startup (app);

  g_action_map_add_action_entries (G_ACTION_MAP (app), app_entries,
                                   G_N_ELEMENTS (app_entries), app);

  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "menubar.ui", &error);
  if (error != NULL) {
     g_print ("%s\n", error->message);
     g_error_free (error);
  }

  /* Extract the menubar */
  GObject *menubar = gtk_builder_get_object (builder, "menubar");
  gtk_application_set_menubar (GTK_APPLICATION (app), G_MENU_MODEL (menubar));

  /* Extract the appmenu */
  GObject *appmenu = gtk_builder_get_object (builder, "appmenu");
  gtk_application_set_app_menu (GTK_APPLICATION (app), G_MENU_MODEL (appmenu));

  g_object_unref (builder);
}

static void
webkit_editor_init (WebKitEditor *app)
{
}

static void
webkit_editor_class_init (WebKitEditorClass *class)
{
  GApplicationClass *application_class = G_APPLICATION_CLASS (class);

  application_class->startup = startup;
  application_class->activate = activate;
}

WebKitEditor *
webkit_editor_new (void)
{
  return g_object_new (webkit_editor_get_type (),
                       "application-id", "org.gtk.Test.faker",
                       "flags", G_APPLICATION_FLAGS_NONE,
                       NULL);
}

int
main (int argc, char **argv)
{
  WebKitEditor *webkit_editor;
  int status;

  webkit_editor = webkit_editor_new ();
  status = g_application_run (G_APPLICATION (webkit_editor), argc, argv);
  g_object_unref (webkit_editor);

  return status;
}