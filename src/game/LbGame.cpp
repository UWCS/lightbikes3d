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
    // Make double sure things have been cleaned up.
    DeInitSubsystems();
}

int LbGameImp::RunGame()
{
    //FILE *fp=fopen("c:\\dbglog.txt","W+");
    //fputs("Testing 123",fp);

    LbOSLayerEvent os_event;
    LbGameEvent game_event;
    LbVector up(0, 0, 1);
    LbVector eye, target;
    float scroll,change=5.0f;
    int count=0,lastms=0,fps=0, startms=0, keycount;
    int lpress=0, rpress=0;
    char msg[16] = "", inp[32] = "", keymsg[32] = "";
    LbOSLayerKeypress keys[32];

    // DEBUG CODE - TO BE REMOVED WHEN GameLogic IS WORKING
    //Directions        // 0 = y+, 1 = x+, 2 = y-, 3 = x-
    float step = 0;
    float dist = 0;
    float Vacc = 0;
    // END

    InitSubsystems();

    quit_flag=false;
    eye = LbVector(0, -20, 10);
    target = LbVector(0, 0, 0);
    startms = lastms = os_sys->GetMS();

    char k ;
    int i , p ;
    bool showscores;

    // Initialise players.
    for ( i = 0 ; i < MAX_PLAYERS ; i++ )
    {
        players [ i ] = new LbPlayerImp ( ) ;
        players [ i ]->SetValid ( false ) ;
        players [ i ]->SetPlaying ( false ) ;
    }

    // Default to not showing scores.
    showscores = false ;

    // Clear the buffer that stores the typed text.
    string textbuf = "" ;

    // Set our handle to the default for new players.
    ownhandle = "Unnamed" ;

    // Clear the typed messages.
    for ( i = 0 ; i < MAX_MESSAGE_LINES ; i ++ )
        txtmsgs [ i ] = "" ;

    // Clear the scoreboard.
    for ( i = 0 ; i < MAX_SCOREBOARD_LINES ; i ++ )
        scoremsgs [ i ] = "" ;

    // Removed fade-in effect.  --JGR
    //graph_sys->TriggerEffect(LB_GFX_FADEINTEXTURE);
    sound_sys->PlayMusicFile ( "TRACK1.MP3" ) ; //just for the moment

    int wave = sound_sys->CacheWaveFile ( "SOUND1.WAV" ) ; //just for the moment

    // Create a single player.

    // DEBUG.
    graph_sys->CreateGraphicsBike();
    graph_sys->CreateGraphicsBike();
    graph_sys->CreateGraphicsBike();
    graph_sys->CreateGraphicsBike();
    graph_sys->CreateGraphicsBike();
    graph_sys->CreateGraphicsBike();
    graph_sys->CreateGraphicsBike();
    graph_sys->CreateGraphicsBike();
    graph_sys->CreateGraphicsBike();
    //DEBUGGING: REMOVE.

    LbLevel ** levels = new LbLevel * [ 3 ] ;
    int x ;
    for (x = 0; x < 3; x ++ )
    {
        levels[x] = new LbLevelImp(3,3);
        levels[x]->SetBlockAt(1,1, LbFullBlock());
    }

    LbArenaImp * arena = new LbArenaImp(levels, 3);

    for (x=0; x < arena->GetZSize(); x++)
    {
        LbLevel *l = arena->GetLevel(x);
        l->GetXSize();
        l->GetYSize();
        l->GetBlockAt(-1,-10);
        l->GetBlockAt(0,0);
        l->GetBlockAt(1,1);
    }

    delete arena;

    //DEBUGGING: REMOVE END


    // HARD CODED START POSITIONS.
    //   THIS WILL BE IN THE LEVEL FILE ULTIMATELY

    start [ 0 ] = LbVector ( 0  , 100 , 0 ) ;
    start [ 1 ] = LbVector ( 0 , -100 , 0 ) ;
    start [ 2 ] = LbVector ( 10 , 0 , 0 ) ;
    start [ 3 ] = LbVector ( -10 , 0 , 0 ) ;

    dirns [ 0 ] = 2 ;
    dirns [ 1 ] = 0 ;
    dirns [ 2 ] = 1 ;
    dirns [ 3 ] = 3 ;

    cols [ 0 ] = LbRGBAColor ( 0.0f , 0.2f , 1.0f , 0.2f ) ;
    cols [ 1 ] = LbRGBAColor ( 0.0f , 1.0f , 0.0f , 0.2f ) ;
    cols [ 2 ] = LbRGBAColor ( 1.0f , 0.0f , 1.0f , 0.2f ) ;
    cols [ 3 ] = LbRGBAColor ( 1.0f , 1.0f , 0.0f , 0.2f ) ;

    // END OF HARD CODED STUFF

    gameinprogress = false ;

    while( ! quit_flag )
    {
        if ( gameinprogress )
        {

            step = ( change * (float)( os_sys->GetMS() - startms ) / 1000.0f );
            scroll += step;
            startms = os_sys->GetMS();
            //if (scroll > 10) change=-0.02f;
            //if (scroll < -10) change=0.02f;
            if ( !(count++ % 20) )
            {
                lastms = (os_sys->GetMS() - lastms);
                fps = lastms ? (20000 / lastms) : 0;
                lastms = os_sys->GetMS();
                sprintf(msg,"FPS:%d  Step:%.2f",fps, step);
            }

            // Set eye to point to bike

            //eye = LbVector(bikePos.getX() + 20 * sin(scroll), bikePos.getY() + 20 * cos(scroll), 10);
            dist = sqrt( pow(thisplayer->GetPosition()->getX() - eye.getX(), 2) +
                         pow(thisplayer->GetPosition()->getY() - eye.getY(), 2) +
                         pow(thisplayer->GetPosition()->getZ() - eye.getZ(), 2) );
            eye = LbVector( thisplayer->GetPosition()->getX() + (eye.getX() - thisplayer->GetPosition()->getX()) * (20 / dist),
                            thisplayer->GetPosition()->getY() + (eye.getY() - thisplayer->GetPosition()->getY()) * (20 / dist),
                            thisplayer->GetPosition()->getZ() + 10 );
            //target = LbVector(0, scroll, 0);
            graph_sys->SetCamera(eye, * thisplayer->GetPosition(),up);

            keycount = 32;
            inp[0] = 0;
            if (input_sys->GetOSKey(&keys[0], &keycount))
            {
                for (int i=0; i<keycount; i++)
                {
                    if (keys[i].down)
                    {
                        switch (keys[i].which)
                        {
                            // Left key press means add a turning point.
                            case LB_OSKEY_LEFT:
                            {
                                sprintf(inp, "Left Key Press");
                                lpress++;
                                sound_sys->PlayWaveFile(wave);
                                thisplayer->GetBike()->AddSegment( * thisplayer->GetPosition() );
                                thisplayer->SetDirection( ( thisplayer->GetDirection() + 3 ) % 4 );

                                // Send message.
                                LbGamePositionUpdate u ;
                                u.playerHash = thisplayer->GetHash ( ) ;
                                u.x1 = 0 ;
                                u.y1 = 0 ;
                                u.x2 = thisplayer->GetPosition()->getX() ;
                                u.y2 = thisplayer->GetPosition()->getY() ;
                                u.level = thisplayer->GetLevel() ;
                                u.sequence = thisplayer->GetNextSequenceNumber() ;
                                u.direction = thisplayer -> GetDirection ( ) ;
                                net_sys->SendPositionUpdate ( u ) ;
                            }
                            break;

                            // Right key press means add a turning point.
                            case LB_OSKEY_RIGHT:
                            {
                                sprintf(inp, "Right Key Press");
                                rpress++;
                                thisplayer->GetBike()->AddSegment( * thisplayer->GetPosition() );
                                thisplayer->SetDirection( ( thisplayer->GetDirection() + 1 ) % 4 );

                                // Send message.
                                LbGamePositionUpdate u ;
                                u.playerHash = thisplayer->GetHash ( ) ;
                                u.x1 = 0 ;
                                u.y1 = 0 ;
                                u.x2 = thisplayer->GetPosition()->getX() ;
                                u.y2 = thisplayer->GetPosition()->getY() ;
                                u.level = thisplayer->GetLevel() ;
                                u.sequence = thisplayer->GetNextSequenceNumber() ;
                                u.direction = thisplayer -> GetDirection ( ) ;
                                net_sys->SendPositionUpdate ( u ) ;
                            }
                            break;
                        }
                    }
                }
                if (keycount>0)
                    sprintf(keymsg, "Left: %d, Right: %d", lpress, rpress);
            }
            else
            {
                sprintf(inp, "Input Error");
            }
        }

        // Get text entered.
        while ( ( k = input_sys->getNextTextKey ( ) ) != 0 )
        {
            switch ( k )
            {
                case '\r' :
                    ProcessCommand ( textbuf ) ;
                    textbuf.erase() ;
                    break ;
                case 27 : // Escape key.
                    textbuf.erase ( ) ;
                    break ;
                case '\t': // Tab key, do nothing.
                    break;
                case 8 : // Backspace key.
                    if ( textbuf.size ( ) > 0 )
                        textbuf.erase ( textbuf.size ( ) - 1 ) ;
                    break ;
                // DEBUG CODE
                case '#' :
                    if ( abs(Vacc) < 0.001 ) Vacc = 5.0f;
                    break;
                // END
                default:
                    textbuf += k ;
                    break ;
            }
        }

        // DEBUG CODE
        /*if (( abs(Vacc) > 0.001 ) || ( bikePos.getZ() > 0.001 )) {
            players[0]->GetBike() ->AddSegment( bikePos );

            Vacc -= 9.81 * (step / change);
            players[0]->SetPosition ( LbVector (
                players[0]->GetPosition().getX(), bikePos.getY(), bikePos.getZ() + Vacc * (step / change) );
            if (bikePos.getZ() < 0) {
                bikePos = LbVector( bikePos.getX(), bikePos.getY(), 0 );
                players[0]->GetBike()->AddSegment( bikePos );
                Vacc = 0;
            }
        }
         */        // END

        graph_sys->StartFrame();
        // Draw here.

        // Draw trails for all bikes.
        for ( i = 0 ; i < MAX_PLAYERS ; i ++ )
        {
            if ( players[i]->IsPlaying() )
            {
                players [ i ] ->GetBike ( ) ->DrawTrail ( ) ;
                players [ i ] -> GetBike( ) ->DrawSegment (
                    players [ i ] ->GetBike ( ) ->GetLastSegment ( ) ,
                    * players[i]->GetPosition() ) ;
            }
        }

        graph_sys->SetTextColor(LbRGBAColor(1.0f,0.0f,0.0f,1.0f));
        graph_sys->DrawText(0.5f,0.82f,1.0f,"LightBikes3d");
        graph_sys->SetTextColor(LbRGBAColor(0.0f,0.0f,1.0f,1.0f));
        graph_sys->DrawText(0.6f,0.9f,0.5f,msg);
        graph_sys->SetTextColor(LbRGBAColor(1.0f,1.0f,0.0f,1.0f));
        graph_sys->DrawText(0.0f,0.25f,1.0f,inp);
        graph_sys->SetTextColor(LbRGBAColor(0.0f,1.0f,1.0f,1.0f));
        graph_sys->DrawText(0.0f,0.1f,1.0f,keymsg);

        // Display the typed text.
        graph_sys->SetTextColor ( LbRGBAColor ( 0.0f , 1.0f , 1.0f , 1.0f ) ) ;
        graph_sys->DrawText ( 0.0f , 0.02f , 0.75f , textbuf.c_str ( ) ) ;

        // Display the chat or status messages.
        for ( i = 0 ; i < MAX_MESSAGE_LINES ; i ++ )
        {
            graph_sys->SetTextColor ( LbRGBAColor ( 0 , 1 , 1 , 1 ) ) ;
            graph_sys->DrawText ( 0.05f , 0.9f - 0.04 * i , 0.5f , txtmsgs[i].c_str ( ) ) ;
        }

        if ( input_sys->IsTabDown ( ) )
        {
            // Display the players' frag count, overlayed Counter Strike style.
            for ( i = 0 ; i < MAX_SCOREBOARD_LINES ; i ++ )
            {
                graph_sys->SetTextColor ( LbRGBAColor ( 0 , 1 , 1 , 1 ) ) ;
                graph_sys->DrawText ( 0.05f , 0.7f - 0.04 * i , 0.5f , scoremsgs[i].c_str ( ) ) ;
            }
        }

        graph_sys->EndFrame();

        if ( gameinprogress )
        {

            // Move all the bikes forward.
            for ( i = 0 ; i < MAX_PLAYERS ; i ++ )
            {
                if ( players[i]->IsPlaying ( ) )
                {
                    switch ( players[i]->GetDirection ( ) )
                    {
                        case 0:
                        {
                            LbVector * v = new LbVector (
                                players[i]->GetPosition()->getX(),
                                players[i]->GetPosition()->getY() + step,
                                players[i]->GetPosition()->getZ() ) ;
                            players[i]->SetPosition( v ) ;
                        }
                        break;
                        case 1:
                        {
                            LbVector * v = new LbVector (
                                players[i]->GetPosition()->getX() + step ,
                                players[i]->GetPosition()->getY() ,
                                players[i]->GetPosition()->getZ() ) ;
                            players[i]->SetPosition( v ) ;
                        }
                        break;
                        case 2:
                        {
                            LbVector * v = new LbVector (
                                players[i]->GetPosition()->getX(),
                                players[i]->GetPosition()->getY() - step,
                                players[i]->GetPosition()->getZ() ) ;
                            players[i]->SetPosition( v ) ;
                        }
                        break;
                        case 3:
                        {
                            LbVector * v = new LbVector (
                                players[i]->GetPosition()->getX() - step,
                                players[i]->GetPosition()->getY() ,
                                players[i]->GetPosition()->getZ() ) ;
                            players[i]->SetPosition( v ) ;
                        }
                        break;
                    }
                }
            }

        }

        // Poll the event queue.
        while(os_sys->PollEvent(os_event))
        {
            /* OS event:
            **  Minimise/retstore
            **  Active/deactivate
            **  Close
            **  Normal keypress (for menus, etc.)
            **  Return: Send player quip
            */
            // Handle the event.
            switch ( os_event.id )
            {
                case LB_OSEVENT_QUIT:
                    quit_flag=true;// bye bye...
                break;
            }
            // Ignore unknown events...
        }

        // Process Network messages (Networking) ie. convert them from
        // strings and packets to game messages, and add them to the
        // network module's queue of game messages.
        net_sys->ProcessMessages ( ) ;

        // Probably won't do this ultimately.
        net_sys->PollSockets ( ) ;
        if ( net_sys->GetStatus ( ) == LB_NET_ERROR )
        {
            ShowStatusMessage ( net_sys->GetError ( ) ) ;
            net_sys->ResetConnections ( ) ;
        }

        // Deal with network messages.
        while ( net_sys->GetNextGameEvent ( game_event ) )
        {
            switch ( game_event.id )
            {
                // Deal with players joining.
                case LB_GAME_PLAYERJOIN :

                    AddPlayer ( game_event.playerHash , game_event.message ) ;
                    ShowStatusMessage (
                        GetPlayerHandle ( game_event.playerHash ) +
                        string ( " has joined the game." ) ) ;

                    // If we are the server send information on the other players.
                    if ( net_sys->GetStatus () == LB_NET_SERVER )
                    {
                        LbGameEvent e ;
                        for ( i = 0 ; i < MAX_PLAYERS ; i ++ )
                        {
                            if ( players[i]->IsValid ( ) == true )
                            {
                                e.id = LB_GAME_PLAYERJOIN ;
                                e.playerHash = players[i]->GetHash ( ) ;
                                e.message = GetPlayerHandle ( players[i]->GetHash ( ) ) + "#" + ItoS ( i ) ;
                                net_sys->SendGameEvent ( e , false ) ;
                            }
                        }
                    }
                break ;

                // Deal with players going.
                case LB_GAME_PLAYERLEAVE :
                    ShowStatusMessage (
                        GetPlayerHandle ( game_event.playerHash ) +
                        string ( " has left the game." ) +
                        game_event.message ) ;
                    RemovePlayer ( game_event.playerHash ) ;
                break ;

                // Deal with players changing name.
                case LB_GAME_HANDCHANGE :
                    ShowStatusMessage (
                        GetPlayerHandle ( game_event.playerHash ) +
                        string ( " is now known as " ) +
                        game_event.message + string ( "." ) ) ;
                    SetPlayerHandle ( game_event.playerHash ,
                                      game_event.message ) ;
                break ;

                // Deal with incoming chat messages.
                case LB_GAME_CHAT:
                    ShowStatusMessage (
                        string ("<" ) +
                        GetPlayerHandle ( game_event.playerHash ) +
                        string ( "> " ) + game_event.message ) ;
                break ;

                // New game message.  Abandon any current games.
                case LB_GAME_NEWGAME:
                    NewGame ( ) ;
                break ;

                // If the server decides the game is over, return to chat.
                case LB_GAME_ENDGAME:
                    EndGame ( ) ;
                break ;

                // If a round is being played kill it off.  Return players
                // to start positions.
                case LB_GAME_NEWROUND:
                    NewRound ( ) ;
                break ;

                // If a new map is being sent.
                case LB_GAME_NEWMAP:
                break ;
            }

            // Rebroadcast iff we are server and it didn't come from us,
            // because otherwise we would have already broadcast it.
            if ( net_sys->GetStatus ( ) == LB_NET_SERVER &&
                game_event.playerHash != net_sys->GetOwnPlayerHash ( ) &&
                game_event.id != LB_GAME_PLAYERJOIN )
                    net_sys->SendGameEvent ( game_event , false ) ;

        }

        if ( net_sys->GetStatus ( ) != LB_NET_DISCONNECTED )
        {
            // Get the new UDP messages
            net_sys->PollSocketsUDP ( ) ;
            LbGamePositionUpdate u ;

            while ( net_sys->GetNextPositionUpdate(u) )
            {
                // Debug code to display the packet.
                char f [ 256 ] ;
                sprintf ( ( char * ) & f ,
                    "Player Hash: %d\nSequence Number: %d\nLevel: %d \n"\
                    "Old Coord: %f , %f\nNew Coord: %f , %f\n"\
                    "New Direction: %d " ,
                    u.playerHash , u.sequence , u.level ,
                    u.x1 , u.y1 , u.x2 , u.y2 , u.direction ) ;
                //MessageBox ( NULL , ( char * ) & f , "Packet" , MB_ICONSTOP ) ;

                // Ignore any packets relating to us, since we know about
                // ourselves, minimize effect of hacked players.
                if ( u.playerHash != net_sys->GetOwnPlayerHash ( ) )
                {
                    // TODO: Check the sequence numbers.

                    // Update our records.
                    LbPlayer * movedplayer ;
                    movedplayer = GetPlayer ( u.playerHash ) ;
                    movedplayer -> SetDirection ( u.direction ) ;
                    movedplayer -> SetLevel ( u.level ) ;
                    movedplayer -> GetBike ( ) -> AddSegment (
                        LbVector ( u.x2 , u.y2 , 0 ) ) ;

                    string s = "handle is " + movedplayer->GetHandle() ;
                    MessageBox ( NULL , s.c_str()  , "debug" , MB_ICONSTOP ) ;
                }

                // Resend the message if we are a server.
                if ( net_sys -> GetStatus ( ) == LB_NET_SERVER )
                {
                    net_sys -> SendPositionUpdate ( u ) ;
                }
            }
        }

        // Put a scoreboard together.  BTW I think we should be able to detect
        // 'kills' (when you cut in front of someone) fairly accurately,
        // experiments needed to distinguish between accidental deaths and
        // kills.
        p = 0 ;
        scoremsgs[0] = "Player Name       Kills      Deaths     Ping" ;
        scoremsgs[1] = "                                            " ;
        for ( i = 2 ; i < MAX_SCOREBOARD_LINES ; i++ )
        {
            while ( p < MAX_PLAYERS && players[p]->IsValid ( ) == false ) p++ ;
            if ( p >= MAX_PLAYERS - 1 )
                scoremsgs [ i ] = "" ;
            else
                scoremsgs [ i ] =
                    " " + Pad ( players[p]->GetHandle ( ) , 18 ) +
                    Pad ( ItoS ( players[p]->GetKills ( ) ) , 11 ) +
                        Pad ( ItoS ( players[p]->GetDeaths ( ) ) , 11 ) +
                        ItoS ( players[p]->GetPing ( ) ) ;
            p ++ ;
        }
    }

    DeInitSubsystems ( ) ;

    //fclose(fp);
    return 0;
}

