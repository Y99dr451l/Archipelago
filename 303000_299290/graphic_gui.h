#ifndef GRAPHIC_GUI_H
#define GRAPHIC_GUI_H

#include <cairomm/context.h>
#include <gtkmm/messagedialog.h>
#include "graphic.h"

void demarrer_graphic(const Cairo::RefPtr<Cairo::Context>& cr,
                      double w, double h, double guizoom);
#endif