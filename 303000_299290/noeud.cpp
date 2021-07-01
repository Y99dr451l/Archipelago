#include "noeud.h"
#include "error.h"

unsigned int Noeud::total_liens;
std::array<unsigned int, NBR_QUARTIERS>  Noeud::somme_quartiers;
std::array<unsigned int, NBR_QUARTIERS>  Noeud::somme_nbp_quartiers;

namespace {
	bool errnoeud;
}

Noeud::Noeud(size_t& uid_, double& x_, double& y_, unsigned int& nbp_,
			 std::vector<Noeud*>& noeuds): uid(uid_), nbp(nbp_), nb_liens(0) {
		test_creation_noeud(uid_, x_, y_, nbp_, noeuds);
		c = { { x_, y_ }, sqrt(nbp_) };
}

Noeud::~Noeud() {}

Logement::Logement(size_t& uid_, double& x_, double& y_, unsigned int& nbp_,
		  std::vector<Noeud*>& noeuds): Noeud::Noeud(uid_, x_, y_, nbp_, noeuds) {
	if (!errnoeud) {
		type = LOGEMENT;
		incrementer_nbp_quartier(nbp_);
		incrementer_quartier();
	}
}

Transport::Transport(size_t& uid_, double& x_, double& y_, unsigned int& nbp_,
	      std::vector<Noeud*>& noeuds):	Noeud::Noeud(uid_, x_, y_, nbp_, noeuds) {									
	if (!errnoeud) {
		type = TRANSPORT;
		incrementer_nbp_quartier(nbp_);
		incrementer_quartier();
	}
}

Production::Production(size_t& uid_, double& x_, double& y_, unsigned int& nbp_,
	      std::vector<Noeud*>& noeuds): Noeud::Noeud(uid_, x_, y_, nbp_, noeuds) { 								
	if (!errnoeud) {
		type = PRODUCTION;
		incrementer_nbp_quartier(nbp_);
		incrementer_quartier();
	}
}

void Noeud::nettoyer_noeud() {
	somme_quartiers.fill(0);
	somme_nbp_quartiers.fill(0);
	total_liens = 0;
}

void Noeud::sauvegarder(std::ofstream& os) {
	os << getUid() << " " << getX() << " " << getY() << " " << getNbp() << std::endl;
}

void Logement::incrementer_quartier() {
	++(somme_quartiers[LOGEMENT]);
}

void Transport::incrementer_quartier() {
	++(somme_quartiers[TRANSPORT]);
}

void Production::incrementer_quartier() {
	++(somme_quartiers[PRODUCTION]);
}

void Logement::decrementer_quartier() {
	--(somme_quartiers[LOGEMENT]);
}

void Transport::decrementer_quartier() {
	--(somme_quartiers[TRANSPORT]);
}

void Production::decrementer_quartier() {
	--(somme_quartiers[PRODUCTION]);
}

void Logement::incrementer_nbp_quartier(unsigned int& nbp) {
	somme_nbp_quartiers[LOGEMENT]+= nbp;
}

void Transport::incrementer_nbp_quartier(unsigned int& nbp) {
	somme_nbp_quartiers[TRANSPORT]+= nbp;
}

void Production::incrementer_nbp_quartier(unsigned int& nbp) {
	somme_nbp_quartiers[PRODUCTION]+= nbp;
}

void Logement::decrementer_nbp_quartier(unsigned int& nbp) {
	somme_nbp_quartiers[LOGEMENT]-= nbp;
}

void Transport::decrementer_nbp_quartier(unsigned int& nbp) {
	somme_nbp_quartiers[TRANSPORT]-= nbp;
}

void Production::decrementer_nbp_quartier(unsigned int& nbp) {
	somme_nbp_quartiers[PRODUCTION]-= nbp;
}

void Noeud::incrementer_total_liens() {
	++total_liens;
}

void Noeud::decrementer_total_liens() {
	--total_liens;
}

