all:
	mkdir bin
	g++ simple.cpp -lSDL2main -lSDL2 -DDRIVER='"opengles2"' -o bin/SDL2-opengles2_default
	g++ rects.cpp -lSDL2main -lSDL2 -DDRIVER='"opengles2"' -o bin/SDL2-opengles2_rects
	g++ image.cpp -lSDL2main -lSDL2 -lSDL2_image -DDRIVER='"opengles2"' -o bin/SDL2-opengles2_image

clean:
	rm -rf bin
