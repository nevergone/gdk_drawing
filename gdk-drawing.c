/* gdk-drawing.c  --  2005.07.13
    Egy ablakban ket nyomogomb, az egyikre kattintva a masiknak megvaltozik a szine (hatter es feliratszin), a szoveg
    betutipusa es merete, tovabba athuzzuk ket atlos vonallal */

#include <gtk/gtk.h>

/* az alkalmazas foablaka */
GtkWidget * MainWindow = NULL;
GtkWidget * First_Button = NULL;
GtkWidget * Second_Button = NULL;

/* az esemeny, amelyben a GDK segitsegevel meghuzzuk a vonalat */
gboolean First_Button_Expose (GtkWidget * widget, GdkEventExpose * event, gpointer user_data)  {
  GtkAllocation GombAlloc;
  GdkWindow * window = NULL;
  GdkGC * GombGrafika_GC = NULL;

  window = widget->window;
  GombAlloc = widget->allocation;
  GombGrafika_GC = gdk_gc_new (window);
  gdk_draw_line (window, GombGrafika_GC, GombAlloc.x, GombAlloc.y, 
                 GombAlloc.width + GombAlloc.x, GombAlloc.height + GombAlloc.y);
  gdk_draw_line (window, GombGrafika_GC, GombAlloc.width + GombAlloc.x, GombAlloc.y, 
                 GombAlloc.x, GombAlloc.height + GombAlloc.y);
  return FALSE;
}


void First_Button_Clicked (GtkWidget * button, gpointer user_data) {
  /* a masik nyomogomb cimkejenek a mutatoja */
  GtkWidget * Child = NULL;
  /* ezekben a valtozokban fogjuk az elkeszitett eloter es hatterszint tarolni */
  GdkColor SzovegSzin;
  GdkColor HatterSzin;
  /* a nyomogomb feliratanak uj stilusat fogjuk tarolni benne */
  GtkStyle * Stilus = NULL;

  /* kikeverjuk az uj eloter es hatterszint az RGB alapkomponensekbol */
  gdk_color_parse ("blue", &SzovegSzin);
  HatterSzin.red = 65535;
  HatterSzin.green = 0;
  HatterSzin.blue = 0;
  /* megszerezzuk a masik nyomogomb cimkejenek a mutatojat */
  Child = gtk_bin_get_child (GTK_BIN (Second_Button));
  /* beallitjuk a masik nyomogomb uj hatterszinet, es a szoveg szinet */
  gtk_widget_modify_bg (Second_Button, GTK_STATE_NORMAL, &HatterSzin);
  gtk_widget_modify_fg (Child, GTK_STATE_NORMAL, &SzovegSzin);
  /* lekerdezzuk a nyomogomb feliratanak a stilusat, majd a felirat stilusat felkoverre es doltre allitjuk */
  Stilus = gtk_widget_get_style (Child);
  pango_font_description_set_weight (Stilus->font_desc, PANGO_WEIGHT_HEAVY);
  pango_font_description_set_style (Stilus->font_desc, PANGO_STYLE_ITALIC);
  gtk_widget_modify_font (Child, Stilus->font_desc);
  /* most kovetkezik a feladat masodik resze, az atlos vonalak meghuzasa a GDK segitsegevel */
  g_signal_connect_after (G_OBJECT (First_Button), "expose-event",  G_CALLBACK (First_Button_Expose), NULL);
}


int main(int argc, char *argv[]) {
  GtkWidget * HBox = NULL;

  /* inicializaljuk a GTK -t, erre mindig szukseg va */
  gtk_init(&argc, &argv);
  /* elkeszitjuk az alkalmazas foablakat, es a nyomogombokat, hozzarendeljuk a szignalt */
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
  /* elvegezzuk a widgetek beallitasait */
  gtk_window_set_title (GTK_WINDOW (MainWindow), "gdk-drawing.c");
  gtk_container_set_border_width (GTK_CONTAINER (MainWindow), 20);
  gtk_box_set_spacing (GTK_BOX (HBox), 10);
  /* megjelenitunk minden widgetet */
  gtk_widget_show_all (MainWindow);
  /* Indul az X esemenyvezerlese */
  gtk_main();
  return 0;
}
