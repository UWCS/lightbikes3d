/*********************************************************************************
    LbGame.cpp
    Class file for the LightBikes2001 Game subsystem

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
#include "LbGameImp.h"

LbGameImp::LbGameImp()
{
    os_sys=NULL;
    graph_sys=NULL;
    input_sys=NULL;
    sound_sys=NULL;
    net_sys=NULL;
}

LbGameImp::~LbGameImp()
{
    // make double sure things have been cleaned up.
    DeInitSubsystems();
}


int LbGameImp::RunGame()
{
    LbOSLayerEvent os_event;
    LbGameEvent game_event;
    LbVector target(0,0,0), up(0,1,0);
    LbVector *eye;
    float scroll,change;
    int count=0,lastms=0,fps=0, startms=0, keycount;
    int lpress=0, rpress=0;
    char msg[16] = "", inp[32] = "", keymsg[32] = "";
    LbOSLayerKeypress keys[32];

    InitSubsystems();

    quit_flag=false;
    eye = new LbVector(0,0,0);
    change = 0.1f;
    startms = lastms = os_sys->GetMS();

    string textbuf = "";
    char k ;
    string chatmessages [ 3 ] = { "" , "" ,"" } ;

    graph_sys->TriggerEffect(LB_GFX_FADEINTEXTURE);
    //net_sys->Test();
    sound_sys->PlayMusicFile("TRACK1.MP3"); //just for the moment

    int wave = sound_sys->CacheWaveFile("SOUND1.WAV"); //just for the moment

    while(!quit_flag)
    {
        scroll = scroll + (change* ( os_sys->GetMS() - startms) / 50.0f );
        startms = os_sys->GetMS();
        if (scroll > 15) change=-0.1f;
        if (scroll < -15) change=0.1f;
        if ( !(count++ % 20) ) {
            lastms = (os_sys->GetMS() - lastms);
            fps = lastms? (20000 / lastms) : 0;
            lastms = os_sys->GetMS();
            sprintf(msg,"FPS:%d",fps);
        }

        delete(eye);
        eye = new LbVector(scroll,0,-20);
        graph_sys->SetCamera(*eye,target,up);

        keycount = 32;
        inp[0] = 0;
        if (input_sys->GetOSKey(&keys[0], &keycount)) {
            for (int i=0; i<keycount; i++) {
                if (keys[i].down)
                    switch (keys[i].which) {
                        case LB_OSKEY_LEFT: sprintf(inp, "Left Key Press"); lpress++; sound_sys->PlayWaveFile(wave); break;
                        case LB_OSKEY_RIGHT:sprintf(inp, "Right Key Press"); rpress++; break;
                    }
            }
            if (keycount>0) sprintf(keymsg, "Left: %d, Right: %d", lpress, rpress);
        } else sprintf(inp, "Input Error");

        // Get text entered.
        while ( ( k = input_sys->getNextTextKey ( ) ) != 0 )
        {
            //Get ASCII codes.
            //char temp[20] ;
            //itoa ( (int)k , temp , 10 ) ;
            //MessageBox ( NULL , temp, "Werd Up Bitch", MB_ICONSTOP ) ;
            if ( k == '\r' )
            {

                ProcessCommand ( textbuf ) ;
                textbuf.erase() ;
            }
            else if ( k == 27 ) //esc
                textbuf.erase ( ) ;
            else if ( k == 8 ) //backspace
            {
                if ( textbuf.size ( ) > 0 )
                    textbuf.erase ( textbuf.size ( ) - 1 ) ;
            }
            else
                textbuf += k ;
        }

        graph_sys->StartFrame();
        // draw here
        graph_sys->SetTextColor(LbRGBAColor(1,0,0,1));
        graph_sys->DrawText(0.5f,0.82f,1.0f,"LightBikes3d");
        graph_sys->SetTextColor(LbRGBAColor(0,0,1,1));
        graph_sys->DrawText(0.6f,0.9f,1.0f,msg);
        graph_sys->SetTextColor(LbRGBAColor(1,1,0,1));
        graph_sys->DrawText(0.0f,0.25f,1.0f,inp);
        graph_sys->SetTextColor(LbRGBAColor(0,1,1,1));
        graph_sys->DrawText(0.0f,0.1f,1.0f,keymsg);

        // Display the typed text.
        graph_sys->SetTextColor ( LbRGBAColor ( 0 , 1 , 1 , 1 ) ) ;
        graph_sys->DrawText ( 0.0f , 0.02f , 0.75f , textbuf.c_str ( ) ) ;

        // Display the chat or status messages.
        graph_sys->SetTextColor ( LbRGBAColor ( 0 , 1 , 1 , 1 ) ) ;
        graph_sys->DrawText ( 0.0f , 0.9f , 0.5f , chatmessages[0].c_str ( ) ) ;
        graph_sys->SetTextColor ( LbRGBAColor ( 0 , 1 , 1 , 1 ) ) ;
        graph_sys->DrawText ( 0.0f , 0.82f , 0.5f , chatmessages[1].c_str ( ) ) ;
        graph_sys->SetTextColor ( LbRGBAColor ( 0 , 1 , 1 , 1 ) ) ;
        graph_sys->DrawText ( 0.0f , 0.74f , 0.5f , chatmessages[2].c_str ( ) ) ;

        graph_sys->EndFrame();

        // poll the event queue.
        while(os_sys->PollEvent(os_event))
        {
            /* OS event:
            **  Minimise/retstore
            **  Active/deactivate
            **  Close
            **  Normal keypress (for menus, etc.)
            **  Return: Send player quip
            */
            // handle the event.
            switch ( os_event.id )
            {
                case LB_OSEVENT_QUIT:
                    quit_flag=true;// bye bye...
                    break;
            }
            // ignore unknown events...
        }



        if ( net_sys->GetStatus ( ) != LB_NET_DISCONNECTED )
        {

            // Process Network messages (Networking) ie. convert them from strings
            // and packets to game messages, and add them to the network module's
            // queue of game messages.
            net_sys->ProcessMessages();

            // Probably won't do this ultimately.
            net_sys->PollSockets();


        // Read in Network messages (Networking)...
            /* Game Events - Messages from network:
           **  Start game
           **  Player position updates
           **  End game
           **  Send level data
           */

        // Deal with network messages.
        while ( net_sys->GetNextGameEvent ( game_event ) )
        {
            switch ( game_event.id )
            {
                case LB_GAME_PLAYERJOIN:
                case LB_GAME_PLAYERLEAVE:
                case LB_GAME_HANDCHANGE:
                case LB_GAME_NEWGAME:
                case LB_GAME_CHANGESERVER :
                case LB_GAME_RESETSERVER :
                     MessageBox ( NULL , "message from server/client" , "Werd Up", MB_ICONSTOP ) ;
                break;

                // Deal with incoming chat messages.
                case LB_GAME_CHAT:
                {
                    chatmessages [ 0 ] = chatmessages [ 1 ] ;
                    chatmessages [ 1 ] = chatmessages [ 2 ] ;
                    chatmessages [ 2 ] =
                        string ("<" );
                        //GetPlayerHandle (game_event.playerHash) +
//                        string ( "> " ) + game_event.message;
                }
                break;
            }

            // Rebroadcast iff we are server and it didn't come from us,
            // because otherwise we would have already broadcast it.
            if ( net_sys->GetStatus ( ) == LB_NET_SERVER &&
                 game_event.playerHash != net_sys->GetOwnPlayerHash ( ) )
                    net_sys->SendGameEvent ( game_event , false ) ;
        }

    }

        // Update game state (Game Logic)...
            /* Updateing actions:
            **  Poll for Input events
            **  Interpret player input
            **  Proccess Network messages
            **  Updating bikes
            **  Check for collisions
            **  MENU (stuff)
            **      Interface?
            **      Messages --> Network
            */

        // Send network -> send game messages
        // send messages to network module to send to server.
    }

    delete eye;

    DeInitSubsystems();

    return 0;
}

