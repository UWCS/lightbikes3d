/*********************************************************************************

    LbOSWin32SoundImp.cpp

    Class file for the LightBikes2001 Win32 OS Sound Layer



    Copyright (C) 2000  University of Warwick Computing Society



    Contributors to this file:

       David Capps



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

#include "LbSound.h"

#include "LbOSWin32Imp.h"



void LbOSLayerSoundImp::Init(HINSTANCE hInst,HWND hWnd, int TickBegan)

{

    init_ok = InitPlugins(hInst, hWnd);

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



bool LbOSLayerSoundImp::InitPlugins(HINSTANCE hInst, HWND hWnd)

{

    Winamp_GetInModule GetInModule;

    Winamp_GetOutModule GetOutModule;



    WinampIn = LoadLibrary("MUSICIN.DLL");

    if (!WinampIn) return false;



    GetInModule = ( Winamp_GetInModule )GetProcAddress(WinampIn, "winampGetInModule2");

    if (!GetInModule) return false;

    plugin_in = GetInModule();



    WinampOut = LoadLibrary("MUSICOUT.DLL");

    if (!WinampOut) return false ;



    GetOutModule = (Winamp_GetOutModule )GetProcAddress(WinampOut, "winampGetOutModule");

    if (!GetOutModule) return false;

    plugin_out = GetOutModule();



    plugin_in->hMainWindow = hWnd;

    plugin_in->hDllInstance = hInst;

    plugin_in->outMod = plugin_out;

    plugin_in->SetInfo = WA_SetInfo1;

    plugin_in->dsp_isactive = WA_Dsp_IsActive1;

    plugin_in->dsp_dosamples = WA_Dsp_DoSamples1;

    plugin_in->SAVSAInit = WA_SA_VSA_Init1;

    plugin_in->SAVSADeInit = WA_SA_VSA_DeInit1;

    plugin_in->SAAddPCMData = WA_SA_AddPCMData1;

    plugin_in->SAGetMode = WA_SA_GetMode1;

    plugin_in->SAAdd = WA_SA_Add1;

    plugin_in->VSASetInfo = WA_VSA_SetInfo1;

    plugin_in->VSAAddPCMData = WA_VSA_AddPCMData1;

    plugin_in->VSAGetMode = WA_VSA_GetMode1;

    plugin_in->VSAAdd = WA_VSA_Add1;

    plugin_in->Init();



    plugin_out->hMainWindow = hWnd;

    plugin_out->hDllInstance = hInst;

    plugin_out->Init();

    plugin_out->SetVolume(255);    //max volume

    plugin_out->SetPan(0);         //centre pan



    plugin_in->outMod = plugin_out;

    return true;

}



void LbOSLayerSoundImp::PlayWave(char *image) {

    if (!sndPlaySound(image, SND_ASYNC | SND_NODEFAULT | SND_MEMORY )) {

        MessageBox( NULL , "Couldn't play sound, arrrrr." , "Error" , MB_ICONSTOP ) ;

    }

}



bool LbOSLayerSoundImp::PlayMusic(char *fname) {

    if (!init_ok) return false;



    if (plugin_in->IsPaused()) {

        plugin_in->UnPause();

        return true;

    } else {

        return (plugin_in->Play(fname))==0;

    }

}



void LbOSLayerSoundImp::StopMusic() {

    if (!init_ok) return;



    plugin_in->Stop();

}



void LbOSLayerSoundImp::PauseMusic() {

    if (!init_ok) return;



    if (plugin_in->IsPaused()) plugin_in->UnPause(); else plugin_in->Pause();

}



void LbOSLayerSoundImp::SetMusicVolume(int level) {

    if (!init_ok) return;



    plugin_in->SetVolume(level);

}



void LbOSLayerSoundImp::SetMusicPan(int slope) {

    if (!init_ok) return;



    plugin_in->SetPan(slope);

}



LbOSLayerSoundImp::LbOSLayerSoundImp()

{

    WinampIn = WinampOut = 0;



}



LbOSLayerSoundImp::~LbOSLayerSoundImp()

{

    if (WinampIn) FreeLibrary(WinampIn);

    if (WinampOut) FreeLibrary(WinampOut);

    WinampIn = WinampOut = 0;

}



