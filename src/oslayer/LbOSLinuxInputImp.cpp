/*********************************************************************************
    LbOSWin32Imp.cpp
    Class file for the LightBikes2001 Win32 OS Layer

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
        Henry Southgate

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
#include "LbOSLinuxImp.h"

#define DINPUT_BUFFERSIZE 32; //size of buffer for DirectInput

const int KeyMapping[] = { DIK_LEFT, DIK_RIGHT };
const int NumKeys = 2;


bool LbOSLayerInputImp::GetOSKey(LbOSLayerKeypress *data, int *num)
{
    DIDEVICEOBJECTDATA didod[32];
    DWORD dwElements;

    if (!di_InitOK) return false;

    dwElements = DINPUT_BUFFERSIZE;

    if (!lpDIDevKeyb) return false;

    HRESULT hr;

    hr = DIERR_INPUTLOST;

    while ( hr == DIERR_INPUTLOST ) {
        hr= lpDIDevKeyb->GetDeviceData(    sizeof(DIDEVICEOBJECTDATA),
                                                &didod[0],
                                                &dwElements,
                                                0 );

        if ( hr == DIERR_INPUTLOST ) {
            hr = lpDIDevKeyb->Acquire();
        }
    }

    if (SUCCEEDED(hr)) {
        if (*num < dwElements) {

        char msg[32];
        sprintf(msg,"Failed buffer oversize: num %d, received %d",*num, dwElements);
        MessageBox(NULL, msg, "LbOSWin32Imp::GetOSKey()", MB_ICONSTOP);
        return false;

        }

        *num = 0;
        int i,j;
        for (i=0; i<dwElements; i++) {
            for (j=0; j<NumKeys; j++) {
                if ( didod[i].dwOfs == KeyMapping[j] ) {
                    data[*num].which = (LbOSLayerKey)j;
                    data[*num].time = didod[i].dwTimeStamp - TickStart;
                    data[*num].sequence = didod[i].dwSequence;
                    data[*num].down = (didod[i].dwData & 0x80);
                    (*num)++;
                }
            }
        }
    } else {
        return false;
    }

    return true;

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

void LbOSLayerInputImp::Init(HINSTANCE hInst,HWND hWnd, int TickBegan)
{
    TickStart = TickBegan;
    di_InitOK = InitDInput(hInst,hWnd);
    tabdown = false ;
}

bool LbOSLayerInputImp::InitDInput(HINSTANCE hInst,HWND hWnd)
{

    lpDInput = NULL;
    lpDIDevKeyb = NULL;

    CoInitialize(NULL);

    HRESULT hr = CoCreateInstance(  CLSID_DirectInput,
                                    NULL,
                                    CLSCTX_INPROC_SERVER,
                                    IID_IDirectInput7,
                                    (void**)&lpDInput );
    if (FAILED(hr)) return false;
    lpDInput->AddRef();
    if (FAILED(lpDInput->Initialize( hInst, DIRECTINPUT_VERSION ))) return false;


    if ( FAILED(lpDInput->CreateDeviceEx(  GUID_SysKeyboard,
                                IID_IDirectInputDevice7,
                                (void**)&lpDIDevKeyb,
                                NULL ) )) return false;
    if ( FAILED(lpDIDevKeyb->SetDataFormat(&c_dfDIKeyboard) )) return false;
    if ( FAILED(lpDIDevKeyb->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) )) return false;

    if ( !lpDIDevKeyb)
        return false;

    DIPROPDWORD dipdw;

    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = DINPUT_BUFFERSIZE; //keyboard buffer size

    hr = lpDIDevKeyb->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );

    lpDIDevKeyb->Acquire();

    if (FAILED(hr)) return false;

    return true;
}


void LbOSLayerInputImp::OnWmChar(char c)
{
textkeybuffer.push_back(c);
}

void LbOSLayerInputImp::OnWmKeyDown(int c)
{
    if ( c == 9 ) tabdown = true ;
}

void LbOSLayerInputImp::OnWmKeyUp(int c)
{
    if ( c == 9 ) tabdown = false ;
}

bool LbOSLayerInputImp::IsTabDown ( )
{
    return tabdown ;
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
