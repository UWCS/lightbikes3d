#ifndef __LBINPUT__
#define __LBINPUT__

class LbInputSys
{
public:

// empty virtual destructor to ensure proper cleanup
virtual ~LbInputSys(){}
};

LbInputSys *CreateInputSys();

#endif