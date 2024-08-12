#ifndef WIN_VBLANK_H
#define WIN_VBLANK_H

#include <windows.h>
#include <d3dkmthk.h>
#include <thread>
#include <iostream>
#include <chrono>

class VblankHandlerWin
{
private:
    bool measure_vblank;
    bool running;
    thread measure_vblank_thread;
    HDC hdc;
    D3DKMT_WAITFORVERTICALBLANKEVENT vblankEvent;

public:
    uint64_t vsync_time[100000];
    uint64_t vsync_count;

    VblankHandlerWin();
    bool measure;
    void get_vblanks();
    void cleanup();
};

#endif
