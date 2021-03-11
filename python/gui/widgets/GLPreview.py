import gi
import time

gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

from OpenGL.GL import *

class GLPreview (Gtk.GLArea):
  def __init__ (self, fps_callback):
    self._last_time = time.time()
    self._frames = 0
    self._fps_callback = fps_callback

    """
    >> Performs GLArea setup
    """

    Gtk.GLArea.__init__(self)
    self.connect('render', self.on_render)

  def on_render(self, area, ctx):
    ctx.make_current()

    """
    >> Handles FPS counter
    """

    self._frames += 1
    if time.time() > self._last_time + 1.0:
      self._fps_callback(self._frames)

      self._last_time = time.time()
      self._frames = 0

    """
    >> Prepares for drawing
    """

    glClearColor(.1, .1, .1, 1)
    glClear(GL_COLOR_BUFFER_BIT)

    self.queue_draw()