void Noeud::incrementer_nbliens() {
	++nb_liens;
}

void Noeud::decrementer_nbliens() {
	--nb_liens;
}

void Logement::me_dessiner() {
	dessiner_logement(c.centre.x, c.centre.y, c.rayon);
}

void Transport::me_dessiner() {
	dessiner_logement(c.centre.x, c.centre.y, c.rayon);
	dessiner_etoile(c.centre.x, c.centre.y, c.rayon);
}

void Production::me_dessiner() {
	double ctse_construction1(3./4), ctse_construction2(1./8), 
		   ctse_construction3(3./2), ctse_construction4(1./4);
	dessiner_logement(c.centre.x, c.centre.y, c.rayon);
	dessiner_rectangle((c.centre.x - ctse_construction1 * c.rayon),
					   (c.centre.y - ctse_construction2 * c.rayon),
					   (ctse_construction3 * c.rayon),
					   (ctse_construction4 * c.rayon));
}

int Noeud::getType() {
	return type;
}

double Noeud::getRayon() {
	return c.rayon;
}

unsigned int Noeud::getNbp() {
	return nbp;
}

double Noeud::getX() {
	return c.centre.x;
}

double Noeud::getY() {
	return c.centre.y;
}

std::array<unsigned int, NBR_QUARTIERS> Noeud::getNbpquartiers() {
	return somme_nbp_quartiers;
}

unsigned int Noeud::getTotalliens() {
	return total_liens;
}

size_t Noeud::getUid() {
	return uid;
}

unsigned int Noeud::get_nbLiens() { 
	return nb_liens;
}

Cercle Noeud::getCercle() {
	return c;
}

double Noeud::getAccess() {
	return access;
}

unsigned int Noeud::getParent() {
	return parent;
}

std::vector<Noeud*> Noeud::getVoisins() {
	return voisins;
}

std::array<unsigned int, NBR_QUARTIERS> Noeud::getSommequartier() {
	return somme_quartiers;
}

bool Noeud::getIn() {
	return in;
}

void Noeud::setUid(size_t n_uid) {
	uid = n_uid;
}

void Noeud::setNbp(size_t n_nbp) {
	nbp = n_nbp;
}

void Noeud::setCercle(Cercle n_cercle) {
	c = n_cercle;
}

void Noeud::setAccess(double n_access) {
	access = n_access;
}

void Noeud::setParent(unsigned int n) {
	parent = n;
}

void Noeud::setIn(bool n_in) {
	in = n_in;
}

bool decodage_chaine_noeud(std::vector<Noeud*>& noeuds,
						   std::istringstream& iss, int& type) {
	Noeud* ptr_n(nullptr);
	size_t uid;
	double x, y;
	unsigned int nbp;
	iss >> uid >> x >> y >> nbp;
	switch (type) {
		case LOGEMENT:
			ptr_n = (new Logement(uid, x, y, nbp, noeuds));
			break;
		case TRANSPORT:
			ptr_n = (new Transport(uid, x, y, nbp, noeuds));
			break;
		case PRODUCTION:
			ptr_n = (new Production(uid, x, y, nbp, noeuds));
			break;
	}
	if (errnoeud) {
		(*ptr_n).~Noeud();
		return true;
	}
	noeuds.push_back(ptr_n);
	return false;
}

bool decodage_chaine_lien(std::vector<Noeud*>& noeuds, std::istringstream& iss,
						  std::vector<std::array<size_t, LIEN>>& liens) {
	size_t depart, arrivee;
	iss >> depart >> arrivee;
	if (test_creation_lien(depart, arrivee, liens, noeuds)) return true;
	std::array<size_t, LIEN> lien = {depart, arrivee};
	liens.push_back(lien);
	Noeud::incrementer_total_liens();
	return false;
}

