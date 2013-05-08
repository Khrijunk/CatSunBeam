#ifndef INPUT_H
#define INPUT_H

#include "../includes.h"
#include "../Camera/Camera.h"
#include "../Textbox/Textbox.h"
#include "../Helper.h"
#include "../Picking.h"

class Input
{
public:
	Input(LPDIRECT3DDEVICE9 device, CCamera *cam, Textbox* text);
	~Input();
	void CheckForInput();
	void initDInput(HINSTANCE hInstance, HWND hWnd);

private:
	LPDIRECT3DDEVICE9 d3ddev;
	CCamera* camera;
	LPDIRECTINPUT8 din;
	LPDIRECTINPUTDEVICE8 dinkeyboard;
	LPDIRECTINPUTDEVICE8 dinmouse;
	BYTE keystate[256];
	DIMOUSESTATE mousestate;
	Textbox* textbox;
	Helper* helper;
	POINT mousePoint;
	Picking* picker;

	bool rightButtonWasDown;
	bool leftButtonWasDown;

	int mouse_x;
	int mouse_y;

	float cameraSpeed;
};

#endif