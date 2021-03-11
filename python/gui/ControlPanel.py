import gi

gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

from .widgets.GLPreview import GLPreview

CONTROL_PANEL_GTK_TITLE = 'Control Panel'
CONTROL_PANEL_GTK_SIZE = (760, 480)

class ControlPanelGTK (Gtk.Window):
  def __init__ (self):
    """
    >> Performs the GTK window setup
    """
    
    # Creates and initializes the GTK window
    Gtk.Window.__init__(self, title = CONTROL_PANEL_GTK_TITLE)
    self.set_default_size(CONTROL_PANEL_GTK_SIZE[0], CONTROL_PANEL_GTK_SIZE[1])
    self.set_resizable(False)

    """
    >> Creates the layout containers
    """

    # Creates the main box, this will contain all the other components
    self.box = Gtk.Box(spacing = 5, orientation = Gtk.Orientation.HORIZONTAL)
    self.add(self.box)

    # Creates the opengl box, this will contain a GLArea, which will render the
    #  current positioning of the robot dog, and the scanned environment.
    self.opengl_box = Gtk.Box(spacing = 5, orientation = Gtk.Orientation.VERTICAL)
    self.box.add(self.opengl_box)

    # Creates the control box, this will contain all the buttons / info fields
    #  to control / check the robot status.
    self.control_box = Gtk.Box(spacing = 5, orientation = Gtk.Orientation.VERTICAL)
    self.control_box.set_margin_top(10)
    self.control_box.set_margin_bottom(10)
    self.control_box.set_margin_left(10)
    self.control_box.set_margin_right(10)
    self.box.pack_start(self.control_box, True, True, 0)

    """
    >> Creates the OpenGL box
    """

    # Adds the FPS text
    self.opengl_box_fps_label = Gtk.Label()
    self.opengl_box_fps_label.set_text('0 FPS')
    self.opengl_box.add(self.opengl_box_fps_label)

    # Creates the OpenGL preview area of the robot.
    self.opengl_box_gl_area = GLPreview(self.gl_fps_change)
    self.opengl_box_gl_area.set_size_request(CONTROL_PANEL_GTK_SIZE[0] / 2, CONTROL_PANEL_GTK_SIZE[1])
    self.opengl_box.add(self.opengl_box_gl_area)

    """
    >> Creates the control box status
    """

    # Adds the battery voltage control status
    self.control_box_voltage_label = Gtk.Label()
    self.set_battery_voltage(0.0)

    self.control_box.add(self.control_box_voltage_label)

    # Adds the divider between status and control.
    self.control_box_separator1 = Gtk.Separator()
    self.control_box_separator1.set_margin_top(15)
    self.control_box_separator1.set_margin_bottom(15)
    self.control_box.add(self.control_box_separator1)

    """
    >> Creates the control box controls
    """
    
    # Creates the enable / disable switch
    self.control_box_enable_disable_box = Gtk.Box(spacing = 15, orientation = Gtk.Orientation.HORIZONTAL)

    self.control_box_enable_disable_label = Gtk.Label()
    self.control_box_enable_disable_label.set_markup('Enable / Disable')
    self.control_box_enable_disable_box.add(self.control_box_enable_disable_label)

    self.control_box_enable_disable_switch = Gtk.Switch()
    self.control_box_enable_disable_box.add(self.control_box_enable_disable_switch)

    self.control_box.add(self.control_box_enable_disable_box)

  def set_battery_voltage(self, voltage):
    self.control_box_voltage_label.set_text(f'Battery Voltage: {voltage}V')

  def gl_fps_change(self, fps):
    self.opengl_box_fps_label.set_text(f'{fps} FPS')

