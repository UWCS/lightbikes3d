/*********************************************************************************
    LbOSWin32Imp.cpp
    Class file for the LightBikes2001 Win32 OS Layer

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*********************************************************************************/
#include "LbStandard.h"
#include "LbPublic.h"
#include "LbOSWin32Imp.h"

LbOSWin32Imp *LbOSWin32Imp::the_oslayer=NULL;

/*
** LbOSLayerSys methods
*/
bool LbOSWin32Imp::PollEvent(LbOSLayerEvent &os_event)
{
MSG msg;

// dispatch any windows messages
while(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
	{
	if(GetMessage(&msg,NULL,0,0)>0)
		{
		assert(msg.message!=WM_CLOSE);

		DispatchMessage(&msg);// send to window proc
		}
	}

/*
** TODO: we should impliment a queue of LB events.
** window proc creates events, then we return 
** them(one by one).
*/
if(quit_flag)
	{
	quit_flag=false;// reset quit flag.
	
	os_event.id=LB_OSEVENT_QUIT;
	return true;
	}

return false;
}


/*
** LbOSWin32Imp methods
*/
void LbOSWin32Imp::Init()
{
hInstance=GetModuleHandle(NULL);

CreateMainWindow();
}

LbOSWin32Imp::LbOSWin32Imp()
{
// there can only be one...
assert(the_oslayer==NULL);

the_oslayer=this;

hwnd_main=NULL;
hInstance=NULL;
hwnd_main=NULL;
hDC=NULL;
hRC=NULL;
quit_flag=false;
}

LbOSWin32Imp::~LbOSWin32Imp()
{
DestroyMainWindow();

assert(the_oslayer==this);
the_oslayer=NULL;
}

void LbOSWin32Imp::CreateMainWindow()
{
DestroyMainWindow(); // be safe

WNDCLASS wnd_class;
/*
** Register the window class
*/
wnd_class.style=0;
wnd_class.lpfnWndProc=(WNDPROC)MainWndProcRedir;
wnd_class.cbClsExtra=0;
wnd_class.cbWndExtra=0;
wnd_class.hInstance=hInstance;
wnd_class.hIcon=LoadIcon(NULL,IDI_APPLICATION);
wnd_class.hCursor=LoadCursor (NULL,IDC_ARROW);
 // gray is better than black for debugging:-)
wnd_class.hbrBackground=(HBRUSH)GetStockObject(DKGRAY_BRUSH);
wnd_class.lpszMenuName=NULL;
wnd_class.lpszClassName="LightBikes3D WndClass";

BOOL rval=RegisterClass(&wnd_class);
assert(rval);

/*
** create the window
*/    /* Create the frame */
hwnd_main=CreateWindow("LightBikes3D WndClass",
							  "LightBikes",
							  WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | 
							  WS_CLIPCHILDREN,
							  CW_USEDEFAULT,CW_USEDEFAULT,
							  640,480,
							  NULL,NULL,
							  hInstance,
							  NULL);
assert(hwnd_main!=NULL);
	
ShowWindow(hwnd_main,SW_SHOWNORMAL);
UpdateWindow (hwnd_main);
}

void LbOSWin32Imp::DestroyMainWindow()
{
if(hwnd_main!=NULL)
	DestroyWindow(hwnd_main);

// pump any remaining messages from the message que.
LbOSLayerEvent dummy_event;
while(PollEvent(dummy_event));
}

LONG WINAPI LbOSWin32Imp::MainWndProcRedir(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
return the_oslayer->MainWndProc(hwnd,uMsg,wParam,lParam);
}

LONG WINAPI LbOSWin32Imp::MainWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
switch(uMsg)
	{
	case WM_CREATE:
		hwnd_main=hwnd;

		CreateOGLContext(hwnd);
	break; 
	
	case WM_SIZE:
		PerformResize();
	break;
	
	case WM_CLOSE:
		quit_flag=true;
	break;

	case WM_DESTROY:
		DestroyOGLContext();
		PostQuitMessage(0);
		hwnd_main=NULL;
	break;
	}

return DefWindowProc (hwnd,uMsg,wParam,lParam); 
}

void LbOSWin32Imp::CreateOGLContext(HWND hwnd)
{
DestroyOGLContext();

// create device context
hDC=GetDC(hwnd);
assert(hDC!=NULL);

// setup pixel format

SetupPixelFormat(hDC);

// create and make current rendering context
hRC=wglCreateContext(hDC);
assert(hRC!=NULL);

BOOL rval=wglMakeCurrent(hDC,hRC);
assert(rval);
}

void LbOSWin32Imp::SetupPixelFormat(HDC dc)
{
PIXELFORMATDESCRIPTOR pfd;

pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);
pfd.nVersion=1;
pfd.dwFlags=PFD_DRAW_TO_WINDOW |
				PFD_SUPPORT_OPENGL |
				PFD_DOUBLEBUFFER;
pfd.iPixelType=PFD_TYPE_RGBA;
pfd.cColorBits=24;
pfd.cRedBits=0; 
pfd.cRedShift=0;
pfd.cGreenBits=0; 
pfd.cGreenShift=0; 
pfd.cBlueBits=0; 
pfd.cBlueShift=0; 
pfd.cAlphaBits=0; 
pfd.cAlphaShift=0; 
pfd.cAccumBits=0; 
pfd.cAccumRedBits=0; 
pfd.cAccumGreenBits=0; 
pfd.cAccumBlueBits=0; 
pfd.cAccumAlphaBits=0; 
pfd.cDepthBits=32;
pfd.cStencilBits=0;
pfd.cAuxBuffers=0; 
pfd.iLayerType=PFD_MAIN_PLANE;
pfd.bReserved=0; 
pfd.dwLayerMask=0; 
pfd.dwVisibleMask=0; 
pfd.dwDamageMask=0;

int pixel_fmt=ChoosePixelFormat(dc,&pfd);
assert(pixel_fmt!=0);

BOOL rval=SetPixelFormat(dc,pixel_fmt,&pfd);
assert(rval);

SetupPalette(dc);
}

void LbOSWin32Imp::SetupPalette(HDC dc)
{
PIXELFORMATDESCRIPTOR pfd;
int pixel_fmt;

pixel_fmt=GetPixelFormat(dc);
DescribePixelFormat(dc,pixel_fmt,sizeof(PIXELFORMATDESCRIPTOR),&pfd);

// we dont support palette mode, yet...
assert(!(pfd.dwFlags & PFD_NEED_PALETTE));
}

void LbOSWin32Imp::PerformResize()
{
if(hRC==NULL)
	return;

RECT client_rect;
GetClientRect(hwnd_main,&client_rect);

glViewport(0,0,client_rect.right,client_rect.bottom);

InvalidateRect(hwnd_main,NULL,TRUE); 
}

void LbOSWin32Imp::DestroyOGLContext()
{
if(hDC!=NULL)
	{
	assert(hwnd_main!=NULL);
	ReleaseDC(hwnd_main,hDC);
	hDC=NULL;
	}
if(hRC!=NULL)
	{
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(hRC);
	hRC=NULL;
	}
}


LbOSLayerSys *CreateOSLayerSys()
{
LbOSWin32Imp *rval=new LbOSWin32Imp;
assert(rval!=NULL);

rval->Init();

return rval;
}