#include "DirectXHelper.h"

// this function initializes and prepares Direct3D for use
void DirectXHelper::initD3D(HWND hWnd, HINSTANCE hInstance)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION); // create the Direct3D interface

    D3DPRESENT_PARAMETERS d3dpp; // create a struct to hold various device information

    ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
	d3dpp.Windowed = !FULLSCREEN;    // program fullscreen, not windowed
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
    d3dpp.hDeviceWindow = hWnd;    // set the window to be used by Direct3D
//#if FULLSCREEN
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
    d3dpp.BackBufferWidth = SCREEN_WIDTH;    // set the width of the buffer
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;    // set the height of the buffer
//#endif

    // create a device class using this information and the info from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);

	v_buffer = NULL;
	
	//uncomment this to see the movement at work
	init_graphics();

	d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);    // turn off the 3D lighting
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(150, 150, 150));
	d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	
	
	camera = new Camera(d3ddev);
	input = new Input(d3ddev, camera);
	input->initDInput(hInstance, hWnd);

	
}


// this is the function used to render a single frame
void DirectXHelper::renderFrame(void)
{
	
    // clear the window to a deep blue
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 115, 255), 1.0f, 0);
	//d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    d3ddev->BeginScene();    // begins the 3D scene

	// select which vertex format we are using
	d3ddev->SetFVF(CUSTOMFVF);

	//get input and place the camera
	input->CheckForInput();
	camera->SetCamera();
	

	// select the vertex buffer to display
    d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));

    // do 3D rendering on the back buffer here
	// copy the vertex buffer to the back buffer
    d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

    d3ddev->EndScene();    // ends the 3D scene

    d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
}


// this is the function that cleans up Direct3D and COM
void DirectXHelper::cleanD3D(void)
{
    d3ddev->Release(); // close and release the 3D device
    d3d->Release(); // close and release Direct3D
	v_buffer->Release();    // close and release the vertex buffer
	delete camera;
	delete input;
}

void ::DirectXHelper::init_graphics(void)
{
    // create the vertices using the CUSTOMVERTEX struct
    CUSTOMVERTEX vertices[] =
    {
        { -3.0f, -3.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(0, 0, 255), },
        { 0.0f, 3.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(0, 255, 0), },
        { 3.0f, -3.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(255, 0, 0), },
    };

	/*CUSTOMVERTEX vertices[] = 
    {
		{-3.0f, -3.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0),},
        { 0.0f, 3.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0), },
        { 3.0f, -3.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255), },
    };*/

    // create a vertex buffer interface called v_buffer
    d3ddev->CreateVertexBuffer(3*sizeof(CUSTOMVERTEX),
                               0,
                               CUSTOMFVF,
                               D3DPOOL_MANAGED,
                               &v_buffer,
                               NULL);

    VOID* pVoid;    // a void pointer

    // lock v_buffer and load the vertices into it
    v_buffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertices, sizeof(vertices));
    v_buffer->Unlock();
}