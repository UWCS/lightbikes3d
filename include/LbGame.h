#ifndef __LBGAME__
#define __LBGAME__

class LbGameSys
{
public:

virtual int RunGame()=0;

// empty virtual destructor to ensure proper cleanup
virtual ~LbGameSys(){}
};


LbGameSys *CreateGameSys();

#endif