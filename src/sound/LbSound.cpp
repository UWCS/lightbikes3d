#include "LbStandard.h"
#include "LbPublic.h"
#include "LbSoundImp.h"


LbSoundImp::LbSoundImp()
{
}

LbSoundImp::~LbSoundImp()
{
}

LbSoundSys *CreateSoundSys()
{
LbSoundSys *rval=new LbSoundImp;
assert(rval!=NULL);

return rval;
}