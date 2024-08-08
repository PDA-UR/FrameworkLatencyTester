#include "win_getpixel_reader.h"
#include "pixelreader.h"

GetpixelReader::GetpixelReader(int x, int y) : PixelReader(x, y)
{
    initGetpixel();
}

// get handle device context for the whole screen
void GetpixelReader::initGetpixel()
{
    hdcScreen = GetDC(NULL);
}

void GetpixelReader::closeGetpixel()
{
    // TODO (?)
}

// get pixel at specified position with GetPixel
unsigned int GetpixelReader::getPixelColor()
{
    PixelReader::getPixelColor();
    COLORREF _color = GetPixel(hdcScreen, X, Y);
    return int(GetRValue(_color)); // red channel is enough for us
}

void GetpixelReader::cleanup() // : PixelReader::cleanup()
{
    closeGetpixel();
}