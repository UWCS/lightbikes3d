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
// Stores open connections.
struct LbSocket
{
    SOCKET socket ;
    SOCKADDR_IN remoteAddress ;
    char readBuffer [ SOCKET_BUFFER_SIZE ] ;
    int readBufferTail ;
    int readBufferHead ;
    char writeBuffer [ SOCKET_BUFFER_SIZE ] ;
    int writeBufferSize ;
    bool error ;
};

class LbNetImp : public LbNetSys
{
    public:
        LbNetImp();
        ~LbNetImp();

        virtual void InitialiseServerUDP ( int server_port , int client_port ) ;
        virtual void InitialiseClientUDP ( int server_port , int client_port ) ;
        virtual bool GetNextPositionUpdate ( LbGamePositionUpdate & u ) ;
        virtual void PollSocketsUDP ( ) ;
        virtual void SendPositionUpdate( LbGamePositionUpdate & u ) ;

        virtual int GetStatus ( ) ;
        virtual bool GetNextGameEvent (  LbGameEvent &e ) ;
        virtual void SendGameEvent ( LbGameEvent &e , bool includeourself ) ;
        virtual void ProcessMessages ( ) ;
        virtual void Init(LbOSLayerSys *os_sys);
        virtual void PollSockets ( );
        virtual void ConnectToServer( const char * address , int port ) ;
        virtual void InitiateServer( const char * address , int port ) ;
        virtual int GetOwnPlayerHash ( ) ;
    private:
        // LbNet's reference to the OS Layer.
        LbOSLayerSys *os;
        virtual void AcceptConnection (  ) ;
        virtual void ReadData ( int c  ) ;
        virtual void SendData ( int c  ) ;
        virtual bool GetTCPMessage ( LbSocket * * s , char * message ) ;
        virtual void PutTCPMessage ( LbSocket * s , const char * message ) ;
        virtual void BroadcastTCPMessage ( const char * message ) ;
        virtual LbSocket * PlayerhashToSocket ( int playerHash ) ;
        virtual int SocketToPlayerhash ( LbSocket * s ) ;
        virtual void ResetConnections ( ) ;
        virtual void LbNetImp::CloseSocket  ( LbSocket &s ) ;

        // Store the game messages ready to be collected by the game logic.
        queue<LbGameEvent> gameMessageQueue ;

        // Array of all connections and the number of valid connections.
        vector<LbSocket> lbsockets;

        // Socket connections with data available waiting to be read from buffer.
        queue<LbSocket*> readSocketQueue ;

        // The index of the socket on which, if this is a server, it is listening.
        int iListCon ;

        // The index of the socket on which, if this is a client, it is connected to the server.
        int iServCon ;

        int ownplayerhash ;

        // type of operation ( client, server, disconnected )
        int mode ;

        const char * serveraddress ;

        // Socket to be used for UDP whether as client or server.
        SOCKET udpsocket ;

        // UDP ports.
        int server_udp_port ;
        int client_udp_port ;

        // Queues for incoming and outgoing position updates.
        queue<LbGamePositionUpdate> gameUpdateReceiveQueue ;
        queue<LbGamePositionUpdate> gameUpdateTransmitQueue ;
    };

#endif