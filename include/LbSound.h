#ifndef __LBSOUND__
#define __LBSOUND__

class LbSoundSys
{
public:

// empty virtual destructor to ensure proper cleanup
virtual ~LbSoundSys(){}
};

LbSoundSys *CreateSoundSys();

#endif