/*********************************************************************************
    LbArena.cpp
    Class file for the LightBikes2001 Game subsystem

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       Chris Salmon

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
** Constructs an empty level
*/
LbLevelImp::LbLevelImp()
{
	xsize = ysize = 0;
}

/*
** Level which is x * y blocks big
*/
LbLevelImp::LbLevelImp(int x, int y)
{
	xsize = x;
	ysize = y;
	blocks = new LbBaseBlock*[xsize * ysize];
	for (int i=0; i < xsize * ysize; i++)
	{
		blocks[i]=new LbEmptyBlock();
		assert(blocks[i]!=NULL);
	}
	assert(blocks!=NULL);
}

LbLevelImp::~LbLevelImp()
{
	delete[] blocks;
}

void LbLevelImp::SetBlockAt(int x, int y, LbBaseBlock &block)
{
	//If in level
	if (x >= 0 && x < xsize &&
		y >= 0 && y < ysize)
	{
		blocks[(xsize * y) + x] = &block;
	}

	//If not in level, do nothing (throw exception?)
}

LbBaseBlock * LbLevelImp::GetBlockAt(int x, int y)
{
	if (xsize - 1 <= x
		&& ysize - 1 <= y)
		return blocks[(xsize * y) + x];
	else
	{
		LbBaseBlock * full = new LbFullBlock();
		assert(full!=NULL);
		return full;
	}
}

