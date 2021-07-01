CXX     = g++
CXXFLAGS = -g -Wall -std=c++11
CXXFILES = projet.cpp ville.cpp noeud.cpp tools.cpp error.cpp
OFILES = $(CXXFILES:.cpp=.o)

projet: $(OFILES)
	$(CXX) $(OFILES) -o projet

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cpp~ *.h~ prog

# DO NOT DELETE THIS LINE
projet.o: projet.cpp ville.h noeud.h tools.h constantes.h error.h
ville.o: ville.cpp ville.h noeud.h tools.h constantes.h error.h
noeud.o: noeud.cpp noeud.h tools.h constantes.h error.h
tools.o: tools.cpp tools.h constantes.h
error.o: error.cpp error.h constantes.h