/**
 ** To go in a library at some point, or be replaced.
 **/
string LbGameImp::Pad ( string s , int l )
{
    if ( s.size ( ) > l ) return s.substr ( 0 , l ) ;
    else if ( s.size ( ) == l ) return s ;
    else return s + string ( l - s.size ( ) , ' ' ) ;
}

/**
 ** To go in a library at some point, or be replaced.
 **/
string LbGameImp::ItoS ( int i )
{
    char temp [ 20 ] ;
    itoa ( i , temp , 10 ) ;
    return string ( temp ) ;
}

/**
 ** Add the message to the messages at the top of the screen.
 ** Implements a nice shifting scrolling effect.
 **/
void LbGameImp::ShowStatusMessage ( const string & txt )
{
    int i ;

    // Fill first unfilled line.
    for ( i = 0 ; i < MAX_MESSAGE_LINES ; i ++ )
        if ( txtmsgs [ i ] == "" )
        {
            txtmsgs [ i ] = txt ;
            return ;
        }

    // Otherwise shift up, and fill last line.
    for ( i = 0 ; i < MAX_MESSAGE_LINES - 1 ; i ++ )
        txtmsgs [ i ] = txtmsgs [ i + 1 ] ;
    txtmsgs [ MAX_MESSAGE_LINES - 1 ] = txt ;
}

