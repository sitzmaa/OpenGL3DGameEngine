#include "OGL3D/Graphics/OGraphicsEngine.h"
#include "glad/glad_wgl.h"
#include "glad/glad.h"
#include "assert.h"
#include "stdexcept"
// Constructor
/*
* POSTCONDITION: GLAD is loaded properly
* FAILURE: on improper load
*/
OGraphicsEngine::OGraphicsEngine()
{

	WNDCLASSEX wc = {}; // Initialize as empty
	// Fill in fields
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = L"OGL3DDummyWindow";
	wc.lpfnWndProc = DefWindowProc; // Use default callback function
	wc.style = CS_OWNDC;

	// Register class and assert success
	auto classId = RegisterClassEx(&wc);
	assert(classId);

	// Create dummy window with default values
	auto dummyWindow = CreateWindowEx(NULL, MAKEINTATOM(classId), L"", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);

	// Assert success
	assert(dummyWindow);

	// Create a descriptor
	auto dummyDC = GetDC(dummyWindow); // get descriptor from window
	PIXELFORMATDESCRIPTOR pixelFormatDesc = {}; // create empty format descriptor
	pixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR); // size
	pixelFormatDesc.nVersion = 1; // version
	pixelFormatDesc.iPixelType = PFD_TYPE_RGBA; // Type
	pixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // set flags for buffer -> support drawing by openGL, buffer is allowed to draw to window, double buffered
	pixelFormatDesc.cColorBits = 32; // number of color bit planes in each color buffer
	pixelFormatDesc.cAlphaBits = 8; // alpha bitplanes
	pixelFormatDesc.cDepthBits = 24; // z-axis buffer
	pixelFormatDesc.cStencilBits = 8; // stencil buffer
	pixelFormatDesc.iLayerType = PFD_MAIN_PLANE; // legacy

	auto pixelFormat = ChoosePixelFormat(dummyDC, &pixelFormatDesc); // assign the pixel format from the descriptor
	SetPixelFormat(dummyDC, pixelFormat, &pixelFormatDesc); // set it

	auto dummyContext = wglCreateContext(dummyDC); // create a context
	assert(dummyContext); // assert success

	wglMakeCurrent(dummyDC, dummyContext); // associate the context w/ descriptor

	if (!gladLoadWGL(dummyDC)) // GLAD load descriptor
		OGL3D_ERROR("OGraphicsEngine - Error - gladLoadWGL failed");

	if (!gladLoadGL()) // GLAD load
		OGL3D_ERROR("OGraphicsEngine - Error - gladLoadGL failed");
	// Trhow on failure

	wglMakeCurrent(dummyDC, 0); // unlink context
	wglDeleteContext(dummyContext); // destroy context
	ReleaseDC(dummyWindow, dummyDC); // release descriptor from window
	DestroyWindow(dummyWindow); // destroy window
}

OGraphicsEngine::~OGraphicsEngine()
{



}


