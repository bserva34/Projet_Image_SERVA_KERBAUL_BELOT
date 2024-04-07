LIBS = `pkg-config --cflags --libs opencv4`
CPP = g++   
CFLAGS = 'pkg-config --cflags gtkmm-3.0'

compile : 
	$(CPP) -o main src/mosaique_video_color.cpp $(LIBS)

run : 
	./main ./data/short_video_color.mp4 ./data/mosaique_color.mp4 ./bash/listeMoyenne.txt 10 16 ./imagetteRedim

clean :
	rm -f main