/**
 ** Return player's handle (name) as a string.
 **/
string LbGameImp::GetPlayerHandle ( int playerhash )
{
    int i ;
    for ( i = 0 ; i < MAX_PLAYERS ; i ++ )
        if ( players[i]->IsValid ( ) == true && playerhash == players[i]->GetHash ( ) )
            if ( players[i]->GetHandle ( ) != "" )
                return players[i]->GetHandle ( ) ;
    return "Noname" ;
}

/**
 ** Return player.
 **/
LbPlayer * LbGameImp::GetPlayer ( int playerhash )
{
    int i ;
    for ( i = 0 ; i < MAX_PLAYERS ; i ++ )
        if ( players[i]->IsValid ( ) == true &&
             playerhash == players[i]->GetHash ( ) )
                return players[i] ;
    return NULL ;
}

/**
 ** Set the handle (name) of a player.
 **/
bool LbGameImp::SetPlayerHandle ( int hash , const string & handle )
{
    int i ;
    for ( i = 0 ; i < MAX_PLAYERS ; i ++ )
    {
        if ( players[i]->IsValid ( ) == true &&
             hash == players[i]->GetHash ( ) )
        {
             players[i]->SetHandle ( handle ) ;
             return true ;
        }
    }
    return false ;
}

/**
 ** Add player.
 **/
