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
#include <string.h>

// Overview.
//
// When running as a client the software connects to a specified server, when
// running as a server the application listens on a port and opens connections
// to clients that attempt to connect.  In either mode, send and receive
// buffers are maintained as part of the socket structure.  In server mode
// there has to be a mapping between players and open connections (sockets).
//
// TCP and UDP connections are maintained (currently just TCP).
//
// The non-urgent (player join / chat etc.) game messages are translated to TCP
// text and added to the queue and sent whenever the network queue is serviced.
// Similarly received messages are decoded and retreived by the game logic
// modules.

LbNetImp::LbNetImp()
{

}

// -- Methods to translate game messages to data to network data and v. v. --

/**
 ** Process the messages waiting on the queue.  If this is a server then it
 ** sends out the messages to the clients.
 **/
void LbNetImp::ProcessMessages()
{
    // Check the TCP message queue.
    char addStr [20] , mesStr [ 20 ] ;
    while ( GetTCPMessage ( (char*)&addStr , (char*)&mesStr ) )
    {
        // Translate each of the messages to game messages and add to the game queue.

        // If we are the server...
        //   If it is a JOIN message...
        //    - inform all players of the new player.
        //   If it is a HAND message...
        //    - inform all players of the change of handle.
        //   If it is a SAY message...
        //    - inform all players of the chat message.

        // As a client, create game messages.
        // CHAT message.
        if ( memcmp ( (char*)&mesStr , "CHAT " , 4 ) == 0 )
        {
            tempEvent.id = LB_GAME_CHATMESSAGE ;
            memcpy( (char*)&tempEvent.message , (char*)&mesStr , 19 ) ;
            tempEvent.playerId = 1 ;
        }
        // JOIN message.
        else if ( memcmp ( (char*)&mesStr , "JOIN " , 4 ) == 0 )
        {
        }
        // SAY message.
        else if ( memcmp ( (char*)&mesStr , "SAY " , 3 ) == 0 )
        {
        }
        // RESET message.
        else if ( memcmp ( (char*)&mesStr , "RESET " , 5 ) == 0 )
        {
        }
    }

    // Check the UDP message queue.
        // Translate the messages to game messages and add to the game queue.
}

/**
 ** Return and remove the next message from the game message queue.
 ** things like "start game" etc.
 **/
bool LbNetImp::GetNextGameEvent ( LbGameEvent &e )
{
    e.id = tempEvent.id ;
    memcpy ( (char*)&e.message , (char*)&tempEvent.message  , 19 ) ;
    e.playerId = tempEvent.playerId ;

    return true;
}



/**
 ** Checks for sockets waiting to be serviced.
 **/
void LbNetImp::PollSockets ( )
{
	// Set the timeout.
	timeval timeout;
	timeout.tv_sec = 0 ;
	timeout.tv_usec = 0 ;

  // Construct read sockets.
  fd_set readscks , writescks , errscks;
  errscks.fd_count = writescks.fd_count = readscks.fd_count = nCon ;
  for (int i = 0 ; i < nCon ; i++ )
 		errscks.fd_array[i] = writescks.fd_array[i] = readscks.fd_array[i] = connections[i].socket ;

  // Call to check the sockets.
  select ( 0 , (fd_set * )&readscks , (fd_set * )&writescks , (fd_set * )&errscks , &timeout );

  // Check for data to read or incoming connections.
  for ( int i = 0 ; i < readscks.fd_count ; i ++ )
  {
		for ( int j = 0 ; j < nCon ; j ++ )
				if ( readscks.fd_array[i] == connections[j].socket )
				{
					if ( j == iListCon ) AcceptConnection ( ) ;
				  else ReadData ( j ) ;
			 }
	}

  // Check for sockets ready to write.
  for ( int i = 0 ; i < writescks.fd_count ; i ++ )
  {
		for ( int j ; j < nCon ; j ++ )
				if ( writescks.fd_array[i] == connections[j].socket )
					//if ( j == iServCon ) AcceptConnection ( ) ;
				  //else
				  SendData ( j ) ;
	}

}

/**
 ** Initialisation of networking subsystem.
 **/
void LbNetImp::Init(LbOSLayerSys *os_sys)
{
    // Store the reference to the OS layer.
    os = os_sys;

    // Start the OS level aspects of the network.
    InitiateNetwork();

    // THIS LINE WILL BE USED BY THE CLIENT.
    // os_sys->ConnectToServer ( LB_SERVER_IP_ADDRESS );

    // ASSUME THAT WE ARE RUNNING A SERVER.
    InitiateServer ( LB_SERVER_TCP_PORT ) ;

    // TO DO: Set up a players list.

    // TO DO: Set up a message queue.
}


