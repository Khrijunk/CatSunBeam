#ifndef DIRECTXHELPER_H
#define DIRECTXHELPER_H

#include "Includes.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "Input/Input.h"
#include "Textbox/Textbox.h"
#include "Particles/Particles.h";

//ADDED BY ZACK
#include "Model/Model.h"
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

    // ADDITIONS BY ZACK
    Model *mesh;
    // END ADDITIONS BY ZACK

	Camera* camera;
	Input* input;
	Textbox* textbox;
	Particles* p;
	Helper* helper;

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