void LbGameImp::AddPlayer ( int hash , const string & handle )
{
    int p = -1 , i;

    // If we are the server find a free slot.
    if ( net_sys->GetStatus () == LB_NET_SERVER )
    {
        for ( i = 0 ; i < MAX_PLAYERS ; i ++ )
        {
            if ( players[i]->IsValid ( ) == false )
            {
                p = i ;
                break ;
            }
        }
    }
    else
    {
        // Parse for the player number.
        int m = handle.find_first_of ( "#" ) ;
        string prm = handle.substr ( m + 1 , handle.size ( ) - m - 1 ) ;

        p = atoi ( prm.c_str() ) ;

    //MessageBox ( NULL , "Client id number." ,
    //           prm.c_str() , MB_ICONSTOP ) ;

    }

    //MessageBox ( NULL , "Client id number." ,
    //           "fisgh ", MB_ICONSTOP ) ;

    if ( players[p]->IsValid ( ) == false )
    {
        players[p]->SetValid ( true ) ;
        players[p]->SetHash ( hash ) ;
        players[p]->SetBike( graph_sys->GetBike(p) );
        if ( handle == "" )
            players[p]->SetHandle ( "New Player" ) ;
        else players[p]->SetHandle ( handle ) ;
        return ;
    }

    // Check for existing player.
    //if ( SetPlayerHandle ( hash , handle ) ) return ;

    // No spare slots.
}

