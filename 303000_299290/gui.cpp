#include "gui.h"
#include "graphic_gui.h"
#include "tools.h"
#include "constantes.h"

#define EPAISSEUR_BORD 5 
#define DISTANCE_BOUTONS 2

using namespace std;

static Gui* ptr_g = (nullptr);
static double zoom(1), pclic_x, pclic_y, rclic_x, rclic_y;
static size_t selected_noeud(no_link);

void init_gui() {
	setMarge(dist_min);
	auto app = Gtk::Application::create("org.gtkmm.example");
	Gui gui;
	ptr_g = &gui;
	app->run((*ptr_g));
}

bool Dessin::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	Gtk::Allocation allocation = get_allocation();
	const int dwidth = allocation.get_width();
	const int dheight = allocation.get_height();
	demarrer_graphic(cr, dwidth, dheight, zoom);
	dessiner_blanc();
	(*ptr_v).calcul_enj();
	(*ptr_v).calcul_mta();
	setwidth(EPAISSEUR);
	setcolor(Noir);
	(*ptr_v).dessiner_ville();
	if (selected_noeud != no_link) {
		setwidth(EPAISSEURII);
		if ((*ptr_g).get_bpath()) {
			setcolor(Vert);
			(*ptr_v).dessiner_path(selected_noeud);
		}
		setcolor(Rouge);
		(*ptr_v).dessiner_select(selected_noeud);
	}
	(*ptr_g).g_setZoom(zoom);
	(*ptr_g).g_setEnj((*ptr_v).getEnj());
	(*ptr_g).g_setCi((*ptr_v).getCi());
	(*ptr_g).g_setMta((*ptr_v).getMta());
	return true;
}

Gui::Gui() {
	setlabel();
	but_exit.signal_clicked().connect( sigc::mem_fun(*this,&Gui::cexit) );
	but_create.signal_clicked().connect( sigc::mem_fun(*this,&Gui::ccreate) );
	but_open.signal_clicked().connect( sigc::mem_fun(*this,&Gui::copen) );
	but_save.signal_clicked().connect( sigc::mem_fun(*this,&Gui::csave) );
	but_path.signal_clicked().connect( sigc::mem_fun(*this,&Gui::cpath) );
	but_zoomin.signal_clicked().connect( sigc::mem_fun(*this,&Gui::czoomin) );
	but_zoomout.signal_clicked().connect( sigc::mem_fun(*this,&Gui::czoomout) );
	but_zoomres.signal_clicked().connect( sigc::mem_fun(*this,&Gui::czoomres) );
	but_editlink.signal_clicked().connect( sigc::mem_fun(*this,&Gui::ceditlink) );
	add_events(Gdk::KEY_PRESS_MASK);
	add_events(Gdk::BUTTON_PRESS_MASK);
	add_events(Gdk::BUTTON_RELEASE_MASK);
	buildgui();
}

Gui::~Gui(){}

void Gui::setlabel() {
	but_exit.set_label("Exit");
	but_create.set_label("New");
	but_open.set_label("Open");
	but_save.set_label("Save");
	but_path.set_label("Shortest path");
	but_zoomin.set_label("Zoom in (I)");
	but_zoomout.set_label("Zoom out (O)");
	but_zoomres.set_label("Reset zoom (R)");
	but_editlink.set_label("Edit link");
	rbhousing.set_label("Housing (1)");
	rbtransport.set_label("Transport (2)");
	rbproduction.set_label("Production (3)");
}