/*
** Set up the networking API.  In this case it's Berkeley sockets for
** Windows which is called WINSOCK.
*/
void LbNetImp::InitiateNetwork()
{
    // This structure can be used to get information about the version of
    // winsock in use.
    WSAData stWSAData;

    // Startup version 1.1 of Winsock, report errors.
    if ( WSAStartup ( MAKEWORD ( 2 , 0 ) , &stWSAData ) != 0 )
        MessageBox( NULL , "An error occured while initialising WinSock." ,
                    "Error" , MB_ICONSTOP ) ;
			iListCon = -1 ;
      nCon = 0;
      nHeadOfReadQ = -1;
      nTailOfReadQ = -1;
}

/*
** Used by a client to connect to a server.
*/
void LbNetImp::ConnectToServer( char * dottedServerAddress )
{
    // Used for return values.
    int nRet;

    // Get the server address as a long number rather than a string.
    unsigned long serverAddress = inet_addr ( dottedServerAddress ) ;

    // Set up a socket to connect to the server.
    SOCKET hClientSock = socket ( AF_INET , SOCK_STREAM , 0 ) ;
    if ( hClientSock == INVALID_SOCKET )
        MessageBox ( NULL , "There was an error opening the socket." ,
                     "Error" , MB_ICONSTOP ) ;

    // Request Asynchronous notification for most events.
    //os -> SetAsync ( hClientSock , FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE ) ;

    MessageBox ( NULL, "connecting to server...", "fish", MB_ICONSTOP);

}

/*
** Used by a server to set up the listening on a port which picks up clients
** attempting to connect.
*/
void LbNetImp::InitiateServer ( int port )
{
    // Used for return values.
    int nRet;

    // Open the socket, reporting any errors.
    //   AF_INET means use IP ADDRESSING
    //   SOCK_STREAM means a stream as opposed to datagram ie. TCP not UDP.
    //   0 means use TCP/IP protocol suite.
    SOCKET hSock = socket ( AF_INET , SOCK_STREAM , 0 );
    if ( hSock == INVALID_SOCKET )
        MessageBox ( NULL , "There was an error opening socket." ,
                     "Error" , MB_ICONSTOP ) ;

    // Set the socket to Asynchronous, we want to be notified of incoming data
    // or connections by a windows message.
    //os -> SetAsync ( hSock , FD_ACCEPT | FD_READ | FD_WRITE | FD_CLOSE ) ;

    // Set the port and address, and report the error.
    SOCKADDR_IN sockName;
    PSOCKADDR_IN pSockName;
    pSockName = &sockName;
    pSockName->sin_family = PF_INET;
    pSockName->sin_addr.S_un.S_addr = (u_long) inet_addr ( "127.0.0.1" ) ;
    pSockName->sin_port = htons ( port ) ;
    nRet = bind ( hSock , (LPSOCKADDR) pSockName, SOCKADDR_LEN ) ;
    if ( nRet == SOCKET_ERROR )
    {
       MessageBox ( NULL, "Error binding to the server port.  Another " \
                          "application may be using this port." , "Error" ,
                          MB_ICONSTOP ) ;
    }

    // Start listening for connections on the port.
    nRet = listen ( hSock , 20 ); // 20 = Max players.
    if ( nRet == SOCKET_ERROR )
    {
       MessageBox ( NULL, "There was an error starting to listen" \
                    "on the port" , "Error" , MB_ICONSTOP ) ;
    }
		iListCon = nCon ;
    connections[nCon].socket = hSock ;
    connections[nCon].remoteAddress = sockName ;
    connections[nCon].readBufferSize = 0 ;
    connections[nCon].writeBufferSize = 0 ;
    nCon ++ ;
}

