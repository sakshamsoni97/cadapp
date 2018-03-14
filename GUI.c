#include <gtk/gtk.h>

GtkWidget *g_lbl_title;
GtkWidget *g_btn_2d3d;
GtkWidget *g_btn_3d2d;
GtkWidget *g_txt_filePath;
GtkFileChooserButton *g_btn_filePicker;
GtkStatusbar *g_bar_status;
GtkImage *g_img_input;
GtkImage *g_img_output;

long int pID = 0;

char* filepath = NULL;

int main(int argc, char *argv[])
{
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "window.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    // get pointers to UI Elements
    g_lbl_title = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_title"));
    g_btn_3d2d = GTK_WIDGET(gtk_builder_get_object(builder, "btn_3d2d"));
    g_btn_2d3d = GTK_WIDGET(gtk_builder_get_object(builder, "btn_2d3d"));
    g_txt_filePath = GTK_WIDGET(gtk_builder_get_object(builder, "txt_filePath"));
    g_btn_filePicker = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "btn_filePicker"));
    g_bar_status = GTK_STATUSBAR(gtk_builder_get_object(builder, "bar_status"));
    g_img_input = GTK_IMAGE(gtk_builder_get_object(builder, "img_input"));
    g_img_output = GTK_IMAGE(gtk_builder_get_object(builder, "img_output"));
    g_object_unref(builder);

    GtkFileFilter *JSONfilter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern (JSONfilter, "*.JSON");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(g_btn_filePicker),JSONfilter);

    gtk_widget_show(window);
    gtk_main();

    return 0;
}

// called when button is clicked
void on_btn_2d3d_clicked()
{
    gtk_statusbar_push(g_bar_status,pID++,"Starting 2D to 3D Conversion\n");
    /*FILE* inputFile = fopen(filepath,"r");
    char *JSONbuffer = NULL;
    if(inputFile!=NULL)
    {
        fseek(inputFile,0,SEEK_END);
        int length = ftell(inputFile);
        fseek(inputFile,0,SEEK_SET);
        JSONbuffer = (char*) malloc(length);
        if(JSONbuffer)
        {
            fread(JSONbuffer,1,length,inputFile);
        }
        fclose(inputFile);
        gtk_statusbar_push(g_bar_status,pID++,"Input File Read\n");
        printf("%s\n",JSONbuffer);
        if(JSONbuffer)
        {
            struct json_object *obj;
            obj = json_tokener_parse(JSONbuffer);
            //printf("%s\n",obj->_pb->buf);
            gtk_statusbar_push(g_bar_status,pID++,"JSON Parsed\n");
            //Rendering functions here
        }
        else
        {
            gtk_statusbar_push(g_bar_status,pID++,"Error: JSON Parsing Error\n");
        }
    }
    else
    {
        gtk_statusbar_push(g_bar_status,pID++,"Error: File Cannot Be Opened\n");
    }*/
    //Call Conversion Wrapper Function
    if(access("Input.png",F_OK)!=-1)
    {
        gtk_image_set_from_file (g_img_input,"Input.png");
    }
    else
    {
        gtk_image_set_from_file (g_img_input,"defaultInput.png");
    }
    if(access("Output.png",F_OK)!=-1)
    {
        gtk_image_set_from_file (g_img_output,"Output.png");
    }
    else
    {
        gtk_image_set_from_file (g_img_output,"defaultOutput.png");
    }
}

void on_btn_3d2d_clicked()
{
    printf("Starting 3D to 2D conversion\n");
    gtk_statusbar_push(g_bar_status,pID++,"Starting 2D to 3D Conversion\n");
    //Call Conversion Wrapper Function
    if(access("Input.png",F_OK)!=-1)
    {
        gtk_image_set_from_file (g_img_input,"Input.png");
    }
    else
    {
        gtk_image_set_from_file (g_img_input,"defaultInput.png");
    }
    if(access("Output.png",F_OK)!=-1)
    {
        gtk_image_set_from_file (g_img_input,"Output.png");
    }
    else
    {
        gtk_image_set_from_file (g_img_input,"defaultOutput.png");
    }
}

void on_btn_filePicker_file_set()
{
    filepath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(g_btn_filePicker));
    gtk_entry_set_text(GTK_ENTRY(g_txt_filePath),filepath);
    printf("%s\n", filepath);
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
