/*********************************************************************************
    LbOSLayer.h
    Header file for the LightBikes2001 OS abstraction layer.

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black
       James Ross
       David Capps
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
#ifndef __LBOSLAYER__
#define __LBOSLAYER__

typedef Winamp_Input_Module* WA_InputPtr;
typedef Winamp_Output_Module* WA_OutputPtr;

enum LbOSLayerEventId
    {
    LB_OSEVENT_QUIT     =0,
    };

enum LbOSLayerKey
    {
    LB_OSKEY_LEFT       =0,
    LB_OSKEY_RIGHT      =1,
    };

struct LbOSLayerEvent
{
LbOSLayerEventId id;
};

struct LbOSLayerKeypress
{
LbOSLayerKey which;     //which key (duh)
BOOL down;              //down or not? (true=just pressed, false=just released)
int time;               //when pressed
int sequence;           //when pressed relative to other keypress messages
};

class LbOSLayerSys
{
public:

virtual bool PollEvent(LbOSLayerEvent &os_event)=0;
virtual void SwapDoubleBuffers()=0;
virtual int GLTextListBase()=0;
virtual int GetMS()=0; //get accurate millisecond count
virtual char* GetDesktop32()=0; //get an image of the desktop. Bwahaha!
virtual bool GetOSKey(LbOSLayerKeypress *data, int *num)=0;
    //I'm intending to mean fast accurate (eg. DirectInput under Win32) here
virtual bool SetupWinampCompatPlugins(WA_InputPtr *inp, WA_OutputPtr *outp)=0;
virtual void InitiateNetwork ( ) = 0 ;

// empty virtual destructor to ensure proper cleanup
virtual ~LbOSLayerSys(){}
};

/// Create an implimentation of the graphics subsystem.
LbOSLayerSys *CreateOSLayerSys();

#endif
