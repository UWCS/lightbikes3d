/*********************************************************************************
    LbArena.cpp
    Class file for the LightBikes2001 Game subsystem

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       Chris Salmon
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

LbLevelImp LbArenaImp::full_level;

LbArenaImp::LbArenaImp(LbLevel ** levs, int numLevs)
{
	//Check that all levels have the same x and y sizes?
	/*if (numLevs > 0)
	{
		int x = levs[0].GetXSize();
		int y = levs[0].GetYSize();

		int i;
		for (i=1; i < numLevs; i++)
		{
			if (levs[i].GetXSize() != x ||
				levs[i].GetYSize() != y)
			{
				//Throw an exception?
			}
		}
	}*/

	levels = levs;
	numLevels = numLevs;
}

LbArenaImp::~LbArenaImp()
{
	delete[] levels;
}

LbLevel * LbArenaImp::GetLevel(int level)
{
	if (numLevels - 1 <= level)
	{
		return levels[level];
	}
        // cant use new, since caller does not know 
        // if to delete it. (use static object...) 
	else return &full_level;
}