void Gui::buildgui() {
	set_title("Archipelago");
	set_border_width(EPAISSEUR_BORD);
	drawa.set_size_request(default_drawing_size, default_drawing_size);
	int bpad(DISTANCE_BOUTONS);
	
	box_general.set_homogeneous(1);
	box_general.set_orientation(Gtk::ORIENTATION_VERTICAL);	
	box_general.pack_start(but_exit,Gtk::PACK_EXPAND_WIDGET,bpad);
	box_general.pack_start(but_create,Gtk::PACK_EXPAND_WIDGET,bpad);
	box_general.pack_start(but_open,Gtk::PACK_EXPAND_WIDGET,bpad);
	box_general.pack_start(but_save,Gtk::PACK_EXPAND_WIDGET,bpad);

	fgeneral.set_label("General");
	fgeneral.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	fgeneral.add(box_general);
	
	box_display.set_homogeneous(1);
	box_display.set_orientation(Gtk::ORIENTATION_VERTICAL);	
	box_display.pack_start(but_path,Gtk::PACK_EXPAND_WIDGET,bpad);
	box_display.pack_start(but_zoomin,Gtk::PACK_EXPAND_WIDGET,bpad);
	box_display.pack_start(but_zoomout,Gtk::PACK_EXPAND_WIDGET,bpad);
	box_display.pack_start(but_zoomres,Gtk::PACK_EXPAND_WIDGET,bpad);
	box_display.pack_start(lzoom,Gtk::PACK_EXPAND_WIDGET,0);
	
	fdisplay.set_label("Display");
	fdisplay.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	fdisplay.add(box_display);
	
	rbtransport.join_group(rbhousing);
	rbproduction.join_group(rbhousing);
	
	box_editor.set_homogeneous(1);
	box_editor.set_orientation(Gtk::ORIENTATION_VERTICAL);	
	box_editor.pack_start(but_editlink,Gtk::PACK_EXPAND_WIDGET,bpad);
	box_editor.pack_start(rbhousing,Gtk::PACK_EXPAND_WIDGET,0);
	box_editor.pack_start(rbtransport,Gtk::PACK_EXPAND_WIDGET,0);
	box_editor.pack_start(rbproduction,Gtk::PACK_EXPAND_WIDGET,0);
	rbhousing.set_active();
	
	feditor.set_label("Editor");
	feditor.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	feditor.add(box_editor);
	
	box_info.set_orientation(Gtk::ORIENTATION_VERTICAL);
	box_info.pack_start(lenj,Gtk::PACK_EXPAND_WIDGET,0);
	box_info.pack_start(lci,Gtk::PACK_EXPAND_WIDGET,0);
	box_info.pack_start(lmta,Gtk::PACK_EXPAND_WIDGET,0);
	
	finfo.set_label("Informations");
	finfo.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	finfo.add(box_info);
	
	box_menu.set_orientation(Gtk::ORIENTATION_VERTICAL);
	box_menu.pack_start(fgeneral,Gtk::PACK_SHRINK,0);
	box_menu.pack_start(fdisplay,Gtk::PACK_SHRINK,0);
	box_menu.pack_start(feditor,Gtk::PACK_SHRINK,0);
	box_menu.pack_start(finfo,Gtk::PACK_SHRINK,0);
	
	fdrawa.set_shadow_type(Gtk::SHADOW_NONE);
	fdrawa.add(drawa);
	
	box_main.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
	box_main.pack_start(box_menu,Gtk::PACK_SHRINK,5);
	box_main.pack_start(fdrawa,Gtk::PACK_EXPAND_WIDGET,0);
	
	add(box_main);

	show_all_children();
}

bool Gui::get_bpath() {
	return but_path.get_active();
}

int Gui::get_rbtype() {
	if (rbhousing.get_active()) return 0;
	if (rbtransport.get_active()) return 1;
	if (rbproduction.get_active()) return 2;
	return 0;
}

void Gui::g_setZoom(double zoom) {
	stringstream temp;
	temp << zoom;
	lzoom.set_label("Zoom: x" + temp.str());
}

void Gui::g_setEnj(double enj) {
	stringstream temp;
	temp << enj;
	lenj.set_label("ENJ: " + temp.str());
}

void Gui::g_setCi(double ci) {
	stringstream temp;
	temp << ci;
	lci.set_label("CI: " + temp.str());
}

void Gui::g_setMta(double mta) {
	stringstream temp;
	temp << mta;
	lmta.set_label("MTA: " + temp.str());
}

void Gui::button_text(Glib::ustring data) {
	std::cout << data << " was pressed" << std::endl;
}

void Gui::cexit() {
	button_text(but_exit.get_label());
	exit(1);
}

void Gui::ccreate() {
	button_text(but_create.get_label());
	(*ptr_v).nettoyer();
	selected_noeud = no_link;
	queue_draw();
}

void Gui::copen() {
	button_text(but_open.get_label());
	Gtk::FileChooserDialog dialog("Choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for((*ptr_g));
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);
	auto filter_text = Gtk::FileFilter::create();
	filter_text->set_name("Text files");
	filter_text->add_mime_type("text/plain");
	dialog.add_filter(filter_text);
	int result = dialog.run();
	switch(result) {
		case(Gtk::RESPONSE_OK): {
			button_text("Open");
			std::string filename = dialog.get_filename();
			std::cout << "File selected: " <<  filename << std::endl;
			(*ptr_v).nettoyer();
			selected_noeud = no_link;
			init(filename);
			queue_draw();
			break;
		}
		case(Gtk::RESPONSE_CANCEL): {
			button_text("Cancel");
			break;
		}
		default: {
			button_text("Unexpected button");
			break;
		}
	}
}

