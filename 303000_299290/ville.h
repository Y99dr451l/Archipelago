#ifndef VILLE_H
#define VILLE_H

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include "noeud.h"

#define NBR_QUARTIERS 3
#define LOGEMENT 0
#define TRANSPORT 1
#define PRODUCTION 2
#define LIEN 2

void init(std::string& nom_fichier);

class Ville {
public:
	Ville();
	virtual ~Ville();
	
	static int lecture(std::string& nom_fichier);
	static void nettoyer();
	void v_sauvegarder(std::string nom_fichier);
	void trier_noeuds();

	void dessiner_ville();
	void dessiner_path(size_t selected_noeud);
	void dessiner_select(size_t selected_noeud);
	
	double getEnj();
	double getCi();
	double getMta();
	
	void calcul_enj();
	void calcul_ci(size_t indice_x, size_t indice_y);
	void incrementer_ci(double ajout);
	void calcul_mta();

	size_t dijkstra(std::vector<Noeud *> &tn, size_t ind_dep, int type);
	double min_access(std::vector<Noeud*>& tn, std::vector<size_t>& ta);
	double calculer_access(std::vector<Noeud*>& tn, std::vector<Noeud*>& voisins_tnn,
						   size_t& lv, size_t& n);
	void tri(std::vector<Noeud*>& tn, std::vector<Noeud*>& voisins,
			 std::vector<size_t>& ta, size_t& lv);
	void swap(std::vector<size_t>& ta, size_t j, size_t indice);

	size_t clicked_indice(double x, double y);
	size_t find_uid(size_t uid);
	void move_noeud(size_t indice, double x, double y);
	bool tests_noeud(Cercle cercle, size_t iuid);
	bool nouveau_noeud(double x, double y, int type);
	size_t nouvel_uid();
	bool test_uid(size_t uid);
	void delete_noeud(size_t indice);
	bool resize_noeud(size_t indice, double x0, double y0, double x1, double y1);
	void toggle_lien(size_t indice1, size_t indice2);

private:
	static std::vector<Noeud**> n_ordre;
	static double ci, enj, mta;
	static std::vector<Noeud*> noeuds;
	static std::vector<std::array<size_t, LIEN>> liens;
};

static Ville* ptr_v = (new Ville());
#endif