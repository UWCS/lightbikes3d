/*********************************************************************************
    LbNetImp.h
    Header file for the LightBikes2001 Network subsystem classes

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

#ifndef __LBNETIMP__
#define __LBNETIMP__

#define SOCKADDR_LEN sizeof(struct sockaddr)

using namespace std;

class LbNetImp : public LbNetSys
{
    public:
        LbNetImp();
        ~LbNetImp();

        virtual bool GetNextGameEvent (  LbGameEvent &e ) ;
        virtual void ProcessMessages ( ) ;
        virtual void Init(LbOSLayerSys *os_sys);
        virtual void PollSockets ( );

    private:
        // LbNet's reference to the OS Layer.
        LbOSLayerSys *os;

        virtual void ConnectToServer( char * ) ;
        virtual void InitiateServer( int ) ;
        virtual void AcceptConnection (  ) ;
        virtual void MakeConnection (  ) ;
        virtual void ReadData ( int c  ) ;
        virtual void SendData ( int c  ) ;
        virtual void CloseNetwork() ;
        virtual bool GetTCPMessage ( int * playerHash , char * message ) ;
        virtual void PutTCPMessage ( int playerHash , char * message ) ;
        virtual void BroadcastTCPMessage ( char * message ) ;

        // Store the game messages ready to be collected by the game logic.
        queue<LbGameEvent> gameMessageQueue ;

        // Stores open connections.  Uses a crude queue implementation.
        struct LbConnection
        {
            SOCKET socket ;
            SOCKADDR_IN remoteAddress ;
            char readBuffer [ SOCKET_BUFFER_SIZE ] ;
            int readBufferSize ;
            char writeBuffer [ SOCKET_BUFFER_SIZE ] ;
            int writeBufferSize ;
        };

        // Array of all connections and the number of valid connections.
        LbConnection connections [ MAX_CONNECTIONS ] ;
        int nCon ;

        // Socket connections with data available to be read from buffer.
        queue<LbConnection*> readSocketQueue ;

        // The index of the socket on which, if this is a server, it is listening.
        int iListCon ;

        // The index of the socket on which, if this is a client, it is connected to the server.
        int iServCon ;
};

#endif