#ifndef __LBGAMEIMP__
#define __LBGAMEIMP__

class LbGameImp : public LbGameSys
{
public:
LbGameImp();
~LbGameImp();

virtual int RunGame();

private:

void InitSubsystems();
void DeInitSubsystems();

LbGraphicsSys *graph_sys;
LbInputSys *input_sys;
LbSoundSys *sound_sys;
LbNetSys *net_sys;
};

#endif