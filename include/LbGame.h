/*********************************************************************************
    LbGame.h
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

#ifndef __LBGAME__
#define __LBGAME__


// The port on the server to be used for connections.
#define LB_SERVER_TCP_PORT 32001
#define LB_SERVER_UDP_PORT 32002

// The number of bytes for storing text waiting to be sent or processed to/from
// the network.
#define SOCKET_BUFFER_SIZE 256

// The number of bytes to read/write from/to the socket in a single attempt.
#define MAX_READ_SIZE 10
#define MAX_WRITE_SIZE 10

// Size of buffer allocated for message.
#define MAX_MESSAGE_LENGTH 256

// The max number of connections, clients only use 1, so really it's the
// max players per server.
#define MAX_CONNECTIONS 100

class LbBaseBlock
{
    virtual void GetGeometrySize(int &num_points,int &num_normals)=0;
    virtual void GetGeometry(LbVector *bpoints,int num_points,LbVector *bnormals,int num_normals)=0;
    virtual void GetColor(LbRGBAColor &col)=0;
    virtual void SetColor(const LbRGBAColor &col)=0;
};

class LbGameSys
{
    public:
        virtual int RunGame()=0;

        // empty virtual destructor to ensure proper cleanup
        virtual ~LbGameSys(){}
};

LbGameSys *CreateGameSys();

#endif