LIBS = `pkg-config --cflags --libs gtkmm-3.0`
CPP = g++   
CFLAGS = 'pkg-config --cflags gtkmm-3.0'

compile : 
	$(CPP) -o main src/main.cpp $(LIBS)

run : 
	./main

clean :
	rm main



