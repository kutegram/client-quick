/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "sdeclarative.h"
#include "sstylefactory.h"
#include "sdeclarativeicon.h"
#include "sdeclarativefocusscopeitem.h"
#include "sdeclarativeimplicitsizeitem.h"
#include "sdeclarativeimageprovider.h"
#include "sdeclarativemaskedimage.h"
#include "sdeclarativescreen.h"
#include "sdeclarativeindicatorcontainer.h"
#include "sdeclarativenetworkindicator.h"
#include "sbatteryinfo.h"
#include "snetworkinfo.h"
#include "spopupmanager.h"
#include "smousegrabdisabler.h"
#include "sdeclarativemagnifier.h"

#include <QCoreApplication>
#include <QtDeclarative>

static const int VERSION_MAJOR = 1;
static const int VERSION_MINOR = 0;

class SymbianPlugin : public QDeclarativeExtensionPlugin
{
		Q_OBJECT

public:

		void initializeEngine(QDeclarativeEngine *engine, const char *uri) {
				QDeclarativeExtensionPlugin::initializeEngine(engine, uri);
				context = engine->rootContext();

				// QVariant.toInt() defaults to zero if QVariant is invalid.
				int versionMajor = context->property("symbianComponentsVersionMajor").toInt();
				int versionMinor = context->property("symbianComponentsVersionMinor").toInt();
				if (versionMajor > VERSION_MAJOR ||
						(versionMajor == VERSION_MAJOR && versionMinor >= VERSION_MINOR)) {
						// Either newer or this version of plugin already initialized.
						// The same plugin might initialized twice: once from
						// "import com.nokia.symbian", and another time from
						// "import Qt.labs.components.native".
						return;
				} else {
						// Intentionally override possible older version of the plugin.
						context->setProperty("symbianComponentsVersionMajor", VERSION_MAJOR);
						context->setProperty("symbianComponentsVersionMinor", VERSION_MINOR);
				}

				engine->addImageProvider(QLatin1String("theme"), new SDeclarativeImageProvider);

				screen = new SDeclarativeScreen(engine, context); // context as parent
				context->setContextProperty("screen", screen);

				style = new SStyleFactory(screen, context);
				context->setContextProperty("platformStyle", style->platformStyle());
				context->setContextProperty("privateStyle", style->privateStyle());

				SDeclarative *declarative = new SDeclarative(context);
				context->setContextProperty("symbian", declarative);
				
				SPopupManager *popupManager = new SPopupManager(context);
		        context->setContextProperty("platformPopupManager", popupManager);

				SBatteryInfo *batteryInfo = new SBatteryInfo(context);
				context->setContextProperty("privateBatteryInfo", batteryInfo);

				SNetworkInfo *networkInfo = new SNetworkInfo(context);
				context->setContextProperty("privateNetworkInfo", networkInfo);

				QObject::connect(engine, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));
				QObject::connect(screen, SIGNAL(displayChanged()), this, SLOT(resetScreen()));
				QObject::connect(style->platformStyle(), SIGNAL(fontParametersChanged()), this, SLOT(resetPlatformStyle()));
				QObject::connect(style->platformStyle(), SIGNAL(layoutParametersChanged()), this, SLOT(resetPlatformStyle()));
				QObject::connect(style->platformStyle(), SIGNAL(colorParametersChanged()), this, SLOT(resetPlatformStyle()));
				QObject::connect(style->privateStyle(), SIGNAL(layoutParametersChanged()), this, SLOT(resetPrivateStyle()));
				QObject::connect(style->privateStyle(), SIGNAL(colorParametersChanged()), this, SLOT(resetPrivateStyle()));
		}

		void registerTypes(const char *uri) {
				qmlRegisterType<SDeclarativeIcon>(uri, 1, 0, "Icon");
				qmlRegisterType<SDeclarativeMaskedImage>(uri, 1, 0, "MaskedImage");
				qmlRegisterType<SDeclarativeImplicitSizeItem>(uri, 1, 0, "ImplicitSizeItem");
				qmlRegisterType<SDeclarativeFocusScopeItem>(uri, 1, 0, "FocusScopeItem");
				qmlRegisterType<SDeclarativeIndicatorContainer>(uri, 1, 0, "UniversalIndicators");
				qmlRegisterType<SDeclarativeNetworkIndicator>(uri, 1, 0, "NetworkIndicator");
				qmlRegisterType<SMouseGrabDisabler>(uri, 1, 0, "MouseGrabDisabler");
				qmlRegisterType<SDeclarativeMagnifier>(uri, 1, 0, "Magnifier");
				qmlRegisterUncreatableType<SDeclarative>(uri, 1, 0, "Symbian", "");
				qmlRegisterUncreatableType<SDeclarativeScreen>(uri, 1, 0, "Screen", "");
				qmlRegisterUncreatableType<SDialogStatus>(uri, 1, 0, "DialogStatus", "");
				qmlRegisterUncreatableType<SPageOrientation>(uri, 1, 0, "PageOrientation", "");
				qmlRegisterUncreatableType<SPageStatus>(uri, 1, 0, "PageStatus", "");
				qmlRegisterUncreatableType<SBatteryInfo>(uri, 1, 0, "BatteryInfo", "");
				qmlRegisterUncreatableType<SNetworkInfo>(uri, 1, 0, "NetworkInfo", "");
		}

public slots:

		void resetScreen() {
				context->setContextProperty("screen", screen);
		}

		void resetPlatformStyle() {
				context->setContextProperty("platformStyle", style->platformStyle());
		}

		void resetPrivateStyle() {
				context->setContextProperty("privateStyle", style->privateStyle());
		}

private:
		QDeclarativeContext *context;
		SDeclarativeScreen *screen;
		SStyleFactory *style;
};

#include "plugin.moc"

Q_EXPORT_PLUGIN2(symbian_plugin_1_0, SymbianPlugin)
