#ifndef __LBGRAPHICS__
#define __LBGRAPHICS__

class LbGraphicsSys
{
public:

// empty virtual destructor to ensure proper cleanup
virtual ~LbGraphicsSys(){}
};

LbGraphicsSys *CreateGraphicsSys();

#endif