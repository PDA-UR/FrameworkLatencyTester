sdl2:
	$(MAKE) -C SDL2-software/ all
	$(MAKE) -C SDL2-opengl/ all
	$(MAKE) -C SDL2-opengles2/ all

glut:
	$(MAKE) -C GLUT/ all

glew:
	$(MAKE) -C GLEW/ all

gtk:
	$(MAKE) -C gtk/ all

swing:
	$(MAKE) -C JavaSwing/ all

java2d:
	$(MAKE) -C Java2D/ all

qt5:
	$(MAKE) -C Qt5/ all

Java2D:
	$(MAKE) -C Java2D/ all

FLTK:
	$(MAKE) -C FLTK/ all

xlib:
	$(MAKE) -C xlib/ all

xcb:
	$(MAKE) -C xcb/ all

sfml:
	$(MAKE) -C SFML/ all

raylib:
	$(MAKE) -C raylib/ all

JavaFX:
	$(MAKE) -C JavaFX/ all

all: glew glut sdl2 gtk swing qt5 java2d FLTK xlib xcb sfml raylib JavaFX

clean:
	$(MAKE) -C SDL2-software/ clean
	$(MAKE) -C SDL2-opengl/ clean
	$(MAKE) -C SDL2-opengles2/ clean
	$(MAKE) -C GLUT/ clean
	$(MAKE) -C GLEW/ clean
	$(MAKE) -C gtk/ clean
	$(MAKE) -C JavaSwing/ clean
	$(MAKE) -C Qt5/ clean
	$(MAKE) -C Java2D/ clean
	$(MAKE) -C FLTK/ clean
	$(MAKE) -C xlib/ clean
	$(MAKE) -C xcb/ clean
	$(MAKE) -C SFML/ clean
	$(MAKE) -C raylib/ clean
	$(MAKE) -C JavaFX/ clean
