/*********************************************************************************
    LbGame.h
    Header file for the LightBikes2001 Game subsystem classes

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
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
#ifndef __LBPLAYER__
#define __LBPLAYER__

class LbPlayer
{
    public:
		// Get player hash.
		virtual int GetHash ( ) = 0 ;

		// Get player handle.
		virtual string GetHandle ( ) = 0 ;

		// Get player kills.
		virtual int GetKills ( ) = 0 ;

		// Get player deaths.
		virtual int GetDeaths ( ) = 0 ;

		// Get player ping.
		virtual int GetPing ( ) = 0 ;

		// Get player kills.
		virtual bool IsValid ( ) = 0 ;

		// Set player has.
		virtual void SetHash ( int h ) = 0 ;

		// Set player handle.
		virtual void SetHandle ( string h) = 0 ;

		// Set player kills.
		virtual void SetKills ( int k ) = 0 ;

		// Set player deaths.
		virtual void SetDeaths ( int d ) = 0 ;

		// Set player ping.
		virtual void SetPing ( int p ) = 0 ;

		// Set player valid.
		virtual void SetValid ( bool v ) = 0 ;

		// Get the graphics bike.
		virtual LbGraphicsBike * GetBike ( ) = 0 ;

		// Get the position.
		virtual LbVector * GetPosition ( ) = 0 ;

		// Get the graphics bike.
		virtual void SetBike ( LbGraphicsBike * g ) = 0 ;

		// Get the position.
		virtual void SetPosition ( LbVector * v ) = 0 ;

		// Set player valid.
		virtual void SetPlaying ( bool v ) = 0 ;

		// Get player kills.
		virtual bool IsPlaying ( ) = 0 ;

		// Set player the direction.
		virtual void SetDirection ( int d ) = 0 ;

		// Get player direction.
		virtual int GetDirection ( ) = 0 ;

		// Empty virtual destructor to ensure proper cleanup.
		virtual ~LbPlayer ( ) { }

	private:
		int hash ;
		string handle ;
		int kills ;
		int death ;
		int ping ;
		bool valid ;
		LbGraphicsBike * bike ;
		LbVector * bikepos ;
		bool playing ;
		int direction ;
};
#endif