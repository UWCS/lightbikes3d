/*********************************************************************************
    LbGame.cpp
    Class file for the LightBikes2001 blocks.

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black
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
** LbBaseBlock methods
*/
void LbBaseBlockImp::GetColor(LbRGBAColor &col)
{
}

void LbBaseBlockImp::SetColor(const LbRGBAColor &col)
{
}

/*
** LbBaseBlockImp methods
*/

LbBaseBlockImp::LbBaseBlockImp()
{
}

LbBaseBlockImp::~LbBaseBlockImp()
{
}

/*
** LbEmptyBlock: LbBaseBlock methods
*/

void LbEmptyBlock::GetGeometrySize(int &num_points,int &num_normals)
{
}

void LbEmptyBlock::GetGeometry(LbVector *bpoints,int num_points,LbVector *bnormals,int num_normals)
{
}

LbCollideEvent LbEmptyBlock::Collide(LbBlockDirection &dir)
{
	return LB_PLAYER_OK;
}

float LbEmptyBlock::GetZInBlock(float &x, float &y)
{
	return 0.0;
}

/*
** LbEmptyBlock methods
*/

LbEmptyBlock::LbEmptyBlock()
{
}

LbEmptyBlock::~LbEmptyBlock()
{
}


/*
** LbFullBlock: LbBaseBlock methods
*/

void LbFullBlock::GetGeometrySize(int &num_points,int &num_normals)
{
}

void LbFullBlock::GetGeometry(LbVector *bpoints,int num_points,LbVector *bnormals,int num_normals)
{
}

LbCollideEvent LbFullBlock::Collide(LbBlockDirection &dir)
{
	return LB_PLAYER_DIE;
}

float LbFullBlock::GetZInBlock(float &x, float &y)
{
	return 1.0;
}

/*
** LbFullBlock methods
*/

LbFullBlock::LbFullBlock()
{
}

LbFullBlock::~LbFullBlock()
{
}

/*
** LbRampBlock: LbBaseBlock methods
*/

void LbRampBlock::GetGeometrySize(int &num_points,int &num_normals)
{
}

void LbRampBlock::GetGeometry(LbVector *bpoints,int num_points,LbVector *bnormals,int num_normals)
{
}

LbCollideEvent LbRampBlock::Collide(LbBlockDirection &dir)
{
	if (block_dir==dir)
		return LB_PLAYER_OK;
	else
		return LB_PLAYER_DIE;
}

float LbRampBlock::GetZInBlock(float &x, float &y)
{
	//I am assuming that +y = N, +x = E
	if (block_dir==LB_DIR_N)
	{
		return y;
	}
	else if (block_dir==LB_DIR_S)
	{
		return 1-y;
	}
	else if (block_dir==LB_DIR_E)
	{
		return x;
	}
	else if (block_dir==LB_DIR_W)
	{
		return 1-x;
	}

	//erm...
	return 0.0;
}

/*
** LbRampBlock methods
*/

LbRampBlock::LbRampBlock(LbBlockDirection dir)
{
block_dir=dir;
}

LbRampBlock::~LbRampBlock()
{
}
