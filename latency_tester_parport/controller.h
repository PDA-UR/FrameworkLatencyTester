#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "main.h"
#include "parport.h"
#include "camera.h"
#include "serial.h"
#include "read_pixel.h"
#include "vblank.h"
#include "damage.h"
#include "gpio.h"

class MeasurementController {
    pivate:
        CameraHandler cameraHandler;
        DamageHandler damageHandler;
        GPIOHandler gpioHandler;
        PixelReader pixelReader;
        SerialHandler serialHandler;
        VblankHandler vblankHandler;

        int iteration;
        int ITERATIONS;
	    uint64_t click_time;
	    //uint64_t start_time;
	    //uint64_t end_time;
	    //uint64_t bright_time;
	    //uint64_t bright_time_2;
	    //uint64_t state_click;
	    //uint64_t state_bright;
	    //uint64_t state_bright_2;
            
        void trigger_evdev()

    public:
        MeasurementController(char *event_handle, int damage_win, int iterations);
        void run();
        void cleanup();
}

#endif
