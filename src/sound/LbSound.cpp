/*********************************************************************************
    LbSound.cpp
    Class file for the LightBikes2001 Sound subsystem

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black
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
#include "LbSoundImp.h"


LbSoundImp::LbSoundImp()
{
    musicIn = 0;
    musicOut = 0;
    musicOK = false;
}

LbSoundImp::~LbSoundImp()
{
    DeinitMusic();
}

void LbSoundImp::Init(LbOSLayerSys *os_sys) {
    os = os_sys;
    InitMusic();
}

bool LbSoundImp::PlayMusicFile(char *fname) {
    if (!musicOK) return false;
    if (!StopMusic()) return false;
    return (musicIn->Play(fname) == 0);
}

bool LbSoundImp::StopMusic() {
    if (!musicOK) return false;
    musicIn->Stop();
    return true;
}

void LbSoundImp::InitMusic() {
    musicOK = os->SetupWinampCompatPlugins(&musicIn, &musicOut);
}

void LbSoundImp::DeinitMusic() {
    if (!musicOK) return;
    StopMusic();
    musicIn->Quit();
    musicOut->Quit();
    musicOK = false;
    musicIn = 0; musicOut = 0;
}

LbSoundSys *CreateSoundSys(LbOSLayerSys *os_sys)
{
LbSoundImp *rval=new LbSoundImp;
assert(rval!=NULL);

rval->Init(os_sys);
return rval;
}
