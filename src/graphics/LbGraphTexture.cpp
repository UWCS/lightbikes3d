/*********************************************************************************
    LbGraphTexture.cpp
    Class file for the LightBikes2001 Graphics subsystem

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black
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

#define TEX_FORMAT  GL_RGBA8
                        //texture format to use. RGB5_A1 means 16 bit colour
                        //5 bits per colour, 1 alpha bit
                        //Alternatives could be:
                        //GL_RGBA8 (32-bit total: 8 bits per component)
                        //GL_RGBA4 (16-bit total: 4 bits per component)
                        //Note an alpha component is required for blending

LbGraphTexture::LbGraphTexture()
{
    valid_texture=false;
    tex_id=0;
    width=0;
    height=0;
    rgb_data=NULL;
    uploaded=false;
}

LbGraphTexture::~LbGraphTexture()
{
    DeleteTexture();
}

void LbGraphTexture::DeleteTexture()
{
    if(valid_texture)
        glDeleteTextures(1,&tex_id);

    valid_texture=false;
    tex_id=0;
}

bool LbGraphTexture::LoadTextureBMP(const char *fname,const LbRGBAColor &trans_col)
{
    ifstream fin;

    LBBITMAPFILEHEADER fhead;
    LBBITMAPINFOHEADER finfo;
    LBRGBQUAD palette[256];

    unsigned char *idx_data=NULL;

    unsigned char *idx_ptr=NULL;
    LbRGBAColor *rgb_ptr=NULL;
    LbRGBAColor *rgb_fdata=NULL;

    int i,j,pad;

    bool top_down;

    try
        {

        fin.open(fname, ios::binary);
        if(fin.fail())
            return false;
    
        /*
        ** TODO: impliment reading of fields and byte swapping
        **       for portability to little endian machines.
        */
        fin.read((char*) &fhead, sizeof(fhead) );
        assert(fin.good());

        fin.read((char*) &finfo, sizeof(finfo) );
        assert(fin.good());

        if (finfo.biBitCount != 8)
            {
            fin.close();
            return false;
            }
        fin.seekg(finfo.biSize+sizeof(fhead),ios::beg);
        assert(fin.good());
        
        memset(palette,0,sizeof(LBRGBQUAD)*256);

        if(finfo.biClrUsed!=0)
            {
            assert(finfo.biClrUsed<=256);
            fin.read((char*) &palette, sizeof(LBRGBQUAD)*finfo.biClrUsed);
            }
        else
            fin.read((char*) &palette, sizeof(LBRGBQUAD)*256);

        assert(fin.good());

        fin.seekg(fhead.bfOffBits,ios::beg);
        assert(fin.good());

        // if height is positive then bitmap is upside down...
        width=finfo.biWidth;
        if(finfo.biHeight<0)
            {
            top_down=true;
            height=-finfo.biHeight;
            }
        else
            {
            top_down=true;
            height=finfo.biHeight;
            }

        // lines always begin on DWORD boundries
         if((width%4)!=0)
            pad=4-(width%4);
        else
            pad=0;


        idx_data=new unsigned char[(width+pad)*height];
        rgb_fdata=new LbRGBAColor[width*height];
        rgb_data = (byte*)rgb_fdata;
        tex_format=GL_RGBA;
        
        if((idx_data==NULL)||(rgb_data==NULL))
            throw LbError("Cannot alloc mem for bitmap");
 
        fin.read((char *)idx_data,(width+pad)*height);
        assert(fin.good());
        fin.close();
        
        
        if(top_down)
            {
            idx_ptr=idx_data;
            rgb_ptr=rgb_fdata;

            for(i=0;i<height;i++)
                {
                for(j=0;j<width;j++)
                    {
                    rgb_ptr->r=palette[*idx_ptr].rgbRed;
                    rgb_ptr->g=palette[*idx_ptr].rgbGreen;
                    rgb_ptr->b=palette[*idx_ptr].rgbBlue;
                    
                    rgb_ptr->a=(rgb_ptr->r+rgb_ptr->g+rgb_ptr->b)/3;

                    idx_ptr++;
                    rgb_ptr++;
                    }
            
                idx_ptr+=pad;
                }
            }
        else
            {
            // must flip bitmap...
            idx_ptr=idx_data+(width+pad)*(height-1);
            rgb_ptr=rgb_fdata;

            for(i=0;i<height;i++)
                {
                for(j=0;j<width;j++)
                    {
                    rgb_ptr->r=palette[*idx_ptr].rgbRed;
                    rgb_ptr->g=palette[*idx_ptr].rgbGreen;
                    rgb_ptr->b=palette[*idx_ptr].rgbBlue;
                                        
                    /*if((rgb_ptr->r==trans_col.r) &&
                       (rgb_ptr->g==trans_col.g) &&
                       (rgb_ptr->b==trans_col.b))
                        rgb_ptr->a=0;
                    else
                        rgb_ptr->a=0xff;
                    
                    rgb_ptr->a=0xff;*/

                    rgb_ptr->a=(rgb_ptr->r+rgb_ptr->g+rgb_ptr->b)/3;

                    rgb_ptr++;
                    idx_ptr++;
                    }
                idx_ptr-=(2*(width+pad));
                }

            }

        }
    catch(...)
        {
        fin.close();

        if(idx_data!=NULL)
            delete idx_data;
        if(rgb_data!=NULL)
            delete rgb_data;

        throw;
        }

return true;
}

bool LbGraphTexture::LoadMemTexture(char *pixels, int xsize, int ysize, GLuint format) {

    width = xsize;
    height = ysize;
    tex_format = format;
    int datasize;
    switch(tex_format) {
//Add additional formats as and when needed. 
        case GL_RGBA:   datasize=4; break;
        case GL_RGB:    datasize=3; break;
        default:        return false;
    }
    rgb_data=new byte[width*height*datasize];
    memcpy(rgb_data, pixels, width*height*datasize);
    return true;
}

bool LbGraphTexture::Upload() {

    if (uploaded) return true;

    glGenTextures(1, &tex_id );
    valid_texture=true;
  
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, TEX_FORMAT, width, height, 0, tex_format, GL_UNSIGNED_BYTE,  rgb_data);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    uploaded=true;
    
    return (!glGetError());
}
