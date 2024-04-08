LIBS = `pkg-config --cflags --libs opencv4`
CPP = g++   
CFLAGS = 'pkg-config --cflags gtkmm-3.0'

compile : 
	$(CPP) -o main src/mosaique_video_color.cpp $(LIBS)

run : 
	./main ./data/bsd.mp4 ./data/bsd_mosa.mp4 ./bash/listeMoyenne.txt 8 1331 ./imagetteRedim

clean :
	rm -f main





