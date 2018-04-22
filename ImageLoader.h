#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

class ImageLoader
{
public:
    ImageLoader(Display* display, int screen, Window windowHandle, GC gcHandle);

    Pixmap LoadImage(char* path);

    ~ImageLoader();

private:
    Display* ownerDisplay;
    int ownerScreen;
    Window ownerWindow;
    GC ownerContext;

};


