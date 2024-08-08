#include "main.h"
#include "gpio.h"
#include "winparport.h"

using namespace std;

WinParportHandler::WinParportHandler() : GPIOHandler()
{
    click_pin = 5;
    bright_pin = 4;
    bright_pin_2 = 3;

    // hInpOutDll = LoadLibrary(L"InpOutx64.DLL");
    hInpOutDll = LoadLibrary(L"InpOut32.DLL");
    if (hInpOutDll != NULL)
    {
        gfpOut32 = (lpOut32)GetProcAddress(hInpOutDll, "Out32");
        gfpInp32 = (lpInp32)GetProcAddress(hInpOutDll, "Inp32");
        gfpIsInpOutDriverOpen = (lpIsInpOutDriverOpen)GetProcAddress(hInpOutDll, "IsInpOutDriverOpen");
        gfpIsXP64Bit = (lpIsXP64Bit)GetProcAddress(hInpOutDll, "IsXP64Bit");
    }

    // TODO: find address for parport device
    address = 0;

    // fd = open("/dev/parport0", O_RDWR);
    // //cout << "parport0 fd: " << fd << endl;

    // ioctl(fd, PPEXCL);
    // //cout << "excl" << endl;
    // ioctl(fd, PPCLAIM);
    // //cout << "claimed" << endl;

    // // todo: use exception handling
    // //int mode = IEEE1284_MODE_EPP; //ECP
    // int mode = IEEE1284_MODE_BYTE;
    // if (ioctl(fd, PPSETMODE, &mode) < 0)
    // {
    // 	//cout << "cant set mode" << endl;
    // 	ioctl(fd, PPRELEASE);
    // 	//cout << "released" << endl;
    // 	close(fd);
    // 	//cout << "closed" << endl;
    // 	exit(0);
    // }
    // //cout << "set mode" << endl;

    // ioctl(fd, PPDATADIR, 1); // nonzero is input
    // //cout << "set data dir" << endl;

    running = true;
    read_thread = thread(&WinParportHandler::readPins, this);
    // //read_thread.run();
}

void WinParportHandler::readPins()
{
    unsigned char res = 0x00;
    while (running)
    {
        if (measure)
        {
            if (gfpIsInpOutDriverOpen())
            {
                short iPort = address;
                res = gfpInp32(iPort);

                bool state_click_new = res & (1 << click_pin);
                bool state_bright_new = res & (1 << bright_pin);
                bool state_bright_2_new = res & (1 << bright_pin_2);
                // cout << (int)state_click_new << (int)state_bright_new << (int) state_bright_2_new << endl;
                if ((int)state_click_new == 1 && state_click == 0)
                    trigger_click();
                if ((int)state_bright_new == 1 && state_bright == 0)
                    trigger_bright();
                if ((int)state_bright_2_new == 1 && state_bright_2 == 0)
                    trigger_bright_2();
            }
        }
        Sleep(0.01);
    }
}

// void ParportHandler::trigger_click() : GPIOHandler::trigger_click()
//{
//
// }
//
// void ParportHandler::trigger_bright() : GPIOHandler::trigger_bright()
//{
//
// }
//
// void ParportHandler::trigger_bright_2() : GPIOHandler::trigger_bright_2()
//{
//
// }

void WinParportHandler::cleanup()
{
    running = false;
    FreeLibrary(hInpOutDll);
    read_thread.join();
}
