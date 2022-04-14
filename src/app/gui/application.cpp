#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtGui/QSplashScreen>
#include <QtGui/QDesktopWidget>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeComponent>
#include "gui/application.h"
#include "telegramclient.h"
#include "systemhandler.h"

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#endif

namespace gui {

Application::Application(int argc, char *argv[])
	: QApplication(argc, argv)
{
#ifdef QT_KEYPAD_NAVIGATION
    //TODO: keypad UI navigation
    QApplication::setNavigationMode(Qt::NavigationModeCursorAuto);
#endif
    QCoreApplication::setAttribute((Qt::ApplicationAttribute) 9, false);
    //TODO: defined macros not only for Symbian.
}

enum ScreenOrientation
{
    Landscape = 0,
    Portrait = 270,
    LandscapeInverted = 180,
    PortraitInverted = 90
};

int Application::run() {
#if defined(Q_OS_SYMBIAN)
	QScopedPointer<QSplashScreen> splashScreen(buildSplashScreen());
    splashScreen->showFullScreen();
#endif
	QScopedPointer<QDeclarativeView> applicationWindow(buildRootView());

    applicationWindow->setAttribute((Qt::WidgetAttribute) 128, true);
#ifdef Q_WS_X11
        WId id = applicationWindow->winId();
        Display *display = QX11Info::display();
        if (display) {
            Atom orientationAngleAtom = XInternAtom(display, "_MEEGOTOUCH_ORIENTATION_ANGLE", False);
            ScreenOrientation orientation = Portrait;
            XChangeProperty(display, id, orientationAngleAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&orientation, 1);
        }
#endif
#if defined(Q_OS_SYMBIAN)
    splashScreen->finish(applicationWindow.data());
    applicationWindow->showFullScreen();
#else
    applicationWindow->showNormal();
#endif

	return QApplication::exec();
}

QDeclarativeView *Application::buildRootView() {
	QScopedPointer<QDeclarativeView> view(new QDeclarativeView());
	QObject::connect(view->engine(), SIGNAL(quit()),
	                 view.data(), SLOT(close()));
	view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
#if defined(Q_OS_SYMBIAN)
    view->engine()->rootContext()->setContextProperty("QTC_DISABLE_STATUS_BAR", QVariant(false));
#else
    view->engine()->rootContext()->setContextProperty("QTC_DISABLE_STATUS_BAR", QVariant(true));
#endif

    TelegramClient* client = new TelegramClient(this);
    view->engine()->rootContext()->setContextProperty("telegram", client);

    SystemHandler* systemHandler = new SystemHandler(client);
    view->engine()->rootContext()->setContextProperty("systemHandler", systemHandler);

	view->engine()->addImportPath(
	    QLatin1String("../../resource/demo/imports"));
	//view->setSource(QUrl::fromLocalFile(
	//  QLatin1String("../../resource/apps/demo/layout/main.qml")));
	view->setSource(QUrl("qrc:/layout/main.qml"));
	return view.take();
}

QSplashScreen *Application::buildSplashScreen() {
    const QPixmap logoPixmap(":/images/kutegram_big.png");
    QDesktopWidget *desktop = QApplication::desktop();
    QRect desktopRect = desktop->availableGeometry();
    QPixmap splashPixmap(desktopRect.width(), desktopRect.height());
    QPainter painter;
    painter.begin(&splashPixmap);
    painter.fillRect(splashPixmap.rect(), QColor::fromRgb(0, 0, 0));
    QRect logoRect((splashPixmap.width() - logoPixmap.width()) / 2,
                   (splashPixmap.height() - logoPixmap.height()) / 2,
                   logoPixmap.width(),
                   logoPixmap.height());
    painter.drawPixmap(logoRect, logoPixmap);
    painter.end();
    QScopedPointer<QSplashScreen> splashScreen(new QSplashScreen(splashPixmap));
    if (desktopRect.width() > desktopRect.height()) {
        splashScreen->setAttribute(Qt::WA_LockLandscapeOrientation, true);
    } else {
        splashScreen->setAttribute(Qt::WA_LockPortraitOrientation, true);
    }
	return splashScreen.take();
}

}
