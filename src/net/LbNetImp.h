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

class LbNetImp : public LbNetSys
{
    public:
        LbNetImp();
        ~LbNetImp();

        virtual bool GetNextGameEvent (  LbGameEvent &e ) ;
        virtual void ProcessMessages ( ) ;

        virtual void Init(LbOSLayerSys *os_sys);
				virtual void PollSockets ( );
        virtual void ConnectToServer( char * ) ;
				virtual void InitiateServer( int ) ;
				virtual void InitiateNetwork() ;

				virtual void AcceptConnection (  ) ;
				virtual void MakeConnection (  ) ;
				virtual void ReadData ( int c  ) ;
				virtual void SendData ( int c  ) ;

				virtual void CloseNetwork() ;
				virtual bool GetTCPMessage ( char * address , char * message ) ;
				virtual void PutTCPMessage ( char * address , char * message ) ;

    private:
        LbOSLayerSys *os;

    LbGameEvent tempEvent;
    // Used by the server code
		struct LbConnection
		{
		    SOCKET socket;
		    SOCKADDR_IN remoteAddress;
		    char readBuffer [ 100 ] ;
		    int readBufferSize ;
		    char writeBuffer [ 100 ] ;
		    int writeBufferSize ;
		    int prevInReadQ ;
		    int prevInWriteQ ;
		};
		LbConnection connections [ MAX_CONNECTIONS ] ;
		int nCon ;
		int nHeadOfReadQ ;
		int nTailOfReadQ ;
		int nHeadOfWriteQ ;
		int nTailOfWriteQ ;
		int iListCon ;
};



#endif