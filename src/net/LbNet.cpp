/*********************************************************************************
    LbNet.cpp
    Class file for the LightBikes2001 Network subsystem

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
#include "LbStandard.h"
#include "LbPublic.h"
#include "LbNetImp.h"

// Overview.
//
// When running as a client the software connects to a specified server, when
// running as a server the application listens on a port and opens connections
// to clients that attempt to connect.  In either mode, send and receive
// buffers are maintained as part of the socket structure.
//
// There has to be a mapping between players and open connections (sockets) on
// the server.  Players are assigned a unique playerHash by the server when
// they connect which is constant while they are connected, the game logic can
// convert this to player objects.
//
// TCP connections are maintained. The non-urgent (player join / chat etc.)
// game messages are translated to text and added to the queue and sent
// whenever the network queue is serviced.  UDP packets are used for the more
// urgent (player turn) messages.
//
// TO DO:
//      UDP Packet acknowlegements/resending.
//      Threading to replace the need for polling from the game logic.
//      Robustness.

LbNetImp::LbNetImp ( )
{
}

// ----- UDP stuff ---------------

/**
 ** Bind a socket to a port for a server to send and receive packets on.
 **/
void LbNetImp::InitialiseServerUDP  ( int server_port , int client_port )
{
    // Create the socket.  Parameters are address family = Internet, socket
    // type = datagram , protocol = UDP.
    udpsocket = socket(AF_INET, SOCK_DGRAM , IPPROTO_UDP ) ;

    if ( udpsocket == INVALID_SOCKET )
    {
        error = "There was an error opening the socket to use for UDP." ;
        mode = LB_NET_ERROR ;
        return  ;
    }

    SOCKADDR_IN sockName;
    sockName.sin_family = AF_INET;
    sockName.sin_addr.s_addr = INADDR_ANY;      // Let WinSock assign address.
    sockName.sin_port = htons ( server_port );  // Use given port.

    // Set the port and address, and report the error.
    PSOCKADDR_IN pSockName ;
    pSockName = &sockName ;

    // Bind the socket to a port and address, and report any errors.
    int nRet = bind ( udpsocket,                    // Socket descriptor
                      ( LPSOCKADDR ) & sockName ,  // Address to which to bind.
                      sizeof ( struct sockaddr ) ) ;// Size of address

    if ( nRet == SOCKET_ERROR )
    {
        error = "Error binding to the server UDP port.  Another application "\
                "may be using this port." ;
        mode = LB_NET_ERROR ;
        return ;
    }

    server_udp_port = server_port ;
    client_udp_port = client_port ;
}

/**
 ** Bind a socket to a port for a client to send and receive packets on.
 **/
void LbNetImp::InitialiseClientUDP  ( int server_port , int client_port )
{
    // Create the socket.  Parameters are address family = Internet, socket
    // type = datagram , protocol = UDP.
    udpsocket = socket(AF_INET, SOCK_DGRAM , IPPROTO_UDP ) ;

    if ( udpsocket == INVALID_SOCKET )
    {
        error = "There was an error opening socket to use for UDP." ;
        mode = LB_NET_ERROR ;
        return ;
    }

    SOCKADDR_IN sockName;
    sockName.sin_family = AF_INET;
    sockName.sin_addr.s_addr = INADDR_ANY;      // Let WinSock assign address
    sockName.sin_port = htons ( client_port );  // Use port given.

    // Bind the socket to a port and address, and report any errors.
    int nRet = bind ( udpsocket,                    // Socket descriptor
                      ( LPSOCKADDR ) & sockName ,  // Address to which to bind.
                      sizeof ( struct sockaddr ) ) ;// Size of address

    if ( nRet == SOCKET_ERROR )
    {
        error = "Error binding to the client UDP port.  Another application "\
                "may be using this port." ;
        mode = LB_NET_ERROR ;
        return ;
    }

    server_udp_port = server_port ;
    client_udp_port = client_port ;
}

