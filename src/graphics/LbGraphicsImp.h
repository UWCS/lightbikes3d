/*********************************************************************************
    LbGraphicsImp.h
    Header file for the LightBikes2001 Graphics subsystem classes

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
#ifndef __LBGRAPHICSIMP__
#define __LBGRAPHICSIMP__

class LbGraphicsImp : public LbGraphicsSys
{
public:

/*
** LbGraphicsSys methods
*/
virtual void CreateGraphicsBike();
virtual void CreateGraphicsLevel(int x,int y);

virtual void SetCamera(const LbVector &pos,const LbVector &target,const LbVector &up);

virtual void DrawText(float x,float y,const char *str);

virtual void StartFrame();
virtual void EndFrame();

/*
** LbGraphicsImp methods
*/
LbGraphicsImp();
~LbGraphicsImp();

void Init();


private:
};



class LbGraphicsBikeImp : public LbGraphicsBike
{
public:

/*
** LbGraphicsBike methods
*/

virtual void SetColor(const LbRGBAColor &new_col);
virtual void SetTexture(const char *tex_name);

virtual void AddSegment(const LbVector &pt);

virtual void DrawSegment(const LbVector &start_pt,const LbVector &end_pt);
virtual void DrawTrail();

/*
** LbGraphicsBikeImp methods
*/
LbGraphicsBikeImp();
~LbGraphicsBikeImp();

private:
};

class LbGraphicsLevelImp : public LbGraphicsLevel
{
public:

/*
** LbGraphicsLevel methods
*/

virtual void SetBlock(LbBaseBlock *btype,int x,int y);
virtual void DrawLevel(LbVector &offset);

/*
** LbGraphicsLevelImp methods
*/
LbGraphicsLevelImp();
~LbGraphicsLevelImp();

private:
};

#endif