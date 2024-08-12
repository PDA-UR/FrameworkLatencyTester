#include "main.h"
#include "winvblank.h"
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <d3dkmthk.h>

#pragma comment(lib, "gdi32.lib")

using namespace std;

VblankHandlerWin::VblankHandlerWin()
{
    hdc = GetDC(NULL);

    // get D3DKMT_HANDLE for adapter and vid id
    D3DKMT_OPENADAPTERFROMHDC openAdapterFromHDC = {};
    openAdapterFromHDC.hDc = hdc;
    NTSTATUS status = D3DKMTOpenAdapterFromHdc(&openAdapterFromHDC);
    if (SUCCEEDED(status))
    {
        std::cerr << "Failed to open adapter from HDC" << std::endl;
        ReleaseDC(NULL, hdc);
    }
    D3DKMT_HANDLE adapterHandle = openAdapterFromHDC.hAdapter;
    D3DDDI_VIDEO_PRESENT_SOURCE_ID vidPnSrcId = openAdapterFromHDC.VidPnSourceId;

    // get D3DKMT_HANDLE for device
    D3DKMT_CREATEDEVICE createDevice = {};
    createDevice.hAdapter = adapterHandle;
    status = D3DKMTCreateDevice(&createDevice);
    if (SUCCEEDED(status))
    {
        std::cerr << "Failed to create device" << std::endl;
        ReleaseDC(NULL, hdc);
    }
    D3DKMT_HANDLE deviceHandle = createDevice.hDevice;

    // define vblankEvent
    vblankEvent = {};
    vblankEvent.hDevice = deviceHandle;
    vblankEvent.hAdapter = adapterHandle;
    vblankEvent.VidPnSourceId = vidPnSrcId;

    running = true;
    measure = false;

    measure_vblank_thread = thread(&VblankHandlerWin::get_vblanks, this);
}

void VblankHandlerWin::get_vblanks()
{
    unsigned int last_sync_count = 0;
    uint64_t last_vblank_time = get_micros();

    while (running)
    {
        NTSTATUS status = D3DKMTWaitForVerticalBlankEvent(&vblankEvent);

        if (SUCCEEDED(status))
        {
            uint64_t new_vblank_time = get_micros();
            last_sync_count++;

            if (measure == 1)
            {
                vsync_time[vsync_count] = new_vblank_time;
                vsync_count++;
            }
        }

        // TODO: das oder Sleep() ??
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}

void VblankHandlerWin::cleanup()
{
    running = false;
    measure_vblank_thread.join();
    ReleaseDC(NULL, hdc);
}
