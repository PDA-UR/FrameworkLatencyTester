#ifndef GETPIXEL_READER_H
#define GETPIXEL_READER_H

#include "pixelreader.h"
#include <windows.h>

class GetpixelReader : public PixelReader
{
private:
    // variables for gtepixel
    HDC hdcScreen;

    void initGetpixel();
    void closeGetpixel();

public:
    GetpixelReader(int x, int y);          // : PixelReader(int x, int y);
    unsigned int getPixelColor() override; // : PixelReader::getPixelColor();
    void cleanup();
};

#endif
