#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

#include <glib/gi18n.h>

static void
open_cb (GSimpleAction *action,
          GVariant      *parameter,
          gpointer       user_data)
{
  GtkWindow *toplevel = user_data;
  WebKitWebView *view = g_object_get_data ((GObject*)toplevel, "webkit-view");
  GtkWidget *dialog = gtk_file_chooser_dialog_new (_("Open html"),
	                                               GTK_WINDOW (toplevel),
	                                               GTK_FILE_CHOOSER_ACTION_OPEN,
	                                               GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
	                                               GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                               NULL);
  if (gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
  {
    gchar *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    gchar *content;
    g_file_get_contents (filename, &content, NULL, NULL);
    webkit_web_view_load_html (view,
                               content,
                               NULL);
  }
  gtk_widget_destroy (dialog);
}

static void
insertImage_cb (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  GtkWindow *window = user_data;
  WebKitWebView *view = g_object_get_data ((GObject*)window, "webkit-view");

  // FIXME: insert local image
  webkit_web_view_run_javascript (view, 
                                  "document.execCommand('insertImage', false, 'http://upload.wikimedia.org/wikipedia/en/5/5a/Webkit_Logo.png')",
                                  NULL,
                                  NULL,
                                  NULL);
}

static void
active_cb (GSimpleAction *action,
           GVariant      *parameter,
           gpointer       user_data)
{
  gchar *script;
  GValue value = G_VALUE_INIT;

  g_value_init (&value, G_TYPE_STRING);
  g_object_get_property (G_OBJECT (action), "name", &value);
  script = g_strdup_printf ("document.execCommand('%s', false, null)", 
                            g_value_get_string (&value));
  
  GtkWindow *window = user_data;
  WebKitWebView *view = g_object_get_data ((GObject*)window, "webkit-view");
  webkit_web_view_run_javascript (view, 
                                  script,
                                  NULL,
                                  NULL,
                                  NULL);
  g_value_unset (&value);
  g_free (script);
}

static void
color_cb (GSimpleAction *action,
          GVariant      *parameter,
          gpointer       user_data)
{
  g_print ("TODO: color_cb\n");
  GtkWindow *window = user_data;
  WebKitWebView *view = g_object_get_data ((GObject*)window, "webkit-view");
  gchar *script;

  script = "document.body.contentDocument.designMode = 'on'";
  // script = "document.body.style.color = \"red\"";
  webkit_web_view_run_javascript (view, 
                                  script,
                                  NULL,
                                  NULL,
                                  NULL);
  g_free (script);
}
static GActionEntry win_entries[] = {
  { "open", open_cb, NULL, NULL, NULL },
  { "insertImage", insertImage_cb, NULL, NULL, NULL },
  { "bold", active_cb, NULL, NULL, NULL },
  { "italic", active_cb, NULL, NULL, NULL },
  { "underline", active_cb, NULL, NULL, NULL },
  { "strikethrough", active_cb, NULL, NULL, NULL },
  { "color", color_cb, NULL, NULL, NULL, NULL }
};

static void
new_window (GApplication *app,
            GFile *file)
{
  GtkWidget *window, *box, *view;
  WebKitWebViewGroup *view_group;
  WebKitSettings *settings;

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

  view = webkit_web_view_new ();
  settings = webkit_settings_new();
  webkit_web_view_set_settings (view, settings);
  // TODO: webkit_web_view_set_editable() in WebKit2 API
  // We can use js to do this, i.e. document.body.contentEditable='true'; and document.designMode='on';
  // TODO: don't be a browser

  g_object_set_data ((GObject*)window, "webkit-view", view);
  gtk_box_pack_start (GTK_BOX (box), view, TRUE, TRUE, 0);

  webkit_web_view_load_html (view,
                             "<html><head><style>body {color:blue}</style></head>"
                             "<body contentEditable='true'>Hello, world!</body></html>",
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
