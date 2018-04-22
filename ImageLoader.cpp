#include <cstdio>
#include "ImageLoader.h"

ImageLoader::ImageLoader(Display *display, int screen, Window windowHandle, GC gcHandle)
{
    ownerDisplay = display;
    ownerScreen = screen;
    ownerWindow = windowHandle;
    ownerContext = gcHandle;
}

ImageLoader::~ImageLoader()
{

}

Pixmap ImageLoader::LoadImage(char *path)
{
    Pixmap bitmap;
    unsigned int bw, bh;
    int hotspotX, hotspotY;

    int res = XReadBitmapFile(ownerDisplay, ownerWindow, path, &bw, &bh, &bitmap, &hotspotX, &hotspotY);

    switch(res)
    {
        case BitmapOpenFailed:
            printf("Open failed.\n");
            break;
        case BitmapFileInvalid:
            printf("File invalid.\n");
            break;
        case BitmapNoMemory:
            printf("No mem.\n");
            break;
        case BitmapSuccess:
            printf("Bitmap loading success !\n");
            break;
        default:
            printf("Unknown error while loading a bitmap. \n");
            break;
    }

    return bitmap;
}