/**
 ** Add position update to a queue to be sent to the server or clients.
 **/
void LbNetImp::SendPositionUpdate ( LbGamePositionUpdate & u )
{
    // Add to queue.
    gameUpdateTransmitQueue.push ( u ) ;
}

/**
 ** Get next position update if there is one.
 **/
bool LbNetImp::GetNextPositionUpdate ( LbGamePositionUpdate & u )
{
    // If the queue is non-empty.
    if ( ! gameUpdateReceiveQueue.empty ( ) )
    {
        LbGamePositionUpdate t = gameUpdateReceiveQueue.front();
        gameUpdateReceiveQueue.pop();
        u = t ;
        return true ;
    }
    return false ;
}

/**
 ** Poll Sockets.  Receive any waiting packets, then send packets.  Position
 ** updates are sent to the server, unless we are the server in which case we
 ** send it to all the clients and it is interpreted as being authorative.
 **/
void LbNetImp::PollSocketsUDP ( )
{
    // Receive ------------------------

    // Keep getting data while there are packets waiting to be read.
    while ( true )
    {
        // Determine if the UDP socket needs reading.
        fd_set readscks , writescks , errscks;
        errscks.fd_count = writescks.fd_count = readscks.fd_count = 1 ;
        errscks.fd_array[0] =
            writescks.fd_array[0] =
                readscks.fd_array[0] = udpsocket ;

        timeval timeout;
        timeout.tv_sec = 0 ;
        timeout.tv_usec = 0 ;

        select ( 0 , ( fd_set * ) & readscks ,
                 ( fd_set * ) & writescks ,
                 ( fd_set * ) & errscks , &timeout ) ;

        // If it doesn't need reading then quit.
        if ( readscks.fd_count == 0 || readscks.fd_array[0] != udpsocket )
            break ;

        // Read data.
        char buf [ 30 ] ;

        SOCKADDR_IN saClient;

        int s = sizeof ( SOCKADDR ) ;

        if ( int e = recvfrom ( udpsocket , (char*)& buf , 30 , 0 ,
                   (LPSOCKADDR)&saClient , & s ) != SOCKET_ERROR && e != 0 )
        {
            LbGamePositionUpdate u ;

            // Parse the message.
            memcpy ( & u.playerHash , & buf [ 4 ] , 4 ) ;
            memcpy ( & u.sequence , & buf [ 8 ] , 4 ) ;
            memcpy ( & u.x1 , & buf [ 12 ] , 4 ) ;
            memcpy ( & u.y1 , & buf [ 16 ] , 4 ) ;
            memcpy ( & u.x2 , & buf [ 20 ] , 4 ) ;
            memcpy ( & u.y2 , & buf [ 24 ] , 4 ) ;
            memcpy ( & u.level , & buf [ 28 ] ,  1 ) ;
            memcpy ( & u.direction , & buf [ 29 ] ,  1 ) ;

            // Add the update to the queue.
            gameUpdateReceiveQueue.push ( u ) ;
        }
    }

    // Transmit ---------------------
    while ( ! gameUpdateTransmitQueue.empty ( ) )
    {
        LbGamePositionUpdate u = gameUpdateTransmitQueue.front();
        gameUpdateTransmitQueue.pop();

        // Create the packet.
        char buf [ 30 ] ;
        strcpy ( buf , "MOVE                          " ) ;
        memcpy ( & buf [ 4 ] , & u.playerHash , 4 ) ;
        memcpy ( & buf [ 8 ] , & u.sequence , 4 ) ;
        memcpy ( & buf [ 12 ] , & u.x1 , 4 ) ;
        memcpy ( & buf [ 16 ] , & u.y1 , 4 ) ;
        memcpy ( & buf [ 20 ] , & u.x2 , 4 ) ;
        memcpy ( & buf [ 24 ] , & u.y2 , 4 ) ;
        memcpy ( & buf [ 28 ] , & u.level , 1 ) ;
        memcpy ( & buf [ 29 ] , & u.direction , 1 ) ;

        if ( mode == LB_NET_CLIENT )
        {
            SOCKADDR_IN saServer;
            PSOCKADDR_IN pSaServer ;
            pSaServer = &saServer ;
            pSaServer->sin_family = PF_INET ;
            pSaServer->sin_addr.S_un.S_addr =
                lbsockets[iServCon].remoteAddress.sin_addr.S_un.S_addr ;
            pSaServer->sin_port = htons ( server_udp_port );

            int nRet = sendto(udpsocket,    // Socket
                  (char * ) & buf,          // Data buffer
                  30,                       // Length of data
                  0,                        // Flags
                  (LPSOCKADDR)pSaServer,    // Server address
                  sizeof(struct sockaddr)); // Length of address
        }
        else if ( mode == LB_NET_SERVER )
        {
            int i ;

            // Send message to all clients.
            for ( i = 0 ; i < lbsockets.size() ; i++ )
            {
                // Skip the socket that the server listens on since this
                // won't have an address to use.
                if ( i != iListCon )
                {
                    SOCKADDR_IN saClient ;
                    PSOCKADDR_IN pSaServer ;
                    pSaServer = & saClient ;
                    pSaServer->sin_family = PF_INET ;
                    pSaServer->sin_addr.S_un.S_addr =
                    lbsockets[i].remoteAddress.sin_addr.S_un.S_addr ;
                    pSaServer->sin_port = htons ( client_udp_port );

                    int nRet = sendto ( udpsocket , // Socket
                      (char * ) & buf ,             // Data buffer
                      30,                           // Length of data
                      0 ,                           // Flags
                      (LPSOCKADDR)pSaServer,        // Server address
                      sizeof(struct sockaddr));     // Length of address
                }
             }
          }
    }


}

