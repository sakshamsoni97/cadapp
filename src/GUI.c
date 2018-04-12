/*! \file GUI.c
    \brief Implements to UI fro the application

    Constructs the graphical UI of the applications, handles errors and calls
    the correct callback depending on the user interaction
*/

#include <gtk/gtk.h>
#include "cinterface.h"
#include <stdlib.h>

GtkWidget *g_lbl_title;
GtkButton *g_btn_2d3d;
GtkButton *g_btn_3d2d;
GtkButton *g_btn_rotate;
GtkButton *g_btn_save;
GtkEntry *g_txt_filePath;
GtkEntry *g_txt_xAxis;
GtkEntry *g_txt_yAxis;
GtkEntry *g_txt_zAxis;
GtkEntry *g_txt_savePath;
GtkFileChooserButton *g_btn_filePicker;
GtkStatusbar *g_bar_status;


long int pID = 0;

char* filepath = NULL;
char* savepath = NULL;

int arc = 0;
char **arv=NULL;

int main(int argc, char **argv)
{
    arc = argc;
    arv = argv;

    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    //Builds UI from its glade specification
    gtk_builder_add_from_file (builder, "window_main.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    // get pointers to UI Elements
    g_lbl_title = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_title"));
    g_btn_3d2d = GTK_BUTTON(gtk_builder_get_object(builder, "btn_3d2d"));
    g_btn_2d3d = GTK_BUTTON(gtk_builder_get_object(builder, "btn_2d3d"));
    g_btn_rotate = GTK_BUTTON(gtk_builder_get_object(builder, "btn_rotate"));
    g_btn_save = GTK_BUTTON(gtk_builder_get_object(builder, "btn_save"));
    g_txt_filePath = GTK_ENTRY(gtk_builder_get_object(builder, "txt_filePath"));
    g_txt_xAxis = GTK_ENTRY(gtk_builder_get_object(builder, "txt_xAxis"));
    g_txt_yAxis = GTK_ENTRY(gtk_builder_get_object(builder, "txt_yAxis"));
    g_txt_zAxis = GTK_ENTRY(gtk_builder_get_object(builder, "txt_zAxis"));
    g_txt_savePath = GTK_ENTRY(gtk_builder_get_object(builder, "txt_savePath"));
    g_btn_filePicker = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "btn_filePicker"));
    g_bar_status = GTK_STATUSBAR(gtk_builder_get_object(builder, "bar_status"));
    g_object_unref(builder);

    //Adds File Filters for *.cdp2 (Projection Representation)
    //and *.cdp3 (Object Representation)
    GtkFileFilter *cdpfilter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern (cdpfilter, "*.cdp3");
    gtk_file_filter_add_pattern (cdpfilter, "*.cdp2");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(g_btn_filePicker),cdpfilter);

    if(NULL==window){
        printf("Object is NULL\n");
    }
    else{
        gtk_widget_show(window);
    }
    gtk_main();

    return 0;
}

// Called when 2D to 3D conversion button is clicked
void on_btn_2d3d_clicked()
{
    gtk_statusbar_push(g_bar_status,pID++,"Starting 2D to 3D Conversion");
    filepath = (char *) gtk_entry_get_text (g_txt_filePath);
    _2d3dDrawWrapper(filepath,arc,arv);
    //gtk_statusbar_push(g_bar_status,pID++,"Finished 2D to 3D Conversion\n");
}

// Called when 3D to 2D conversion button is clicked
void on_btn_3d2d_clicked()
{
    gtk_statusbar_push(g_bar_status,pID++,"Starting 3D to 2D Conversion");
    filepath = (char *) gtk_entry_get_text (g_txt_filePath);
    switch(_3d2dDrawWrapper(filepath,arc,arv)){
      case 0: gtk_statusbar_push(g_bar_status,pID++,"Finished 3D to 2D Conversion");
              break;
      case 1: gtk_statusbar_push(g_bar_status,pID++,"Unable to open File");
              break;
      case 2: gtk_statusbar_push(g_bar_status,pID++,"Wrong Filename/Path");
              break;
    }
}

// Called when Rotation button is clicked
void on_btn_rotate_clicked()
{
  _rotateWrapper((float) atof((char *)gtk_entry_get_text(g_txt_xAxis)),(float) atof((char *)gtk_entry_get_text(g_txt_yAxis)),(float) atof((char *)gtk_entry_get_text(g_txt_zAxis)),arc,arv);
  gtk_statusbar_push(g_bar_status,pID++,"Rotating");
}

//Called when the File Chooser Button is clicked
void on_btn_filePicker_file_set()
{
    filepath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(g_btn_filePicker));
    gtk_entry_set_text(g_txt_filePath,filepath);
    gtk_entry_set_text(g_txt_savePath,filepath);
    gtk_statusbar_push(g_bar_status,pID++,"Input File Selected");
}

//Called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}

//Called when the object/projection has to be saved
void on_btn_save_clicked()
{
  gtk_statusbar_push(g_bar_status,pID++,"Saving File");
  savepath = (char *) gtk_entry_get_text (g_txt_savePath);
  switch(_SaveWrapper(savepath)){
    case 0: gtk_statusbar_push(g_bar_status,pID++,"File Saved");
            break;
    case 1: gtk_statusbar_push(g_bar_status,pID++,"Unable to open File");
            break;
    case 2: gtk_statusbar_push(g_bar_status,pID++,"Wrong Filename/Path");
            break;
  }
}
