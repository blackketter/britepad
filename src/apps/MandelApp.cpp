#include "BritepadShared.h"
#include "MandelApp.h"
#include "Screen.h"
#include "Debug.h"
#include "Sound.h"

void MandelApp::run() {
  BritepadApp::run();
/*
  // draw a row
  if (lastRow > screen.clipBottom()) {
    lastRow = screen.clipTop();
  }
  from: https://en.wikipedia.org/wiki/Mandelbrot_set#Computer_drawings
 For each pixel (Px, Py) on the screen, do:
{
  x0 = scaled x coordinate of pixel (scaled to lie in the Mandelbrot X scale (-2.5, 1))
  y0 = scaled y coordinate of pixel (scaled to lie in the Mandelbrot Y scale (-1, 1))
  x = 0.0
  y = 0.0
  iteration = 0
  max_iteration = 1000
  while (x*x + y*y < 2*2  AND  iteration < max_iteration) {
    xtemp = x*x - y*y + x0
    y = 2*x*y + y0
    x = xtemp
    iteration = iteration + 1
  }
  color = palette[iteration]
  plot(Px, Py, color)
}
  lastRow++;
}
*/
}

