#include "Includes.h"
#include "DirectXHelper.h"
//#include "Particles.h"

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc;
	DirectXHelper* helper = new DirectXHelper();
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
#if !FULLSCREEN
     wc.hbrBackground = (HBRUSH)COLOR_WINDOW;    
#endif
	wc.lpszClassName = "WindowClass";

    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL,
                          "WindowClass",
                          "Sleeping Cat",
#if FULLSCREEN
                          WS_EX_TOPMOST | WS_POPUP,    // fullscreen values
                          0, 0,    // the starting x and y positions should be 0
                          SCREEN_WIDTH, SCREEN_HEIGHT,    // set the window to 640 x 480
#else
						  WS_OVERLAPPEDWINDOW,
                          0, 0,
                          SCREEN_WIDTH, SCREEN_HEIGHT,
#endif
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hWnd, nCmdShow);

    // set up and initialize Direct3D
    helper->initD3D(hWnd, hInstance);
	//p.intBuffers(helper->d3ddev);
    // enter the main loop:

    MSG msg;

    while(TRUE)
    {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT)
            break;

        helper->renderFrame();
		//p.run_particles(helper->d3ddev);
		 if(KEY_DOWN(VK_ESCAPE))
            PostMessage(hWnd, WM_DESTROY, 0, 0);

    }

    // clean up DirectX and COM
    helper->cleanD3D();

	delete helper;
	
    return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;

    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}
