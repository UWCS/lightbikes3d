/*********************************************************************************
    LbGraphics.cpp
    Class file for the LightBikes2001 Graphics subsystem

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black
       James Ross
       David Capps

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

#define FADE_WHITE      //defining FADE_WHITE makes it fade to/from white
                        //rather than black

#define TEX_FORMAT  GL_RGB5_A1
                        //texture format to use. RGB5_A1 means 16 bit colour
                        //5 bits per colour, 1 alpha bit
                        //Alternatives could be:
                        //GL_RGBA8 (32-bit total: 8 bits per component)
                        //GL_RGBA4 (16-bit total: 4 bits per component)
                        //Note an alpha component is required for blending

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
    campos = pos);
    camtgt = target;
    camup = up;
}

void LbGraphicsImp::SetupOrtho()
{       //sets up a 'virtual screen' of size 640x480 for drawing onto
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,640,0,480,-0.1,1000);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void LbGraphicsImp::FinishOrtho()
{       //reverses the Ortho 'virtual screen' to its previous state
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

int LbGraphicsImp::LoadBMPTexture(char *fname, int transcolour)
{
    ifstream fin(fname, ios::binary);
    BITMAPFILEHEADER fhead;
    BITMAPINFOHEADER finfo;
    RGBQUAD palette[256];
    fin.read((char*) &fhead, sizeof(fhead) );
    fin.read((char*) &finfo, sizeof(finfo) );

    if (finfo.biBitCount != 8) {
        fin.close();
        return -1; //only deal with 256 colour BMP's so far
    }

    fin.read((char*) &palette, sizeof(palette) );
    fin.seekg(fhead.bfOffBits,SEEK_SET);

    int numpixel = finfo.biWidth * finfo.biHeight, tmp;

    char *data = (char*)malloc( numpixel );
    char *pic = (char*)malloc(numpixel * 4 );
    fin.read( data, numpixel );
    fin.close();

    char *pixel;
    RGBQUAD *bigpixel;
    int *trans;

    pixel = data;
    bigpixel = (RGBQUAD*)pic;
    for (int i=0; i < numpixel; i++) {
        *bigpixel = palette[(byte)(*pixel)];
        trans = (int*)bigpixel;

        if ( *trans == transcolour ) (*bigpixel).rgbReserved = 0;
            else (*bigpixel).rgbReserved = 0xff;

        tmp = (*bigpixel).rgbRed;
        (*bigpixel).rgbRed = (*bigpixel).rgbBlue;
        (*bigpixel).rgbBlue = tmp;
        bigpixel++;
        pixel++;
    }

    int num;
    glGenTextures(1, (UINT*)&num );
    glBindTexture(GL_TEXTURE_2D, num);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, TEX_FORMAT, finfo.biWidth, finfo.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,  pic);
    glBindTexture(GL_TEXTURE_2D, num);

    free(pic);
    free(data);
    return num;
}

int LbGraphicsImp::LoadMemTexture( char *pixels, int xsize, int ysize)
{
    int num;
    glGenTextures(1, (UINT*)&num );
    glBindTexture(GL_TEXTURE_2D, num);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, TEX_FORMAT, xsize, ysize, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, num);
    return num;
}

void LbGraphicsImp::ActivateTexture(int texID)
{       //ActivateTexture(0) activates the default texture
    glBindTexture(GL_TEXTURE_2D, texID);
}

void LbGraphicsImp::DeleteTexture(int texID)
{
    glDeleteTextures(1, (UINT*)&texID);
}

void LbGraphicsImp::DrawText(float x,float y,const char *str)
{       //TODO: colour, font?, size?
/*  SetupOrtho();
        glListBase(os->GLTextListBase());
        glColor4fv( &textcolor.r );
        glRasterPos3f(x*640.0f, y*480.0f, 0.1f);
        glCallLists(strlen(str),GL_UNSIGNED_BYTE,str);
    FinishOrtho(); */

    char chr;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDisable(GL_DEPTH_TEST);
    SetupOrtho();
    glEnable(GL_TEXTURE_2D);
    ActivateTexture(fontID);

    int cval, texx, texy, cpos = 0;
    float xpos = x * 640, ypos = y * 480;

    glBegin(GL_TRIANGLES);
    //draw text as triangles. Triangles are usually faster than Quads
    //if the driver would have converted them anyway - well, no loss
    //if it wouldn't, we might see a slight speed increase
    //can't use a TRIANGLE_STRIP because a vertex can have different
    //texture coords depending on which character you're drawing
    chr = str[0];

    while (chr != 0) {

        cval = ((byte)chr) - 30;
        texx = (cval % 10);
        texy = (cval / 10);

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f((GLfloat)texx*24.0f/256.0f, 1.0f-(GLfloat)(texy+1)*24.0f/256.0f);
        glVertex2f(xpos, ypos);
        glTexCoord2f((GLfloat)(texx+1)*24.0f/256.0f, 1.0f-(GLfloat)(texy+1)*24.0f/256.0f);
        glVertex2f(xpos+24.0f, ypos);
        glTexCoord2f((GLfloat)texx*24.0f/256.0f, 1.0f-(GLfloat)texy*24.0f/256.0f);
        glVertex2f(xpos, ypos+24.0f);
        glTexCoord2f((GLfloat)(texx+1)*24.0f/256.0f, 1.0f-(GLfloat)(texy+1)*24.0f/256.0f);
        glVertex2f(xpos+24.0f, ypos);
        glTexCoord2f((GLfloat)texx*24.0f/256.0f, 1.0f-(GLfloat)texy*24.0f/256.0f);
        glVertex2f(xpos, ypos+24.0f);
        glTexCoord2f((GLfloat)(texx+1)*24.0f/256.0f, 1.0f-(GLfloat)texy*24.0f/256.0f);
        glVertex2f(xpos+24.0f, ypos+24.0f);

        chr = str[++cpos];
        xpos += 24.0f;
    }

