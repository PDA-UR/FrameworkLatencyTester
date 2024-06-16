#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/io.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/ppdev.h>
#include <linux/parport.h>

//#define base 0x37A //0x378

int parportfd;

void init()
{
	parportfd = open("/dev/parport0", O_RDWR);
	std::cout << "parport0 fd: " << parportfd << std::endl;

	ioctl(parportfd, PPEXCL);
	std::cout << "excl" << std::endl;
	ioctl(parportfd, PPCLAIM);
	std::cout << "claimed" << std::endl;

	int mode = IEEE1284_MODE_ECP;
	//int mode = IEEE1284_MODE_BYTE;
	if (ioctl(parportfd, PPSETMODE, &mode) < 0)
	{
		std::cout << "cant set mode" << std::endl;
		ioctl(parportfd, PPRELEASE);
		std::cout << "released" << std::endl;
		close(parportfd);
		std::cout << "closed" << std::endl;
		exit(0);
	}
	std::cout << "set mode" << std::endl;

	ioctl(parportfd, PPDATADIR, 1); // nonzero is input
	std::cout << "set data dir" << std::endl;

}

void cleanup()
{
	ioctl(parportfd, PPRELEASE);
	std::cout << "released" << std::endl;
	close(parportfd);
	std::cout << "closed" << std::endl;
}

bool check_pin(unsigned char pin, unsigned char data)
{
	unsigned char mask = 0x01 << pin;
	return data & mask;
}

int main(int argc, char** argv)
{
	init();

	//int on[1] = {0xFF};
	//int off[1] = {0x00};
	//int buf[1] = {0x00};

	unsigned char res = 0x00;

	for(int i = 0; i < 100; i++) {
		ioctl(parportfd, PPRDATA, &res);
		//std::cout << "read: " << (int)res << std::endl;
		for (unsigned char pin = 0x00; pin < 8; pin++)
		{
			std::cout << check_pin(pin, res);
		}
		std::cout << std::endl;
		usleep(100000);

		//size_t size = read(parportfd, &buf, 1);
		//if ((int) size == -1)
		//{
		//	std::cerr << "error " << errno << std::endl;
		//	break;
		//}
		//std::cout << "read: " << buf[0] << " size: " << (int) size << std::endl;

		//write(parportfd, on, 1);
		//write(parportfd, off, 1);
		//std::cout << "low" << std::endl;
		//usleep(1000000);
	}

	//int result = iopl(3);
	//std::cout << "iopl result: " << result << std::endl;

	//if (ioperm(base, 1, 1))
	//{
	//	std::cout << "let's go" << std::endl;
	//	for(int i = 0; i < 10; i++) {
	//		
	//		std::cout << "before outb" << std::endl;
	//		outb(0x00, base);
	//		std::cout << "after outb" << std::endl;
	//		usleep(1000000);
	//		outb(0xFF, base);
	//		usleep(1000000);
	//	}
	//}
	//else
	//{
	//	std::cout << "no permission" << std::endl;
	//}
	
	cleanup();
}
