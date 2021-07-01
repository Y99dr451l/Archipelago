#ifndef GUI_H
#define GUI_H

#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/frame.h>
#include <gtkmm/aspectframe.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/label.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/filechooserdialog.h>
#include <iostream>
#include <sstream>
#include <string>
#include "ville.h"

void init_gui();

class Dessin : public Gtk::DrawingArea {
public:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};

class Gui : public Gtk::Window {
public:
	Gui();
	virtual ~Gui();
	
	void g_setEnj(double enj);
	void g_setCi(double ci);
	void g_setMta(double mta);
	void g_setZoom(double zoom);
	bool get_bpath();
	int get_rbtype();
	
protected:
	void setlabel();
	void button_text(Glib::ustring data);
	void buildgui();
	
	void cexit();
	void ccreate();
	void copen();
	void csave();
	void cpath();
	void czoomin();
	void czoomout();
	void czoomres();
	void ceditlink();
	bool on_key_press_event(GdkEventKey* event) override;
	bool on_button_press_event(GdkEventButton* event) override;
	bool on_button_release_event(GdkEventButton* event) override;
	
	Gtk::Frame fgeneral, fdisplay, feditor, finfo, fdrawa;
	Gtk::Box box_main, box_menu, box_general, box_display, box_editor, box_info;
	Gtk::Button but_exit, but_create, but_open, but_save,
				but_zoomin, but_zoomout, but_zoomres;
	Dessin drawa;
	Gtk::Label lzoom, lenj, lci, lmta;
	Gtk::ToggleButton but_path, but_editlink;
	Gtk::RadioButton rbhousing, rbtransport, rbproduction;
};
#endif