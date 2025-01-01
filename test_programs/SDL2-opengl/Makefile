all:
	mkdir bin
	g++ simple.cpp -lSDL2main -lSDL2 -DDRIVER='"opengl"' -o bin/SDL2-opengl_default
	g++ rects.cpp -lSDL2main -lSDL2 -DDRIVER='"opengl"' -o bin/SDL2-opengl_rects
	g++ image.cpp -lSDL2main -lSDL2 -lSDL2_image -DDRIVER='"opengl"' -o bin/SDL2-opengl_image
	g++ custom.cpp -lSDL2main -lSDL2 -lSDL2_image -DDRIVER='"opengl"' -o bin/SDL2-opengl_custom

clean:
	rm -rf bin
