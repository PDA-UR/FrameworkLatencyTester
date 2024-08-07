#include "main.h"
#include "pixelreader.h"

#include <unistd.h>

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

	while(1)
	{
		start = get_micros();
		pixelColor = getPixelColor();
		end = get_micros();

		if (pixelColor != 0)
		{
			read_start_time = start;
            read_end_time = end;
			return;
		}

		usleep(1);
	}
	return;
}

void PixelReader::measure_fw_latency()
{
    //struct input_event inputEvent;
    //int err = -1;
    running = true;

    while(running)
    {
        if(measure)
        {
            measure = false;
		//cout << "evdev" << endl;
            start_time = get_micros();

            wait_for_color(COLOR_WHITE); // wait for test program to react

		//cout << "white" << endl;
            end_time = get_micros();
        }
    }
}

void PixelReader::trigger_measurement()
{
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
    PixelReader::cleanup();
}
