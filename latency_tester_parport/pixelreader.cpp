#include "main.h"
#include "pixelreader.h"

#include <unistd.h>
#include <stdio.h>
#include <iostream>

using namespace std;

PixelReader::PixelReader(int x, int y)
{
    X = x;
    Y = y;
}

// wait until our pixel has a specified color
void PixelReader::wait_for_color(unsigned int color)
{
	unsigned int pixelColor;
	uint64_t start, end;
    //cout << "wait for color" << endl;

	while(1)
	{
		start = get_micros();
		pixelColor = getPixelColor();
		end = get_micros();

		if (pixelColor != 0)
		{
			read_start_time = start;
            		read_end_time = end;
			//cout << "pixelcolor " << pixelColor << endl;
			return;
		}

		usleep(1);
	}
	return;
}

void PixelReader::measure_fw_latency()
{
    //cout << "measure_fw_latency" << endl;

    //struct input_event inputEvent;
    //int err = -1;
    running = true;
    measure = true;

    while(running)
    {
    //cout << "running" << endl;
        if(measure)
        {
    //cout << "running and measure" << endl;
            measure = false;
		//cout << "evdev" << endl;
            start_time = get_micros();

            wait_for_color(COLOR_WHITE); // wait for test program to react

		//cout << "white" << endl;
            end_time = get_micros();
	    running = false;
        }
    }
}

// never called?
void PixelReader::trigger_measurement()
{
    //cout << "trigger measurement" << endl;
    measure = true;
}

void PixelReader::reset()
{
    start_time = 0;
    end_time = 0;
}

void PixelReader::cleanup()
{
    //closeXShm();
    running = false;
    //PixelReader::cleanup();
}

unsigned int PixelReader::getPixelColor()
{
    
}
