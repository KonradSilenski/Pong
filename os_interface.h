#ifndef __OSINT__
#define __OSINT__

#pragma push_macro("NAMESPACE")
#define NAMESPACE os_

#pragma push_macro ("C")
#undef C
#pragma push_macro ("C_")
#undef C_
#pragma push_macro ("N")
#undef N

#define C_(a,b) a##b
#define C(a,b) C_(a,b)
#define N(a) C(NAMESPACE, a)

#define PRINT_ERROR(a, args...) printf("ERROR %s() %s Line %d: " a "\n", __FUNCTION__, __FILE__, __LINE__, ##args);

#define EXIT_IF(a) do { if(a) { PRINT_ERROR(#a); abort(); } } while(0)

#if RAND_MAX == 32767
#define Rand32() ((rand() << 16) + (rand() << 1) + (rand() & 1))
#else
#define Rand32() rand()
#endif

#include <windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    struct {
        HWND hwnd;
        BITMAPINFO frame_bitmap_info;
        HBITMAP frame_bitmap;
        HDC frame_device_context;
        bool quit;
    } win32;
    struct {
        int width, height;
        uint32_t *pixels;
        bool has_been_set;
    } frame_buffer;
} Sys_dat;

typedef struct {
    union { unsigned int width, w; };
    union { unsigned int height, h; };
    unsigned int pixel_density;
} Scrn_info;

extern bool pause;
extern bool keyboard[256];
extern Sys_dat sys_dat;
extern Scrn_info scrn_info;

LRESULT CALLBACK N(Internal_WindowProcessMessage)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);   // Processes window events and performs related functionality.

// Initialises values and creates the window accordingly.
bool N(Init) (void);

// Dispatches window events to be processed.
void N(NextEvent) (void);

// Resets the window buffer.
void N(DrawScreen) (void);

// Initialises window size values.
void N(WindowSize) (int width, int height);

// Initialises buffer values.
void N(SetWindowFrameBuffer) (int width, int height);

// Updates buffer with new bitmap data.
void N(UpdateBuffer) (void);

#pragma pop_macro("NAMESPACE")
#pragma pop_macro("C")
#pragma pop_macro("C_")
#pragma pop_macro("N")

#endif