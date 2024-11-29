#include "controller.h"
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

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <functional>

MeasurementController::MeasurementController(char *event_handle, int damage_win, int iterations)
{
    //signal(SIGINT, signalHandlerInt);
    //signal(SIGTERM, signalHandlerTerm);

    ITERATIONS = iterations;

    inputHandler = new EvdevHandler(event_handle);

    //inputHandler.register_callback(&MeasurementController::trigger_evdev);

    // init parallel port
    
    // serial port
    serialHandler = new SerialHandler("/dev/ttyUSB0");

    // todo this should be an exception
    if (!serialHandler->initialized)
    {
	    cleanup();
	    exit(SIGABRT);
    }

    calibrate();

    measuring = true;

    // fw tester
    // todo don't hard code position
    pixelReader = new XShmReader(200, 200);
    inputHandler->register_callback(bind(&PixelReader::measure_fw_latency, pixelReader));
    //inputHandler.register_callback(&(pixelReader.measure_fw_latency));

    // vblank
    vblankHandler = new VblankHandler();

    // xdamage
    damageHandler = new DamageHandler(damage_win);

    // run parallel port
    gpioHandler = new ParportHandler();

    // camera handler
    cameraHandler = new CameraHandler();
}

void MeasurementController::calibrate()
{
    usleep(2 * 1000 * 1000);
    serialHandler->writeMessage((char*) msg_toggle, 1); 
    usleep(500 * 1000);
    serialHandler->writeMessage((char*) msg_toggle, 1);
    usleep(500 * 1000);

    serialHandler->writeMessage((char*) msg_calibrate, 1);
    //memset(&serial_read_buffer, '\0', sizeof(serial_read_buffer));
    usleep(2 * 1000 * 1000);
    serialHandler->readString();

    //cout << "read " << serial_read_num_bytes << " bytes from buffer" << endl;
    //cout << "calib: " << serial_read_buffer << endl;

    //usleep(10000);
    //while(read(serial_port, &serial_read_buffer, sizeof(serial_read_buffer)))
    //{
    //        usleep(1000);
    //}
    //while(read(serial_port, &serial_read_buffer, sizeof(serial_read_buffer)) > 0)
    //{
    //        cout << "still reading from port..." << serial_read_buffer << endl;
    //        usleep(100000);
    //}

    usleep(1000000);
    //ioctl(serial_port, TCFLSH, 2);
    //usleep(10000);
    serialHandler->flush();
    usleep(10000);

    //while(read(serial_port, &serial_read_buffer, sizeof(serial_read_buffer)) > 0)
    //{
    //        cout << "still reading from port..." << serial_read_buffer << endl;
    //        usleep(100000);
    //}
}

void MeasurementController::run()
{
    iteration = 0;

    usleep(100 * 1000);

    cout << "iteration,click_time,input_time,end_time,bright_time,bright_time_2,read_start_time,read_end_time,yalmd_latency,tearing_offset,vblanks,damage" << endl;

    while(measuring)
    {
        inputHandler->reset();
        pixelReader->reset();
        gpioHandler->reset();
	    //click_time = 0;
	    //pixelReader.start_time = 0;
	    //pixelReader.end_time = 0;
	    //bright_time = 0;
	    //bright_time_2 = 0;
	    //state_click = 0;
	    //state_bright = 0;
	    //state_bright_2 = 0;

	    vblankHandler->measure = true;
	    damageHandler->measure = true;
	    
	    usleep(20000);

	    gpioHandler->measure = true;
	    serialHandler->writeMessage((char*) msg_measure, 1);

	    //cout << "measure" << endl;

	    while (inputHandler->input_time == 0 || gpioHandler->click_time == 0 || pixelReader->end_time == 0 || gpioHandler->bright_time == 0 || gpioHandler->bright_time_2 == 0)
	    {
            usleep(10);
            // click
            // bright
            // bright_2
            //cout << inputHandler->input_time << " - " << gpioHandler->click_time << " - " << pixelReader->end_time << " - " << gpioHandler->bright_time << " - " << gpioHandler->bright_time_2 << endl;
	    //usleep(100000);
	    }

	    //cout << "after big while" << endl;

	    //parport_active = 0;
	    gpioHandler->measure = false;
	    usleep(20000);

	    //measure_vblank = 0;
	    //measure_xdamage = 0;
	    vblankHandler->measure = false;
	    damageHandler->measure = false;

	    usleep(300000);

	    int input_latency = inputHandler->input_time - gpioHandler->click_time;
	    int framework_latency = pixelReader->end_time - pixelReader->start_time;
	    int display_latency = gpioHandler->bright_time - pixelReader->end_time;
	    int ete_latency = gpioHandler->bright_time - gpioHandler->click_time;
	    int sum_latency = input_latency + framework_latency + display_latency;
	    int yalmd_latency = serialHandler->readInt();

        double tearing_offset = 0.;
	//cout << bright_time - bright_time_2 << endl;
        if (gpioHandler->bright_time_2 < gpioHandler->bright_time)
        {
            tearing_offset = cameraHandler->runTearingDetection();
        }

	    //cout << "return from yalmd " << serial_read_buffer << endl;
	    //cout << "click to bright1: " << (int)(bright_time - click_time) << endl;
	    //cout << "click to bright2: " << (int)(bright_time_2 - click_time) << endl;
	    //cout << "bright1 to bright2: " << (int)(bright_time_2 - bright_time) << endl;
	    //cout << "yalmd:" << yalmd_latency << endl;

	    cout << iteration << ","
		 << gpioHandler->click_time << ","
		 << inputHandler->input_time << ","
		 << pixelReader->end_time << ","
		 << gpioHandler->bright_time << ","
		 << gpioHandler->bright_time_2 << ","
		 << pixelReader->read_start_time << ","
		 << pixelReader->read_end_time << ","
		 << yalmd_latency << ","
         << tearing_offset << ",";

		for (int i = 0; i < vblankHandler->vsync_count; i++)
		{
			cout << vblankHandler->vsync_time[i] << ";";
			vblankHandler->vsync_time[i] = 0;
		}

		cout << ",";

		if (damageHandler->use_xdamage)
		{
			for (int i = 0; i < damageHandler->damage_count; i++)
			{
				cout << damageHandler->damage_time[i] << ";";
				damageHandler->damage_time[i] = 0;
			}
		}

		cout << endl;

		vblankHandler->vsync_count = 0;
		damageHandler->damage_count = 0;

	    iteration++;

	    if (iteration > ITERATIONS) break;

	    usleep(200 * 1000);
    }

    cleanup();
}

void MeasurementController::trigger_evdev()
{
    //start_time = get_micros();
}

// make sure we clean up and print current logs when the program is killed
// log is only printed when terminated, not when interrupted
void MeasurementController::signalHandlerInt(int sig)
{
    cleanup();
    exit(sig);
}

void MeasurementController::signalHandlerTerm(int sig)
{
    cleanup();
    exit(sig);
}

void MeasurementController::cleanup()
{
    measuring = false;
    //cout << "cleanup serial" << endl;
    serialHandler->cleanup();
    //cout << "cleanup vblank" << endl;
    vblankHandler->cleanup();
    //cout << "cleanup damage" << endl;
    damageHandler->cleanup();
    //cout << "cleanup gpio" << endl;
    gpioHandler->cleanup();
    //cout << "cleanup input" << endl;
    inputHandler->cleanup();
    //cout << "cleanup pixelReader" << endl;
    pixelReader->cleanup();
}
