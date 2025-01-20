all:
	mkdir bin
	gcc -Wall simple.c -o bin/raylib_default -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	gcc -Wall rects.c -o bin/raylib_rects -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	gcc -Wall custom.c -o bin/raylib_custom -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	gcc -Wall image.c -o bin/raylib_image -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm -rf bin
