all:
	mkdir bin
	g++ simple.cpp -lGL -lGLU -lglut -o bin/GLUT_default
	g++ simple_doublebuf.cpp -lGL -lGLU -lglut -o bin/GLUT_default_doublebuf
	g++ rects.cpp -lGL -lGLU -lglut -o bin/GLUT_rects
	g++ image.cpp -lGL -lGLU -lglut -lSOIL -o bin/GLUT_image
	

clean:
	rm -rf bin
