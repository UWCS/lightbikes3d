/*********************************************************************************
    LbGraphicsBike.cpp
    Implimentation file for the bike renderer.

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
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
#include "LbStandard.h"
#include "LbPublic.h"
#include "LbGameImp.h"

/*
** LbPlayer methods
*/

// Get player name.
int LbPlayerImp::GetHash ( )
{
	return hash ;
}

// Get player handle.
string LbPlayerImp::GetHandle ( )
{
	return handle ;
}

// Get player kills.
int LbPlayerImp::GetKills ( )
{
	return kills ;
}

// Get player deaths.
int LbPlayerImp::GetDeaths ( )
{
	return deaths ;
}

// Get player ping.
int LbPlayerImp::GetPing ( )
{
	return ping ;
}

// Get player kills.
bool LbPlayerImp::IsValid ( )
{
	return valid ;
}

// Set player hash.
void LbPlayerImp::SetHash ( int h )
{
	hash = h ;
}

// Set player handle.
void LbPlayerImp::SetHandle ( string h)
{
	handle = h ;
}

// Set player kills.
void LbPlayerImp::SetKills ( int k )
{
	kills = k ;
}

// Set player deaths.
void LbPlayerImp::SetDeaths ( int d )
{
	deaths = d ;
}

// Set player ping.
void LbPlayerImp::SetPing ( int p )
{
	ping = p ;
}

// Set player valid.
void LbPlayerImp::SetValid ( bool v )
{
	valid = v ;
}

// Get the graphics bike.
LbGraphicsBike * LbPlayerImp::GetBike ( )
{
	return bike ;
}

// Get the position.
LbVector * LbPlayerImp::GetPosition ( )
{
	return bikepos ;
}

// Get the graphics bike.
void LbPlayerImp::SetBike ( LbGraphicsBike * g )
{
	bike = g ;
}

// Get the position.
void LbPlayerImp::SetPosition ( LbVector * v )
{
	bikepos = v ;
}

// Set player valid.
void LbPlayerImp::SetPlaying ( bool v )
{
	playing = v ;
}

// Get player kills.
bool LbPlayerImp::IsPlaying ( )
{
	return playing ;
}

// Set player the direction.
void LbPlayerImp::SetDirection ( int d )
{
	direction = d ;
}

// Get player direction.
int LbPlayerImp::GetDirection ( )
{
	return direction ;
}

/*
** LbGraphicsBikeImp methods
*/
LbPlayerImp::LbPlayerImp()
{
}

LbPlayerImp::~LbPlayerImp()
{
}
