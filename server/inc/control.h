#include <gtk/gtk.h>

#pragma

#define GTK_CONTROL_WINDOW_TITLE    "Control"
#define GTK_CONTROL_WINDOW_WIDTH    500
#define GTK_CONTROL_WINDOW_HEIGHT   400

class ControlWindow {
public:
    ControlWindow(GtkApplication *app);
private:
    GtkWidget *m_Window;

    GtkWidget *m_Box, *m_ControlBox, *m_StatusBox, *m_EnableDisableBox;

    GtkWidget *m_EnableDisableSwitch;

    GtkWidget *m_EnableDisableLabel;
};
