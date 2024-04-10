LIBS = -lm
CPP = g++   

compileMoyenne :
	$(CPP) src/moyenne_imagette.cpp -o main $(LIBS)

moyenneImagette :
	./main bash/listeNom.txt bash/listeResultat.txt

compileCarteMoyenne :
	$(CPP) src/carte_moyenne.cpp -o main $(LIBS)

carteMoyenne :
	./main dataImg/leopard.pgm dataImg/img_bloc.pgm dataImg/img_mosaique.pgm bash/listeResultat.txt

clean :
	rm main