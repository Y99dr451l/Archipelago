#include "tools.h"

static int epaisseur;
static Couleur color;
static double marge(0);

void setwidth(int n_epaisseur) {
	epaisseur = n_epaisseur;
}

void setcolor(Couleur couleur) {
	color = couleur;
}

void setMarge(double nouvelle_marge) {
	marge = nouvelle_marge;
}

void changer_couleur(Couleur c) {
	switch (c) {
		case (Noir):
			modifier_couleur(false, false, false);
			break;
		case (Blanc):
			modifier_couleur(true, true, true);
			break;
		case (Rouge):
			modifier_couleur(true, false, false);
			break;
		case (Vert):
			modifier_couleur(false, true, false);
			break;
	}
}

double distance_points(S2d point1, S2d point2) {
	return (sqrt((point1.x - point2.x) * (point1.x - point2.x) +
			(point1.y - point2.y) * (point1.y - point2.y))); 
}

bool collision_cercles(Cercle c1, Cercle c2) {
	if (distance_points(c1.centre, c2.centre) <=
		(c1.rayon + c2.rayon + marge)) return true;
	return false;
}

double distance_point_droite(S2d p, Droite d) {
	S2d vect_1_3 = { p.x - d.point.x,p.y - d.point.y };
	double d_scal = (vect_1_3.x * d.vect_dir.x + vect_1_3.y * d.vect_dir.y) /
					sqrt(d.vect_dir.x * d.vect_dir.x + d.vect_dir.y * d.vect_dir.y);
	double c_test = sqrt(vect_1_3.x * vect_1_3.x + vect_1_3.y * vect_1_3.y);
	double d_test = sqrt(c_test * c_test - d_scal * d_scal);
	return d_test;
}

double prod_scal(S2d vecteur_1, S2d vecteur_2) {
	return (vecteur_1.x * vecteur_2.x + vecteur_1.y * vecteur_2.y); 
}

bool collision_segment_cercle_entre(Cercle c, S2d p1, S2d p2) {
	S2d vecteur_1 = {p2.x - p1.x, p2.y - p1.y};
	S2d vecteur_p1_c = {c.centre.x - p1.x, c.centre.y - p1.y};
	if (prod_scal(vecteur_1, vecteur_p1_c) <= 0) return false;
	S2d vecteur_2 = {-vecteur_1.x, -vecteur_1.y};
	S2d vecteur_p2_c = {c.centre.x - p2.x, c.centre.y - p2.y};
	if (prod_scal(vecteur_2, vecteur_p2_c) <= 0) return false;
	Droite d = {p1 , vecteur_1};
	if (distance_point_droite(c.centre, d) <= c.rayon + marge) return true; 
	return false;
}

void dessiner_lien(double x0, double y0, double x1, double y1) {
	changer_epaisseur(epaisseur);
	changer_couleur(color);
	dessiner_trait(x0, y0, x1, y1);
}

void dessiner_logement(double& centre_x, double& centre_y, double& rayon) {
	changer_epaisseur(rayon);
	changer_couleur(Blanc);
	dessiner_cercle(centre_x, centre_y, rayon/2);
	changer_epaisseur(epaisseur);
	changer_couleur(color);
	dessiner_cercle(centre_x, centre_y, rayon);
}

void dessiner_etoile(double& centre_x, double& centre_y, double& rayon) {
	double projection(rayon*sqrt(2)/2.);
	dessiner_trait(centre_x - rayon, centre_y,
				   centre_x + rayon, centre_y);
	dessiner_trait(centre_x, centre_y - rayon,
				   centre_x, centre_y + rayon);
	dessiner_trait(centre_x - projection, centre_y + projection,
				   centre_x + projection, centre_y - projection);
	dessiner_trait(centre_x - projection, centre_y - projection,
				   centre_x + projection, centre_y + projection);
}

void dessiner_rectangle(double origine_x, double origine_y,
						double longueur, double largeur) {
	dessiner_trait(origine_x, origine_y, origine_x + longueur, origine_y);
	dessiner_trait(origine_x, origine_y, origine_x, origine_y + largeur);
	dessiner_trait(origine_x + longueur, origine_y + largeur,
				   origine_x, origine_y + largeur);
	dessiner_trait(origine_x + longueur, origine_y + largeur,
				   origine_x + longueur, origine_y);
}

void dessiner_blanc() {
	changer_couleur(Blanc);
	rafraichir();
	changer_couleur(color);
}

void errormsg(std::string errorstr) {
	std::cout << errorstr << std::endl;
	//errorwin(errorstr);
}