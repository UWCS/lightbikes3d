#include "LbStandard.h"
#include "LbPublic.h"
#include "LbGameImp.h"


LbGameImp::LbGameImp()
{
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
InitSubsystems();

/*
** put main loop here.
*/

DeInitSubsystems();

return 0;
}

void LbGameImp::InitSubsystems()
{
graph_sys=CreateGraphicsSys();
input_sys=CreateInputSys();
sound_sys=CreateSoundSys();
net_sys=CreateNetSys();
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
};

LbGameSys *CreateGameSys()
{
LbGameSys *rval=new LbGameImp;
assert(rval!=NULL);

return rval;
}