// ------ end of UDP stuff ---------

/**
 ** Returns LB_NET_CLIENT , LB_NET_SERVER or LB_NET_DISCONNECTED.
 **/
int LbNetImp::GetStatus ( )
{
    return mode ;
}

// ----- Stuff for translating game messages, to text or packets and v.v. -----

/**
 ** Initialisation of networking subsystem.
 **/
void LbNetImp::Init ( LbOSLayerSys *os_sys )
{
   // Store the reference to the OS layer.
    os = os_sys;

    // Set the mode.
    mode = LB_NET_DISCONNECTED ;

    iListCon = -1 ;
    iServCon = -1 ;
    ownplayerhash = -1 ;

    // Start the OS level aspects of the network.
    os->InitiateNetwork();
}

/**
 ** Return and remove the next message from the game message queue.
 ** things like "start game" etc.
 **/
bool LbNetImp::GetNextGameEvent ( LbGameEvent &e )
{
    if ( !gameMessageQueue.empty ( ) )
    {
        LbGameEvent t = gameMessageQueue.front();
        gameMessageQueue.pop();
        e.id = t.id ;
        e.message = t.message ;
        e.playerHash = t.playerHash ;
        return true ;
    }
    return false ;
}

/**
 ** Add a LbGameEvent to the send queue.
 **/