/**
 ** Remove player.
 **/
void LbGameImp::RemovePlayer ( int hash  )
{
    int i;
    for ( i = 0 ; i < MAX_PLAYERS ; i ++ )
        if ( players[i]->IsValid ( ) == true &&
             hash == players[i]->GetHash ( ) )
            players[i]->SetValid ( false ) ;
}

/**
 ** New Round.
 **/
void LbGameImp::NewRound ( )
{
    // Set up start positions etc.
}

/**
 ** New Game.
 **/
void LbGameImp::NewGame ( )
{
    int i ;
    for ( i = 0 ; i < MAX_PLAYERS ; i++ )
    {
        // Start the play.
        if ( players[i]->IsValid ( ) )
        {
            players[i]->SetKills ( 0 ) ;
            players[i]->SetDeaths ( 0 ) ;
            players[i]->GetBike()->AddSegment ( start [ i ] ) ;
            players[i]->GetBike()->SetColor ( cols [ i ] ) ;
            players[i]->SetPosition( & start [ i ] ) ;
            players[i]->SetDirection ( dirns [ i ] ) ;
            players[i]->SetPlaying ( true ) ;
        }

        if ( players[i]->GetHash ( ) == net_sys->GetOwnPlayerHash ( ) )
            thisplayer = players[ i ] ;

        if ( net_sys->GetStatus () == LB_NET_SERVER )
            thisplayer = players[ 0 ] ;

    }

    gameinprogress = true ;

}

