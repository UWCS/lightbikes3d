/*********************************************************************************
    LbGraphicsImp.h
    Header file for the LightBikes2001 Graphics subsystem classes

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black
       David Capps
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
#ifndef __LBGRAPHICSIMP__
#define __LBGRAPHICSIMP__

/*
** TODO: fix the way we define these structures
*/
#include <pshpack1.h>
struct LBBITMAPFILEHEADER
    { 
    unsigned short           bfType; 
    unsigned int    bfSize;
    unsigned short  bfReserved1; 
    unsigned short  bfReserved2;
    unsigned int    bfOffBits;
    };

struct LBBITMAPINFOHEADER
    { 
    unsigned int    biSize;
    int             biWidth;
    int             biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount; 
    unsigned int    biCompression;
    unsigned int    biSizeImage;
    int             biXPelsPerMeter;
    int             biYPelsPerMeter;
    unsigned int    biClrUsed;
    unsigned int    biClrImportant; 
    };
struct LBRGBQUAD
    {
    unsigned char   rgbBlue; 
    unsigned char   rgbGreen; 
    unsigned char   rgbRed; 
    unsigned char   rgbReserved; 
    };
#include <poppack.h>

class LbGraphTexture
{
public:

LbGraphTexture();
~LbGraphTexture();

bool LoadTextureBMP(const char *fname,const LbRGBAColor &trans_col);
void DeleteTexture();
void ActivateTexture()
    {
    if(valid_texture)
        glBindTexture(GL_TEXTURE_2D,tex_id);
    assert(!glGetError());
    }

private:

GLuint tex_id;
bool valid_texture;
};

// Forward declarations.
class LbGraphicsBikeImp;
class LbGraphicsLevelImp;

class LbGraphicsImp : public LbGraphicsSys
{
public:
    
    /*
    ** LbGraphicsSys methods
    */
    virtual void CreateGraphicsBike();
    virtual void CreateGraphicsLevel(int x,int y);
    
    virtual void SetCamera(const LbVector &pos,const LbVector &target,const LbVector &up);
    
    virtual void DrawText(float x,float y,float scale, const char *str);
    virtual void SetTextColor(LbRGBAColor color);
    
    virtual void TriggerEffect(LbGraphicsEffect effect);
    
    virtual void StartFrame();
    virtual void EndFrame();
    
    virtual LbGraphicsBike *GetBike(int index);
    
    /*
    ** LbGraphicsImp methods
    */
    LbGraphicsImp();
    ~LbGraphicsImp();
    
    void Init(LbOSLayerSys *os_sys);
    
    
private:
    
    LbGraphTexture font_tex;

    void DrawEffect();
    void SetupOrtho();
    void FinishOrtho();
    int LoadBMPTexture(char *fname, int transcolour);
    int LoadMemTexture( char *pixels, int xsize, int ysize);
    void ActivateTexture(int texID);
    void DeleteTexture(int texID);
    
    vector<LbGraphicsBikeImp> lbbikes;
    vector<LbGraphicsLevelImp> lblevels;
    int frameCount;
    int sfxID;
    int gfxStart;
    int sfxnums[8];
    LbVector campos, camtgt, camup;
    LbOSLayerSys *os;
    LbGraphicsEffect cureffect;
    LbRGBAColor textcolor;
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
    
    // DEBUG
    virtual LbVector GetLastSegment();
    // END
    
    /*
    ** LbGraphicsBikeImp methods
    */
    LbGraphicsBikeImp();
    ~LbGraphicsBikeImp();
    
private:
    
    vector<LbVector> lbtrail;
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
