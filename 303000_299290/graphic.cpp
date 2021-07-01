#include "graphic_gui.h"

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr); 
static double width, height, mindim, zoom(1.);

void modifier_couleur(int r, int g, int b) {
	(*ptcr)->set_source_rgb(r, g, b);
}

void demarrer_graphic(const Cairo::RefPtr<Cairo::Context>& cr,
					  double w, double h, double guizoom) {
	ptcr = &cr;
	width = w;
	height = h;
	zoom = guizoom;
	if (height<width) mindim = height;
	else mindim = width;
}

void dessiner_cercle(double centre_x, double centre_y, double rayon) {
	(*ptcr)->arc(modeletodrawa_x(centre_x),
				 modeletodrawa_y(centre_y),
				 modeletodrawa_r(rayon), -M_PI, M_PI);
	(*ptcr)->stroke();
}

double modeletodrawa_r(double rayon) {
	return rayon*mindim*zoom/(2.*dim_max);
}

double modeletodrawa_x(double x) {
	return mindim*(x + dim_max/zoom)*zoom/(2.*dim_max);
}

double modeletodrawa_y(double y) {
	return mindim*(dim_max/zoom - y)*zoom/(2.*dim_max);
}

double drawatomodele_x(double dx) {
	return dx*2.*dim_max/(mindim*zoom) - dim_max/zoom;
}

double drawatomodele_y(double dy) {
	return dim_max/zoom - dy*2.*dim_max/(mindim*zoom);
}

void dessiner_trait(double x0, double y0, double x1, double y1) {
	(*ptcr)->move_to(modeletodrawa_x(x0),
					 modeletodrawa_y(y0));
	(*ptcr)->line_to(modeletodrawa_x(x1),
					 modeletodrawa_y(y1));
	(*ptcr)->stroke();
}
 /*
void errorwin(std::string& errormsg) {
	Gtk::MessageDialog dialog("ERROR");
	dialog.set_secondary_text(errormsg);
	dialog.run();
}
*/

void rafraichir() {
	(*ptcr)->paint();
	(*ptcr)->stroke();
}

void changer_epaisseur(int n_epaisseur) {
	(*ptcr)->set_line_width(modeletodrawa_r(n_epaisseur));
}