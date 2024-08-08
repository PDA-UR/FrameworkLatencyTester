#ifndef WIN_PARPORT_H
#define WIN_PARPORT_H

#include "gpio.h"
// TODO: brauchts das? #include "stdafx.h"
#include <thread>
#include <windows.h>
#include <stdio.h>

typedef void(__stdcall *lpOut32)(short, short);
typedef short(__stdcall *lpInp32)(short);
typedef BOOL(__stdcall *lpIsInpOutDriverOpen)(void);
typedef BOOL(__stdcall *lpIsXP64Bit)(void);

class WinParportHandler : public GPIOHandler
{
private:
    int address;
    HINSTANCE hInpOutDll;
    thread read_thread;
    void readPins();
    bool running;

    lpOut32 gfpOut32;
    lpInp32 gfpInp32;
    lpIsInpOutDriverOpen gfpIsInpOutDriverOpen;
    lpIsXP64Bit gfpIsXP64Bit;

public:
    WinParportHandler(); // : GPIOHandler();
    void cleanup();
};

#endif
