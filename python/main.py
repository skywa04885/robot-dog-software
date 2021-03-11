import gi

gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

from gui.ControlPanel import ControlPanelGTK

def create_control_panel():
  control_panel = ControlPanelGTK()
  control_panel.connect('destroy', Gtk.main_quit)
  control_panel.show_all()

create_control_panel();
Gtk.main()