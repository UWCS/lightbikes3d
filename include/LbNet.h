#ifndef __LBNET__
#define __LBNET__

class LbNetSys
{
public:

// empty virtual destructor to ensure proper cleanup
virtual ~LbNetSys(){}
};

LbNetSys *CreateNetSys();

#endif