#/bin/bash

ITERATIONS=20
COMPUTER="geva"
DISPLAYNAME="strix"
MOUSE="g300"
#REFRESHRATES=(60 120 240) # 60
REFRESHRATES=(60) # 60
CONNECTION="DP-4"
DISPLAYRESOLUTION="1920x1080"
#COMPOSITORSTATES=("comp" "nocomp")
COMPOSITORSTATES=("nocomp")
FRAMEWORKS=("pygame")
#FRAMEWORKS=("FLTK" "GLEW" "GLUT" "gtk" "Java2D" "JavaSwing" "pygame" "pyglet" "pyqt5" "pyqt6" "Qt5" "SDL2-opengl" "SDL2-opengles2" "SDL2-software" "tkinter" "wxpython" "xcb" "xlib")
#FRAMEWORKS=("xlib")
#FRAMEWORKS=("SDL2-software" "tkinter" "wxpython" "xcb" "xlib")
#FWPARAMS=("default" "rects")
FWPARAMS=("default")
DATE="2024-07-25"

sigint() {
    exit 0
}

trap sigint INT

activate_compositor () {
	xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set true
}

deactivate_compositor () {
	xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set false
}

sudo rmmod lp

for refreshrate in ${REFRESHRATES[@]}; do
	echo $CONNECTION
	echo "xrandr --output ${CONNECTION} --mode $DISPLAYRESOLUTION --refresh $refreshrate"
	xrandr --output ${CONNECTION} --mode $DISPLAYRESOLUTION --refresh $refreshrate
	sleep 5s

	for comp in ${COMPOSITORSTATES[@]}; do
		if [ $comp == "comp" ]
		then
			activate_compositor
		else
			deactivate_compositor
		fi
		DATA_DIR="${DATE}_${COMPUTER}_${MOUSE}_${DISPLAYNAME}_${refreshrate}_${comp}"
		echo $DATA_DIR
		for fw in ${FRAMEWORKS[@]}; do
			for param in ${FWPARAMS[@]}; do
				echo "$fw $param"
				./start_measurement_new.sh $fw $param $ITERATIONS $DATA_DIR
			done
		done

	done
done

exit 0


##########

xrandr --output HDMI-2 --mode 1920x1080 --refresh 240

sleep 5s

DATA_DIR="2024-05-31_pi5_g300_strix_240_comp"

#compton -b "glx" --vsync="opengl" --daemon=true

xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set true

#qdbus org.kde.KWin /Compositor suspend

sleep 5s

FW="SDL2"
PAR="opengles2"

#./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
./start_measurement_new.sh $FW $PAR $ITERATIONS $DATA_DIR

sleep 5s

DATA_DIR="2024-05-31_pi5_g300_strix_240_nocomp"
xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set false

./start_measurement_new.sh $FW $PAR $ITERATIONS $DATA_DIR

exit 0

#./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh FLTK rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh gtk rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLEW rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh OPENGL_GLUT default_doublebuf $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pygame rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyglet rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyqt5 default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyqt5 rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyqt6 default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyqt6 rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh Qt5 default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh Qt5 rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh SDL2 software $ITERATIONS $DATA_DIR
#./start_measurement_new.sh SDL2 rects_software $ITERATIONS $DATA_DIR
#./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh wxpython default $ITERATIONS $DATA_DIR
./start_measurement_new.sh wxpython rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib rects $ITERATIONS $DATA_DIR


DATA_DIR="2024-05-28_pi5_g300_strix_240_nocomp"

xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set false

sleep 5s

./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyglet rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt6 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt6 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 software $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 rects_software $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh wxpython default $ITERATIONS $DATA_DIR
./start_measurement_new.sh wxpython rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib rects $ITERATIONS $DATA_DIR


xrandr --output HDMI-2 --mode 1920x1080 --refresh 120

sleep 5s

DATA_DIR="2024-05-28_pi5_g300_strix_120_comp"

#compton -b "glx" --vsync="opengl" --daemon=true

xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set true

#qdbus org.kde.KWin /Compositor suspend

sleep 5s

./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyglet rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt6 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt6 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 software $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 rects_software $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh wxpython default $ITERATIONS $DATA_DIR
./start_measurement_new.sh wxpython rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib rects $ITERATIONS $DATA_DIR


DATA_DIR="2024-05-28_pi5_g300_strix_120_nocomp"

xfconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set false

sleep 5s

./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyglet rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt6 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt6 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 software $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 rects_software $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh wxpython default $ITERATIONS $DATA_DIR
./start_measurement_new.sh wxpython rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib rects $ITERATIONS $DATA_DIR



randr --output HDMI-2 --mode 1920x1080 --refresh 60

leep 5s

ATA_DIR="2024-05-28_pi5_g300_strix_60_comp"

compton -b "glx" --vsync="opengl" --daemon=true

fconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set true

qdbus org.kde.KWin /Compositor suspend

leep 5s

./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyglet rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt6 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt6 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 software $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 rects_software $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
/start_measurement_new.sh tkinter rects $ITERATIONS $DATA_DIR
/start_measurement_new.sh wxpython default $ITERATIONS $DATA_DIR
/start_measurement_new.sh wxpython rects $ITERATIONS $DATA_DIR
/start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
/start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
/start_measurement_new.sh xlib default $ITERATIONS $DATA_DIR
/start_measurement_new.sh xlib rects $ITERATIONS $DATA_DIR


ATA_DIR="2024-05-28_pi5_g300_strix_60_nocomp"

fconf-query --channel=xfwm4 --property=/general/use_compositing --type=bool --set false

leep 5s

./start_measurement_new.sh FLTK default $ITERATIONS $DATA_DIR
./start_measurement_new.sh FLTK rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk default $ITERATIONS $DATA_DIR
./start_measurement_new.sh gtk rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Java2D rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing default $ITERATIONS $DATA_DIR
./start_measurement_new.sh JavaSwing rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLEW rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh OPENGL_GLUT default_doublebuf $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pygame rects $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyglet default $ITERATIONS $DATA_DIR
#./start_measurement_new.sh pyglet rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt6 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh pyqt6 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 default $ITERATIONS $DATA_DIR
./start_measurement_new.sh Qt5 rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 software $ITERATIONS $DATA_DIR
./start_measurement_new.sh SDL2 rects_software $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter default $ITERATIONS $DATA_DIR
./start_measurement_new.sh tkinter rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh wxpython default $ITERATIONS $DATA_DIR
./start_measurement_new.sh wxpython rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xcb rects $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib default $ITERATIONS $DATA_DIR
./start_measurement_new.sh xlib rects $ITERATIONS $DATA_DIR
