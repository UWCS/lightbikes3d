/*********************************************************************************
    LbGame.cpp
    Class file for the LightBikes2001 Game subsystem

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black
       James Ross

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
	os_sys=NULL;
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
	bool quit_flag;
	LbOSLayerEvent os_event;
	
	InitSubsystems();
	
	quit_flag=false;
	
	while(!quit_flag)
	{
		graph_sys->StartFrame();
		// draw here
		graph_sys->EndFrame();
		os_sys->SwapDoubleBuffers();
		
		// poll the event queue.
		while(os_sys->PollEvent(os_event))
		{
			/* OS event:
			**	Minimise/retstore
			**	Active/deactivate
			**	Close
			**	Normal keypress (for menus, etc.)
			**	Return: Send player quip
			*/
			// handle the event.
			switch(os_event.id)
			{
				case LB_OSEVENT_QUIT:
					quit_flag=true;// bye bye...
					break;
			}
			// ignore unknown events...
		}
		// Read in Network messages (Networking)...
			/* Messages from network:
			**	Start game
			**	Player position updates
			**	End game
			**	Send level data
			*/
		// Update game state (Game Logic)...
			/* Updateing actions:
			**	Poll for Input events
			**	Interpret player input
			**	Proccess Network messages
			**	Updating bikes
			**	Check for collisions
			**	MENU (stuff)
			**		Interface?
			**		Messages --> Network
			*/
		// Send Network messages (Networking)...
	}
	
	DeInitSubsystems();
	
	return 0;
}

void LbGameImp::InitSubsystems()
{
	os_sys=CreateOSLayerSys();
	
	graph_sys=CreateGraphicsSys(os_sys);
	input_sys=CreateInputSys(os_sys);
	sound_sys=CreateSoundSys(os_sys);
	net_sys=CreateNetSys(os_sys);
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
	
	if(os_sys!=NULL)
	{
		delete os_sys;
		os_sys=NULL;
	}
};

LbGameSys *CreateGameSys()
{
	LbGameSys *rval=new LbGameImp;
	assert(rval!=NULL);
	
	return rval;
}