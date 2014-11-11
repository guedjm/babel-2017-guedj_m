#include "IDisplay.h"
#include "DisplayQt.h"

int main(int argc, char *argv[])
{
    IDisplay *disp = new DisplayQt(argc, argv);
    return (disp->start());
}
