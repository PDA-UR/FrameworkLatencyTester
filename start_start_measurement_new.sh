#!/bin/sh

ITERATIONS=100

xrandr --output DP-0 --mode 1920x1080 --refresh 240

sleep 5s

DATA_DIR="2024-04-23_jetson_g300_strix_240_comp"

#compton -b "glx" --vsync="opengl" --daemon=true

xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set true

#qdbus org.kde.KWin /Compositor suspend

sleep 5s

./start_measurement_new.sh xlib default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 rects $ITERATIONS $DATA_DIR

./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_opengl $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_vsync $ITERATIONS $DATA_DIR

./start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet rects $ITERATIONS $DATA_DIR

./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengl $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengles2 $ITERATIONS $DATA_DIR

#./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLEW rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT rects $ITERATIONS $DATA_DIR

DATA_DIR="2024-04-23_jetson_g300_strix_240_nocomp"

xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set false

sleep 5s

./start_measurement_new.sh xlib default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 rects $ITERATIONS $DATA_DIR

./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_opengl $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_vsync $ITERATIONS $DATA_DIR

./start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet rects $ITERATIONS $DATA_DIR

./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengl $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengles2 $ITERATIONS $DATA_DIR


xrandr --output DP-0 --mode 1920x1080 --refresh 120

sleep 5s

DATA_DIR="2024-04-23_jetson_g300_strix_120_comp"

#compton -b "glx" --vsync="opengl" --daemon=true

xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set true

#qdbus org.kde.KWin /Compositor suspend

sleep 5s

./start_measurement_new.sh xlib default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 rects $ITERATIONS $DATA_DIR

./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_opengl $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_vsync $ITERATIONS $DATA_DIR

./start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet rects $ITERATIONS $DATA_DIR

./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengl $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengles2 $ITERATIONS $DATA_DIR

#./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLEW rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT rects $ITERATIONS $DATA_DIR

DATA_DIR="2024-04-23_jetson_g300_strix_120_nocomp"

xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set false

sleep 5s

./start_measurement_new.sh xlib default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 rects $ITERATIONS $DATA_DIR

./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_opengl $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_vsync $ITERATIONS $DATA_DIR

./start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet rects $ITERATIONS $DATA_DIR

./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengl $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengles2 $ITERATIONS $DATA_DIR


xrandr --output DP-0 --mode 1920x1080 --refresh 60

sleep 5s

DATA_DIR="2024-04-23_jetson_g300_strix_60_comp"

#compton -b "glx" --vsync="opengl" --daemon=true

xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set true

#qdbus org.kde.KWin /Compositor suspend

sleep 5s

./start_measurement_new.sh xlib default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 rects $ITERATIONS $DATA_DIR

./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_opengl $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_vsync $ITERATIONS $DATA_DIR

./start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet rects $ITERATIONS $DATA_DIR

./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengl $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengles2 $ITERATIONS $DATA_DIR

#./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLEW rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT rects $ITERATIONS $DATA_DIR

DATA_DIR="2024-04-23_jetson_g300_strix_60_nocomp"

xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set false

sleep 5s

./start_measurement_new.sh xlib default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 rects $ITERATIONS $DATA_DIR

./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_opengl $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default_vsync $ITERATIONS $DATA_DIR

./start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet rects $ITERATIONS $DATA_DIR

./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengl $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengles2 $ITERATIONS $DATA_DIR



exit 0

DATA_DIR="2024-04-04_jetson_g300_u2417h_60_nocomp"

xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set false

sleep 5s
./start_measurement_new.sh xlib default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 rects $ITERATIONS $DATA_DIR

#./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pygame default_doublebuf $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pygame default_opengl $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pygame default_vsync $ITERATIONS $DATA_DIR
#
##./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
##./start_measurement_new.sh OPENGL_GLEW rects $ITERATIONS $DATA_DIR
##./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
##./start_measurement_new.sh OPENGL_GLUT rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
##./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh SDL2 opengl $ITERATIONS $DATA_DIR
#./start_measurement_new.sh SDL2 opengles2 $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT default_doublebuf $ITERATIONS $DATA_DIR


exit 0

xrandr --output DP-0 --mode 1920x1080 --refresh 120

sleep 5s

DATA_DIR="2024-03-17_jetson_viper_alien_120_comp"

#compton -b "glx" --vsync="opengl" --daemon=true

xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set true

#qdbus org.kde.KWin /Compositor suspend

sleep 5s

#./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLEW rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengl $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengles2 $ITERATIONS $DATA_DIR

DATA_DIR="2024-03-17_jetson_viper_alien_120_nocomp"

xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set false

sleep 5s

#./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLEW rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh SDL2 opengl $ITERATIONS $DATA_DIR
#./start_measurement_new.sh SDL2 opengles2 $ITERATIONS $DATA_DIR



xrandr --output DP-0 --mode 1920x1080 --refresh 60

sleep 5s

DATA_DIR="2024-03-17_jetson_viper_alien_60_comp"

#compton -b "glx" --vsync="opengl" --daemon=true

xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set true

#qdbus org.kde.KWin /Compositor suspend

sleep 5s

#./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLEW rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengl $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengles2 $ITERATIONS $DATA_DIR

DATA_DIR="2024-03-17_jetson_viper_alien_60_nocomp"

xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set false

sleep 5s

#./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLEW rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh SDL2 opengl $ITERATIONS $DATA_DIR
#./start_measurement_new.sh SDL2 opengles2 $ITERATIONS $DATA_DIR








exit 0

#./start_measurement_new.sh xlib default $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengl $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengles2 $ITERATIONS $DATA_DIR
#./start_measurement_new.sh SDL2 software $ITERATIONS $DATA_DIR

#./start_measurement_new.sh xlib rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLEW rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh SDL2 rects_opengl $ITERATIONS $DATA_DIR
#./start_measurement_new.sh SDL2 rects_opengles2 $ITERATIONS $DATA_DIR
#./start_measurement_new.sh SDL2 rects_software $ITERATIONS $DATA_DIR


sleep 5s

DATA_DIR="2024-03-13_jetson_viper_dell_60_nocomp"

#compton -b "glx" --vsync="opengl" --daemon=true

xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set false

#qdbus org.kde.KWin /Compositor suspend

sleep 5s

#./start_measurement_new.sh xlib default $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengl $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 opengles2 $ITERATIONS $DATA_DIR
#./start_measurement_new.sh SDL2 software $ITERATIONS $DATA_DIR

#./start_measurement_new.sh xlib rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyglet rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLEW rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh SDL2 rects_opengl $ITERATIONS $DATA_DIR
#./start_measurement_new.sh SDL2 rects_opengles2 $ITERATIONS $DATA_DIR
#./start_measurement_new.sh SDL2 rects_software $ITERATIONS $DATA_DIR