/**
 ** Should return player's actual handle (name) as a string,
 ** currently just gives number.
 **/
string LbGameImp::GetPlayerHandle ( int playerhash )
{
    char temp[20] ;
    itoa ( playerhash , temp , 10 ) ;
    return string(temp);
}

/**
 ** Takes a command entered, and deals with it appropriately.
 **/
void LbGameImp::ProcessCommand ( string t )
{
    LbGameEvent e ;
    t += " " ;
    if ( t.substr ( 0 , 1 ) == "\\" || t.substr ( 0 , 1 ) == "/" )
    {
        int m = t.find_first_of ( " " ) ;
        string cmd = t.substr ( 1 , m - 1 ) ,
               prm = t.substr ( m + 1 , t.size ( ) - m - 2 ) ;

        if ( cmd == "quit" )
            { quit_flag = true ; return ; }
        else if ( cmd == "connect" )
            { net_sys->ConnectToServer ( prm.c_str() , LB_SERVER_TCP_PORT ) ; return ; }
        else if ( cmd == "startserver" )
            { net_sys->InitiateServer ( prm.c_str() , LB_SERVER_TCP_PORT ) ; return ; }
        else if ( cmd == "join" )
            e.id = LB_GAME_PLAYERJOIN ;
        else if ( cmd == "hand" )
            e.id = LB_GAME_HANDCHANGE ;
        else if ( cmd == "leave" )
            e.id = LB_GAME_PLAYERLEAVE ;

        e.message=prm;
    }
    else
    {
        e.id = LB_GAME_CHAT ;
        e.message=t;
    }

    e.playerHash = net_sys->GetOwnPlayerHash ( ) ;
    net_sys->SendGameEvent ( e , ( net_sys->GetStatus ( ) == LB_NET_SERVER ) ) ;
}

void LbGameImp::InitSubsystems()
{
    os_sys=CreateOSLayerSys();
    graph_sys=CreateGraphicsSys(os_sys);
    input_sys=CreateInputSys(os_sys);
    sound_sys=CreateSoundSys(os_sys);
    net_sys=CreateNetSys(os_sys);
}

void LbGameImp::DeInitSubsystems()
{
    if(graph_sys!=NULL)
    {
        delete graph_sys;
        graph_sys=NULL;
    }

    if(input_sys!=NULL)
    {
        delete input_sys;
        input_sys=NULL;
    }

    if(sound_sys!=NULL)
    {
        delete sound_sys;
        sound_sys=NULL;
    }

    if(net_sys!=NULL)
    {
        delete net_sys;
        net_sys=NULL;
    }

    if(os_sys!=NULL)
    {
        delete os_sys;
        os_sys=NULL;
    }
};

LbGameSys *CreateGameSys()
{
    LbGameSys *rval=new LbGameImp;
    assert(rval!=NULL);

    return rval;
}
