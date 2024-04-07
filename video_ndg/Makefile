LIBS = `pkg-config --cflags --libs opencv4`
CPP = g++   
CFLAGS = 'pkg-config --cflags gtkmm-3.0'

compile : 
	$(CPP) -o main src/mosaique_video_ndg.cpp $(LIBS)

run : 
	./main ./data/short_video_gray.mp4 ./data/mosaique_gray.mp4 ./bash/liste_moyenne.txt 5 192 ./imagetteRedim

clean :
	rm -f main