bool test_creation_noeud(size_t& uid, double& x, double& y, unsigned int& nbp,
	std::vector<Noeud*>& noeuds) {
	errnoeud = false;
	if (uid == no_link) {
		errormsg(error::reserved_uid());
		errnoeud = true;
		return true;
	}
	if (nbp < min_capacity) {
		errormsg(error::too_little_capacity(nbp));
		errnoeud = true;
		return true;
	}
	if (nbp > max_capacity) {
		errormsg(error::too_much_capacity(nbp));
		errnoeud = true;
		return true;
	}
	S2d centre_noeud;
	Cercle c;
	for (int i(0), taille(noeuds.size()); i < taille; ++i) {
		if ((*noeuds[i]).getUid() == uid) {
			errormsg(error::identical_uid(uid));
			errnoeud = true;
			return true;
		}
		centre_noeud = { x, y };
		c = { centre_noeud, sqrt(nbp) };
		if (collision_cercles(c, (*noeuds[i]).getCercle())) {
			errormsg(error::node_node_superposition(uid, (*noeuds[i]).getUid()));
			errnoeud = true;
			return true;
		}
	}
	return false;
}

bool test_creation_lien(size_t& depart, size_t& arrivee,
						std::vector<std::array<size_t, 2>>& liens,
						std::vector<Noeud*>& noeuds) {
	int compte(0), existe(0);
	size_t liens_depart(0), liens_arrivee(0);
	constexpr size_t sortie(2), increment(1);
	if (depart == arrivee) {
		errormsg(error::self_link_node(depart));
		return true;
	}
	for (int i(0), taille(liens.size()); i < taille; ++i) {
		if ((liens[i].front() == depart && liens[i].back() == arrivee) ||
			(liens[i].back() == depart && liens[i].front() == arrivee)) {
			errormsg(error::multiple_same_link(depart, arrivee));
			return true;
		}
	}
	for (int i(0), taille(noeuds.size()); i < taille; ++i) {
		if ((*noeuds[i]).getUid() == depart) {
			if (((*noeuds[i]).getType() == LOGEMENT) && 
			   ((*noeuds[i]).get_nbLiens() + increment > max_link)) {
				errormsg(error::max_link(depart));
				return true;
			}
			liens_depart = i;
			++compte;
			++existe;
			if (compte == sortie) break;
		}
		if ((*noeuds[i]).getUid() == arrivee) {
			if (((*noeuds[i]).getType() == LOGEMENT) && 
			   ((*noeuds[i]).get_nbLiens() + increment > max_link)) {
				errormsg(error::max_link(arrivee));
				return true;
			}
			liens_arrivee = i;
			++compte;
			if (compte == sortie) break;
		}
	}
	if (compte != sortie) {
		switch (existe) {
			case (false) : errormsg(error::link_vacuum(depart));
				return true;
				break;
			case (true) : errormsg(error::link_vacuum(arrivee));
				return true;
				break;
		}
	}
	S2d p1 = { (*noeuds[liens_depart]).getX(), (*noeuds[liens_depart]).getY() };
	S2d p2 = { (*noeuds[liens_arrivee]).getX(), (*noeuds[liens_arrivee]).getY() };
	for (int i(0), taille(noeuds.size()); i < taille; ++i) {
		if (collision_segment_cercle_entre((*noeuds[i]).getCercle(), p1, p2)) {
			errormsg(error::node_link_superposition((*noeuds[i]).getUid()));
			return true;
		}
	}
	(*noeuds[liens_depart]).nouveau_voisin(noeuds[liens_arrivee]);
	(*noeuds[liens_arrivee]).nouveau_voisin(noeuds[liens_depart]);
	(*noeuds[liens_depart]).incrementer_nbliens();
	(*noeuds[liens_arrivee]).incrementer_nbliens();
	return false;
}

void Noeud::nouveau_voisin(Noeud* noeud) {
	voisins.push_back(noeud);
}

void Noeud::supprimer_voisins(int indice) {
	voisins[indice] = voisins.back();
	voisins.pop_back();
}