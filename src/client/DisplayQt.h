#ifndef DISPLAYQT_HH_
# define DISPLAYQT_HH_

#include "IDisplay.h"

class DisplayQt : public IDisplay
{
private:
    int _argc;
    char **_argv;
public:
    DisplayQt(int, char **);
    virtual ~DisplayQt();
    virtual int 	start();
};

#endif