void LbNetImp::SendGameEvent ( LbGameEvent &e , bool includeourself )
{
    string cmd , msgtext = e.message , ply ;
    char buf [ 20 ] ;
    itoa ( e.playerHash , buf , 10 ) ;
    ply = buf ;

    switch ( e.id )
    {
        case LB_GAME_PLAYERJOIN :  cmd = "JOIN" ;  break ;
        case LB_GAME_PLAYERLEAVE :  cmd = "LEAVE" ;  break ;
        case LB_GAME_HANDCHANGE :  cmd = "HAND";  break ;
        case LB_GAME_AWAY :  cmd = "AWAY" ;  break ;
        case LB_GAME_BACK :  cmd = "BACK" ;  break ;
        case LB_GAME_CHAT :  cmd = "CHAT" ;  break ;
        case LB_GAME_NEWGAME :  cmd = "NEWGAME" ;  break ;
        case LB_GAME_ENDGAME :  cmd = "ENDGAME" ;  break ;
        case LB_GAME_NEWROUND :  cmd = "NEWROUND" ;  break ;
        case LB_GAME_NEWMAP :  cmd = "NEWMAP" ;  break ;
    }

    // Construct the message.
    msgtext = cmd + " " + ply + " " + msgtext + "\r\n";

    // If we are a server.
    if ( mode == LB_NET_SERVER )
    {
        BroadcastTCPMessage ( msgtext.c_str ( ) ) ;
    }
    else if ( mode == LB_NET_CLIENT )
    {
        PutTCPMessage ( & lbsockets [ iServCon ] ,
                        msgtext.c_str ( ) ) ;
    }

    // Keep a copy for ourselves.  Only if it came from us, otherwise
    // we'll already have the message and just be relaying it.
    if ( includeourself )
        gameMessageQueue.push ( e ) ;
}

/**
 ** Return our own id as assigned by server.
 **/
int LbNetImp::GetOwnPlayerHash ( )
{
    return ownplayerhash ;
}

/**
 ** Process the messages waiting on the queue.
 **/
void LbNetImp::ProcessMessages ( )
{
    // Check the TCP message queue.
    char messagebuf [ MAX_MESSAGE_LENGTH ] ;
    LbSocket * s ;

    while ( GetTCPMessage ( & s , messagebuf ) )
    {
        // Assign the buffer to a proper string object, parse for command
        // and playerhash.
        string completemessage = string ( messagebuf ) ;
        int n = completemessage.find_first_of ( " " , 0 ) ;
        int m = completemessage.find_first_of ( " " , n + 1 ) ;
        int q = completemessage.find_last_not_of ( "\r\n" ) ;
        string commandstring = completemessage.substr ( 0 , n ) ;
        string playerhashstring = completemessage.substr ( n , m - n ) ;
        string msgtext = completemessage.substr ( m + 1, q - m ) ;
        int playerhash = atoi ( playerhashstring.c_str ( ) ) ;

        if ( commandstring == "WELCOME" )
        {
            // Store our own player hash.
            ownplayerhash = playerhash ;
            continue ;
        }

        // As a client, create game messages.
        LbGameEvent t ;

        // Deal with the messages.
        if ( commandstring == "JOIN" ) t.id = LB_GAME_PLAYERJOIN ;
        else if ( commandstring == "LEAVE" ) t.id = LB_GAME_PLAYERLEAVE ;
        else if ( commandstring == "HAND" ) t.id = LB_GAME_HANDCHANGE ;
        else if ( commandstring == "CHAT" ) t.id = LB_GAME_CHAT ;
        else if ( commandstring == "AWAY" ) t.id = LB_GAME_AWAY ;
        else if ( commandstring == "BACK" ) t.id = LB_GAME_BACK ;
        else if ( commandstring == "NEWGAME" ) t.id = LB_GAME_NEWGAME ;
        else if ( commandstring == "NEWROUND" ) t.id = LB_GAME_NEWROUND ;
        else if ( commandstring == "ENDGAME" ) t.id = LB_GAME_ENDGAME ;
        else if ( commandstring == "NEWMAP" ) t.id = LB_GAME_NEWMAP ;

        // Copy the message text.
        t.message=msgtext;
        t.playerHash = playerhash ;

        // If we are server check for players sending wrong player numbers.
        //if ( mode == LB_NET_SERVER &&
        //     playerhash != SocketToPlayerhash ( s ) )
        //    MessageBox ( NULL , "Client sent invalid data." ,
        //        "Error" , MB_ICONSTOP ) ;
        if ( playerhash == -1 )
            t.playerHash = SocketToPlayerhash ( s ) ;

        // Add the game message to the queue.
        gameMessageQueue.push ( t ) ;

        }
}

/**
 ** Get the playerHash for a particular socket.
 **/
