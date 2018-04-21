#include <iostream>

#include <stdio.h>

// X11 Libs
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <X11/X.h>
#include <X11/Xcms.h>
#include <X11/Xresource.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/keysymdef.h>
#include <X11/keysym.h>
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/Xprotostr.h>

Display* dis;
int screen;
Window win;
GC gc;

void print_current_displayenv();
void init_system(Display* dis, int* screen, Window* win, GC* gc);
int handle_x_events();
void draw_x_window();

void print_display_pixformats(Display *display);
void print_screen_info(Display* display, int screen);

int main()
{
    print_current_displayenv();

    int res = setenv("DISPLAY", ":0.0", 1);
    if(res != 0)
    {
        printf("%d", errno);
        exit(1);
    }

    init_system(dis, &screen, &win, &gc);

    print_display_pixformats(dis);
    print_screen_info(dis, screen);

    int winres = 0;
    while(true)
    {
        winres = handle_x_events();

        if(winres)
        {
            break;
        }
    }

    XFreeGC(dis, gc);
    XDestroyWindow(dis, win);
    XCloseDisplay(dis);

    return 0;
}

void init_system(Display* myDisplay, int* screen, Window* win, GC* gc)
{
    unsigned long black;
    unsigned long white;

    myDisplay = XOpenDisplay((char*)0);
    if(myDisplay == NULL)
    {
        printf("Call to XOpenDisplay failed with code %d", errno);
        exit(1);
    }

    dis = myDisplay;
    int screenref = DefaultScreen(dis);
    *screen = screenref;

    black = BlackPixel(dis, screenref);
    white = WhitePixel(dis, screenref);

    Window windowref = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, 400, 300, 5, black, white);
    *win = windowref;

    XSetStandardProperties(dis, windowref, "Hello Window !", "Hi!", None, NULL, 0, NULL);

    XSelectInput(dis, windowref, ExposureMask|ButtonPressMask|KeyPressMask);

    GC gcref = XCreateGC(dis, windowref, 0, 0);
    *gc = gcref;
    XSetBackground(dis, gcref, white);
    XSetForeground(dis, gcref, black);

    XMapWindow(dis, windowref);

    XClearWindow(dis, windowref);
    XMapRaised(dis, windowref);

    XFlush(dis);
}

int handle_x_events()
{
    XEvent event;
    KeySym key;
    char textbuf[256];

    XNextEvent(dis, &event);

    if(event.type == Expose && event.xexpose.count == 0)
    {
        draw_x_window();
    }

    if(event.type == KeyPress && XLookupString(&event.xkey, textbuf, 255, &key, 0) == 1)
    {
        if(textbuf[0] == 'q')
        {
            printf("Shutting down!\n");

            return 1;
        }
    }

    if(event.type == ButtonPress)
    {
        printf("Press (%i, %i)\n", event.xbutton.x, event.xbutton.y);
    }

    return 0;
}

void draw_x_window()
{
    char str[128];
    strcpy(str, "Hello X11");
    XDrawString(dis, win, gc, 0, 0, str, (int)strlen(str));

    XFillRectangle(dis, win, gc, 20, 20, 50, 50);

    XFlush(dis);
}

void print_display_pixformats(Display *display)
{
    int amount = 0;
    XPixmapFormatValues* formats = XListPixmapFormats(display, &amount);

    if(formats == NULL)
    {
        printf("Unable to get pixel formats");
        return;
    }

    for(int i = 0; i < amount; i++)
    {
        XPixmapFormatValues* current = formats + i;

        printf("Format %d : Depth = %d, BPP = %d, Pad = %d\n",
               i,
               current->depth,
               current->bits_per_pixel,
               current->scanline_pad);
    }

    XFree(formats);
}

void print_screen_info(Display* display, int screen)
{
    printf("Screen %i info :\n", screen);

    int ibo = XImageByteOrder(display);
    if(ibo == LSBFirst)
    {
        printf("Image Byte order = LSB First\n");
    }
    else if(ibo == MSBFirst)
    {
        printf("Image Byte order = MSB First\n");
    }
    else
    {
        printf("Image Byte order = Unknown\n");
    }

    int bitmapUnit = XBitmapUnit(display);
    printf("Bitmap Unit = %i\n", bitmapUnit);

    int bbo = XBitmapBitOrder(display);
    if(bbo == LSBFirst)
    {
        printf("Bitmap Bit Order = LSB First\n");
    }
    else if(bbo == MSBFirst)
    {
        printf("Bitmap Bit Order = MSB First\n");
    }
    else
    {
        printf("Bitmap Bit Order = Unknown\n");
    }

    int bitmapPad = XBitmapPad(display);
    printf("Bitmap Pad = %i\n", bitmapPad);

    int dh = XDisplayHeight(display, screen);
    printf("Display Height = %i\n", dh);

    int dw = XDisplayWidth(display, screen);
    printf("Display Width = %i\n", dw);

}

void print_current_displayenv()
{
    char* value = getenv("DISPLAY");

    if(value == NULL)
    {
        printf("DISPLAY env is NULL.\n");
    }
    else
    {
        printf("Current DISPLAY is: %s \n", value);
    }
}