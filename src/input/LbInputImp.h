/*********************************************************************************
    LbInputImp.h
    Header file for the LightBikes2001 Input subsystem classes

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       David Black

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
#ifndef __LBINPUTIMP__
#define __LBINPUTIMP__

class LbInputImp : public LbInputSys
{
public:

LbInputImp();
~LbInputImp();

virtual void Poll();
virtual void Init(LbOSLayerSys *os_sys);
virtual bool GetOSKey(LbOSLayerKeypress *data, int *num);
virtual char getNextTextKey();
virtual bool IsTabDown() ;
private:

LbOSLayerSys *os;
LbOSLayerInput *os_input;

BYTE keybuf[256];
};

#endif