int LbNetImp::SocketToPlayerhash ( LbSocket * s )
{
    int i ;
    for ( i = 0 ; i < lbsockets.size ( ) ; i ++ )
        if ( s == &lbsockets [ i ] )
            break ;
    return ( int ) lbsockets [ i ].remoteAddress.sin_addr.S_un.S_addr + i ;
}

/**
 ** Get the socket for a particular playerHash
 **/
LbSocket * LbNetImp::PlayerhashToSocket ( int playerhash )
{
    int i ;
    for ( i = 0 ; i < lbsockets.size ( ) ; i ++ )
        if ( ( int ) (
            lbsockets [ i ].remoteAddress.sin_addr.S_un.S_addr + i ) ==
                playerhash )
            break ;
    return ( & lbsockets [ i ] ) ;
}

// ----- Lower level network functions. -----

/**
 ** Checks for sockets waiting to be serviced.
 **/
void LbNetImp::PollSockets ( )
{
    // Set the timeout.
    timeval timeout;
    timeout.tv_sec = 0 ;
    timeout.tv_usec = 0 ;
     int i;

    // Construct read sockets.
    fd_set readscks , writescks , errscks;
    errscks.fd_count = writescks.fd_count = readscks.fd_count = lbsockets.size ( ) ;
    for (i = 0 ; i < lbsockets.size ( ) ; i++ )
    {
        errscks.fd_array[i] =
            writescks.fd_array[i] =
                readscks.fd_array[i] = lbsockets[i].socket ;
    }

    // Call to check the sockets.
    select ( 0 , ( fd_set * ) & readscks ,
                 ( fd_set * ) & writescks ,
                 ( fd_set * ) & errscks , &timeout );

    // Check for data to read or incoming lbsockets.
    for (i = 0 ; i < readscks.fd_count ; i ++ )
    {
        for ( int j = 0 ; j < lbsockets.size ( ) ; j ++ )
                if ( readscks.fd_array [ i ] == lbsockets [ j ].socket )
                    if ( j == iListCon ) AcceptConnection ( ) ;
                    else ReadData ( j ) ;
    }

    // Check for sockets ready to write.
    for (i = 0 ; i < writescks.fd_count ; i ++ )
    {
        for ( int j = 0 ; j < lbsockets.size ( ) ; j ++ )
            if ( writescks.fd_array [ i ] == lbsockets [ j ].socket )
                SendData ( j ) ;
    }

    // Check for sockets with errors.
    for (i = 0 ; i < errscks.fd_count ; i ++ )
    {
        for ( int j = 0 ; j < lbsockets.size ( ) ; j ++ )
            if ( errscks.fd_array [ i ] == lbsockets [ j ].socket )
                SocketError( j ) ;
    }

}

/**
 ** Determine what to do about an error with a socket.  We may be able to drop
 ** the player and continue.
 **/
void LbNetImp::SocketError ( int c )
{
    if ( mode == LB_NET_SERVER )
    {
        if ( c == iListCon )
        {
            error = "Socket error on server listen port." ;
            mode = LB_NET_ERROR ;
            return ;
        }
        else
        {
            CloseSocket ( lbsockets [ c ] ) ;
        }
    }

    if ( mode == LB_NET_CLIENT )
    {
        if ( c == iServCon )
        {
            error = "Lost connection to server." ;
            mode = LB_NET_ERROR ;
            return ;
        }
    }
}

/**
 ** Called when starting server or changing remote server.
 **/
void LbNetImp::ResetConnections ( )
{
    // If we are running a server kill it off.  If we already connected,
    // then leave the game first.  Either way close all sockets.
    for ( int i = 0 ; i < lbsockets.size ( ) ; i ++ )
        closesocket ( lbsockets [ i ] . socket ) ;
    lbsockets.resize ( 0 ) ;

    // Clear the socket read queue.
    while ( ! readSocketQueue.empty ( ) )
        readSocketQueue.pop ( ) ;

    // Clear the message queue.
    while ( ! gameMessageQueue.empty ( ) )
        gameMessageQueue.pop ( ) ;

    iListCon = -1 ;
    iServCon = -1 ;
    ownplayerhash = -1 ;

    mode = LB_NET_DISCONNECTED ;
}

