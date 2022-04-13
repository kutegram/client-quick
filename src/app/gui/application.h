#ifndef APP_GUI_APPLICATION_H
#define APP_GUI_APPLICATION_H

#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include <QtCore/QScopedPointer>
#include <QtDeclarative/QDeclarativeView>

namespace gui {

class Application : public QApplication
{
public:
	Application(int argc, char *argv[]);

public:
    int run();

private:
	QDeclarativeView *buildRootView();
	QSplashScreen *buildSplashScreen();
};

}

#endif
