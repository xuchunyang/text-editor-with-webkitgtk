#include <gtk/gtk.h>
#include <webkit/webkit.h>

#include <glib.h>
#include <glib/gstdio.h>

#include <glib/gi18n.h>

static void
entry_activate_cb (GtkEntry *entry, WebKitWebView *view)
{
  WebKitDOMDocument *document;
  WebKitDOMElement *body, *div;

  document = webkit_web_view_get_dom_document (view);
  body = webkit_dom_document_query_selector (document,
                                             "body",
                                             NULL);
  div = webkit_dom_document_create_element (document,
                                            "div",
                                            NULL);
  webkit_dom_node_set_text_content (WEBKIT_DOM_NODE (div),
                                    gtk_entry_get_text (entry),
                                    NULL);
  webkit_dom_node_append_child (WEBKIT_DOM_NODE (body),
                                WEBKIT_DOM_NODE (div),
                                NULL);
  gtk_entry_set_text (entry, "");
}

// Insert Picture
static void
insert_pic_clicked_cb (GtkButton *button, WebKitWebView *view)
{
  // Create a dialog to choose file
  GtkWidget *dialog, *toplevel;

  toplevel = gtk_widget_get_toplevel (button);
  if (gtk_widget_is_toplevel (toplevel))
  {
    dialog = gtk_file_chooser_dialog_new ("Open a image",
                                          toplevel,
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          _("_Cancel"), GTK_RESPONSE_CANCEL,
                                          _("_Open"), GTK_RESPONSE_ACCEPT,
                                          NULL);
  }
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
  {
    char *filename, *command;

    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    command = g_strconcat ("document.execCommand('insertImage', null, '",
                           filename,
                           "');"
                           );
    g_debug (command);
    webkit_web_view_execute_script (view, command);
    gdk_window_destroy (dialog);
    g_free (filename);
    g_free (command);
  }
}

static void
about_cb (GSimpleAction *about_action, 
          GVariant *parameter,
          gpointer userdata)
{
  const gchar *authors[] = {"Chunyang Xu", NULL};
  gtk_show_about_dialog (NULL,
                         "program-name", "Faker",
                         "title", "About Faker",
                         "logo-icon-name", "applications-office",
                         "comments", "A Sample Rich Text Editor.",
                         "website", "http://github.com/xuchunyang",
                         "website-label", "Host on Github",
                         "authors", authors,
                         "copyright", "Copyright \xc2\xa9 2014 Chunyang Xu",
                         NULL);
}

static void
fullscreen_cb (GSimpleAction *fullscreen_action,
               GVariant *parameter,
               gpointer userdata)
{
  GtkWidget *window = userdata;
  gtk_window_fullscreen (GTK_WINDOW (window));
}

static void
copy_callback (GSimpleAction *fullscreen_action,
               GVariant *parameter,
               gpointer userdata)
{
  g_print ("copy active\n");
}

static void
paste_callback (GSimpleAction *fullscreen_action,
               GVariant *parameter,
               gpointer userdata)
{
  //   webkit_web_view_paste_clipboard (view);
  g_print ("paste active\n");
}

static void
window_cut (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  g_print ("window_cut\n");
}

static void
window_copy (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  g_print ("window_copy\n");
}

static void
window_paste (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  g_print ("window_paste\n");
}

static void
window_undo (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  g_print ("window_undo\n");
}

static void
window_redo (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  g_print ("window_redo\n");
}

static void
window_insertImage (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  g_print ("window_insertImage\n");
}

static void
window_bold (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  GtkWindow *window = user_data;
  WebKitWebView *view = g_object_get_data ((GObject*)window, "webkit-view");
  webkit_web_view_execute_script (view, "document.execCommand(\"bold\", false, false)");
}

static void
window_italic (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  GtkWindow *window = user_data;
  WebKitWebView *view = g_object_get_data ((GObject*)window, "webkit-view");
  webkit_web_view_execute_script (view, "document.execCommand(\"italic\", false, false)");
}

static void
window_underline (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  GtkWindow *window = user_data;
  WebKitWebView *view = g_object_get_data ((GObject*)window, "webkit-view");
  webkit_web_view_execute_script (view, "document.execCommand(\"underline\", false, false)");
}

static void
window_strikethrough (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  GtkWindow *window = user_data;
  WebKitWebView *view = g_object_get_data ((GObject*)window, "webkit-view");
  webkit_web_view_execute_script (view, "document.execCommand(\"strikethrough\", false, false)");
}

static void
window_font (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  g_print ("window_font\n");
}

static void
window_color (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  g_print ("window_color\n");
}

static GActionEntry win_entries[] = {
  { "redo", window_redo, NULL, NULL, NULL },
  { "undo", window_undo, NULL, NULL, NULL },
  { "cut", window_cut, NULL, NULL, NULL },
  { "copy", window_copy, NULL, NULL, NULL },
  { "paste", window_paste, NULL, NULL, NULL },
  { "insertImage", window_insertImage, NULL, NULL, NULL },
  { "bold", window_bold, NULL, NULL, NULL },
  { "italic", window_italic, NULL, NULL, NULL },
  { "underline", window_underline, NULL, NULL, NULL },
  { "strikethrough", window_strikethrough, NULL, NULL, NULL },
  { "font", window_font, NULL, NULL, NULL },
  { "color", window_color, NULL, NULL, NULL },
  // { "fullscreen", activate_toggle, NULL, "false", change_fullscreen_state }
};