/**
 ** Used by a client to connect to a server.
 **/
void LbNetImp::InitialiseClientTCP ( const char * serveraddress , int port )
{
    // Figure out the name.
    unsigned long ipaddress ;

    // Try and parse it.
    ipaddress = inet_addr ( serveraddress ) ;
    if ( ipaddress == INADDR_NONE )
    {
        // Try a lookup.
        hostent * f = gethostbyname ( serveraddress ) ;
        if ( f == NULL )
        {
            mode = LB_NET_ERROR ;
            error = "Couldn't resolve server hostname.  Check it is valid." ;
            return ;
        }
        memcpy ( & ipaddress , (*f).h_addr_list[0] , 4 ) ;
    }

    // Kill off any network stuff that is already running.
    ResetConnections ( ) ;

    int n = lbsockets.size ( ) ;
    lbsockets.resize ( n + 1 ) ;

    // Open the socket, reporting any errors.
    //   AF_INET means use IP ADDRESSING
    //   SOCK_STREAM means a stream as opposed to datagram ie. TCP not UDP.
    //   IPPROTO_TCP means use TCP/IP protocol suite.
    SOCKET hSock = socket ( AF_INET , SOCK_STREAM , IPPROTO_TCP ) ;
    if ( hSock == INVALID_SOCKET )
    {
        error = "Error opening the socket to connect to server." ;
        mode = LB_NET_ERROR ;
        return ;
    }

    SOCKADDR_IN sockName ;
    PSOCKADDR_IN pSockName ;
    pSockName = & sockName ;
    pSockName->sin_family = PF_INET ;
    pSockName->sin_addr.S_un.S_addr = ipaddress ;
    pSockName->sin_port = htons ( port ) ;

    int nRet = connect ( hSock , ( LPSOCKADDR ) pSockName , SOCKADDR_LEN ) ;
    if ( nRet == SOCKET_ERROR )
    {
       error = "Error connecting to server.  Check address is correct." ;
       mode = LB_NET_ERROR ;
       return ;
    }

    iServCon = n ;
    lbsockets [ n ] .socket = hSock ;
    lbsockets [ n ] .remoteAddress = sockName ;
    lbsockets [ n ] .readBufferTail = 0 ;
    lbsockets [ n ] .readBufferHead = 0 ;
    lbsockets [ n ] .writeBufferSize = 0 ;
    lbsockets [ n ].writeBuffer[ 0 ] = '\0' ;
    lbsockets [ n ] .error = false ;

    mode = LB_NET_CLIENT ;
}

/**
 ** Used by a server to set up the listening on a port which picks up clients
 ** attempting to connect.
 **/
