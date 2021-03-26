#include "control.h"

ControlWindow::ControlWindow (GtkApplication *app):
    m_Window (gtk_application_window_new (app)),
    // >> Boxes
    m_Box (gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0.0)),
    m_ControlBox (gtk_box_new (GTK_ORIENTATION_VERTICAL, 0.0)),
    m_StatusBox (gtk_box_new (GTK_ORIENTATION_VERTICAL, 0.0)),
    m_EnableDisableBox (gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0.0)),
    // >> Buttons
    // >> Labels
    m_EnableDisableLabel (gtk_label_new ("Enable / Disable")),
    // >> Switches
    m_EnableDisableSwitch (gtk_switch_new ())
{
    // Sets the window properties.
    gtk_window_set_title (GTK_WINDOW (m_Window), GTK_CONTROL_WINDOW_TITLE);
    gtk_window_set_default_size (GTK_WINDOW (m_Window), GTK_CONTROL_WINDOW_WIDTH, GTK_CONTROL_WINDOW_HEIGHT);

    // Creates the main box, and the control / status box.
    gtk_container_add (GTK_CONTAINER (m_Window), m_Box);
    gtk_box_pack_start (GTK_BOX (m_Box), m_ControlBox, false, false, 5.0);
    gtk_box_pack_start (GTK_BOX (m_Box), m_StatusBox, false, false, 5.0);

    // Creates the enable / disable switch.
    gtk_box_pack_start (GTK_BOX (m_ControlBox), m_EnableDisableBox, false, false, 5.0);

    gtk_box_pack_start (GTK_BOX (m_EnableDisableBox), m_EnableDisableLabel, false, false, 5.0);
    gtk_box_pack_start (GTK_BOX (m_EnableDisableBox), m_EnableDisableSwitch, false, false, 5.0);

    // Shows all the widgets and the window itself.
    gtk_widget_show_all (m_Window);
}