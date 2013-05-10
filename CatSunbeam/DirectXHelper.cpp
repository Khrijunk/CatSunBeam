/*****************************************************************************
Tutorial for skybox code: http://www.gameengineer.net/samples-graphics.html
******************************************************************************/

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
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	//d3dpp.BackBufferFormat = D3DFMT_D24S8;    // set the back buffer format to 32-bit
    d3dpp.BackBufferWidth = SCREEN_WIDTH;    // set the width of the buffer
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;    // set the height of the buffer
    //d3dpp.EnableAutoDepthStencil = true;
	 d3dpp.EnableAutoDepthStencil = true;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    // create a device class using this information and the info from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);

	v_buffer = NULL;
	
	//uncomment this to see the movement at work
	init_graphics();

	d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);    // turn off the 3D lighting
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(100, 100, 100));
	d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	
	RECT rect;
	SetRect(&rect, 20, 20, SCREEN_WIDTH, 256);
	
	camera = new CCamera();
	textbox = new Textbox(d3ddev, 16, rect);
	input = new Input(d3ddev, camera, textbox);
	stencil = new Stencil(d3ddev);
	D3DXVECTOR3 s,m,m1,v,a;
	s.x = 0.0f;
	s.y = 12.0f;
	s.z = -15.0f;
	m.x = -3.0f;
	m.y = 0.0f;
	m.z = -15.0f;
	m1.x = 0.0f;
	m1.y = 12.0f;
	m1.z = 5.0f;
	v.x = 2.0f;
	v.y =  -.25f;
	v.z = 2.0f;
	a.x = 0.0f;
	a.y = .15f;
	a.z = 0.0f;
	p = new Particles(s,m,m1,v,a);
	p->intBuffers(d3ddev);
	D3DXVECTOR3 s2,mm,mm1,v1,a1;
	s2.x = 0.0f;
	s2.y = 10.0f;
	s2.z = -10.0f;
	mm.x = -3.0f;
	mm.y = 0.0f;
	mm.z = -15.0f;
	mm1.x = 0.0f;
	mm1.y = 10.0f;
	mm1.z = 5.0f;
	v1.x = 2.0f;
	v1.y =  -.25f;
	v1.z = 2.0f;
	a1.x = 0.0f;
	a1.y = .15f;
	a1.z = 0.0f;
	p2 = new Particles(s2,mm,mm1,v1,a1);
	D3DXVECTOR3 s3,mmm,mmm1,v2,a2;
	s3.x = 0.0f;
	s3.y = 8.0f;
	s3.z = -5.0f;
	mmm.x = -3.0f;
	mmm.y = 0.0f;
	mmm.z = -15.0f;
	mmm1.x = 0.0f;
	mmm1.y = 10.0f;
	mmm1.z = 5.0f;
	v2.x = 2.0f;
	v2.y =  -.25f;
	v2.z = 2.0f;
	a2.x = 0.0f;
	a2.y = .15f;
	a2.z = 0.0f;
	p3 = new Particles(s3,mmm,mmm1,v2,a2);
	//flame = new Flames(s2,mm,mm1,v1,a1);
	//flame->intBuffers(d3ddev);

	helper = new Helper();
	input->initDInput(hInstance, hWnd);

	
}