void LbNetImp::InitialiseServerTCP ( int port )
{
    // Kill off any network stuff that is already running.
    ResetConnections ( ) ;

    // If we already running a server kill it off.  If we are connected,
    // then leave the game first.  Either way close all sockets.
    for ( int i = 0 ; i < lbsockets.size ( ) ; i ++ )
        closesocket ( lbsockets [ i ] . socket ) ;
    lbsockets.resize ( 0 ) ;


    int n = lbsockets.size ( ) ;
    lbsockets.resize ( n + 1 ) ;

    // Open the socket, reporting any errors.
    //   AF_INET means use IP ADDRESSING
    //   SOCK_STREAM means a stream as opposed to datagram ie. TCP not UDP.
    //   IPPROTO_TCP means use TCP/IP protocol suite.
    SOCKET hSock = socket ( AF_INET , SOCK_STREAM , IPPROTO_TCP );
    if ( hSock == INVALID_SOCKET )
    {
        error = "There was an error opening socket for server." ;
        mode = LB_NET_ERROR ;
        return ;
    }

    // Set the port and address, and report the error.
    SOCKADDR_IN sockName ;
    PSOCKADDR_IN pSockName ;
    pSockName = &sockName ;
    pSockName->sin_family = PF_INET ;
    pSockName->sin_addr.s_addr = INADDR_ANY;
    pSockName->sin_port = htons ( port ) ;
    int nRet = bind ( hSock , ( LPSOCKADDR ) pSockName , SOCKADDR_LEN ) ;
    if ( nRet == SOCKET_ERROR )
    {
       error = "Error binding to the server port.  Another " \
               "application may be using this port." ;
       mode = LB_NET_ERROR ;
       return ;
    }

    // Start listening for connections on the port.
    nRet = listen ( hSock , 20 ); // 20 = Max players.
    if ( nRet == SOCKET_ERROR )
    {
       error = "There was an error starting to listen on the port" ;
       mode = LB_NET_ERROR ;
       return ;
    }

    iListCon = n ;
    lbsockets [ n ] .socket = hSock ;
    lbsockets [ n ] .remoteAddress = sockName ;
    lbsockets [ n ] .readBufferTail = 0 ;
    lbsockets [ n ] .readBufferHead = 0 ;
    lbsockets [ n ] .writeBuffer[ 0 ] = '\0' ;
    lbsockets [ n ] .writeBufferSize = 0 ;
    lbsockets [ n ] .error = false ;

    mode = LB_NET_SERVER ;
    ownplayerhash = SocketToPlayerhash ( & lbsockets [ iListCon ] ) ;
}

/**
 ** Called to accept the connection when a client connects.
 **/
void LbNetImp::AcceptConnection (  )
{
    int n = lbsockets.size ( ) ;
    lbsockets.resize ( n + 1 ) ;

    int nLen = SOCKADDR_LEN ;
    SOCKET hSock = lbsockets [ iListCon ].socket , hNewSock ;
    SOCKADDR_IN remoteName ;

    // Accept the connection.
    hNewSock = accept ( hSock , (LPSOCKADDR)&remoteName , ( LPINT ) & nLen ) ;
    if ( hNewSock == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK )
    {
        error = "An error occured accepting an incoming connection." ;
        mode = LB_NET_ERROR ;
        return ;
    }

    // Create a new entry in the client connections list.
    lbsockets [ n ] .socket = hNewSock ;
    lbsockets [ n ] .remoteAddress = remoteName ;
    lbsockets [ n ] .readBufferTail = 0 ;
    lbsockets [ n ] .readBufferHead = 0 ;
    lbsockets [ n ]. writeBuffer[ 0 ] = '\0' ;
    lbsockets [ n ] .writeBufferSize = 0 ;
    lbsockets [ n ] .error = false ;

    string m ;
    char buf [ 33 ] ;
    itoa ( SocketToPlayerhash ( & lbsockets [ n ] ) , buf , 10 ) ;
    m = string ( "WELCOME " ) + string ( buf ) ;
    m += " Lightbikes 2001 (C) Uni. of Warwick Computing Society.\r\n" ;
    PutTCPMessage ( & lbsockets [ n ] , m.c_str ( ) ) ;
}

/**
 ** Called to read data.
 **/
void LbNetImp::ReadData  ( int c )
{
    if ( lbsockets [ c ] .error ) return ;

    // Read the data from the client.
    int nRet  = recv ( lbsockets [ c ].socket ,
                  ( LPSTR )&lbsockets [ c ].readBuffer +
                  lbsockets [ c ] .readBufferTail , MAX_READ_SIZE , 0 ) ;
    if ( nRet == SOCKET_ERROR && WSAGetLastError ( ) == WSAECONNRESET )
    // && WSAGetLastError ( ) != WSAEWOULDBLOCK )
    {
        // We have lost the connection.  A leave game message must be
        // generated, and the socket closed.
        CloseSocket ( lbsockets [ c ] ) ;
    }
    else lbsockets [ c ].readBufferTail += nRet ;

    // See if we have an end of line character indicating the message
    // is complete. If we do stick this on the tail of the queue.
    if ( lbsockets [ c ] . readBuffer [
         lbsockets [ c ] . readBufferTail - 1 ] == 10 )
        readSocketQueue.push ( &lbsockets [ c ] ) ;
}


