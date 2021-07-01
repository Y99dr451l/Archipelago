#include <iostream>
#include "ville.h"
#include "gui.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc > 2) {
		cout << "Saisie incorrecte: execution sans argument "
				"ou avec l'adresse d'un seul fichier";
		exit(1);
	}
	if(argc == 2) {
		string s(argv[1]);
		init(s);
	}
	init_gui();
	return 0;
}