/*  glTexCoord2f(0.0f,0.0f);        //This code draws the font on the screen
    glVertex2f(0.0f,0.0f);          //Just to check it's loaded OK
    glTexCoord2f(1.0f,0.0f);
    glVertex2f(256.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex2f(256.0f,256.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex2f(0.0f,256.0f);
*/
    glEnd();

    FinishOrtho();
    
    glEnable(GL_DEPTH_TEST);

}

void LbGraphicsImp::SetTextColor(LbRGBAColor color)
{
    textcolor = color;
}

void LbGraphicsImp::TriggerEffect(LbGraphicsEffect effect)
{       //Current overrides current effect. TODO: Implement a list of effects?
    cureffect = effect;
    effectprogress = 0.0f;
}

void LbGraphicsImp::DrawEffect()
{
    if (!cureffect) return; //exit straight away if cur effect is NONE

    effectprogress++; //for the moment, inc. by 1.0f per frame.
    float progress = (float)(effectprogress) / 250.0f; //as a percentage 0.0f-1.0f

#ifdef FADE_WHITE
    float fadecolour = 1.0f;
#else
    float fadecolour = 0.0f;
#endif

    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_BLEND);

    switch (cureffect)
    {
        case LB_GFX_FADEIN:

            SetupOrtho();
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            glBegin(GL_TRIANGLE_STRIP);
                glColor4f(fadecolour,fadecolour,fadecolour, 1.0f - progress );
                glVertex2f(0.0f,0.0f);
                glVertex2f(640.0f,0.0f);
                glVertex2f(0.0f,480.0f);
                glVertex2f(640.0f,480.0f);
            glEnd();
            FinishOrtho();
            break;

        case LB_GFX_FADEOUT:

            SetupOrtho();
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            glBegin(GL_TRIANGLE_STRIP);
                glColor4f(fadecolour,fadecolour,fadecolour, progress );
                glVertex2f(0.0f,0.0f);
                glVertex2f(640.0f,0.0f);
                glVertex2f(0.0f,480.0f);
                glVertex2f(640.0f,480.0f);
            glEnd();
            FinishOrtho();
            break;

        case LB_GFX_FADEINTEXTURE:

            SetupOrtho();
            glEnable(GL_TEXTURE_2D);
            ActivateTexture(sfxID);
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            glBegin(GL_TRIANGLE_STRIP);
                glColor4f(fadecolour,fadecolour,fadecolour, 1.0f - progress );
                glTexCoord2f(0.0f,480.0f/512.0f);
                glVertex2f(0.0f,0.0f);
                glTexCoord2f(640.0f/1024.0f,480.0f/512.0f);
                glVertex2f(640.0f,0.0f);
                glTexCoord2f(0.0f,0.0f);
                glVertex2f(0.0f,480.0f);
                glTexCoord2f(640.0f/1024.0f,0.0f);
                glVertex2f(640.0f,480.0f);
            glEnd();
            FinishOrtho();
            break;

        case LB_GFX_FADEOUTTEXTURE:

            SetupOrtho();
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_TEXTURE_2D);
            ActivateTexture(sfxID);
            glBegin(GL_TRIANGLE_STRIP);
                glColor4f(fadecolour,fadecolour,fadecolour, progress );
                glTexCoord2f(0.0f,0.0f);
                glVertex2f(0.0f,0.0f);
                glTexCoord2f(1.0f,0.0f);
                glVertex2f(640.0f,0.0f);
                glTexCoord2f(0.0f,1.0f);
                glVertex2f(0.0f,480.0f);
                glTexCoord2f(1.0f,1.0f);
                glVertex2f(640.0f,480.0f);
            glEnd();
            FinishOrtho();
            break;


    }

    glPopAttrib();

    if (effectprogress >= 250.0f) cureffect = LB_GFX_NONE;
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
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    gluLookAt( campos.getX(), campos.getY(), campos.getZ(),
        camtgt.getX(), camtgt.getY(), camtgt.getZ(),
        camup.getX(), camup.getY(), camup.getZ() );
        //if we use LoadIdentity after this point we lose the camera position
        //so ALWAYS push and pop the matrix instead for each block of drawing
        //so the original camera matrix is always available

    glPushMatrix();

    glRotatef(((GLfloat)frameCount / 5), 0.0f, 0.0f, -1.0f);
    glTranslatef(-1.5f, 0.0f, -8.0f);
	glBegin(GL_TRIANGLES);
        glColor3f(0.0f, 0.4f, 0.8f);
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glVertex3f( 1.0f,-1.0f, 0.0f);
        glVertex3f(-1.0f,-1.0f, 0.0f);
	glEnd();
	
    glPopMatrix(); //restore the camera matrix for next block of drawing...
    glPushMatrix();//and push it again to save a copy

    glTranslatef(1.5f, 0.0f, -8.0f);
    glRotatef(((GLfloat)frameCount / 5), 1.0f, 0.0f, 0.0f);
    glRotatef(((GLfloat)frameCount / 7), 0.0f, 1.0f, 0.0f);
    glRotatef(((GLfloat)frameCount / 11), 0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS);
		// TOP
        glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
        glVertex3f( 1.0f, 1.0f,-1.0f);
        glVertex3f(-1.0f, 1.0f,-1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f( 1.0f, 1.0f, 1.0f);
		// BOTTOM
        glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
        glVertex3f( 1.0f,-1.0f, 1.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glVertex3f( 1.0f,-1.0f,-1.0f);
		// FRONT
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glVertex3f( 1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);
        glVertex3f( 1.0f,-1.0f, 1.0f);
		// BACK
        glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
        glVertex3f( 1.0f,-1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glVertex3f(-1.0f, 1.0f,-1.0f);
        glVertex3f( 1.0f, 1.0f,-1.0f);
		// LEFT
        glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);
		// RIGHT
        glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
        glVertex3f( 1.0f, 1.0f,-1.0f);
        glVertex3f( 1.0f, 1.0f, 1.0f);
        glVertex3f( 1.0f,-1.0f, 1.0f);
        glVertex3f( 1.0f,-1.0f,-1.0f);
	glEnd();

    
    glPopMatrix();

/*  SetupOrtho();
    ActivateTexture(sfxID);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glColor3f(1.0f,1.0f,1.0f);
        glTexCoord2f(0.0f,0.0f);
        glVertex2f(0.0f,0.0f);
        glTexCoord2f(1.0f,0.0f);
        glVertex2f(128.0f,0.0f);
        glTexCoord2f(1.0f,1.0f);
        glVertex2f(128.0f,128.0f);
        glTexCoord2f(0.0f,1.0f);
        glVertex2f(0.0f,128.0f);
    glEnd();
    FinishOrtho();
*/
	// ***********************************************************************

    if (glGetError()) {
                MessageBox(NULL, "Error rendering.", "LbGraphics::StartFrame()", MB_ICONSTOP);
	}
}

void LbGraphicsImp::EndFrame()
{
    DrawEffect(); //after drawing so it affects everything drawn this frame.
	frameCount++;
    glFinish(); //not really needed, but good style
    os->SwapDoubleBuffers();
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
    cureffect = LB_GFX_NONE;

    fontID = LoadBMPTexture("font.bmp",0);

    char *tex = os->GetDesktop32();
    sfxID = LoadMemTexture(tex, 1024, 512);
    delete tex;
}

LbGraphicsImp::LbGraphicsImp()
{
    campos = LbVector(0.0f,0.0f,-1.0f);
    camtgt = LbVector();
    camup = LbVector(0.0f,1.0f,0.0f);
}

LbGraphicsImp::~LbGraphicsImp()
{
    DeleteTexture(fontID);
    DeleteTexture(sfxID);
}

LbGraphicsSys *CreateGraphicsSys(LbOSLayerSys *os_sys)
{
LbGraphicsImp *rval=new LbGraphicsImp;
assert(rval!=NULL);

rval->Init();

return rval;
}