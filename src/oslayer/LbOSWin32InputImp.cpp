/*********************************************************************************
    LbOSWin32Imp.cpp
    Class file for the LightBikes2001 Win32 OS Layer

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

#include "LbStandard.h"
#include "LbPublic.h"
#include "LbOSWin32Imp.h"

bool LbOSLayerInputImp::GetOSKey(LbOSLayerKeypress *data, int *num)
{
return false;
}
    //I'm intending to mean fast accurate (eg. DirectInput under Win32) here
char LbOSLayerInputImp::getNextTextKey ( )
{
    if ( textkeybuffer.empty ( ) )
         return 0 ;

    char t = textkeybuffer.front ( ) ;
    textkeybuffer.pop_front ( ) ;
    return t ;
}

void LbOSLayerInputImp::Init(HINSTANCE hInst,HWND hWnd)
{
}

void LbOSLayerInputImp::OnWmChar(char c)
{
textkeybuffer.push_back(c);
}


LbOSLayerInputImp::LbOSLayerInputImp()
{
lpDInput=NULL;
lpDIDevKeyb=NULL; 
}

LbOSLayerInputImp::~LbOSLayerInputImp()
{
if(lpDInput!=NULL)
    lpDInput->Release();

if(lpDIDevKeyb!=NULL)
    lpDIDevKeyb->Release();
}