#ifndef TOOLS_H
#define TOOLS_H

#include <cmath>
#include <string>
#include <iostream>
#include "graphic.h"
#include "constantes.h"
#include "graphic_color.h"

#define EPAISSEUR 4
#define EPAISSEURII 6

struct S2d {
	double x;
	double y;
};
struct Cercle {
	S2d centre;
	double rayon;
};
struct Droite {
	S2d point;
	S2d vect_dir;
};

void setMarge(double nouvelle_marge);
void setwidth(int n_epaisseur);
void setcolor(Couleur couleur);
void changer_couleur(Couleur c);

double distance_points(S2d point1, S2d point2);
double distance_point_droite(S2d p, Droite d);
bool collision_cercles(Cercle c1, Cercle c2);
double prod_scal(S2d vecteur_1, S2d vecteur_2);
bool collision_segment_cercle_entre(Cercle c, S2d p1, S2d p2);

void dessiner_lien(double x0, double y0, double x1, double y1);
void dessiner_etoile(double& centre_x, double& centre_y, double& grandeur);
void dessiner_rectangle(double origine_x, double origine_y,
						double longueur, double largeur);
void dessiner_logement(double& centre_x, double& centre_y, double& rayon);
void dessiner_blanc();
void errormsg(std::string errorstr);
#endif