/**
 ** End Game.
 **/
void LbGameImp::EndGame ( )
{
    gameinprogress = false ;
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

        // If a command only allowed on the server is issued.
        if ( cmd == "newgame" ||
             cmd == "newround" ||
             cmd == "endgame" ||
             cmd == "newmap" )
        {
            if ( net_sys->GetStatus ( ) == LB_NET_SERVER )
            {
                e.message = prm ;

                if ( cmd == "newgame" )
                    e.id = LB_GAME_NEWGAME ;
                else if  ( cmd == "newround" )
                    e.id = LB_GAME_NEWROUND ;
                else if  ( cmd == "endgame" )
                    e.id = LB_GAME_ENDGAME ;
                else if ( cmd == "newmap" )
                    e.id = LB_GAME_NEWMAP ;
            }
            else
            {
                ShowStatusMessage ( "Error: This is not a server.") ;
                return ;
            }
        }

        // If a general program message was typed.
        else if ( cmd == "quit" )
        {
            quit_flag = true ;
            return ;
        }
        else if ( cmd == "connect" )
        {
            net_sys->InitialiseClientTCP ( prm.c_str() , LB_SERVER_TCP_PORT ) ;
            if ( net_sys->GetStatus ( ) != LB_NET_CLIENT )
            {
                ShowStatusMessage ( net_sys->GetError ( ) ) ;
                net_sys->ResetConnections ( ) ;
                return ;
            }

            net_sys -> InitialiseClientUDP ( LB_SERVER_UDP_PORT , LB_CLIENT_UDP_PORT ) ;
            if ( net_sys->GetStatus ( ) != LB_NET_CLIENT )
            {
                ShowStatusMessage ( net_sys->GetError ( ) ) ;
                net_sys->ResetConnections ( ) ;
                return ;
            }

            ShowStatusMessage ( "Connected to server." ) ;

            // Send the join message.
            e.id = LB_GAME_PLAYERJOIN ;
            e.message = ownhandle;
        }
        else if ( cmd == "startserver" )
        {
            if ( net_sys->GetStatus() == LB_NET_SERVER )
            {
                ShowStatusMessage ( "Server already running." ) ;
                return ;
            }

            net_sys->InitialiseServerTCP ( LB_SERVER_TCP_PORT ) ;
            if ( net_sys->GetStatus ( ) != LB_NET_SERVER )
            {
                ShowStatusMessage ( net_sys->GetError ( ) ) ;
                net_sys->ResetConnections ( ) ;
                return ;
            }

            net_sys -> InitialiseServerUDP ( LB_SERVER_UDP_PORT , LB_CLIENT_UDP_PORT ) ;
            if ( net_sys->GetStatus ( ) != LB_NET_SERVER )
            {
                ShowStatusMessage ( net_sys->GetError ( ) ) ;
                net_sys->ResetConnections ( ) ;
                return ;
            }

            ShowStatusMessage ( "Server started." ) ;

            // Send the join message.
            e.id = LB_GAME_PLAYERJOIN ;
            e.message = ownhandle;

        }
        else if ( cmd == "hand" )
        {
            ownhandle = prm ;
            e.message = prm ;
            e.id = LB_GAME_HANDCHANGE ;
        }
        else if ( cmd == "leave" )
        {
            e.message = prm ;
            e.id = LB_GAME_PLAYERLEAVE ;
        }
        else
        {
            // Command was invalid.
            ShowStatusMessage ( "Error: Unknown command." ) ;
            return ;
        }
    }
    else
    {
        // No command means it's chat.
        e.id = LB_GAME_CHAT ;

        // Strip whitespace from the start and end of t.
        int i , j ;
        i = t.find_first_not_of ( "\n\r \t" , 0 ) ;
        j = t.find_last_not_of ( "\n\r \t" ) ;

        // If the user only entered whitespace then forget it.
        if ( j == -1 ) return ;

        // Copy the non-whitespace part of the entry.
        e.message = t.substr ( i , j - i + 1 ) ;
    }

    // Attach our player hash and dispatch the message.
    e.playerHash = net_sys->GetOwnPlayerHash ( ) ;
    net_sys->SendGameEvent ( e , ( net_sys->GetStatus ( ) == LB_NET_SERVER ) ) ;
}

/**
 ** Starts stuff up.
 **/
void LbGameImp::InitSubsystems()
{
    os_sys=CreateOSLayerSys();
    graph_sys=CreateGraphicsSys(os_sys);
    input_sys=CreateInputSys(os_sys);
    sound_sys=CreateSoundSys(os_sys);
    net_sys=CreateNetSys(os_sys);
}

/**
 ** Shuts stuff down.
 **/
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

/**
 ** Create stuff.
 **/
LbGameSys *CreateGameSys()
{
    LbGameSys *rval=new LbGameImp;
    assert(rval!=NULL);

    return rval;
}