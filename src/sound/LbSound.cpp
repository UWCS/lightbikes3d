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
    musicOK = true;
}

LbSoundImp::~LbSoundImp()
{
    char *image;
    while (!soundcache.empty()) {
        image = soundcache.front();
        soundcache.pop_front();
        free(image);
    }
}

void LbSoundImp::Init(LbOSLayerSys *os_sys) {
    os = os_sys;
    os_sound = os_sys->GetOSSound();
}

bool LbSoundImp::PlayMusicFile(char *fname) {
    if (!musicOK) return false;
    if (!StopMusic()) return false;
    return os_sound->PlayMusic(fname);
}

bool LbSoundImp::StopMusic() {
    if (!musicOK) return false;
    os_sound->StopMusic();
    return true;
}

int LbSoundImp::CacheWaveFile(char *fname) {
    ifstream fin(fname, ios::binary);
    fin.seekg(0, ios::end); // End of file
    int size = fin.tellg();
    if (size < 80) {
        fin.close();
        return -1;
    }
    fin.seekg(0, ios::beg); // End of file

    char *image = (char*)malloc (size);
    fin.read( image, size);
    fin.close();
    soundcache.push_back(image);
    return soundcache.size()-1;
}

bool LbSoundImp::PlayWaveFile(int cachenumber) {
    if ( (cachenumber < 0) || (cachenumber >= soundcache.size() ) )
        return false;
    char *image = soundcache[cachenumber];
    os_sound->PlayWave(image);
    return true;
}

LbSoundSys *CreateSoundSys(LbOSLayerSys *os_sys)
{
LbSoundImp *rval=new LbSoundImp;
assert(rval!=NULL);

rval->Init(os_sys);
return rval;
}
