/*********************************************************************************
    LbOSWin32Imp.h
    Header file for the LightBikes2001 OS layer for Win32

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black
       James Ross
       David Capps
       Chris Skepper

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
#ifndef __LBOSWIN32IMP__
#define __LBOSWIN32IMP__

class LbOSWin32Imp : public LbOSLayerSys
{
public:

/*
** LbOSLayerSys methods
*/

virtual bool PollEvent(LbOSLayerEvent &os_event);
virtual void SwapDoubleBuffers();
virtual int GLTextListBase();
virtual int GetMS();
virtual char* GetDesktop32();
virtual bool GetOSKey(LbOSLayerKeypress *data, int *num);
virtual bool SetupWinampCompatPlugins(WA_InputPtr *inp, WA_OutputPtr *outp);
virtual void InitiateNetwork() ;

/*
** LbOSWin32Imp methods
*/
LbOSWin32Imp();
~LbOSWin32Imp();

void Init();

private:

void CreateMainWindow();
void DestroyMainWindow();
void CreateOGLContext(HWND hwnd);
void SetupPixelFormat(HDC dc);
void SetupPalette(HDC dc);
void PerformResize();
void DestroyOGLContext();
void GetDesktopImage();
bool Init_DInput();
void Deinit_DInput();
void Deinit_WinampPlugins();

static LONG WINAPI MainWndProcRedir(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
LONG WINAPI MainWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

static LbOSWin32Imp *the_oslayer;

HINSTANCE hInstance, WinampIn, WinampOut;
HWND hwnd_main;
HDC hDC;
HGLRC hRC;
bool quit_flag;
int TextBase;
LARGE_INTEGER freq;
char *desktop;
LPDIRECTINPUT7          g_DI;
LPDIRECTINPUTDEVICE7    g_KDIDev;
BYTE                    olddiks[256]; //no, it's not rude! Old DInput KeyS
int TickStart,PerfStart;

LbNetSys * ns;

};

#endif
