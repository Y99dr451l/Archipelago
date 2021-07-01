#include "ville.h"
#include "tools.h"
#include "error.h"

using namespace std;

static bool getline_filtre(ifstream& fichier, string& ligne);

Ville::Ville() {}
Ville::~Ville() {}

std::vector<Noeud*> Ville::noeuds;
std::vector<std::array<size_t, LIEN>> Ville::liens;
std::vector<Noeud**> Ville::n_ordre;
double Ville::ci, Ville::enj, Ville::mta;

void init(string& nom_fichier) {
	setMarge(0);
	Ville::nettoyer();
	Ville::lecture(nom_fichier);
}

void Ville::nettoyer() {
	Noeud::nettoyer_noeud();
	for (size_t i(0), taille(noeuds.size()) ; i < taille ; i++) {
		(*noeuds[i]).~Noeud();
		noeuds[i] = nullptr;
	}
	noeuds.clear();
	liens.clear();
	n_ordre.clear();
	ci = 0;
	enj = 0;
	mta = 0;
}

int Ville::lecture(string& nom_fichier) {
	int type(LOGEMENT), temp;
	string ligne;
	ifstream fichier(nom_fichier);
	while (getline_filtre(fichier, ligne)) {
		istringstream iss1(ligne);
		iss1 >> temp;
		for (int i(0) ; i < temp ; i++) {
			getline_filtre(fichier, ligne);
			istringstream iss2(ligne);
			if (type < NBR_QUARTIERS) {
				if (decodage_chaine_noeud(noeuds, iss2, type)) {
					nettoyer();
					return true;
				}
			} else {
				if (decodage_chaine_lien(noeuds, iss2, liens)) {
					nettoyer();
					return true;
				}
			}
		}
		++type;
	}
	setMarge(dist_min);
	cout << error::success();
	return false;
}

static bool getline_filtre(ifstream& fichier, string& ligne) {
	if (!getline(fichier >> ws, ligne)) return false;
	if (ligne.front() == '#') return getline_filtre(fichier, ligne);
	else return true;
}

void Ville::v_sauvegarder(std::string nom_fichier) {
	ofstream os(nom_fichier);
	trier_noeuds();
	array<unsigned int, NBR_QUARTIERS> nb_quart(Noeud::getSommequartier());
	for (size_t i(0), k(0), taille(noeuds.size()+1) ; i < taille ; ++k) {
		if (k < NBR_QUARTIERS) {
			os << nb_quart[k] << endl;
			for (size_t j(0), taille(nb_quart[k]); j < taille ; i++, ++j) {
				(**(n_ordre[i])).sauvegarder(os);
			}
		} else {
			os << Noeud::getTotalliens() << std::endl;
			for (size_t j(0), taille(liens.size()); j < taille ; ++j) {
				os << liens[j].front() << " " << liens[j].back() << endl;
			}
			i++;
		}
	}
}

void Ville::trier_noeuds() {
	for (int i(0), taille(NBR_QUARTIERS) ; i < taille ; i++) {
		for (int j(0), taille(noeuds.size()) ; j < taille ; ++j) {
			if ((*noeuds[j]).getType() == i ) {
				Noeud** ptr_ptr_n(&noeuds[j]);
				n_ordre.push_back(ptr_ptr_n);
			}
		}
	}
}

void Ville::dessiner_ville() {
	dessiner_blanc();
	size_t indice_x, indice_y;
	int sortie(0), cond_sortie(LIEN);
	ci = 0;
	for (size_t i(0), taille(liens.size()) ; i < taille ; i++) {
		for (size_t j(0), taille(noeuds.size()) ; j < taille ; ++j) {
			if ((*noeuds[j]).getUid() == liens[i].front()) { 
				indice_x = j;
				++sortie;
				if (sortie == cond_sortie) break;
			}
			if ((*noeuds[j]).getUid() == liens[i].back()) {
				indice_y = j;
				++sortie;
				if (sortie == cond_sortie) break;
			}
		}
		dessiner_lien((*noeuds[indice_x]).getX(), (*noeuds[indice_x]).getY(),
					   (*noeuds[indice_y]).getX(), (*noeuds[indice_y]).getY());
		calcul_ci(indice_x, indice_y);
	}
	for (size_t i(0), taille(noeuds.size()) ; i < taille ; i++) {
		(*noeuds[i]).me_dessiner();
	}
}

void Ville::dessiner_path(size_t selected_noeud) {
	if ((*noeuds[selected_noeud]).getType() == LOGEMENT) {
		for (int i(TRANSPORT); i <= PRODUCTION; i++) {
			size_t ind1(dijkstra(noeuds, selected_noeud, i)), ind2(0);
			if (ind1 != no_link) {
				ind2 = (*noeuds[ind1]).getParent();
				while (ind1 != selected_noeud) {
					dessiner_lien((*noeuds[ind1]).getX(), (*noeuds[ind1]).getY(),
								(*noeuds[ind2]).getX(), (*noeuds[ind2]).getY());
					(*noeuds[ind1]).me_dessiner();
					ind1 = ind2;
					if (ind2 != selected_noeud) ind2 = (*noeuds[ind1]).getParent();
				}
			}
		}
	}
}

