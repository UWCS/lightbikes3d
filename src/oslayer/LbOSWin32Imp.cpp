/*********************************************************************************
    LbOSWin32Imp.cpp
    Class file for the LightBikes2001 Win32 OS Layer

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
#include <dinput.h>
#include "LbStandard.h"
#include "LbPublic.h"
#include "LbOSWin32Imp.h"

#define DINPUT_BUFFERSIZE 32; //size of buffer for DirectInput
#define WM_USER_TCP_EVENT WM_USER+1

const int KeyMapping[] = { DIK_LEFT, DIK_RIGHT };
const int NumKeys = 2;

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

void LbOSWin32Imp::SwapDoubleBuffers()
{
    SwapBuffers(hDC);
}

int LbOSWin32Imp::GLTextListBase()
{
    return TextBase;
}

int LbOSWin32Imp::GetMS()
{
    LARGE_INTEGER rslt;
    QueryPerformanceCounter(&rslt);
    return (int)( 1000 * rslt.QuadPart / freq.QuadPart) - PerfStart;
}

char* LbOSWin32Imp::GetDesktop32()
{
    char *rslt = (char*)malloc(1024 * 512 * 4);
    memcpy(rslt, desktop, 1024*512*4);
    return rslt;
}

void LbOSWin32Imp::GetDesktopImage()
{
    HWND dWnd = GetDesktopWindow();
    HDC dDC = GetDC(0);
    //for the moment, just get 640x480
    desktop = (char*)malloc(1024 * 512 * 4);
    int *pixel = (int*)desktop;

    for (int y=0; y < 480; y++) {
        for (int x=0; x < 640; x++) {
            *pixel = GetPixel(dDC,x,y);
            (*pixel) |= 0xff000000;
            pixel++;
        }
        pixel += (1024-640);
    }
}

/*
** LbOSWin32Imp methods
*/
void LbOSWin32Imp::Init()
{
    LARGE_INTEGER rslt;

    hInstance=GetModuleHandle(NULL);
    TextBase=-1;
    GetDesktopImage();
    CreateMainWindow();

    QueryPerformanceFrequency(&freq);   //DC: Get ready to use hi-performance counter
    QueryPerformanceCounter(&rslt);
    TickStart = GetTickCount();
    PerfStart = (int)( 1000 * rslt.QuadPart / freq.QuadPart);

    Init_DInput();
    WinampIn = WinampOut = 0;

    ShowWindow(hwnd_main,SW_SHOWNORMAL); //Finally, once init is done
    UpdateWindow (hwnd_main);            //show the window


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
Deinit_DInput();
Deinit_WinampPlugins();
free(desktop);
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
//                            WS_OVERLAPPEDWINDOW |
                              WS_POPUP |
                              WS_CLIPSIBLINGS |
                              WS_VISIBLE |
                              WS_CLIPCHILDREN,
//                            CW_USEDEFAULT,CW_USEDEFAULT,
                              0,0,
                              640,480,
                              NULL,NULL,
                              hInstance,
                              NULL);
assert(hwnd_main!=NULL);

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

    TextBase = glGenLists(256); //init for standard ASCII text
    wglUseFontBitmaps(hDC,0,255,TextBase);
    //actually send the font bitmaps to OGL
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
pfd.cColorBits=16;
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
pfd.cDepthBits=16;
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
GetWindowRect(hwnd_main,&client_rect);

glViewport(0,0,client_rect.right,client_rect.bottom);

InvalidateRect(hwnd_main,NULL,TRUE);
}

