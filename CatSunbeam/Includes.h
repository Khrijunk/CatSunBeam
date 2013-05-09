#ifndef INCLUDES_H
#define INCLUDES_H

// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <string>
#include <sstream>

// ALTERATIONS BY ZACK --------------
#include <vector>
// END ALTERATIONS BY ZACK ----------

// define the screen resolution
#define SCREEN_WIDTH GetSystemMetrics(SM_CXSCREEN)//1280
#define SCREEN_HEIGHT GetSystemMetrics(SM_CYSCREEN)//720

// define the keyboard macro
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1: 0)


// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//determines if the program should be full-screen or not
//1 - fullscreen, 0 - windowed
#define FULLSCREEN 0

struct CUSTOMVERTEX{FLOAT X, Y, Z; D3DVECTOR NORMAL; FLOAT U, V;};
struct CUSTOMVERTEXNONORMAL{FLOAT X, Y, Z; FLOAT U, V;};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
#define CUSTOMFVFNONORMAL (D3DFVF_XYZ | D3DFVF_TEX1)
//#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
//struct CUSTOMVERTEX {FLOAT X, Y, Z; DWORD COLOR;};

using namespace std;


//structs
struct Ray
{
	D3DXVECTOR3 origin;
	D3DXVECTOR3 direction;
};

struct BoundingSphere
{
	//BoundingSphere();

	D3DXVECTOR3 center;
	float radius;
};

#endif