LIBS = `pkg-config --cflags --libs gtkmm-3.0 opencv4`
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

compileVideoCouleur : 
	$(CPP) src/mosaique_video_color.cpp -o videoCouleur $(LIBS)

compileVideoNDG : 
	$(CPP) src/mosaique_video_ndg.cpp -o videoNdg $(LIBS)

compileInterpolationNDG : 
	$(CPP) src/mosaique_video_ndg_interpolation.cpp -o videoNdgInterpolated $(LIBS)

compileInterpolationColor : 
	$(CPP) src/mosaique_video_color_interpolation.cpp -o videoColorInterpolated $(LIBS)

cleanCompiled :
	rm -f main videoNdg videoNdgInterpolated videoColorInterpolated videoCouleur moyenne moyenneColor carteMoyenne carteMoyenneColor

cleanImagettes :
	rm -f bash/*.txt iRedim/*.pgm iRedimColor/*.ppm

cleanAll :
	make cleanCompiled
	make cleanImagettes





