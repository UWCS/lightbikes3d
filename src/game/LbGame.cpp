/*********************************************************************************
    LbGame.cpp
    Class file for the LightBikes2001 Game subsystem

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
#include "LbGameImp.h"


LbGameImp::LbGameImp()
{
graph_sys=NULL;
input_sys=NULL;
sound_sys=NULL;
net_sys=NULL;
}

LbGameImp::~LbGameImp()
{
// make double sure things have been cleaned up.
DeInitSubsystems();
}


int LbGameImp::RunGame()
{
InitSubsystems();

/*
** put main loop here.
*/

DeInitSubsystems();

return 0;
}

void LbGameImp::InitSubsystems()
{
graph_sys=CreateGraphicsSys();
input_sys=CreateInputSys();
sound_sys=CreateSoundSys();
net_sys=CreateNetSys();
}

void LbGameImp::DeInitSubsystems()
{
if(graph_sys!=NULL)
	{
	delete graph_sys;
	graph_sys=NULL;
	}

if(input_sys!=NULL)
	{
	delete input_sys;
	input_sys=NULL;
	}

if(sound_sys!=NULL)
	{
	delete sound_sys;
	sound_sys=NULL;
	}

if(net_sys!=NULL)
	{
	delete net_sys;
	net_sys=NULL;
	}
};

LbGameSys *CreateGameSys()
{
LbGameSys *rval=new LbGameImp;
assert(rval!=NULL);

return rval;
}