// this is the function used to render a single frame
void DirectXHelper::renderFrame(void)
{
	D3DXMATRIX matWorld;
    // clear the window to a deep blue
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 115, 255), 100.0f, 0);
	//d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 100.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    d3ddev->BeginScene();    // begins the 3D scene

	// select which vertex format we are using
	d3ddev->SetFVF(CUSTOMFVF);

	helper->IncreaseTimer();
	//textbox->SetString(helper->toString(helper->GetTime()));
	//get input and place the camera
	input->CheckForInput();
	camera->Update();
	d3ddev->SetTransform(D3DTS_VIEW, camera->GetViewMatrix());
	d3ddev->SetTransform(D3DTS_PROJECTION, camera->GetProjectionMatrix());

	textbox->Draw();
	
	// select the vertex buffer to display
    d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
    // do 3D rendering on the back buffer here
	// copy the vertex buffer to the back buffer
	//D3DXMATRIX matTranslate;
	//D3DXMATRIX matScale;

	//D3DXMatrixTranslation(&matTranslate, 0, 0, 0);
	//D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.5f);

	//d3ddev->SetTransform(D3DTS_WORLD, &(matScale * matTranslate));
    //d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
	p->run_particles(d3ddev,0);

	d3ddev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
    d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, false );
	d3ddev->SetRenderState( D3DRS_LIGHTING, false );

	// Render the sky box
    d3ddev->SetFVF( CUSTOMFVFNONORMAL );
    d3ddev->SetStreamSource( 0, v_buffer, 0, sizeof(CUSTOMVERTEXNONORMAL));
    
	D3DXMATRIX rotate;

	// Set the world matrix to identity for the skybox
	D3DXMatrixTranslation(&matWorld, 0, 0, 0);
	D3DXMatrixRotationY(&rotate, D3DXToRadian(60));
	d3ddev->SetTransform( D3DTS_WORLD, &(rotate * matWorld) );

    // Render the 6 faces of the skybox
	// DrawPrimitive is used to draw polygons when the vertices are stored in a device resource 
	// called a vertex buffer. Vertex buffers are blocks of memory allocated by the device that
	// we use to store vertex data.
    for ( ULONG i = 0; i < 6; ++i )
    {
		// Set the texture for this primitive
        d3ddev->SetTexture( 0, skyTextures[i] );

		// Render the face (one strip per face from the vertex buffer)  There are 2 primitives per face.
        d3ddev->DrawPrimitive( D3DPT_TRIANGLESTRIP, i * 4, 2 );

    } // Next side

	d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, true );
	d3ddev->SetRenderState( D3DRS_LIGHTING, true );
    

    // ADDED BY ZACK
    
	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//alpha in material's diffuse component is for alpha
	d3ddev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	d3ddev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//set blending factors so that alpha component determines transparancy
	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    window->Render(helper->GetTime(), 0);
	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	room->Render(helper->GetTime(), 0);
	d3ddev->SetRenderState( D3DRS_SPECULARENABLE, true );
	floor->Render(helper->GetTime(), 0);
	d3ddev->SetRenderState( D3DRS_SPECULARENABLE, false );

	stencil->RenderShadow(cat2, D3DXVECTOR3(0, -1, 0), lightDir, helper);

	d3ddev->SetRenderState( D3DRS_SPECULARENABLE, true );
    cat2->Render(helper->GetTime(), 1);
	stencil->RenderReflection(cat2, floor, helper);
	
	d3ddev->SetRenderState( D3DRS_SPECULARENABLE, false );

	
    // END ADDED BY ZACK

	

    d3ddev->EndScene();    // ends the 3D scene

    d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
}


// this is the function that cleans up Direct3D and COM
void DirectXHelper::cleanD3D(void)
{
    d3ddev->Release(); // close and release the 3D device
    d3d->Release(); // close and release Direct3D
	//v_buffer->Release();    // close and release the vertex buffer
	//pixelShader->Release();
	delete camera;
	delete input;
	delete textbox;
	//delete p;
}

