/*********************************************************************************

    LbOSLinuxImp.h
    Header file for the LightBikes2001 OS layer for Linux

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black
       James Ross
       David Capps
       Chris Skepper
       Henry Southgate

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

#ifndef __LBOSLINUXIMP__

#define __LBOSLINUXIMP__


#include <SDL.h>

#ifdef WIN32
#include "winamp_in.h"
#include "winamp_out.h"
#endif



using namespace std;



class LbOSLayerSoundImp : public LbOSLayerSound
{

public:

virtual void Init(HINSTANCE hInst,HWND hWnd, int TickBegan);
virtual bool PlayMusic(char *fname);
virtual void StopMusic();
virtual void PauseMusic();
virtual void SetMusicVolume(int level);
virtual void SetMusicPan(int slope);
virtual void PlayWave(char *image);

LbOSLayerSoundImp();

~LbOSLayerSoundImp();

private:

virtual bool InitPlugins(HINSTANCE hInst, HWND hWnd);

Winamp_Input_Module *plugin_in;
Winamp_Output_Module *plugin_out;
bool init_ok;
HINSTANCE WinampIn, WinampOut;

};



class LbOSLayerInputImp : public LbOSLayerInput
{
  
 public:
  virtual bool GetOSKey(LbOSLayerKeypress *data, int *num);
  //I'm intending to mean fast accurate (eg. DirectInput under Win32) here
  virtual char getNextTextKey ( ) ;
  virtual void Init(HINSTANCE hInst,HWND hWnd, int TickBegan);
  virtual void OnWmChar(char c);
  virtual bool IsTabDown ( ) ;
  virtual void OnWmKeyDown(int c) ;
  virtual void OnWmKeyUp(int c) ;

  LbOSLayerInputImp();
  ~LbOSLayerInputImp();

 private:
  bool tabdown ;
  LPDIRECTINPUT7  lpDInput;
  LPDIRECTINPUTDEVICE7 lpDIDevKeyb;
  
  bool di_InitOK;
  int TickStart;
  bool InitDInput(HINSTANCE hInst,HWND hWnd);

  deque<char> textkeybuffer ;
};


class LbOSLinuxImp : public LbOSLayerSys
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
  virtual void InitiateNetwork() ;

  virtual LbOSLayerInput *GetOSInput() {
    return os_input;
  }

  virtual LbOSLayerSound *GetOSSound() {
    return os_sound;
  }


  /*
  ** LbOSLinuxImp methods
  */
  
  LbOSLinuxImp();
  ~LbOSLinuxImp();
  
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
  
  static LONG WINAPI MainWndProcRedir(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
  LONG WINAPI MainWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
  static LbOSLinuxImp *the_oslayer;
  
  HINSTANCE hInstance;
  HWND hwnd_main;
  HDC hDC;
  HGLRC hRC;
  
  
  bool quit_flag;
  int TextBase;
  LARGE_INTEGER freq;
  char *desktop;
  
  
  int TickStart,PerfStart;
  
  LbOSLayerInputImp *os_input;
  LbOSLayerSoundImp *os_sound;

};
#endif

