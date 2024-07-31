#include "main.h"
#include "parport.h"

using namespace std;

bool parport_active = 0;

void init_parport()
{
	parportfd = open("/dev/parport0", O_RDWR);
	//cout << "parport0 fd: " << parportfd << endl;

	ioctl(parportfd, PPEXCL);
	//cout << "excl" << endl;
	ioctl(parportfd, PPCLAIM);
	//cout << "claimed" << endl;

	//int mode = IEEE1284_MODE_EPP; //ECP
	int mode = IEEE1284_MODE_BYTE;
	if (ioctl(parportfd, PPSETMODE, &mode) < 0)
	{
		//cout << "cant set mode" << endl;
		ioctl(parportfd, PPRELEASE);
		//cout << "released" << endl;
		close(parportfd);
		//cout << "closed" << endl;
		exit(0);
	}
	//cout << "set mode" << endl;

	ioctl(parportfd, PPDATADIR, 1); // nonzero is input
	//cout << "set data dir" << endl;
}

void read_partport()
{
	//cout << "read parport" << endl;
	//uint64_t last_time = get_micros();
	unsigned char res = 0x00;
	while(measuring)
	{
		//cout << "in measuring parport" << endl;
		if (parport_active)
		{
			//uint64_t before_time = get_micros();
			//cout << "lets read" << endl;
			ioctl(parportfd, PPRSTATUS, &res);
			//ioctl(parportfd, PPRDATA, &res);
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



void cleanup_parport()
{
    ioctl(parportfd, PPRELEASE);
    close(parportfd);
    read_parport_thread.join();
}
