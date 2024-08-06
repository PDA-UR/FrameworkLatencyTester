#include <linux/ppdev.h>
#include <linux/parport.h>

#include "main.h"
#include "parport.h"

using namespace std;

Parport::Parport()
{
    click_pin = 5;
    bright_pin = 4;
    bright_pin_2 = 3;

	fd = open("/dev/parport0", O_RDWR);
	//cout << "parport0 fd: " << fd << endl;

	ioctl(fd, PPEXCL);
	//cout << "excl" << endl;
	ioctl(fd, PPCLAIM);
	//cout << "claimed" << endl;

    // todo: use exception handling
	//int mode = IEEE1284_MODE_EPP; //ECP
	int mode = IEEE1284_MODE_BYTE;
	if (ioctl(fd, PPSETMODE, &mode) < 0)
	{
		//cout << "cant set mode" << endl;
		ioctl(fd, PPRELEASE);
		//cout << "released" << endl;
		close(fd);
		//cout << "closed" << endl;
		exit(0);
	}
	//cout << "set mode" << endl;

	ioctl(fd, PPDATADIR, 1); // nonzero is input
	//cout << "set data dir" << endl;

    running = true;
    read_thread = thread(read);
    read_thread.run();
}

void Parport::read()
{
	//cout << "read parport" << endl;
	//uint64_t last_time = get_micros();
	unsigned char res = 0x00;
	while(running)
	{
		//cout << "in measuring parport" << endl;
		if (measure)
		{
			//uint64_t before_time = get_micros();
			//cout << "lets read" << endl;
			ioctl(fd, PPRSTATUS, &res);
			//ioctl(fd, PPRDATA, &res);
			//if ((int)res != 0)
			//{
				//cout << "par " << (int)res << endl;
			//}
			bool state_click_new = res & (1 << click_pin);
			bool state_bright_new = res & (1 << bright_pin);
			bool state_bright_2_new = res & (1 << bright_pin_2);
			//cout << (int)state_click_new << (int)state_bright_new << (int) state_bright_2_new << endl;
			if ((int)state_click_new == 1 && state_click == 0) trigger_click();
			if ((int)state_bright_new == 1 && state_bright == 0) trigger_bright();
			if ((int)state_bright_2_new == 1 && state_bright_2 == 0) trigger_bright_2();
			//usleep(1000000);
			//uint64_t current_time = get_micros();
			//cout << "read time " << dec << current_time - before_time << "us; result " << hex << (int)res << dec << endl;
			//last_time = current_time;


		}
		usleep(10);
	}
	//cout << "end read parport" << endl;
}

void Parport::trigger_click() : GPIOHandler::trigger_click()
{

}

void Parport::trigger_bright() : GPIOHandler::trigger_bright()
{

}

void Parport::trigger_bright_2() : GPIOHandler::trigger_bright_2()
{

}

void Parport::cleanup()
{
    running = false;
    ioctl(fd, PPRELEASE);
    close(fd);
    read_thread.join();
}
