#include "LbStandard.h"
#include "LbPublic.h"
#include "LbNetImp.h"


LbNetImp::LbNetImp()
{
}

LbNetImp::~LbNetImp()
{
}

LbNetSys *CreateNetSys()
{
LbNetSys *rval=new LbNetImp;
assert(rval!=NULL);

return rval;
}