void ::DirectXHelper::init_graphics(void)
{

    // ADDED BY ZACK
    floor = new Model(d3d, d3ddev, "floor.x");
    window = new Model(d3d, d3ddev, "window.x");
    room = new Model(d3d, d3ddev, "room.x");
	cat2 = new AnimatedModel(d3d, d3ddev, "siamesweCatAnimated.x");
    //cat = new Model(d3d, d3ddev, "cat2.x");
	
    // END ADDED BY ZACK

	float size = 1000.0f;

	//initialize the skybox
	CUSTOMVERTEXNONORMAL skyboxMesh[24] =
	{
	// Front quad, NOTE: All quads face inward
	{-size, -size,  size,  0.0f, 1.0f },
	{-size,  size,  size,  0.0f, 0.0f },
	{ size, -size,  size,  1.0f, 1.0f },
	{ size,  size,  size,  1.0f, 0.0f },
	
	// Back quad
	{ size, -size, -size,  0.0f, 1.0f },
	{ size,  size, -size,  0.0f, 0.0f },
	{-size, -size, -size,  1.0f, 1.0f },
	{-size,  size, -size,  1.0f, 0.0f },
	
	// Left quad
	{-size, -size, -size,  0.0f, 1.0f },
	{-size,  size, -size,  0.0f, 0.0f },
	{-size, -size,  size,  1.0f, 1.0f },
	{-size,  size,  size,  1.0f, 0.0f },
	
	// Right quad
	{ size, -size,  size,  0.0f, 1.0f },
	{ size,  size,  size,  0.0f, 0.0f },
	{ size, -size, -size,  1.0f, 1.0f },
	{ size,  size, -size,  1.0f, 0.0f },

	// Top quad
	{-size,  size,  size,  0.0f, 1.0f },
	{-size,  size, -size,  0.0f, 0.0f },
	{ size,  size,  size,  1.0f, 1.0f },
	{ size,  size, -size,  1.0f, 0.0f },
	
	// Bottom quad
	{-size, -size, -size,  0.0f, 1.0f },
	{-size, -size,  size,  0.0f, 0.0f },
	{ size, -size, -size,  1.0f, 1.0f },
	{ size, -size,  size,  1.0f, 0.0f }
	};





    // create a vertex buffer interface called v_buffer
    d3ddev->CreateVertexBuffer(24*sizeof(CUSTOMVERTEXNONORMAL), 0, CUSTOMFVFNONORMAL, D3DPOOL_MANAGED, &v_buffer, NULL);

    VOID* pVoid;    // a void pointer

    // lock v_buffer and load the vertices into it
    v_buffer->Lock(0, sizeof(CUSTOMVERTEXNONORMAL) * 24, (void**)&pVoid, 0);
	 memcpy( pVoid, skyboxMesh, sizeof(CUSTOMVERTEXNONORMAL) * 24 );
    v_buffer->Unlock();

	// Load Textures.  I created a global array just to keep things simple.  The order of the images
	// is VERY important.  The reason is the skybox mesh (g_SkyboxMesh[]) array was created above
	// in this order. (ie. front, back, left, etc.)
    D3DXCreateTextureFromFile( d3ddev, ("Images/SkyBox_Front.png") , &skyTextures[0] );
    D3DXCreateTextureFromFile( d3ddev, ("Images/SkyBox_Back.png")  , &skyTextures[1] );
    D3DXCreateTextureFromFile( d3ddev, ("Images/SkyBox_Left.png")  , &skyTextures[2] );
    D3DXCreateTextureFromFile( d3ddev, ("Images/SkyBox_Right.png") , &skyTextures[3] );
    D3DXCreateTextureFromFile( d3ddev, ("Images/SkyBox_Top.png")   , &skyTextures[4] );
    D3DXCreateTextureFromFile( d3ddev, ("Images/SkyBox_Bottom.jpg"), &skyTextures[5] );
	
	lightDir  =D3DXVECTOR3(0.0f, -14.0f, 5.0f);

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_SPOT;
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Position = D3DXVECTOR3(0.0f, 14.0f, -5.0f);
	light.Direction = lightDir;
	light.Range = 40.0f;
	light.Falloff = 1.0f;
	light.Theta = 10.0f;
	light.Phi = 10.0f;

	d3ddev->SetLight(0, &light);
	d3ddev->LightEnable(0, true);

	/*D3DLIGHT9 light2;
	ZeroMemory(&light2, sizeof(light2));
	light2.Type = D3DLIGHT_DIRECTIONAL;
	light2.Diffuse = D3DXCOLOR(0.01f, 0.01f, 0.01f, 1.0f);
	light2.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light2.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	//light2.Position = D3DXVECTOR3(0.0f, 20.0f, -13.0f);
	light2.Direction = D3DXVECTOR3(0.0f, 1.0f, -4.0f);
	//light2.Range = 50.0f;
	//light2.Falloff = 1.0f;
	//light2.Theta = 1.0f;
	//light2.Phi = 2.0f;

	d3ddev->SetLight(1, &light2);
	//d3ddev->LightEnable(1, true);*/
}