/*********************************************************************************
    LbGraphicsBike.cpp
    Implimentation file for the bike renderer.

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
#include "LbGraphicsImp.h"

/*
** LbGraphicsBike methods
*/

void LbGraphicsBikeImp::SetColor(const LbRGBAColor &new_col)
{
}

void LbGraphicsBikeImp::SetTexture(const char *tex_name)
{
}

void LbGraphicsBikeImp::AddSegment(const LbVector &pt)
{
    int n = lbtrail.size();
    lbtrail.resize(n + 1);
    lbtrail[n] = pt;
}

void LbGraphicsBikeImp::DrawSegment(const LbVector &start_pt,const LbVector &end_pt)
{
    // OpenGL code to render a single segment of trail...
    glBegin(GL_QUADS);
        glColor4f(0.2f, 0.2f, 0.9f, 0.2f);
        glVertex3f( start_pt.getX(), start_pt.getZ()    , -start_pt.getY() );
        glVertex3f( start_pt.getX(), start_pt.getZ() + 1, -start_pt.getY() );
        glVertex3f(   end_pt.getX(),   end_pt.getZ() + 1,   -end_pt.getY() );
        glVertex3f(   end_pt.getX(),   end_pt.getZ()    ,   -end_pt.getY() );
        glVertex3f( start_pt.getX(), start_pt.getZ() + 1, -start_pt.getY() );
        glVertex3f( start_pt.getX(), start_pt.getZ()    , -start_pt.getY() );
        glVertex3f(   end_pt.getX(),   end_pt.getZ()    ,   -end_pt.getY() );
        glVertex3f(   end_pt.getX(),   end_pt.getZ() + 1,   -end_pt.getY() );
    glEnd();
}

void LbGraphicsBikeImp::DrawTrail()
{
    for (int i = 1; i < lbtrail.size(); i++) {
        // Render single section
        // (from i-1 to i)
        DrawSegment(lbtrail[i - 1], lbtrail[i]);
    }
}

// DEBUG
LbVector LbGraphicsBikeImp::GetLastSegment()
{
    if (lbtrail.size() > 0) {
        return lbtrail[lbtrail.size() - 1];
    } else {
        return LbVector(0, 0, 0);
    }
}
// END

/*
** LbGraphicsBikeImp methods
*/
LbGraphicsBikeImp::LbGraphicsBikeImp()
{
}

LbGraphicsBikeImp::~LbGraphicsBikeImp()
{
}
