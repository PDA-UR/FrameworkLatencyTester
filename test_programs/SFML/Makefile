all:
	mkdir bin
	g++ -Wall simple.cpp -lsfml-graphics -lsfml-window -lsfml-system -o bin/SFML_default
	g++ -Wall rects.cpp -lsfml-graphics -lsfml-window -lsfml-system -o bin/SFML_rects
	g++ -Wall custom.cpp -lsfml-graphics -lsfml-window -lsfml-system -o bin/SFML_custom
	g++ -Wall image.cpp -lsfml-graphics -lsfml-window -lsfml-system -o bin/SFML_image

clean:
	rm -rf bin