bool LbOSWin32Imp::Init_DInput() {

    g_DI = NULL;
    g_KDIDev = NULL;

    CoInitialize(NULL);

    HRESULT hr = CoCreateInstance(  CLSID_DirectInput,
                                    NULL,
                                    CLSCTX_INPROC_SERVER,
                                    IID_IDirectInput7,
                                    (void**)&g_DI );
    if (!SUCCEEDED(hr)) return false;
    g_DI->AddRef();
    g_DI->Initialize( hInstance, DIRECTINPUT_VERSION );


/*  if ( DirectInputCreateEx(   hInstance,
                                DIRECTINPUT_VERSION,
                                IID_IDirectInput7,
                                (void**)&g_DI,
                                NULL ) ) return false;
    //DirectInputCreateEx is what we SHOULD use, not CoCreateInstance...
    //...but not if we're using crappy Borland DirectX libraries...
    //...well, compatibility is high with this method...
*/


    if ( g_DI->CreateDeviceEx(  GUID_SysKeyboard,
                                IID_IDirectInputDevice7,
                                (void**)&g_KDIDev,
                                NULL ) ) return false;
    if ( g_KDIDev->SetDataFormat(&c_dfDIKeyboard) ) return false;
    if ( g_KDIDev->SetCooperativeLevel( hwnd_main, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) return false;

    if ( g_KDIDev)
        g_KDIDev->Acquire();
    else
        return false;

    DIPROPDWORD dipdw;

    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = DINPUT_BUFFERSIZE; //keyboard buffer size

    MessageBox(NULL, "Please wait...", "LbOSWin32Imp::GetOSKey()", MB_ICONSTOP);
    //DirectInput doesn't work unless I put the above messagebox in.
    //Can SOMEBODY who understands DirectInput please figure out what
    //the hell is going on?
    hr = g_KDIDev->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );

    if (!SUCCEEDED(hr)) return false;

    return true;
}

bool LbOSWin32Imp::GetOSKey(LbOSLayerKeypress *data, int *num) {


    DIDEVICEOBJECTDATA didod[32];
    DWORD dwElements;

    dwElements = DINPUT_BUFFERSIZE;

    if (!g_KDIDev)
    return false;

    HRESULT hr;

    hr = DIERR_INPUTLOST;

    while ( hr == DIERR_INPUTLOST ) {
        hr= g_KDIDev->GetDeviceData(    sizeof(DIDEVICEOBJECTDATA),
                                                &didod[0],
                                                &dwElements,
                                                0 );

        if ( hr == DIERR_INPUTLOST ) {
            hr = g_KDIDev->Acquire();
        }
    }

//    if ( FAILED(hr) ) return false;

    if (SUCCEEDED(hr)) {
        if (*num < dwElements) {

        char msg[32];
        sprintf(msg,"Failed buffer oversize: num %d, received %d",*num, dwElements);
        MessageBox(NULL, msg, "LbOSWin32Imp::GetOSKey()", MB_ICONSTOP);
        return false;

        }

        *num = 0;
        for (int i=0; i<dwElements; i++) {
            for (int j=0; j<NumKeys; j++) {
                if ( didod[i].dwOfs == KeyMapping[j] ) {
                    data[*num].which = (LbOSLayerKey)j;
                    data[*num].time = didod[i].dwTimeStamp - TickStart;
                    data[*num].sequence = didod[i].dwSequence;
                    data[*num].down = (didod[i].dwData & 0x80);
                    (*num)++;
                }
            }
        }
    } else {
        char msg[32];
        sprintf(msg,"Failed buffer get on %d",hr);
//        MessageBox(NULL, msg, "LbOSWin32Imp::GetOSKey()", MB_ICONSTOP);
        return false;
    }
//      The above code implements buffered input key reading. This is
//      far preferable - it'll pick up a keystroke and store it for you
//      if you miss a poll for a while. More importantly, it timestamps
//      the key presses. But I haven't got it working yet. Bleh.


/*
    BYTE diks[256];
    HRESULT hr;

    hr = DIERR_INPUTLOST;

    while ( hr == DIERR_INPUTLOST ) {
        hr = g_KDIDev->GetDeviceState( sizeof(diks), &diks );

        if ( hr == DIERR_INPUTLOST ) {
            hr = g_KDIDev->Acquire();
        }
    }

    if ( FAILED(hr) ) return false;
    *num = 0;
    if ( (diks[DIK_LEFT] & 0x80) && !(olddiks[DIK_LEFT] & 0x80)) {
        data[*num].which = LB_OSKEY_LEFT;
        data[*num].time = 0;
        data[*num].sequence = 0;
        (*num)++;
    }
    if ( (diks[DIK_RIGHT] & 0x80) && !(olddiks[DIK_RIGHT] & 0x80)) {
        data[*num].which = LB_OSKEY_RIGHT;
        data[*num].time = 0;
        data[*num].sequence = 0;
        (*num)++;
    }
    memcpy(olddiks, diks, 256);
*/
return true;

}

void LbOSWin32Imp::Deinit_DInput() {

    if (g_DI) {
        if (g_KDIDev) {
            g_KDIDev->Unacquire();
            g_KDIDev->Release();
            g_KDIDev = NULL;
        }

        g_DI->Release();
        g_DI = NULL;
    }

    CoUninitialize();
}

