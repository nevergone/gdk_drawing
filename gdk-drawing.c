#include <gtk/gtk.h>

/*
   gdk-drawing.c  --  2005.07.13
   Compile: gcc -Wall gdk-drawing.c `pkg-config --cflags --libs gtk+-2.0` -o gdk-drawing
*/

/* application main window */
GtkWidget * MainWindow = NULL;
GtkWidget * First_Button = NULL;
GtkWidget * Second_Button = NULL;

/* GDK event: diagonal line drawing */
gboolean First_Button_Expose (GtkWidget *widget, GdkEventExpose *event, gpointer user_data)  {
  GtkAllocation GombAlloc;
  GdkWindow *window = NULL;
  GdkGC *Button_GC = NULL;

  window = widget->window;
  GombAlloc = widget->allocation;
  Button_GC = gdk_gc_new (window);
  gdk_draw_line (window, Button_GC, GombAlloc.x, GombAlloc.y,
                 GombAlloc.width + GombAlloc.x, GombAlloc.height + GombAlloc.y);
  gdk_draw_line (window, Button_GC, GombAlloc.width + GombAlloc.x, GombAlloc.y,
                 GombAlloc.x, GombAlloc.height + GombAlloc.y);
  return FALSE;
}


void First_Button_Clicked (GtkWidget *button, gpointer user_data) {
  GtkWidget *Child = NULL;
  GdkColor TextColor;
  GdkColor BgColor;
  GtkStyle *Style = NULL;

  /* foreground and background color */
  gdk_color_parse ("blue", &TextColor);
  BgColor.red = 65535;
  BgColor.green = 0;
  BgColor.blue = 0;
  Child = gtk_bin_get_child (GTK_BIN (Second_Button));
  /* modify colors */
  gtk_widget_modify_bg (Second_Button, GTK_STATE_NORMAL, &BgColor);
  gtk_widget_modify_fg (Child, GTK_STATE_NORMAL, &TextColor);
  /* get label style, and set bold and italic*/
  Style = gtk_widget_get_style (Child);
  pango_font_description_set_weight (Style->font_desc, PANGO_WEIGHT_HEAVY);
  pango_font_description_set_style (Style->font_desc, PANGO_STYLE_ITALIC);
  gtk_widget_modify_font (Child, Style->font_desc);
  g_signal_connect_after (G_OBJECT (First_Button), "expose-event",  G_CALLBACK (First_Button_Expose), NULL);
}


int main(int argc, char *argv[]) {
  GtkWidget *HBox = NULL;

  gtk_init(&argc, &argv);
  MainWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect_swapped (G_OBJECT (MainWindow), "delete-event", G_CALLBACK (gtk_main_quit), G_OBJECT (MainWindow));
  HBox = gtk_hbutton_box_new ();
  gtk_container_add (GTK_CONTAINER (MainWindow), HBox);
  First_Button = gtk_button_new_with_label ("First Button");
  gtk_container_add (GTK_CONTAINER (HBox), First_Button);
  g_signal_connect (G_OBJECT (First_Button), "clicked", G_CALLBACK (First_Button_Clicked), NULL);
  Second_Button = gtk_button_new_with_label ("Second Button");
  gtk_container_add (GTK_CONTAINER (HBox), Second_Button);
  g_signal_connect_swapped (G_OBJECT (Second_Button), "clicked", G_CALLBACK (gtk_main_quit), G_OBJECT (MainWindow));
  gtk_window_set_title (GTK_WINDOW (MainWindow), "gdk-drawing.c");
  gtk_container_set_border_width (GTK_CONTAINER (MainWindow), 20);
  gtk_box_set_spacing (GTK_BOX (HBox), 10);
  gtk_widget_show_all (MainWindow);
  gtk_main();
  return 0;
}

