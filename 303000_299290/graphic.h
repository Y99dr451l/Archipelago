#ifndef GRAPHIC_H
#define GRAPHIC_H

//#include <iostream>
#include <cmath>

#include "graphic_color.h"
#include "constantes.h"

void dessiner_cercle(double centre_x, double centre_y, double rayon);
void dessiner_trait(double x0, double y0, double x1, double y1);
double modeletodrawa_r(double rayon);
double modeletodrawa_x(double x);
double modeletodrawa_y(double y);
double drawatomodele_x(double dx);
double drawatomodele_y(double dy);
void rafraichir();
void modifier_couleur(int r, int g, int b);
//void errorwin(std::string& errormsg);
void changer_epaisseur(int n_epaisseur);
#endif