static GActionEntry app_entries[] = {
  { "about", about_cb, NULL, NULL, NULL }
};

static void
new_window (GApplication *app,
            GFile *file)
{
  GtkWidget *window, *box, *toolbar, *scroll, *view;

  GtkToolItem *button;
  GtkStyleContext *style_context;

  GSimpleAction *about_action;
  GSimpleAction *fullscreen_action;
  GSimpleAction *copy_action;
  GSimpleAction *paste_action;

  gchar *html = NULL;

  window = gtk_application_window_new (G_APPLICATION (app));
  gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);
  gtk_window_set_position (window, GTK_WIN_POS_CENTER);
  gtk_window_set_title (GTK_WINDOW (window), "Faker");
  g_signal_connect (window, "delete-event",
                    G_CALLBACK (gtk_main_quit), NULL);

  g_action_map_add_action_entries (G_ACTION_MAP (window), win_entries,
                                   G_N_ELEMENTS (win_entries), window);

  // Menu bar
  /* 新建action */
  copy_action = g_simple_action_new ("copy", NULL);
  /* 绑定callback函数 */
  g_signal_connect (copy_action, "activate", G_CALLBACK (copy_callback),
                    GTK_WINDOW (window));
  /* 添加到window */
  g_action_map_add_action (G_ACTION_MAP (window), G_ACTION (copy_action));
  // paste Action
  paste_action = g_simple_action_new ("paste", NULL);
  g_signal_connect (paste_action, "activate", G_CALLBACK (paste_callback),
                    GTK_WINDOW (window));
  g_action_map_add_action (G_ACTION_MAP (window), G_ACTION (paste_action));

  // 创建一个简单的 Action
  about_action = g_simple_action_new ("about", NULL);
  fullscreen_action = g_simple_action_new ("fullscreen", NULL);
  /* Connect the "activate" signal to the appropriate callback function. 
  * It will indicate that the action was just activated.
  */
  g_signal_connect (about_action, "activate", G_CALLBACK (about_cb), 
                    GTK_WINDOW (window));
  g_signal_connect (fullscreen_action, "activate", G_CALLBACK (fullscreen_cb),
                    GTK_WINDOW (window));
  /* Adds the about_action to the overall action map. An Action map is an 
   * interface that contains a number of named GAction instances 
   * (such as about_action) 
   */
  g_action_map_add_action (G_ACTION_MAP (window), G_ACTION (about_action));
  g_action_map_add_action (G_ACTION_MAP (window), G_ACTION (fullscreen_action));

  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
  gtk_container_set_border_width (GTK_CONTAINER (box), 6);
  gtk_container_add (GTK_CONTAINER (window), box);

  // Create a toolbar
  toolbar = gtk_toolbar_new ();
  /* Set the toolbar to be the primary toolbar of the application */
  style_context = gtk_widget_get_style_context (toolbar);
  gtk_style_context_add_class (style_context, GTK_STYLE_CLASS_PRIMARY_TOOLBAR);

  // FIXME: replace gtk_tool_button_new_from_stock ()
  button = gtk_tool_button_new (NULL, "New");
  gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (button), "document-new");
  gtk_tool_item_set_is_important (button, TRUE);   /* 显示标签 */
  gtk_actionable_set_action_name (GTK_ACTIONABLE (button), "app.new");
  gtk_toolbar_insert (GTK_TOOLBAR (toolbar), button, 0);

  button = gtk_tool_button_new (NULL, "Open");
  gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (button), "document-open");
  gtk_tool_item_set_is_important (button, TRUE);   /* 显示标签 */
  gtk_actionable_set_action_name (GTK_ACTIONABLE (button), "app.new");
  gtk_toolbar_insert (GTK_TOOLBAR (toolbar), button, 1);

  button = gtk_tool_button_new (NULL, "Save");
  gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (button), "document-save");
  gtk_tool_item_set_is_important (button, TRUE);   /* 显示标签 */
  gtk_actionable_set_action_name (GTK_ACTIONABLE (button), "app.save");
  gtk_toolbar_insert (GTK_TOOLBAR (toolbar), button, 2);

  button = gtk_tool_button_new (NULL, "Undo");
  gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (button), "edit-undo");
  gtk_tool_item_set_is_important (button, TRUE);   /* 显示标签 */
  gtk_actionable_set_action_name (GTK_ACTIONABLE (button), "win.undo");
  gtk_toolbar_insert (GTK_TOOLBAR (toolbar), button, 3);

  button = gtk_tool_button_new (NULL, "Redo");
  gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (button), "edit-redo");
  gtk_tool_item_set_is_important (button, TRUE);   /* 显示标签 */
  gtk_actionable_set_action_name (GTK_ACTIONABLE (button), "win.redo");
  gtk_toolbar_insert (GTK_TOOLBAR (toolbar), button, 4);

  // FIXME: stock 已经废弃了，要设置button的图标，如何正确使用icon还不清楚
  button = gtk_tool_button_new_from_stock (GTK_STOCK_CUT);
  gtk_tool_item_set_is_important (button, TRUE);   /* 显示标签 */
  gtk_actionable_set_action_name (GTK_ACTIONABLE (button), "win.cut");
  gtk_toolbar_insert (GTK_TOOLBAR (toolbar), button, 5);

  button = gtk_tool_button_new_from_stock (GTK_STOCK_COPY);
  gtk_tool_item_set_is_important (button, TRUE);   /* 显示标签 */
  gtk_actionable_set_action_name (GTK_ACTIONABLE (button), "win.copy");
  gtk_toolbar_insert (GTK_TOOLBAR (toolbar), button, 6);

  button = gtk_tool_button_new_from_stock (GTK_STOCK_PASTE);
  gtk_tool_item_set_is_important (button, TRUE);   /* 显示标签 */
  gtk_actionable_set_action_name (GTK_ACTIONABLE (button), "win.paste");
  gtk_toolbar_insert (GTK_TOOLBAR (toolbar), button, 7);

  gtk_box_pack_start (GTK_BOX (box), toolbar, FALSE, FALSE, 0);

  scroll = gtk_scrolled_window_new (NULL, NULL);
  g_object_set (scroll, "shadow-type", GTK_SHADOW_IN, NULL);
  gtk_box_pack_start (GTK_BOX (box), scroll, TRUE, TRUE, 0);

  view = webkit_web_view_new ();
  gtk_container_add (GTK_CONTAINER (scroll), view);

  g_object_set_data ((GObject*)window, "webkit-view", view);

  if (file != NULL)
    {
      gchar *contents;
      gsize length;

      if (g_file_load_contents (file, NULL, &contents, &length, NULL, NULL))
        {
          html = g_strdup (contents);
          g_free (contents);
        }
    }
  else
    {
      html = "<html><body></body></html>";
    }

  webkit_web_view_load_string (WEBKIT_WEB_VIEW (view),
                               html,
                               "text/html",
                               "UTF-8",
                               NULL);
  // Set html editable from html or webkit setting
  webkit_web_view_set_editable (view, TRUE);
  gtk_widget_show_all (window);
}

