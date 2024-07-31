#include <OGL3D/Window/OWindow.h>
#include <OGL3D/Game/OGame.h>
#include "glad/glad_wgl.h"
#include "glad/glad.h"
#include <Windows.h>
#include <assert.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		OWindow* window = (OWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		break;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return NULL;
}


OWindow::OWindow()
{
	WNDCLASSEX wc = {}; // Create as empty object
	// Set Fields
	wc.cbSize = sizeof(WNDCLASSEX); // Size is size of object
	wc.lpszClassName = L"OGL3DWindow"; // Name of object
	wc.lpfnWndProc = &WndProc; // Callback function
	wc.style = CS_OWNDC;


	auto classId = RegisterClassEx(&wc); // Register
	assert(classId); // Developer Assert

	RECT rc = { 0,0,1024,768 }; // Set size of window
	AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, false); // adjust window to account for borders and systems style

	// Create a window and save the window handle
	m_handle = CreateWindowEx(NULL, MAKEINTATOM(classId), L"OpenGL 3D Game", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, NULL, NULL);

	assert(m_handle); // assert creation

	SetWindowLongPtr((HWND)m_handle, GWLP_USERDATA, (LONG_PTR)this); // Associate window with this object

	ShowWindow((HWND)m_handle, SW_SHOW); // Make window start as shown
	UpdateWindow((HWND)m_handle); // update window state


	// Create OpenGL Render Context

	auto hDC = GetDC(HWND(m_handle)); // Get descriptor from handle
	
	int pixelFormatAttributes[] = { // Flags -- See graphicsEngine
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0

	};

	int pixelFormat = 0;
	UINT numFormats = 0;
	wglChoosePixelFormatARB(hDC, pixelFormatAttributes,nullptr,1,&pixelFormat,&numFormats);
	assert(numFormats);

	PIXELFORMATDESCRIPTOR pixelFormatDesc = {};

	DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDesc); // Convert into x32 compatible pixel format
	SetPixelFormat(hDC, pixelFormat, &pixelFormatDesc);

	int openGLAttributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	m_context = wglCreateContextAttribsARB(hDC, 0, openGLAttributes);
	assert(m_context);

}

OWindow::~OWindow()
{
	wglDeleteContext(HGLRC(m_context));
	DestroyWindow(HWND(m_handle)); // clean up window on program end
}

void OWindow::makeCurrentContext()
{
	wglMakeCurrent(GetDC(HWND(m_handle)), HGLRC(m_context));
}

void OWindow::present(bool vsync)
{
	wglSwapIntervalEXT(vsync);
	wglSwapLayerBuffers(GetDC(HWND(m_handle)),WGL_SWAP_MAIN_PLANE);
}
