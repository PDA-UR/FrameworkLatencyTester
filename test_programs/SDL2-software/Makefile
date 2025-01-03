all:
	mkdir bin
	g++ simple.cpp -lSDL2main -lSDL2 -DDRIVER='"software"' -o bin/SDL2-software_default
	g++ rects.cpp -lSDL2main -lSDL2 -DDRIVER='"software"' -o bin/SDL2-software_rects
	g++ image.cpp -lSDL2main -lSDL2 -lSDL2_image -DDRIVER='"software"' -o bin/SDL2-software_image
	g++ custom.cpp -lSDL2main -lSDL2 -lSDL2_image -DDRIVER='"software"' -o bin/SDL2-software_custom

clean:
	rm -rf bin
