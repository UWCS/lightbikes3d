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

#include "LbPlayer.h"

#define MAX_PLAYERS 10
#define MAX_MESSAGE_LINES 4
#define MAX_SCOREBOARD_LINES 10

using namespace std;

class LbGameImp : public LbGameSys
{
    public:
        LbGameImp();
        ~LbGameImp();

        virtual int RunGame();

    private:
        virtual void InitSubsystems();
        virtual void DeInitSubsystems();
        virtual void ProcessCommand ( string t ) ;
        virtual void ShowStatusMessage ( const string & txt ) ;
        virtual string GetPlayerHandle ( int hash ) ;
        virtual bool SetPlayerHandle ( int hash , const string & handle ) ;
        virtual void RemovePlayer ( int hash ) ;
        virtual void AddPlayer ( int hash , const string & handle ) ;
        virtual void NewGame ( ) ;
        virtual void NewRound ( ) ;
        virtual void EndGame ( ) ;
        virtual LbPlayer * GetPlayer ( int playerhash ) ;

        // Will get rid of this l8r.
        virtual string Pad ( string s , int l ) ;
        virtual string ItoS ( int i ) ;

        LbOSLayerSys *os_sys;
        LbGraphicsSys *graph_sys;
        LbInputSys *input_sys;
        LbSoundSys *sound_sys;
        LbNetSys *net_sys;

        bool quit_flag;
        string txtmsgs [ MAX_MESSAGE_LINES ] ;
        string scoremsgs [ MAX_SCOREBOARD_LINES ] ;

        // Used to store our handle between servers.
        string ownhandle ;

        LbPlayer * players [ MAX_PLAYERS ] ;
        LbPlayer * thisplayer ;

        bool gameinprogress ;

        int dirns [ 10 ]  ;
        LbRGBAColor cols [ 10 ];
        LbVector start  [ 10 ];
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
class LbEmptyBlock : public LbBaseBlockImp
{
    public:
    /*
    ** LbBaseBlock methods
    */

    virtual void GetGeometrySize(int &num_points,int &num_normals);
    virtual void GetGeometry(LbVector *bpoints,int num_points,LbVector *bnormals,int num_normals);

    virtual LbCollideEvent Collide(LbBlockDirection &dir);
    virtual float GetZInBlock(float &x, float &y);

    /*
    ** LbFullBlock methods
    */

    LbEmptyBlock();

    ~LbEmptyBlock();

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
virtual LbCollideEvent Collide(LbBlockDirection &dir);
virtual float GetZInBlock(float &x, float &y);

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
virtual LbCollideEvent Collide(LbBlockDirection &dir);
virtual float GetZInBlock(float &x, float &y);
/*
** LbFullBlock methods
*/

LbRampBlock(LbBlockDirection dir);
~LbRampBlock();

private:

LbBlockDirection block_dir;
};

class LbLevelImp;

class LbArenaImp : public LbArena
{
    LbLevel **levels;
    int numLevels;
    static LbLevelImp full_level;

    public:
    LbArenaImp(LbLevel **levs, int num);
    ~LbArenaImp();

    virtual LbLevel * GetLevel(int level);
    inline int GetZSize() { return numLevels; };
};

class LbLevelImp : public LbLevel
{
    LbBaseBlock **blocks;
    int xsize, ysize;
    static LbFullBlock full_block;

    public:
    LbLevelImp();
    LbLevelImp(int x, int y);
    ~LbLevelImp();

    virtual void SetBlockAt(int x, int y, LbBaseBlock &block);
    virtual LbBaseBlock * GetBlockAt(int x, int y);

    inline int GetXSize() { return xsize; };
    inline int GetYSize() { return ysize; };
};


class LbPlayerImp : public LbPlayer
{
    public:

        /*
        ** LbPlayer methods
        */

        // Get player name.
        virtual int GetHash ( ) ;

        // Get player handle.
        virtual string GetHandle ( ) ;

        // Get player kills.
        virtual int GetKills ( ) ;

        // Get player deaths.
        virtual int GetDeaths ( ) ;

        // Get player ping.
        virtual int GetPing ( ) ;

        // Get player valid.
        virtual bool IsValid ( ) ;

        // Set player hash.
        virtual void SetHash ( int h ) ;

        // Set player handle.
        virtual void SetHandle ( string h) ;

        // Set player kills.
        virtual void SetKills ( int k ) ;

        // Set player deaths.
        virtual void SetDeaths ( int d ) ;

        // Set player ping.
        virtual void SetPing ( int p ) ;

        // Set player valid.
        virtual void SetValid ( bool v ) ;

        // Get the graphics bike.
        virtual LbGraphicsBike * GetBike ( ) ;

        // Get the position.
        virtual LbVector * GetPosition ( ) ;

        // Get the graphics bike.
        virtual void SetBike ( LbGraphicsBike * g ) ;

        // Get the position.
        virtual void SetPosition ( LbVector * v ) ;

        // Set player valid.
        virtual void SetPlaying ( bool v ) ;

        // Get player kills.
        virtual bool IsPlaying ( ) ;

        // Set player the direction.
        virtual void SetDirection ( int d ) ;

        // Get player direction.
        virtual int GetDirection ( ) ;

        virtual int GetLevel ( ) ;
        virtual void SetLevel ( int l )  ;
        virtual int GetNextSequenceNumber ( ) ;

        // END

        /*
        ** LbPlayer methods
        */
        LbPlayerImp();
        ~LbPlayerImp();

    private:
        int hash ;
        string handle ;
        int kills ;
        int deaths ;
        int ping ;
        bool valid ;
        LbGraphicsBike * bike ;
        LbVector * bikepos ;
        bool playing ;
        int direction ;
        int level ;
        int sequence ;
};


#endif