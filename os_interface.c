#include <windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "os_interface.h"
#include "obj_data.h"
#include "draw.h"

Sys_dat sys_dat = {0};
Scrn_info scrn_info = {0};
bool keyboard[256] = {0};
bool pause = false;

bool os_Init (void)
{
    {
        static WNDCLASS wc = {0};
        static const wchar_t window_class_name[] = L"Window Class";
        wc.lpszClassName = (PCSTR)window_class_name;   
        wc.lpfnWndProc = os_Internal_WindowProcessMessage;
        RegisterClass(&wc);

        sys_dat.win32.frame_bitmap_info.bmiHeader.biSize = sizeof(sys_dat.win32.frame_bitmap_info.bmiHeader);
        sys_dat.win32.frame_bitmap_info.bmiHeader.biPlanes = 1;
        sys_dat.win32.frame_bitmap_info.bmiHeader.biBitCount = 32;
        sys_dat.win32.frame_bitmap_info.bmiHeader.biCompression = BI_RGB;
        sys_dat.win32.frame_device_context = CreateCompatibleDC(0);

        sys_dat.win32.hwnd = 
        CreateWindow((PCSTR)window_class_name, "Pong", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
        if (sys_dat.win32.hwnd == NULL) { return -1; }
        sys_dat.win32.quit = false;
    }

    return true;
}

LRESULT CALLBACK os_Internal_WindowProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static bool has_focus = true;
    switch (msg)
    {
        case WM_QUIT:
        case WM_CLOSE: DestroyWindow(hwnd); break;
        case WM_DESTROY: sys_dat.win32.quit = true; break;
        case WM_PAINT: 
        {
            static PAINTSTRUCT paint;
            static HDC device_context;
            device_context = BeginPaint(hwnd, &paint);
                BitBlt
                (
                    device_context,
                    paint.rcPaint.left, paint.rcPaint.top,
                    paint.rcPaint.right - paint.rcPaint.left, paint.rcPaint.bottom - paint.rcPaint.top,
                    sys_dat.win32.frame_device_context,
                    paint.rcPaint.left, paint.rcPaint.top,
                    SRCCOPY
                );
            EndPaint(hwnd, &paint);
        }
        break;
        case WM_SIZE: 
        {
            sys_dat.win32.frame_bitmap_info.bmiHeader.biWidth = LOWORD(lParam);
            sys_dat.win32.frame_bitmap_info.bmiHeader.biHeight = HIWORD(lParam);

            if (sys_dat.win32.frame_bitmap) DeleteObject(sys_dat.win32.frame_bitmap);
            sys_dat.win32.frame_bitmap = CreateDIBSection(NULL, &sys_dat.win32.frame_bitmap_info, DIB_RGB_COLORS, (void**)&sys_dat.frame_buffer.pixels, 0, 0);
            SelectObject(sys_dat.win32.frame_device_context, sys_dat.win32.frame_bitmap);

            sys_dat.frame_buffer.width = LOWORD(lParam);
            sys_dat.frame_buffer.height = HIWORD(lParam);

            scrn_info.width = sys_dat.frame_buffer.width;
            scrn_info.height = sys_dat.frame_buffer.height;

            scrn_info.pixel_density = sys_dat.frame_buffer.width*sys_dat.frame_buffer.height;
        }
        break;
        case WM_KILLFOCUS:
        {
            has_focus = false;
            memset(keyboard, 0, 256 * sizeof(keyboard[0]));
        }
        break;
        case WM_SETFOCUS: has_focus = true; break;
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            if (has_focus)
            {
                static bool key_is_down, key_was_down;
                key_is_down = ((lParam & (1 << 31)) == 0);
                key_was_down = ((lParam & (1 << 30)) != 0);
                if (key_is_down != key_was_down)
                {
                    keyboard[(uint8_t)wParam] = key_is_down;
                    if (key_is_down)
                    {
                        switch(wParam)
                        {
                            case VK_ESCAPE: sys_dat.win32.quit = true; break;
                            case VK_SPACE: (pause == true) ? (pause = false) : (pause = true); break;
                        }
                    }
                }
            }
        }
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void os_NextEvent (void)
{
    static MSG msg = { 0 };
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        DispatchMessage(&msg);
    }
}

void os_DrawScreen (void){
    InvalidateRect(sys_dat.win32.hwnd, NULL, FALSE);
    UpdateWindow(sys_dat.win32.hwnd);
}

void os_WindowSize (int width, int height)
{
    RECT rect = { 0, 0, width, height };

    AdjustWindowRect(
        &rect,
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
        FALSE
    );

    SetWindowPos(
        sys_dat.win32.hwnd,
        HWND_TOP,
        0, 0,
        rect.right - rect.left,
        rect.bottom - rect.top,
        SWP_NOMOVE | SWP_SHOWWINDOW
    );
}

void os_SetWindowFrameBuffer (int width, int height) {
	sys_dat.frame_buffer.has_been_set = true;
	sys_dat.frame_buffer.width  = width;
	sys_dat.frame_buffer.height = height;
}

void os_UpdateBuffer (void)
{
    for (int i = 0; i < 1000; ++i) sys_dat.frame_buffer.pixels[Rand32() % scrn_info.pixel_density] = 0;
    DrawNet(sys_dat.frame_buffer.pixels, scrn_info.width, scrn_info.height, 0x00808080);
    draw_number(sys_dat.frame_buffer.pixels, scrn_info.width, scrn_info.height, scrn_info.width/2+150-(FONT_W*2), scrn_info.height/2 + 125, enemy_dat.score, 0x00808080);
    draw_number(sys_dat.frame_buffer.pixels, scrn_info.width, scrn_info.height, scrn_info.width/2-150-(FONT_W*2), scrn_info.height/2 + 125, player_dat.score, 0x00808080);
    draw_paddle(sys_dat.frame_buffer.pixels, scrn_info.width, scrn_info.height, player_dat.x , player_dat.y, 0x00ffffff);
    draw_paddle(sys_dat.frame_buffer.pixels, scrn_info.width, scrn_info.height, enemy_dat.x , enemy_dat.y, 0x00ffffff);
    draw_ball(sys_dat.frame_buffer.pixels, scrn_info.width, scrn_info.height, ball_dat.x , ball_dat.y, 0x00ffffff);
}