// Called to accept the connection when a client connects.
void LbNetImp::AcceptConnection (  )
{
	  int nLen = SOCKADDR_LEN , nRet;
    SOCKET hSock = connections[iListCon].socket , hNewSock;
    SOCKADDR_IN remoteName ;

		// Check connection limit not exceeded.
    if ( nCon >= MAX_CONNECTIONS - 1 )
    {
    		MessageBox ( NULL, "Too many clients trying to connect " ,
                           "Error.", MB_ICONSTOP ) ;
    }

    // Accept the connection.
    hNewSock = accept ( hSock , (LPSOCKADDR)&remoteName , (LPINT) &nLen ) ;
    if ( hNewSock == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK )
    		MessageBox ( NULL, "An error occured accepting an incoming "\
                           "connection.", "Error.", MB_ICONSTOP ) ;

		// Create a new entry in the client connections list.
		connections[nCon].socket = hNewSock ;
		connections[nCon].remoteAddress = remoteName ;
		connections[nCon].readBufferSize = 0 ;
		sprintf ( connections[nCon].writeBuffer , "Welcome to Lightbikes 2001" );
		connections[nCon].writeBufferSize = 26 ;
		connections[nCon].prevInReadQ = -1 ;
		nCon++;
}

// Called to read data.
void LbNetImp::ReadData  ( int c )
{
    int nRet ;

	  // Read the data from the client.  SHOULD READ BIGGER CHUNKS.
	  nRet = recv ( connections[ c ].socket ,
	                (LPSTR)&connections[ c ].readBuffer +
	                connections[c].readBufferSize , 1 , 0 );
	 if ( nRet == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK )
	    MessageBox ( NULL, "An error occured reading data from the "\
	                 "connection.", "Error.", MB_ICONSTOP ) ;
	 else connections[c].readBufferSize += nRet ;

	 // See if we have an end of line character indicating the message
	 // is complete.
	 if ( connections[ c ].readBuffer[
	      connections[c].readBufferSize - 1 ] == 10 )
	 {
	      // If we do stick this on the tail of the queue.
	      if (nHeadOfReadQ == -1 ) nHeadOfReadQ = c ;
	      if ( nTailOfReadQ != c && nTailOfReadQ != -1 )
	           connections[nTailOfReadQ].prevInReadQ = c;
	      nTailOfReadQ = c;
   }
}

// Called by client when server accepts connection.
void LbNetImp::MakeConnection ( )
{
    MessageBox(NULL, "client connected to server", "fish", MB_ICONSTOP);
    // Create a new entry in the client connections list.

}

// Called to send data when socket is available to send data in queue.
void LbNetImp::SendData ( int c )
{
    int nRet;

		// Find the appropriate connection.

		// Send the contents of the writebuffer, no MTU checking.
		if ( connections[ c ].writeBufferSize > 0 )
	  {
	    nRet = send ( connections[ c ].socket ,
	                  (LPSTR)&connections[ c ].writeBuffer ,
                    connections[ c ].writeBufferSize , 0 );
			if ( nRet == SOCKET_ERROR &&
							WSAGetLastError() != WSAEWOULDBLOCK )
				MessageBox ( NULL, "An error occured writing data to the "\
                                       "socket.", "Error.", MB_ICONSTOP ) ;
			else connections[ c ].writeBufferSize = 0 ;
		}
}

/*
** Get the next message in the queue.
*/
bool LbNetImp::GetTCPMessage ( char * address , char * message )
{
    if ( nHeadOfReadQ == -1 ) return false;

    sprintf( address , inet_ntoa ( connections[ nHeadOfReadQ ].remoteAddress.sin_addr ) );
    sprintf( message , connections[ nHeadOfReadQ ].readBuffer );
    connections [ nHeadOfReadQ ].readBufferSize = 0 ;

    nHeadOfReadQ = connections[ nHeadOfReadQ ].prevInReadQ;
    return true;
}

/*
** Add this message to the queue.
*/
void LbNetImp::PutTCPMessage ( char * address , char * message )
{
    // Find the correct socket.
    unsigned long lAddress = inet_addr ( address ) ;
    int i;
    for ( i = 0 ; i < nCon ; i++ )
        if ( connections [ i ].remoteAddress.sin_addr.S_un.S_addr == lAddress )
            break;

    // Stick it in the write buffer. SHOULDN'T OVERWRITE OLD BUFFER.
    sprintf ( (char*)&connections [ i ].writeBuffer , message ) ;
    connections [ i ].writeBufferSize = strlen ( message ) ;
}

/*
** Shuts down the network cleanly.
*/
void LbNetImp::CloseNetwork()
{
    for ( int i = 0 ; i < nCon ; i++ )
        closesocket ( connections [ i ].socket ) ;
}




LbNetImp::~LbNetImp()
{
}

LbNetSys *CreateNetSys(LbOSLayerSys *os_sys)
{
    LbNetSys *rval=new LbNetImp;
    assert(rval!=NULL);

    rval->Init(os_sys);

    return rval;
}