static void
activate (GApplication *application)
{
  new_window (application, NULL);
}

static void
open (GApplication  *application,
      GFile        **files,
      gint           n_files,
      const gchar   *hint)
{
  gint i;

  for (i = 0; i < n_files; i++)
    new_window (application, files[i]);
}

typedef GtkApplication WebKitEditor;
typedef GtkApplicationClass WebKitEditorClass;

G_DEFINE_TYPE (WebKitEditor, webkit_editor, GTK_TYPE_APPLICATION)

/* Callback function for the response signal "activate" from the "quit" action 
 * found in the function directly below.
 */ 
static void
quit_cb (GSimpleAction *simple,
         GVariant      *parameter,
         gpointer       user_data)
{
  GApplication *application = user_data;

  g_application_quit (application);
}

static void
perferences_cb (GSimpleAction *simple,
                GVariant *parameter,
                gpointer userdata)
{
  GApplication *app = userdata;

  g_print ("perferences_cb");
}

/* Callback function for the "new" action */
static void
new_cb (GSimpleAction *simple,
         GVariant      *parameter,
         gpointer       user_data)
{
  g_print ("You clicked \"New\"\n");
}

/* Startup function for the menu we are creating in this sample */
static void
startup (GApplication *app,
         gpointer      user_data)
{
  GSimpleAction *new_action, *quit_action, *perferences_action;
  GtkBuilder *builder;
  GError *error = NULL;

  G_APPLICATION_CLASS (webkit_editor_parent_class)->startup (app);
  
  // FIXME: 或许应该使用 GActionGroup
  perferences_action = g_simple_action_new ("perferences", NULL);
  quit_action = g_simple_action_new ("quit", NULL);
  new_action = g_simple_action_new ("new", NULL);

  g_signal_connect (perferences_action, "activate", G_CALLBACK (perferences_cb), app);
  g_signal_connect (quit_action, "activate", G_CALLBACK (quit_cb), app);
  g_signal_connect (new_action, "activate", G_CALLBACK (new_cb), app);

  g_action_map_add_action (G_ACTION_MAP (app), G_ACTION (perferences_action));
  g_action_map_add_action (G_ACTION_MAP (app), G_ACTION (quit_action));
  g_action_map_add_action (G_ACTION_MAP (app), G_ACTION (new_action));

  /* A builder to add the User Interface designed with GLADE to the grid: */
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
  application_class->open = open;
}

WebKitEditor *
webkit_editor_new (void)
{
  return g_object_new (webkit_editor_get_type (),
                       "application-id", "org.gtk.Test.faker",
                       "flags", G_APPLICATION_HANDLES_OPEN,
                       NULL);
}

// FIXME: split to 3 files, app.h, app.c and main.c
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