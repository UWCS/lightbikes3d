/*********************************************************************************
    LbGraphics.h
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
#ifndef __LBGRAPHICS__
#define __LBGRAPHICS__

class LbGraphicsBike
{
public:

/// Change color of lightbikes trail segments.
virtual void SetColor(const LbRGBAColor &new_col)=0;
/// Change texture of lightbikes trail segments.
virtual void SetTexture(const char *tex_name)=0;


/// Add a static trail segment, will be optimized for drawing.
virtual void AddSegment(const LbVector &pt)=0;

/// Draw a dynamic(could change due to network updates) trail segment.
virtual void DrawSegment(const LbVector &start_pt,const LbVector &end_pt)=0;
/// Draws the static segments which belong to the bike.
virtual void DrawTrail()=0;

/// empty virtual destructor to ensure proper cleanup
virtual ~LbGraphicsBike(){}
};

class LbGraphicsLevel
{
public:

virtual void SetBlock(LbBaseBlock *btype,int x,int y)=0;
virtual void DrawLevel(LbVector &offset)=0;

/// empty virtual destructor to ensure proper cleanup
virtual ~LbGraphicsLevel(){}
};

class LbGraphicsSys
{
public:

virtual void CreateGraphicsBike()=0;
virtual void CreateGraphicsLevel(int x,int y)=0;

virtual void SetCamera(const LbVector &pos,const LbVector &target,const LbVector &up)=0;

virtual void DrawText(float x,float y,const char *str)=0;

virtual void StartFrame()=0;
virtual void EndFrame()=0;
 
// empty virtual destructor to ensure proper cleanup
virtual ~LbGraphicsSys(){}
};

/// Create an implimentation of the graphics subsystem.
LbGraphicsSys *CreateGraphicsSys(LbOSLayerSys *os_sys);

#endif