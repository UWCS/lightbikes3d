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

LbNetImp::LbNetImp()
{
}

/**
 ** Process the messages waiting on the queue.  If this is a server then it
 ** sends out the messages to the clients.
 **/
void LbNetImp::ProcessMessages()
{
    // Check the TCP message queue.
    char addressString [20] , messageString [ 20 ] ;
    while ( os->GetTCPMessage ( (char*)&addressString , (char*)&messageString ) )
    {
        // Translate each of the messages to game messages and add to the game queue.
        MessageBox ( NULL , (char*)&addressString , (char*)&messageString , MB_ICONSTOP ) ;
    }

    // Check the UDP message queue.
        // Translate the messages to game messages and add to the game queue.
}

/**
 ** Return the next message from the game message queue.
 ** things like "start game" etc.
 **/
void LbNetImp::GetNextGameMessage ( )
{

}

/**
 ** Initialisation of networking subsystem.
 **/
void LbNetImp::Init(LbOSLayerSys *os_sys)
{
    // Store the reference to the OS layer.
    os = os_sys;

    // Start the OS level aspects of the network.
    os->InitiateNetwork();

    // THIS LINE WILL BE USED BY THE CLIENT.
    // os_sys->ConnectToServer ( LB_SERVER_IP_ADDRESS );

    // ASSUME THAT WE ARE RUNNING A SERVER.
    os->InitiateServer ( LB_SERVER_TCP_PORT ) ;

    // TO DO: Set up a players list.

    // TO DO: Set up a message queue.
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