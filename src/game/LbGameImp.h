/*********************************************************************************
    LbGameImp.h
    Header file for the LightBikes2001 Game subsystem classes

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
#ifndef __LBGAMEIMP__
#define __LBGAMEIMP__

using namespace std;

class LbGameImp : public LbGameSys
{
public:
    LbGameImp();
    ~LbGameImp();

    virtual int RunGame();

private:

    void InitSubsystems();
    void DeInitSubsystems();
    void ProcessCommand ( string t ) ;
    string GetPlayerHandle ( int playerhash ) ;

    LbOSLayerSys *os_sys;
    LbGraphicsSys *graph_sys;
    LbInputSys *input_sys;
    LbSoundSys *sound_sys;
    LbNetSys *net_sys;

    bool quit_flag;
};

/*
** Block implimentations...
*/
// impliment common functionality
class LbBaseBlockImp : public LbBaseBlock
{
    public:
/*
** LbBaseBlock methods
*/
virtual void GetColor(LbRGBAColor &col);
virtual void SetColor(const LbRGBAColor &col);
/*
** LbBaseBlockImp methods
*/

LbBaseBlockImp();
~LbBaseBlockImp();

private:
};


class LbFullBlock : public LbBaseBlockImp
{
public:
/*
** LbBaseBlock methods
*/

virtual void GetGeometrySize(int &num_points,int &num_normals);
virtual void GetGeometry(LbVector *bpoints,int num_points,LbVector *bnormals,int num_normals);
/*
** LbFullBlock methods
*/

LbFullBlock();
~LbFullBlock();
private:
};

class LbRampBlock : public LbBaseBlockImp
{
public:
/*
** LbBaseBlock methods
*/

virtual void GetGeometrySize(int &num_points,int &num_normals);
virtual void GetGeometry(LbVector *bpoints,int num_points,LbVector *bnormals,int num_normals);

/*
** LbFullBlock methods
*/

LbRampBlock(LbBlockDirection dir);
~LbRampBlock();

private:

LbBlockDirection block_dir;
};

#endif