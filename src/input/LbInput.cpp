#include "LbStandard.h"
#include "LbPublic.h"
#include "LbInputImp.h"


LbInputImp::LbInputImp()
{
}

LbInputImp::~LbInputImp()
{
}

LbInputSys *CreateInputSys()
{
LbInputSys *rval=new LbInputImp;
assert(rval!=NULL);

return rval;
}