//Start of crappy useless Winamp functions. Yay!

void WA_SetInfo1(int bitrate, int srate, int stereo, int synched) { }
int WA_Dsp_IsActive1() { return 0; }
int WA_Dsp_DoSamples1(short *samples, int numsamples, int bps, int nch, int srate) { return numsamples; }
void WA_SA_VSA_Init1(int maxlatency, int srate) { }
void WA_SA_VSA_DeInit1() { }
void WA_SA_AddPCMData1(void *data, int nch, int bps, int timestamp) { }
int WA_SA_GetMode1() { return 1; }
void WA_SA_Add1(void *data, int timestamp, int csa) { }
void WA_VSA_AddPCMData1(void *data, int nch, int bps, int timestamp) { }
int WA_VSA_GetMode1(int *specnch, int *wavench) { return 0; }
void WA_VSA_Add1(void *data, int timestamp) { }
void WA_VSA_SetInfo1(int nch, int srate) { }

//End of crappy useless Winamp functions

bool LbOSWin32Imp::SetupWinampCompatPlugins(WA_InputPtr *inp, WA_OutputPtr *outp) {

    Winamp_Input_Module* __stdcall (* GetInModule ) (void);
    Winamp_Output_Module* __stdcall (* GetOutModule) (void);

    WinampIn = LoadLibrary("MUSICIN.DLL");
    if (!WinampIn) return false;

    GetInModule = ( Winamp_Input_Module* __stdcall (*)(void) )GetProcAddress(WinampIn, "winampGetInModule2");
    if (!GetInModule) return false;
    *inp = GetInModule();

    WinampOut = LoadLibrary("MUSICOUT.DLL");
    if (!WinampOut) return false ;

    GetOutModule = (Winamp_Output_Module* __stdcall (*)(void) )GetProcAddress(WinampOut, "winampGetOutModule");
    if (!GetOutModule) return false;
    *outp = GetOutModule();

    (*inp)->hMainWindow = hwnd_main;
    (*inp)->hDllInstance = hInstance;
    (*inp)->outMod = *outp;
    (*inp)->SetInfo = WA_SetInfo1;
    (*inp)->dsp_isactive = WA_Dsp_IsActive1;
    (*inp)->dsp_dosamples = WA_Dsp_DoSamples1;
    (*inp)->SAVSAInit = WA_SA_VSA_Init1;
    (*inp)->SAVSADeInit = WA_SA_VSA_DeInit1;
    (*inp)->SAAddPCMData = WA_SA_AddPCMData1;
    (*inp)->SAGetMode = WA_SA_GetMode1;
    (*inp)->SAAdd = WA_SA_Add1;
    (*inp)->VSASetInfo = WA_VSA_SetInfo1;
    (*inp)->VSAAddPCMData = WA_VSA_AddPCMData1;
    (*inp)->VSAGetMode = WA_VSA_GetMode1;
    (*inp)->VSAAdd = WA_VSA_Add1;
    (*inp)->Init();

    (*outp)->hMainWindow = hwnd_main;
    (*outp)->hDllInstance = hInstance;
    (*outp)->Init();
    (*outp)->SetVolume(255);    //max volume
    (*outp)->SetPan(0);         //centre pan

    (*inp)->outMod = *outp;
    return true;
}

void LbOSWin32Imp::Deinit_WinampPlugins() {
    if (WinampIn) FreeLibrary(WinampIn);
    if (WinampOut) FreeLibrary(WinampOut);
    WinampIn = WinampOut = 0;
}

void LbOSWin32Imp::DestroyOGLContext()
{
    if(TextBase!=-1) {
        glDeleteLists(TextBase,256);
        TextBase=-1;
    }

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

/*
** Set up the networking API.  In this case it's Berkeley sockets for
** Windows which is called WINSOCK version 2.
*/
void LbOSWin32Imp::InitiateNetwork()
{
    // This structure can be used to get information about the version of
    // winsock in use.
    WSAData stWSAData;

    // Startup version 2.0 of Winsock, report errors.
    if ( WSAStartup ( MAKEWORD ( 2 , 0 ) , &stWSAData ) != 0 )
        MessageBox( NULL , "An error occured while initialising WinSock." ,
                    "Error" , MB_ICONSTOP ) ;
}


LbOSLayerSys *CreateOSLayerSys()
{
    LbOSWin32Imp *rval=new LbOSWin32Imp;
    assert(rval!=NULL);

    rval->Init();

    return rval;
}
