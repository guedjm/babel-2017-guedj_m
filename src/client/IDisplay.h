#ifndef IDISPLAY_HH_
# define IDISPLAY_HH_


class IDisplay
{	
public:
    virtual			~IDisplay() {}
    virtual int     start() = 0;
};

#endif