void Ville::dessiner_select(size_t indice) {
	(*noeuds[indice]).me_dessiner();
}

double Ville::getEnj() {
	return enj;
}

double Ville::getCi() {
	return ci;
}

double Ville::getMta() {
	return mta;
}

void Ville::calcul_enj() {
	std::array<unsigned int, NBR_QUARTIERS> nbp_q = Noeud::getNbpquartiers();
	double temp0(nbp_q[LOGEMENT]), temp1(nbp_q[TRANSPORT]),
		   temp2(nbp_q[PRODUCTION]), temp;
	if (temp0+temp1+temp2 == 0) temp = 0;
	else temp = ((temp0 - (temp1 + temp2))/(temp0 + temp1 + temp2));
	enj = temp;
}

void Ville::calcul_ci(size_t indice_x, size_t indice_y) {
	S2d point1, point2;
	double distance_liens, vitesse_co, capacite_co;
	point1.x = (*noeuds[indice_x]).getX();
	point1.y = (*noeuds[indice_x]).getY();
	point2.x = (*noeuds[indice_y]).getX();
	point2.y = (*noeuds[indice_y]).getY();
	distance_liens = distance_points(point1, point2);
	if (((*noeuds[indice_x]).getType() == TRANSPORT) &&
		((*noeuds[indice_y]).getType() == TRANSPORT))
		vitesse_co = fast_speed;
	else vitesse_co = default_speed;
	if ((*noeuds[indice_x]).getNbp() <= (*noeuds[indice_y]).getNbp())
		capacite_co = (*noeuds[indice_x]).getNbp();
	else capacite_co = (*noeuds[indice_y]).getNbp();
	incrementer_ci(distance_liens * capacite_co * vitesse_co);
}

void Ville::incrementer_ci(double ajout) {
	ci += ajout;
}

void Ville::calcul_mta() {
	double total(0);
	double nb_log(0);
	size_t dij1, dij2;
	mta = 0;
	for (size_t i(0), taille(noeuds.size()) ; i < taille ; i++) {
		if ((*noeuds[i]).getType() == LOGEMENT) {
			++nb_log;
			dij1 = dijkstra(noeuds, i, TRANSPORT);
			if (dij1 == no_link) {
				mta = infinite_time;
				break;
			}
			total += (*noeuds[dij1]).getAccess();
			dij2 = dijkstra(noeuds, i, PRODUCTION);
			if (dij2 == no_link) {
				mta = infinite_time;
				break;
			}
			total += (*noeuds[dij2]).getAccess();
		}
	}
	if (nb_log == 0) mta = 0;
	else if (mta < infinite_time) mta = (total/nb_log);
}

size_t Ville::dijkstra(vector<Noeud*>& tn, size_t ind_dep, int type) {
	vector<Noeud*> vs((*noeuds[ind_dep]).getVoisins());
	if (vs.size() == 0) return no_link;
	vector<size_t> ta;
	for (size_t i(0), taille(tn.size()) ; i < taille ; i++) {
		ta.push_back(i);
		(*tn[i]).setIn(true);
		(*tn[i]).setAccess(infinite_time);
		(*tn[i]).setParent(no_link);
	}
	swap(ta, 0, ind_dep);
	(*tn[ta.front()]).setAccess(0);
	size_t n(0);
	double alt(0);
	for (size_t i(0), max(tn.size()); i < max ; ++i) {
		int mina(min_access(tn, ta));
		if (mina == -1) return no_link;
		n = ta[mina];
		if ((*tn[n]).getType() == type && (*tn[n]).getAccess() != infinite_time)
			return n;
		(*tn[n]).setIn(false);
		if ((*tn[n]).getType() != PRODUCTION) {
		std::vector<Noeud*> voisins_tnn((*tn[n]).getVoisins());
			for (size_t lv(0); lv < ((*tn[n]).getVoisins()).size(); ++lv) {
				if ((*voisins_tnn[lv]).getIn() == true) {
					alt = (*tn[n]).getAccess()+calculer_access(tn, voisins_tnn, lv, n);
					if ((*voisins_tnn[lv]).getAccess() > alt) {
						(*voisins_tnn[lv]).setAccess(alt);
						(*voisins_tnn[lv]).setParent(n);
						tri(tn, voisins_tnn, ta, lv);
					}
				}
			}
		}
	}
	return no_link;
}

