#ifndef DIRECTXHELPER_H
#define DIRECTXHELPER_H

#include "Includes.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "Input/Input.h"
#include "Textbox/Textbox.h"
#include "Particles/Particles.h"
#include "Stencil/Stencil.h"

//ADDED BY ZACK
#include "Model/Model.h"
#include "AnimatedModel.h"
// END ADDED BY ZACK

class DirectXHelper
{
public:
	// function prototypes
	void initD3D(HWND hWnd, HINSTANCE hInstance); // sets up and initializes Direct3D
	void renderFrame(void); // renders a single frame
	void cleanD3D(void); // closes Direct3D and releases memory
	void init_graphics(void);

private:
	// global declarations
	LPDIRECT3D9 d3d; // the pointer to our Direct3D interface
	LPDIRECT3DDEVICE9 d3ddev; // the pointer to the device class
	LPDIRECT3DVERTEXBUFFER9 v_buffer;    // the pointer to the vertex buffer
	IDirect3DPixelShader9* pixelShader;
	LPDIRECT3DTEXTURE9 skyTextures[6];
	D3DXVECTOR3 lightDir;

    // ADDITIONS BY ZACK
    Model *room;
    Model *window;
    Model *floor;
    Model *cat;
    AnimatedModel *cat2;
    // END ADDITIONS BY ZACK

	CCamera* camera;
	Input* input;
	Textbox* textbox;
	Particles* p;
	Particles* p2;
	Particles* p3;
	//Flames* flame;
	Helper* helper;
	Stencil* stencil;

	float newYAngle;
	float xPosition;
	float zPosition;
	float yPosition;
	float xDirection;
	float yDirection;
	float zDirection;
	float angle;
	float yAngle;
	float speed;

};

#endif