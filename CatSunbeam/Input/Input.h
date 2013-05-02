#ifndef INPUT_H
#define INPUT_H

#include "../includes.h"
#include "../Camera/Camera.h"

class Input
{
public:
	Input(LPDIRECT3DDEVICE9 device, Camera *cam);
	~Input();
	void CheckForInput();
	void initDInput(HINSTANCE hInstance, HWND hWnd);

private:
	LPDIRECT3DDEVICE9 d3ddev;
	Camera* camera;
	LPDIRECTINPUT8 din;
	LPDIRECTINPUTDEVICE8 dinkeyboard;
	LPDIRECTINPUTDEVICE8 dinmouse;
	BYTE keystate[256];
	DIMOUSESTATE mousestate;

	bool rightButtonWasDown;

};

#endif