#include <QApplication>
#include "DisplayQt.h"
#include "babelwindow.h"

DisplayQt::DisplayQt(int argc, char **argv)
{
    this->_argc = argc;
    this->_argv = argv;
}

DisplayQt::~DisplayQt()
{
}

int	DisplayQt::start()
{
    QApplication a(this->_argc, this->_argv);
    BabelWindow	myWindow;
    myWindow.show();
    return a.exec();
}
