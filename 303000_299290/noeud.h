#ifndef NOEUD_H
#define NOEUD_H

#include <vector>
#include <array>
#include <sstream>
#include <fstream>

#include "tools.h"

#define NBR_QUARTIERS 3
#define LOGEMENT 0
#define TRANSPORT 1
#define PRODUCTION 2
#define LIEN 2

class Noeud {
private:
	size_t uid;
	unsigned int nbp;
	unsigned int nb_liens;
	
public:
	Noeud();
	Noeud(size_t& uid_, double& x_, double& y_, unsigned int& nbp_,
		  std::vector<Noeud*>& noeuds);
	virtual ~Noeud();
	
	static void nettoyer_noeud();
	static std::array<unsigned int, NBR_QUARTIERS> getSommequartier();
	void sauvegarder(std::ofstream& os);
	void nouveau_voisin(Noeud* noeud);
	virtual void me_dessiner() = 0;

	void incrementer_nbliens();
	void decrementer_nbliens();
	static void incrementer_total_liens();
	static void decrementer_total_liens();
	virtual void incrementer_quartier() = 0;
	virtual void decrementer_quartier() = 0;
	virtual void incrementer_nbp_quartier(unsigned int& nbp) = 0;
	virtual void decrementer_nbp_quartier(unsigned int& nbp) = 0;
	
	double getRayon();
	unsigned int getNbp();
	double getX();
	double getY();
	Cercle getCercle();
	size_t getUid();
	unsigned int get_nbLiens();
	int getType();
	static unsigned int getTotalliens();
	static std::array<unsigned int, NBR_QUARTIERS> getNbpquartiers();
	std::vector<Noeud*> getVoisins();
	unsigned int getParent();
	bool getIn();
	double getAccess();
	
	void setUid(size_t n_uid);
	void setNbp(size_t n_nbp);
	void setCercle(Cercle n_cercle);
	void setParent(unsigned int n);
	void setIn(bool n_in);
	void setAccess(double n_access);
	void supprimer_voisins(int indice);

protected:
	int type;
	Cercle c;
	std::vector<Noeud*> voisins;
	bool in;
	double access;
	unsigned int parent;
	static std::array<unsigned int, NBR_QUARTIERS>  somme_quartiers;
	static std::array<unsigned int, NBR_QUARTIERS>  somme_nbp_quartiers;
	static unsigned int total_liens;
};

class Logement : public Noeud {
	public:
	Logement(size_t& uid_, double& x_, double& y_, unsigned int& nbp_,
		  std::vector<Noeud*>& noeuds);
	void me_dessiner();
	void incrementer_quartier();
	void decrementer_quartier();
	void incrementer_nbp_quartier(unsigned int& nbp);
	void decrementer_nbp_quartier(unsigned int& nbp);
};

class Transport : public Noeud {
	public:
	Transport(size_t& uid_, double& x_, double& y_, unsigned int& nbp_,
	      std::vector<Noeud*>& noeuds);
	void me_dessiner();
	void incrementer_quartier();
	void decrementer_quartier();
	void incrementer_nbp_quartier(unsigned int& nbp);
	void decrementer_nbp_quartier(unsigned int& nbp);
};

class Production : public Noeud {
	public:
	Production(size_t& uid_, double& x_, double& y_, unsigned int& nbp_,
	      std::vector<Noeud*>& noeuds);
	void me_dessiner();
	void incrementer_quartier();
	void decrementer_quartier();
	void incrementer_nbp_quartier(unsigned int& nbp);
	void decrementer_nbp_quartier(unsigned int& nbp);
};

bool decodage_chaine_noeud(std::vector<Noeud*>& noeuds,
						   std::istringstream& iss, int& type);
bool decodage_chaine_lien(std::vector<Noeud*>& noeuds, std::istringstream& iss,
						  std::vector<std::array<size_t, 2>>& liens);
bool test_creation_noeud(size_t& uid, double& x, double& y,
			 unsigned int& nbp, std::vector<Noeud*>& noeuds);
bool test_creation_lien(size_t& depart, size_t& arrivee,
						std::vector<std::array<size_t, 2>>& liens,
						std::vector<Noeud*>& noeuds);
#endif