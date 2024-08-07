#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "main.h"
#include "camera.h"
#include "damage.h"
#include "serial.h"
#include "vblank.h"
#include "pixelreader.h"
#include "xshm_reader.h"
#include "gpio.h"
#include "parport.h"
#include "inputhandler.h"
#include "evdevhandler.h"

class MeasurementController {
    private:
        CameraHandler cameraHandler;
        DamageHandler damageHandler;
        SerialHandler serialHandler;
        VblankHandler vblankHandler;
        PixelReader pixelReader;
        GPIOHandler gpioHandler;
        InputHandler inputHandler;

        int iteration;
        int ITERATIONS;
	    uint64_t click_time;
        bool measuring;
            
        void calibrate();
        void trigger_evdev();
        void signalHandlerTerm(int sig);
        void signalHandlerInt(int sig);

    public:
        MeasurementController(char *event_handle, int damage_win, int iterations);
        void run();
        void cleanup();
};

#endif
