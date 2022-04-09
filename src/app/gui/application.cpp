#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtGui/QSplashScreen>
#include <QtGui/QDesktopWidget>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeComponent>
#include "gui/application.h"

namespace demo { namespace gui {

Application::Application(int argc, char *argv[])
	: QApplication(argc, argv)
{
}

int Application::run() {
	QScopedPointer<QSplashScreen> splashScreen(buildSplashScreen());
        splashScreen->showNormal();
	QScopedPointer<QDeclarativeView> applicationWindow(buildRootView());
	splashScreen->finish(applicationWindow.data());
        applicationWindow->showNormal();
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
	view->engine()->addImportPath(
	    QLatin1String("../../resource/demo/imports"));
	//view->setSource(QUrl::fromLocalFile(
	//  QLatin1String("../../resource/apps/demo/layout/main.qml")));
	view->setSource(QUrl("qrc:/layout/main.qml"));
	return view.take();
}

QSplashScreen *Application::buildSplashScreen() {
	const QPixmap logoPixmap(":/images/logo.png");
        QDesktopWidget *desktop = QApplication::desktop();
        QRect desktopRect = desktop->availableGeometry();
#if defined(Q_OS_SYMBIAN)
        QPixmap splashPixmap(desktopRect.width(), desktopRect.height());
        QPainter painter;
        painter.begin(&splashPixmap);
        QLinearGradient backgroundGradient(
                splashPixmap.rect().width() / 2, 0,
                splashPixmap.rect().width() / 2, splashPixmap.rect().height());
        backgroundGradient.setColorAt(0, QColor::fromRgb(40, 50, 57));
        backgroundGradient.setColorAt(1, QColor::fromRgb(19, 25, 29));
        painter.fillRect(splashPixmap.rect(), backgroundGradient);
        QRect logoRect((splashPixmap.width() - logoPixmap.width()) / 2,
                       (splashPixmap.height() - logoPixmap.height()) / 2,
                       logoPixmap.width(),
                       logoPixmap.height());
        painter.drawPixmap(logoRect, logoPixmap);
        painter.end();
        QScopedPointer<QSplashScreen> splashScreen(new QSplashScreen(splashPixmap));
#else
        QScopedPointer<QSplashScreen> splashScreen(new QSplashScreen(logoPixmap));
#endif
//	if (desktopRect.width() > desktopRect.height()) {
//		splashScreen->setAttribute(Qt::WA_LockLandscapeOrientation, true);
//	} else {
//		splashScreen->setAttribute(Qt::WA_LockPortraitOrientation, true);
//	}
	return splashScreen.take();
}

}}