/**
 ** Called by server to close connection and report the player as gone.
 **/
void LbNetImp::CloseSocket  ( LbSocket &s )
{
    s.error = true ;

    LbGameEvent e ;
    e.id = LB_GAME_PLAYERLEAVE ;
    e.message = "Connection closed." ;
    e.playerHash = SocketToPlayerhash ( &s ) ;
    gameMessageQueue.push ( e ) ;
}

/**
 ** Called to send data when socket is available to send data in queue.
 **/
void LbNetImp::SendData ( int c )
{
    if ( lbsockets [ c ] .error ) return ;

    // Send the contents of the writebuffer.
    if ( lbsockets [ c ] . writeBufferSize > 0 )
    {
        int nRet = send ( lbsockets [ c ] . socket ,
                         ( LPSTR ) & lbsockets [ c ] . writeBuffer ,
                         lbsockets [ c ] . writeBufferSize , 0 ) ;
        if ( nRet == SOCKET_ERROR && WSAGetLastError ( ) == WSAECONNRESET )
        {
            // We have lost the connection.  A leave game message must be
            // generated, and the socket closed.
            CloseSocket ( lbsockets [ c ] ) ;
        }
        else
        {
            lbsockets[ c ].writeBuffer[ 0 ] = '\0' ;
            lbsockets[ c ].writeBufferSize = 0 ;
        }
    }
}

/**
 ** Get the next message in the socket read queue.
 **/
bool LbNetImp::GetTCPMessage ( LbSocket * * s , char * message )
{
    // Get the message out of the buffer and return it in "message".
    if ( ! readSocketQueue.empty ( ) )
    {
        LbSocket * c = readSocketQueue.front ( ) ;
        int i = 0;
        while ( true )
        {
            message [ i ] =
                ( * c ) . readBuffer [ i + ( * c ) . readBufferHead ] ;
            if ( message [ i ] == '\n' )
                break ;
            i++ ;
        }

        message [ i + 1 ] = '\0' ;
        ( * c ) . readBufferHead += i + 1;
        * s = c ;

        // Only remove from queue when all messages are read.
        if ( ( * c ) . readBufferHead >= ( * c ) . readBufferTail )
            readSocketQueue.pop ( ) ;

        return true ;
    }
    return false ;
}

/**
 ** Add this message to the socket write buffer.
 **/
void LbNetImp::PutTCPMessage ( LbSocket * s , const char * message )
{
    // Stick it in the write buffer.
    strcat ( ( * s ) . writeBuffer , message ) ;
    ( * s ) . writeBufferSize += strlen ( message ) ;
}

/**
 ** Used by server to send a message to all clients.  Doesn't use the
 ** broadcast address, instead just sends a message to all players.
 **/
void LbNetImp::BroadcastTCPMessage ( const char * message )
{
    // Write to all sockets except the listening one.
    for ( int i = 0 ; i < lbsockets.size ( ) ; i ++ )
        if ( i != iListCon )
            PutTCPMessage ( & lbsockets [ i ] , message ) ;
}

/**
 ** Return a string describing the last error that occured.
 **/
string LbNetImp::GetError()
{
    return error ;
}

/**
 ** Shuts down the network cleanly, closing all sockets.
 **/
LbNetImp::~LbNetImp ( )
{
    for ( int i = 0 ; i < lbsockets.size ( ) ; i ++ )
        closesocket ( lbsockets [ i ] . socket ) ;
}

/**
 ** Create.
 **/
LbNetSys *CreateNetSys ( LbOSLayerSys *os_sys )
{
    LbNetSys *rval = new LbNetImp;
    assert ( rval != NULL ) ;

    rval->Init ( os_sys ) ;

    return rval;
}