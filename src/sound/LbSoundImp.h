/*********************************************************************************
    LbSoundImp.h
    Header file for the LightBikes2001 Sound subsystem classes

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
#ifndef __LBSOUNDIMP__
#define __LBSOUNDIMP__

class LbSoundImp : public LbSoundSys
{
public:

LbSoundImp();
~LbSoundImp();

bool PlayMusicFile(char *fname);
bool StopMusic();


void Init(LbOSLayerSys *os_sys);
virtual int CacheWaveFile(char *fname);
virtual bool PlayWaveFile(int cachenumber);

private:

LbOSLayerSys *os;
LbOSLayerSound *os_sound;
bool musicOK; //is music inited and OK

deque<char*> soundcache ;
};

#endif
