#include "LbStandard.h"
#include "LbPublic.h"
#include "LbGraphicsImp.h"


LbGraphicsImp::LbGraphicsImp()
{
}

LbGraphicsImp::~LbGraphicsImp()
{
}

LbGraphicsSys *CreateGraphicsSys()
{
LbGraphicsSys *rval=new LbGraphicsImp;
assert(rval!=NULL);

return rval;
}