double Ville::min_access(vector<Noeud*>& tn, vector<size_t>& ta) {
	for (size_t i(0), taille(ta.size()) ; i < taille ; i++) {
		if ((*tn[ta[i]]).getIn() == true) return i;
	}
	return -1;
}

double Ville::calculer_access(vector<Noeud*>& tn, std::vector<Noeud*>& voisins_tnn,
							  size_t& lv, size_t& n) {
	S2d point1, point2;
	point1.x = (*tn[n]).getX();
	point1.y = (*tn[n]).getY();
	point2.x = (*voisins_tnn[lv]).getX();
	point2.y = (*voisins_tnn[lv]).getY();
	if ((*tn[n]).getType() == TRANSPORT && (*voisins_tnn[lv]).getType() == TRANSPORT)
		return distance_points(point1, point2)/fast_speed;
	return distance_points(point1, point2)/default_speed;
}

void Ville::tri(vector<Noeud*>& tn, vector<Noeud*>& voisins,
				vector<size_t>& ta, size_t& lv) {
	size_t uid_v((*voisins[lv]).getUid()), indice;
	for (size_t i(0), taille(ta.size()) ; i < taille ; i++) {
		if ((*tn[ta[i]]).getUid() == uid_v) {
			indice = i;
			break;
		}
	}
	for (size_t i(0), taille(ta.size()) ; i < taille ; i++) {
		if ((*tn[ta[i]]).getAccess() > (*tn[ta[indice]]).getAccess()) {
			for (size_t j(i) ; j < indice ; ++j) {
				swap(ta, j, indice);
			}
		break;
		}
	}
}

void Ville::swap(vector<size_t>& ta, size_t j, size_t indice) {
	size_t temp(ta[indice]);
	ta[indice] = ta[j];
	ta[j] = temp;
}

size_t Ville::clicked_indice(double x, double y) {
	S2d clicked_point {x, y};
	for (size_t i (0); i < noeuds.size(); i++) {
		Cercle cercle((*noeuds[i]).getCercle());
		S2d centre_noeud(cercle.centre);
		if (distance_points(clicked_point, centre_noeud) <= (*noeuds[i]).getRayon())
			return i;
	}
	return no_link;
}

size_t Ville::find_uid(size_t uid) {
	for (size_t i (0); i < noeuds.size(); i++) {
		if ((*noeuds[i]).getUid() == uid) return i;
	}
	return no_link;
}

void Ville::move_noeud(size_t indice, double x, double y) {
	S2d icentre {x, y};
	Cercle tempcercle {icentre, (*noeuds[indice]).getRayon()};
	size_t iuid((*noeuds[indice]).getUid());
	if (tests_noeud(tempcercle, iuid)) (*noeuds[indice]).setCercle(tempcercle);
}

bool Ville::nouveau_noeud(double x, double y, int type) {
	size_t uid(nouvel_uid());
	S2d centre {x, y};
	Cercle cercle {centre, sqrt(min_capacity)};
	for (size_t i(0); i < liens.size(); i++) {
		Cercle cercle1((*noeuds[find_uid(liens[i][0])]).getCercle());
		Cercle cercle2((*noeuds[find_uid(liens[i][1])]).getCercle());
		if (collision_segment_cercle_entre(cercle, cercle1.centre, cercle2.centre)) {
			errormsg(error::node_link_superposition(uid));
			return false;
		}
	}
	string ligne(to_string(uid) + " " + to_string(x) + " " +
				to_string(y) + " " + to_string(min_capacity));
	istringstream iss(ligne);
	setMarge(dist_min);
	decodage_chaine_noeud (noeuds, iss, type);
	return true;
}

size_t Ville::nouvel_uid() {
	bool found(false);
	size_t uid(0);
	while (!found) {
		if (test_uid(uid)) found = true;
		++uid;
	}
	return uid - 1;
}

bool Ville::test_uid(size_t uid) {
	for (size_t i(0); i < noeuds.size(); i++) {
		if (uid == (*noeuds[i]).getUid()) return false;
	}
	return true;
}

void Ville::delete_noeud(size_t indice) {
	for (size_t i(0); i < noeuds.size(); i++) {
		vector<Noeud*> vs((*noeuds[i]).getVoisins());
		for (size_t j(0); j < vs.size(); j++) {
			if ((*vs[j]).getUid() == (*noeuds[indice]).getUid()) {
				(*noeuds[i]).supprimer_voisins(j);
				(*noeuds[i]).decrementer_nbliens();
			}
		}
	}
	for (size_t i(0); i < liens.size(); i++) {
		if (liens[i][0] == (*noeuds[indice]).getUid() ||
			liens[i][1] == (*noeuds[indice]).getUid()) {
			liens[i] = liens.back();
			liens.pop_back();
			(*noeuds[indice]).decrementer_total_liens();
			--i;
		}
	}
	(*noeuds[indice]).decrementer_quartier();
	unsigned int dnbp((*noeuds[indice]).getNbp());
	(*noeuds[indice]).decrementer_nbp_quartier(dnbp);
	noeuds[indice] = noeuds.back();
	noeuds.pop_back();
}