void Gui::csave() {
	button_text(but_save.get_label());
	Gtk::FileChooserDialog dialog("Save a file", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for((*ptr_g));
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Save", Gtk::RESPONSE_OK);
	auto filter_text = Gtk::FileFilter::create();
	filter_text->set_name("Text files");
	filter_text->add_mime_type("text/plain");
	dialog.add_filter(filter_text);
	int result = dialog.run();
	switch(result) {
		case(Gtk::RESPONSE_OK): {
			button_text("Save");
			std::string filename = dialog.get_filename();
			std::cout << "File selected: " <<  filename << std::endl;
			(*ptr_v).v_sauvegarder(filename);
			break;
		}
		case(Gtk::RESPONSE_CANCEL): {
			button_text("Cancel");
			break;
		}
		default: {
			button_text("Unexpected button");
			break;
		}
	}
	
}

void Gui::cpath() {
	button_text(but_path.get_label());
	queue_draw();

}

void Gui::czoomin() {
	button_text(but_zoomin.get_label());
	double newzoom = zoom + delta_zoom; 
	if (newzoom <= max_zoom + delta_zoom/2) { //à cause de delta_zoom = 0.200..0111
		zoom = newzoom;
		queue_draw();
	}
}

void Gui::czoomout() {
	button_text(but_zoomout.get_label());
	double newzoom = zoom - delta_zoom;
	if (newzoom >= min_zoom) {
		zoom = newzoom;
		queue_draw();
	}
}

void Gui::czoomres() {
	button_text(but_zoomres.get_label());
	zoom = 1;
	queue_draw();
}

void Gui::ceditlink() {
	button_text(but_editlink.get_label());
}

bool Gui::on_key_press_event(GdkEventKey* key_event) {
	if(key_event->type == GDK_KEY_PRESS) {
		switch(gdk_keyval_to_unicode(key_event->keyval)) {
			case 'r':
				czoomres();
				break;
			case 'i':
				czoomin();
				break;
			case 'o':
				czoomout();
				break;
			case '1':
				rbhousing.set_active();
				break;
			case '2':
				rbtransport.set_active();
				break;
			case '3':
				rbproduction.set_active();
				break;
		}
	}
	return Gtk::Window::on_key_press_event(key_event);
}

bool Gui::on_button_press_event(GdkEventButton* event) {
	double origin_x = drawa.get_allocation().get_x(), 
		   origin_y = drawa.get_allocation().get_y(), 
		   width = drawa.get_allocation().get_width(), 
		   height = drawa.get_allocation().get_height();
	if (event->type == GDK_BUTTON_PRESS && event->x >= origin_x &&
		event->x <= origin_x + width && event->y >= origin_y &&
		event->y <= origin_y + height) {
		pclic_x =drawatomodele_x(event->x - origin_x);
		pclic_y = drawatomodele_y(event->y - origin_y);
		size_t cind((*ptr_v).clicked_indice(pclic_x, pclic_y));
		if (event->button == 1) {
			if (cind == no_link) {
				if (selected_noeud == no_link) {
					(*ptr_v).nouveau_noeud(pclic_x, pclic_y, get_rbtype());
					queue_draw();
					return true;
				}
			} else if (cind == selected_noeud) {
				(*ptr_v).delete_noeud(selected_noeud);
				selected_noeud = no_link;
				queue_draw();
				return true;
			} else {
				if (but_editlink.get_active() && selected_noeud != no_link) {
					(*ptr_v).toggle_lien(selected_noeud,
										 (*ptr_v).clicked_indice(pclic_x, pclic_y));
					queue_draw();
					return true;
				} else {
					selected_noeud = cind;
					queue_draw();
					return true;
				}
			}
		} else if (event->button == 3) {
			if (selected_noeud != no_link) {
				(*ptr_v).move_noeud(selected_noeud, pclic_x, pclic_y);
				queue_draw();
				return true;
			}
		}
	}
	return Gtk::Window::on_button_press_event(event);
}

bool Gui::on_button_release_event(GdkEventButton* event) {
	double origin_x = drawa.get_allocation().get_x(), 
		   origin_y = drawa.get_allocation().get_y(), 
		   width = drawa.get_allocation().get_width(), 
		   height = drawa.get_allocation().get_height();
	if(event->type == GDK_BUTTON_RELEASE && event->x >= origin_x &&
		event->x <= origin_x + width && event->y >= origin_y &&
		event->y <= origin_y + height) {
		rclic_x = drawatomodele_x(event->x - origin_x);
		rclic_y = drawatomodele_y(event->y - origin_y);
		size_t cind((*ptr_v).clicked_indice(rclic_x, rclic_y));
		if (event->button == 1) {
			if (pclic_x == rclic_x && pclic_y == rclic_y) {
				if (cind == no_link) {
					selected_noeud = no_link;
					queue_draw();
					return true;
				}
			} else {
				if (selected_noeud != no_link) {
					(*ptr_v).resize_noeud(selected_noeud, pclic_x, pclic_y,
										  rclic_x, rclic_y);
					queue_draw();
					return true;
				}
			}
		}
	}
	return Gtk::Window::on_button_release_event(event);
}