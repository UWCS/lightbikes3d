/*********************************************************************************
    LbNet.h
    Header file for the LightBikes2001 Networking subsystem classes

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black
       Chris Skepper

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
#ifndef __LBNET__
#define __LBNET__

enum LbGameEventId
{
    LB_GAME_PLAYERJOIN = 0 ,
    LB_GAME_PLAYERLEAVE = 1 ,
    LB_GAME_HANDCHANGE = 2 ,
    LB_GAME_CHATMESSAGE = 3 ,
    LB_GAME_NEWGAME = 4 ,
    LB_GAME_STARTSERVER = 20 ,
    LB_GAME_STOPSERVER = 21 ,
    LB_GAME_RESETSERVER = 22
};

struct LbGameEvent
{
    LbGameEventId id ;
    int playerId;
    char message[20] ;
};

class LbNetSys
{
    public:
        virtual bool GetNextGameEvent (  LbGameEvent &e ) = 0 ;
        virtual void ProcessMessages ( ) = 0 ;
        virtual void Init(LbOSLayerSys *os_sys) = 0 ;
				virtual void PollSockets ( ) = 0 ;
				virtual void ConnectToServer ( char *) = 0 ;
				virtual void InitiateServer ( int ) = 0 ;
				virtual void InitiateNetwork ( ) = 0 ;
				virtual void CloseNetwork ( ) = 0 ;

				virtual void AcceptConnection (  ) = 0;
				virtual void MakeConnection (  ) = 0;
				virtual void ReadData ( int c  ) = 0;
				virtual void SendData ( int c  ) = 0;

				virtual bool GetTCPMessage ( char * address , char * message ) = 0 ;
				virtual void PutTCPMessage ( char * address , char * message ) = 0 ;

        // empty virtual destructor to ensure proper cleanup
        virtual ~LbNetSys(){}
};

LbNetSys *CreateNetSys(LbOSLayerSys *os_sys);

#endif