/*********************************************************************************
    LbGraphics.cpp
    Class file for the LightBikes2001 Graphics subsystem

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
#include "LbGraphicsImp.h"
#include <GL/glu.h>

/*
** LbGraphicsSys methods
*/
void LbGraphicsImp::CreateGraphicsBike()
{
}

void LbGraphicsImp::CreateGraphicsLevel(int x,int y)
{
}

void LbGraphicsImp::SetCamera(const LbVector &pos,const LbVector &target,const LbVector &up)
{
}

void LbGraphicsImp::DrawText(float x,float y,const char *str)
{
}

void LbGraphicsImp::StartFrame()
{
	// Initalise the 'scene'...
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	// ***********************************************************************
	// Code is here for initial testing ONLY! REMOVE THIS CODE BEFORE RELEASE!
	// ***********************************************************************
	
	glLoadIdentity();
	glRotatef(((GLfloat)frameCount / 5), 0.0f, 0.0f, -1.0f);
	glTranslatef(-1.5f, 0.0f, -8.0f);
	glBegin(GL_TRIANGLES);
		glColor3f(0.0f, 0.4f, 0.8f);
		glVertex3f( 0.0f, 1.0f, 0.0f);
		glVertex3f( 1.0f,-1.0f, 0.0f);
		glVertex3f(-1.0f,-1.0f, 0.0f);
	glEnd();
	
	glLoadIdentity();
	glTranslatef(1.5f, 0.0f, -8.0f);
	glRotatef(((GLfloat)frameCount / 5), 1.0f, 0.0f, 0.0f);
	glRotatef(((GLfloat)frameCount / 7), 0.0f, 1.0f, 0.0f);
	glRotatef(((GLfloat)frameCount / 11), 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
		// TOP
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f( 1.0f, 1.0f,-1.0f);
		glVertex3f(-1.0f, 1.0f,-1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f( 1.0f, 1.0f, 1.0f);
		// BOTTOM
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f( 1.0f,-1.0f, 1.0f);
		glVertex3f(-1.0f,-1.0f, 1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glVertex3f( 1.0f,-1.0f,-1.0f);
		// FRONT
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f( 1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f,-1.0f, 1.0f);
		glVertex3f( 1.0f,-1.0f, 1.0f);
		// BACK
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f( 1.0f,-1.0f,-1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glVertex3f(-1.0f, 1.0f,-1.0f);
		glVertex3f( 1.0f, 1.0f,-1.0f);
		// LEFT
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f,-1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glVertex3f(-1.0f,-1.0f, 1.0f);
		// RIGHT
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f( 1.0f, 1.0f,-1.0f);
		glVertex3f( 1.0f, 1.0f, 1.0f);
		glVertex3f( 1.0f,-1.0f, 1.0f);
		glVertex3f( 1.0f,-1.0f,-1.0f);
	glEnd();
	
	// ***********************************************************************
	
	if (glGetError()) {
		MessageBox(NULL, "Error redering.", "LbGraphics::StartFrame()", MB_ICONSTOP);
	}
}

void LbGraphicsImp::EndFrame()
{
	frameCount++;
}


void LbGraphicsImp::Init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// Clear to black.
	glClearDepth(1.0);				// Enables clearing of depth buffer.
	glDepthFunc(GL_LESS);				// Depth test to use.
	glEnable(GL_DEPTH_TEST);			// Actually enables depth testing.
	glShadeModel(GL_SMOOTH);			// Enables smooth colouring.
	
	// Set up Projection...
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.0f, 0.1f, 100.0f);
	//gluLookAt(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
	
	if (glGetError()) {
		MessageBox(NULL, "Error initalising OpenGL.", "LbGraphics::Init()", MB_ICONSTOP);
	}
	frameCount = 0;
}

LbGraphicsImp::LbGraphicsImp()
{
}

LbGraphicsImp::~LbGraphicsImp()
{
}

LbGraphicsSys *CreateGraphicsSys(LbOSLayerSys *os_sys)
{
LbGraphicsImp *rval=new LbGraphicsImp;
assert(rval!=NULL);

rval->Init();

return rval;
}