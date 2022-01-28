#include "p5.h"
using namespace std;

//->    FRAMERATE
extern int fCount;
extern int fps;
double delta;
double now;
double lastFrame;
double elapsedTime = 0;
float framerate;
const float& frameRate = framerate;

//->    SCREEN
extern int* internBuffer;

//->    KEYBOARD
bool keypressed;
bool keyreleased;
unsigned int key_ = 0;
const bool& isKeyReleased = keyreleased;
const bool& isKeyPressed = keypressed;
const unsigned int& key = key_;

//->    MOUSE
int tmp_mx = 0;
int tmp_my = 0;
int mx = 0;
int my = 0;
int pmx = 0;
int pmy = 0;
bool mousepressed;
const int& mouseX = mx;
const int& mouseY = my;
const int& pmouseX = pmx;
const int& pmouseY = pmy;
const bool& mousePressed = mousepressed;

//-> WINDOW
HWND hWnd;
RECT win_dim;

void paint(HDC& hdc);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    MSG                 msg;
    WNDCLASS            wndClass;

    wndClass.style          = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc    = WndProc;
    wndClass.cbClsExtra     = 0;
    wndClass.cbWndExtra     = 0;
    wndClass.hInstance      = hInstance;
    wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName   = NULL;
    wndClass.lpszClassName  = TEXT("GettingStarted");

    RegisterClass(&wndClass);

    setup();

    win_dim = {0, 0, width, height};
    AdjustWindowRect(&win_dim, WS_OVERLAPPEDWINDOW, false);

    hWnd = CreateWindow(
            TEXT("GettingStarted"),   // window class name
            TEXT("Getting Started"),  // window caption
            WS_OVERLAPPEDWINDOW,      // window style
            CW_USEDEFAULT,            // initial x position
            CW_USEDEFAULT,            // initial y position
            win_dim.right - win_dim.left,            // initial x size
            win_dim.bottom - win_dim.top,            // initial y size
            NULL,                     // parent window handle
            NULL,                     // window menu handle
            hInstance,                // program instance handle
            NULL);                    // creation parameters

    ShowWindow(hWnd, iCmdShow);

    UpdateWindow(hWnd);

    lastFrame = milliseconds();

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if(GetFocus() == hWnd)
            InvalidateRect(hWnd, NULL, false);

        LPPOINT mouse_pos;
        LPRECT windows_pos;
        GetCursorPos(mouse_pos);
        GetWindowRect(hWnd, windows_pos);
        tmp_mx = mouse_pos->x - (windows_pos->left - win_dim.left);
        tmp_my = mouse_pos->y - (windows_pos->top - win_dim.top);
    }

    return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
                         WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;


    switch(message)
    {
        case WM_PAINT:
            now = milliseconds();
            delta = now - lastFrame;

            elapsedTime += delta;

            if(elapsedTime > 1000.0 / fps){
                hdc = BeginPaint(hWnd, &ps);

                draw();

                if(keypressed)
                    keyPressed();
                if(keyreleased)
                    keyReleased();

                if(mouseX != pmouseX || mouseY != pmouseY)
                    mouseMoved();

                paint(hdc);


                EndPaint(hWnd, &ps);

                framerate = 1000.0 / elapsedTime;
                elapsedTime = 0;
                fCount++;

                keypressed = false;
                keyreleased = false;
                if((GetKeyState(VK_LBUTTON) & 0x8000) != 0 || (GetKeyState(VK_RBUTTON) & 0x8000) != 0)
                    mousepressed = true;
                else
                    mousepressed = false;

                pmx = mx;
                pmy = my;
                mx = tmp_mx;
                my = tmp_my;
            }

            lastFrame = now;

            return 0;
        case WM_KEYDOWN:
            key_ = wParam;
            keypressed = true;
            return 0;
        case WM_KEYUP:
            keyreleased = true;
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProc

void paint(HDC& hdc){

    /*
    arr is a char array containing the RGB value of each pixel sequentially
    Arithmetic operations done to each element of R here
    */

    HBITMAP map = CreateBitmap(width, // width. 512 in my case
                               height, // height
                               1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
                               8*4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
                               (void*) internBuffer); // pointer to array
    // Temp HDC to copy picture
    HDC src = CreateCompatibleDC(hdc); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
    SelectObject(src, map); // Inserting picture into our temp HDC
    // Copy image from temp HDC to window
    BitBlt(hdc, // Destination
           0,  // x and
           0,  // y - upper-left corner of place, where we'd like to copy
           width, // width of the region
           height, // height
           src, // source
           0,   // x and
           0,   // y of upper left corner  of part of the source, from where we'd like to copy
           SRCCOPY); // Defined DWORD to juct copy pixels. Watch more on msdn;

    DeleteDC(src); // Deleting temp HDC
}