bool Ville::resize_noeud(size_t indice, double x0, double y0, double x1, double y1) {
	Cercle cercle((*noeuds[indice]).getCercle());
	S2d point0 {x0, y0},
		point1 {x1, y1},
		icentre(cercle.centre);
	double rayon_debut(distance_points(icentre, point0)),
		   rayon_fin(distance_points(icentre, point1)),
		   rayon_final((*noeuds[indice]).getRayon() + rayon_fin - rayon_debut),
		   dnewnbp(rayon_final * rayon_final);
	if (dnewnbp < min_capacity || rayon_final < 0) dnewnbp = min_capacity;
	if (dnewnbp > max_capacity) dnewnbp = max_capacity;
	unsigned int newnbp(dnewnbp),
				 oldnbp((*noeuds[indice]).getNbp());
	Cercle tempcercle {icentre, sqrt(dnewnbp)};
	size_t iuid((*noeuds[indice]).getUid());
	if (tests_noeud(tempcercle, iuid)) {
		(*noeuds[indice]).setCercle(tempcercle);
		(*noeuds[indice]).setNbp(newnbp);
		(*noeuds[indice]).decrementer_nbp_quartier(oldnbp);
		(*noeuds[indice]).incrementer_nbp_quartier(newnbp);
		return true;
	}
	return false;
}

bool Ville::tests_noeud(Cercle cercle, size_t iuid) {
	for (size_t i(0); i < noeuds.size(); i++) {
		Cercle cercle1((*noeuds[i]).getCercle());
		if ((*noeuds[i]).getUid() != iuid) {
			if (collision_cercles(cercle, cercle1)) {
				errormsg(error::node_node_superposition(iuid, (*noeuds[i]).getUid()));
				return false;
			}
		}
	}
	for (size_t i(0); i < liens.size(); i++) {
		S2d centre1((*noeuds[find_uid(liens[i][0])]).getCercle().centre),
			centre2((*noeuds[find_uid(liens[i][1])]).getCercle().centre);
		if (liens[i][0] != iuid && liens[i][1] != iuid) {
			if (collision_segment_cercle_entre(cercle, centre1, centre2)) {
				errormsg(error::node_link_superposition(iuid));
				return false;
			}
		} else {
			if (liens[i][0] == iuid) centre1 = cercle.centre;
			else if (liens[i][1] == iuid) centre2 = cercle.centre;
			for (size_t j(0); j < noeuds.size(); j++) {
				if ((*noeuds[j]).getUid() != iuid) {
					if (collision_segment_cercle_entre((*noeuds[j]).getCercle(),
													   centre1, centre2)) {
						errormsg(error::node_link_superposition((*noeuds[j]).
																getUid()));
						return false;
					}
				}
			}
		}
	}
	return true;
}

void Ville::toggle_lien(size_t indice1, size_t indice2) {
	bool lien_existe(false);
	vector<Noeud*> vs((*noeuds[indice1]).getVoisins());
	for (size_t i(0); i < vs.size(); ++i) {
		if ((*vs[i]).getUid() == (*noeuds[indice2]).getUid()) {
			(*noeuds[indice1]).supprimer_voisins(i);
			(*noeuds[indice1]).decrementer_nbliens();
			lien_existe = true;
			break;
		}
	}
	vs = ((*noeuds[indice2]).getVoisins());
	if (lien_existe) {
		for (size_t i(0); i < vs.size(); ++i) {
			if ((*vs[i]).getUid() == (*noeuds[indice1]).getUid()) {
				(*noeuds[indice2]).supprimer_voisins(i);
				(*noeuds[indice2]).decrementer_nbliens();
				break;
			}
		}
		for (size_t i(0); i < liens.size(); i++) {
			if ((liens[i][0] == (*noeuds[indice1]).getUid() &&
				liens[i][1] == (*noeuds[indice2]).getUid()) ||
				(liens[i][1] == (*noeuds[indice1]).getUid() &&
				liens[i][0] == (*noeuds[indice2]).getUid())) {
				liens[i] = liens.back();
				liens.pop_back();
				Noeud::decrementer_total_liens();
				break;
			}
		}
	} else {
		size_t depart((*noeuds[indice1]).getUid()),
			   arrivee((*noeuds[indice2]).getUid());
		if (!test_creation_lien(depart, arrivee, liens, noeuds)) {
			std::array<size_t, LIEN> nouveau_lien = {depart, arrivee};
			liens.push_back(nouveau_lien);
			Noeud::incrementer_total_liens();
		}
	}																		
}