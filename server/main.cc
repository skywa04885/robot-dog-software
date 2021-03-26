#include <iostream>
#include <thread>

#include <gtk/gtk.h>

#include "inc/control.h"

void activate(GtkApplication *app, gpointer udata) {
    ControlWindow controlWindow(app);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new ("nl.fannst.robot_dog.gui", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), nullptr);

    g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return 0;
}
