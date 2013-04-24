#ifndef INCLUDES_H
#define INCLUDES_H

// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

// define the screen resolution
#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

// define the keyboard macro
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

//determines if the program should be full-screen or not
//1 - fullscreen, 0 - windowed
#define FULLSCREEN 0

#endif