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
// the server.  Players are assigned a unique playerHash which is constant
// while they are connected, the game logic can convert this to player objects.
//
// TCP and UDP connections are maintained (currently just TCP).
//
// The non-urgent (player join / chat etc.) game messages are translated to TCP
// text and added to the queue and sent whenever the network queue is serviced.
// Similarly received messages are decoded and retreived by the game logic
// modules.
//
// TO DO:
//        Threading to make polling better.
//        Use UDP, define UDP packets.

LbNetImp::LbNetImp ( )
{

}

/**
 ** Returns LB_NET_CONNECTEDTOSERVER , LB_NET_SERVER or LB_NET_DISCONNECTED.
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
    else if ( mode == LB_NET_CONNECTEDTOSERVER )
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
        string completemessage = messagebuf ;
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
        if ( mode == LB_NET_SERVER &&
             playerhash != SocketToPlayerhash ( s ) &&
             t.id != LB_GAME_PLAYERJOIN )
            MessageBox ( NULL , "Client sent invalid data." ,
                "Error" , MB_ICONSTOP ) ;

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
        if ( ( int ) ( lbsockets [ i ].remoteAddress.sin_addr.S_un.S_addr + i ) == playerhash )
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
        errscks.fd_array[i] = writescks.fd_array[i] = readscks.fd_array[i] = lbsockets[i].socket ;
    }

    // Call to check the sockets.
    select ( 0 , ( fd_set * ) & readscks , ( fd_set * ) & writescks , (fd_set * ) & errscks , &timeout );

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

    // Check for sockets ready to write.
    for (i = 0 ; i < errscks.fd_count ; i ++ )
    {
         //MessageBox ( NULL, "err scks" ,
         //           "Error" , MB_ICONSTOP ) ;
        for ( int j = 0 ; j < lbsockets.size ( ) ; j ++ )
            if ( errscks.fd_array [ i ] == lbsockets [ j ].socket )
                ;//;
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
void LbNetImp::ConnectToServer ( const char * dottedServerAddress , int port )
{
    // Kill off any network stuff that is already running.
    ResetConnections ( ) ;

    int n = lbsockets.size ( ) ;
    lbsockets.resize ( n + 1 ) ;

    // Get the server address as a long number rather than a string.
    unsigned long serverAddress = inet_addr ( dottedServerAddress ) ;

    // Set up a socket to connect to the server.
    SOCKET hSock = socket ( AF_INET , SOCK_STREAM , 0 ) ;
    if ( hSock == INVALID_SOCKET )
        MessageBox ( NULL , "There was an error opening the socket." ,
                     "Error" , MB_ICONSTOP ) ;

    SOCKADDR_IN sockName ;
    PSOCKADDR_IN pSockName ;
    pSockName = & sockName ;
    pSockName->sin_family = PF_INET ;
    pSockName->sin_addr.S_un.S_addr = serverAddress ;
    pSockName->sin_port = htons ( port ) ;

    int nRet = connect ( hSock , ( LPSOCKADDR ) pSockName , SOCKADDR_LEN ) ;
    if ( nRet == SOCKET_ERROR )
    {
       MessageBox ( NULL, "Error binding to the port to connect to server.  "\
                    "Another application may be using this port." ,
                    "Error" , MB_ICONSTOP ) ;
    }

    iServCon = n ;
    lbsockets [ n ] .socket = hSock ;
    lbsockets [ n ] .remoteAddress = sockName ;
    lbsockets [ n ] .readBufferSize = 0 ;
    lbsockets [ n ] .writeBufferSize = 0 ;
    lbsockets [ n ] .error = false ;

    mode = LB_NET_CONNECTEDTOSERVER ;
}

/**
 ** Used by a server to set up the listening on a port which picks up clients
 ** attempting to connect.
 **/
void LbNetImp::InitiateServer ( const char * address , int port )
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

    // Get the server address as a long number rather than a string.
    unsigned long serverAddress = inet_addr ( address ) ;

    // Open the socket, reporting any errors.
    //   AF_INET means use IP ADDRESSING
    //   SOCK_STREAM means a stream as opposed to datagram ie. TCP not UDP.
    //   0 means use TCP/IP protocol suite.
    SOCKET hSock = socket ( AF_INET , SOCK_STREAM , 0 );
    if ( hSock == INVALID_SOCKET )
        MessageBox ( NULL , "There was an error opening socket." ,
                     "Error" , MB_ICONSTOP ) ;

    // Set the port and address, and report the error.
    SOCKADDR_IN sockName ;
    PSOCKADDR_IN pSockName ;
    pSockName = &sockName ;
    pSockName->sin_family = PF_INET ;
    pSockName->sin_addr.S_un.S_addr = serverAddress ;
    pSockName->sin_port = htons ( port ) ;
    int nRet = bind ( hSock , ( LPSOCKADDR ) pSockName , SOCKADDR_LEN ) ;
    if ( nRet == SOCKET_ERROR )
    {
       MessageBox ( NULL, "Error binding to the server port.  Another " \
                          "application may be using this port." , "Error" ,
                          MB_ICONSTOP ) ;

                                 int q = WSAGetLastError() ;
                                 assert ( q == 0 );
    }

    // Start listening for connections on the port.
    nRet = listen ( hSock , 20 ); // 20 = Max players.
    if ( nRet == SOCKET_ERROR )
    {
       MessageBox ( NULL, "There was an error starting to listen" \
                    "on the port" , "Error" , MB_ICONSTOP ) ;
    }

    iListCon = n ;
    lbsockets [ n ] .socket = hSock ;
    lbsockets [ n ] .remoteAddress = sockName ;
    lbsockets [ n ] .readBufferSize = 0 ;
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
        MessageBox ( NULL , "An error occured accepting an incoming "\
                            "connection." , "Error." , MB_ICONSTOP ) ;

    // Create a new entry in the client connections list.
    lbsockets [ n ] .socket = hNewSock ;
    lbsockets [ n ] .remoteAddress = remoteName ;
    lbsockets [ n ] .readBufferSize = 0 ;
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
                  lbsockets [ c ] .readBufferSize , MAX_READ_SIZE , 0 ) ;
    if ( nRet == SOCKET_ERROR && WSAGetLastError ( ) == WSAECONNRESET )
    // && WSAGetLastError ( ) != WSAEWOULDBLOCK )
    {
        // We have lost the connection.  A leave game message must be
        // generated, and the socket closed.
        CloseSocket ( lbsockets [ c ] ) ;
    }
    else lbsockets [ c ].readBufferSize += nRet ;

    // See if we have an end of line character indicating the message
    // is complete. If we do stick this on the tail of the queue.
    if ( lbsockets [ c ] . readBuffer [
         lbsockets [ c ] . readBufferSize - 1 ] == 10 )
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
                         (LPSTR)&lbsockets [ c ] . writeBuffer ,
                         lbsockets [ c ] . writeBufferSize , 0 ) ;
        if ( nRet == SOCKET_ERROR && WSAGetLastError ( ) == WSAECONNRESET )
        {
            // We have lost the connection.  A leave game message must be
            // generated, and the socket closed.
            CloseSocket ( lbsockets [ c ] ) ;
        }
        else lbsockets[ c ].writeBufferSize = 0 ;
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
        readSocketQueue.pop ( ) ;
        memcpy ( message , ( * c ) . readBuffer , ( * c ) . readBufferSize ) ;
        message [ ( * c ) . readBufferSize ] = '\0';
        ( * c ) . readBufferSize = 0 ;
        * s = c ;
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
    strcpy ( ( char * ) & ( ( * s ) . writeBuffer ) + ( * s ) . writeBufferSize , message ) ;
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