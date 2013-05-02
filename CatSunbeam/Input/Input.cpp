#include "Input.h"

Input::Input(LPDIRECT3DDEVICE9 device, Camera *cam)
{
	camera = cam;
	d3ddev = device;
}

Input::~Input()
{
	dinkeyboard->Unacquire();
	dinmouse->Unacquire();
	din->Release();
}

void Input::initDInput(HINSTANCE hInstance, HWND hWnd)
{
	

	DirectInput8Create(hInstance,
					   DIRECTINPUT_VERSION,
					   IID_IDirectInput8,
					   (void**)&din,
					   NULL);

	din->CreateDevice(GUID_SysKeyboard,
					  &dinkeyboard,
					  NULL);

	din->CreateDevice(GUID_SysMouse,
					  &dinmouse,
					  NULL);

	dinkeyboard->SetDataFormat(&c_dfDIKeyboard);
	dinmouse->SetDataFormat(&c_dfDIMouse);

	dinkeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	dinmouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND);

	dinmouse->Acquire();
	
	rightButtonWasDown = false;

	return;

}

void Input::CheckForInput()
{
	//create keyboard array
	
	dinkeyboard->Acquire();
	dinkeyboard->GetDeviceState(256, (LPVOID)keystate);

	//create mouse struct
	
	dinmouse->Acquire();
	dinmouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mousestate);

	if(keystate[DIK_W]&0x80)
		{
			camera->AddToXPos(sin(D3DXToRadian(camera->GetAngle())));
			camera->AddToZPos(cos(D3DXToRadian(camera->GetAngle())));
		}

		if(keystate[DIK_S]&0x80)
		{
			camera->AddToXPos(-sin(D3DXToRadian(camera->GetAngle())));
			camera->AddToZPos(-cos(D3DXToRadian(camera->GetAngle())));
		}

		if(keystate[DIK_A]&0x80)
		{
			camera->AddToXPos(-sin(D3DXToRadian(camera->GetAngle() + 90)));
			camera->AddToZPos(-cos(D3DXToRadian(camera->GetAngle() + 90)));
		}

		if(keystate[DIK_D]&0x80)
		{
			camera->AddToXPos(-sin(D3DXToRadian(camera->GetAngle() - 90)));
			camera->AddToZPos(-cos(D3DXToRadian(camera->GetAngle() - 90)));
		}
		
		//check mouse
		if(mousestate.rgbButtons[1])
		{
			if(!rightButtonWasDown)
				ShowCursor(false);
			camera->ChangeDirection(mousestate);
			rightButtonWasDown = true;
		}
		else if(rightButtonWasDown)
		{
			ShowCursor(true);
			rightButtonWasDown = false;
		}
}