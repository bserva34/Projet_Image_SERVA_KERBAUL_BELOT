LIBS = `pkg-config --cflags --libs gtkmm-3.0`
CPP = g++   
CFLAGS = 'pkg-config --cflags gtkmm-3.0'

compile : 
	$(CPP) -o main src/main.cpp $(LIBS)

run : 
	./main

compileMoyenne :
	$(CPP) src/moyenne_imagette.cpp -o moyenne $(LIBS)

compileMoyenneColor :
	$(CPP) src/moyenne_imagetteColor.cpp -o moyenneColor $(LIBS)

compileCarteMoyenne :
	$(CPP) src/carte_moyenne.cpp -o carteMoyenne $(LIBS)

compileCarteMoyenneColor :
	$(CPP) src/carte_moyenneColor.cpp -o carteMoyenneColor $(LIBS)

cleanCompiled :
	rm -f main moyenne moyenneColor carteMoyenne carteMoyenneColor

cleanImagettes :
	rm -f bash/*.txt imagettesRedim/*.pgm

cleanAll :
	make cleanCompiled
	make cleanImagettes





