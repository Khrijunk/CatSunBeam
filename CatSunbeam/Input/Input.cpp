#include "Input.h"

Input::Input(LPDIRECT3DDEVICE9 device, Camera *cam, Textbox* text)
{
	camera = cam;
	d3ddev = device;
	textbox = text;
	helper = new Helper();
	picker = new Picking(d3ddev);
}

Input::~Input()
{
	dinkeyboard->Unacquire();
	dinmouse->Unacquire();
	din->Release();
	delete helper;
	delete picker;
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


	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DIPROPAXISMODE_REL; //or ABS here
 
	dinmouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);

	
	dinkeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	dinmouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	dinkeyboard->SetDataFormat(&c_dfDIKeyboard);
	dinmouse->SetDataFormat(&c_dfDIMouse);

	dinmouse->Acquire();
	
	rightButtonWasDown = false;
	leftButtonWasDown = false;

}

void Input::CheckForInput()
{
	
	//create keyboard array
	
	dinkeyboard->Acquire();
	dinkeyboard->GetDeviceState(256, (LPVOID)keystate);

	//create mouse struct
	
	dinmouse->Acquire();
	dinmouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mousestate);

    //ZACK CHANGED!!!!!
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

        if(keystate[DIK_E]&0x80)
		{
			camera->AddToZPos(-sin(D3DXToRadian(camera->GetAngle() + 90)));
			camera->AddToYPos(-cos(D3DXToRadian(camera->GetAngle() + 90)));
		}

        if(keystate[DIK_Q]&0x80)
		{
			camera->AddToZPos(sin(D3DXToRadian(camera->GetAngle() + 90)));
			camera->AddToYPos(cos(D3DXToRadian(camera->GetAngle() + 90)));
		}
        //END OF ZACK'S CHANGES

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

		if(mousestate.rgbButtons[0])
		{
			leftButtonWasDown = true;
		}
		else if(leftButtonWasDown)
		{
			GetCursorPos(&mousePoint);
			//textbox->SetString("x: " + helper->toString(mousePoint.x) + " y: " + helper->toString(mousePoint.y));
			Ray ray;
			ray = picker->CalcPickingRay(mousePoint.x, mousePoint.y);
			
			BoundingSphere sphere;

			sphere.center = D3DXVECTOR3(0,0,0);
			sphere.radius = 3;

			if(picker->PickingTest(&ray, &sphere))
			{
				textbox->SetString("You clicked the white triangle");
			}
			else
				textbox->ClearString();
			
			
			leftButtonWasDown = false;
		}
			
}