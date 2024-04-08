LIBS = `pkg-config --cflags --libs opencv4`
CPP = g++   
CFLAGS = 'pkg-config --cflags gtkmm-3.0'

compile : 
	$(CPP) -o main src/mosaique_video_color.cpp $(LIBS)

run : 
	./main ./data/bsd.mp4 ./data/bsd_mosa.mp4 ./bash/listeResultatCouleur.txt 10 41129 ../../Master_Imagine_Local/collection_imagette/couleur_10

clean :
	rm -f main





