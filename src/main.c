#include "calc.h"

GtkWidget *display = NULL;

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    GtkWidget *window = create_ui();
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
    return 0;
}