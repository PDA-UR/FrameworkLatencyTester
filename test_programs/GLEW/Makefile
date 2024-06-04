all:
	mkdir bin
	g++ simple.cpp -lGL -lGLEW -lSDL2 -o bin/GLEW_default
	g++ rects.cpp -lGL -lGLEW -lSDL2 -o bin/GLEW_rects
	g++ image.cpp -lGL -lGLEW -lSDL2 -lSDL2_image -o bin/GLEW_image

clean:
	rm -rf bin
