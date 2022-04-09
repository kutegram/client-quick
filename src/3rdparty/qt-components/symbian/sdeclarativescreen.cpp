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

#include "sdeclarativescreen.h"
#include "sdeclarativescreen_p.h"
#include <QApplication>
#include <QResizeEvent>
#include <QDesktopWidget>
#include <QDeclarativeEngine>
#include <QDeclarativeView>
#include <qmath.h>

#ifdef Q_OS_SYMBIAN
#include <aknappui.h>
#include <hal.h>
#endif

//#define Q_DEBUG_SCREEN
#include <QDebug>

static const qreal DEFAULT_TWIPS_PER_INCH = 1440.0;
static const qreal DEFAULT_DPI = 211.7;
static const int DEFAULT_WIDTH = 360;
static const int DEFAULT_HEIGHT = 640;
static const qreal CATEGORY_SMALL_LIMIT = 3.2;
static const qreal CATEGORY_MEDIUM_LIMIT = 4.5;
static const qreal CATEGORY_LARGE_LIMIT = 7.0;
static const qreal DENSITY_SMALL_LIMIT = 140.0;
static const qreal DENSITY_MEDIUM_LIMIT = 180.0;
static const qreal DENSITY_LARGE_LIMIT = 270.0;

static QDeclarativeView *findDeclarativeView(
    const QWidgetList &widgets, const QDeclarativeEngine *engine)
{
    QDeclarativeView *result = 0;
    for (int i = 0; i < widgets.count() && !result; i++) {
         QDeclarativeView *declarativeView = qobject_cast<QDeclarativeView *>(widgets.at(i));
         if (declarativeView && declarativeView->engine() == engine)
             result = declarativeView;
    }
    return result;
}

SDeclarativeScreenPrivate::SDeclarativeScreenPrivate(
    SDeclarativeScreen *qq,
    QDeclarativeEngine *engine) :
    q_ptr(qq),
    currentOrientation(SDeclarativeScreen::Default),
    allowedOrientations(SDeclarativeScreen::Default),
    dpi(DEFAULT_DPI),
    screenSize(),
    displaySize(QSize(DEFAULT_WIDTH, DEFAULT_HEIGHT)),
    settingDisplay(false),
    initCalled(false),
    initDone(false),
    engine(engine)
{
    Q_Q(SDeclarativeScreen);
    screenSize = currentScreenSize();
#ifdef Q_OS_SYMBIAN
#ifndef __WINS__
    int twipsX(0);
    int twipsY(0);
    int pixelsX(0);
    int pixelsY(0);

    if (HAL::Get(HALData::EDisplayXTwips, twipsX) == KErrNone
        && HAL::Get(HALData::EDisplayYTwips, twipsY) == KErrNone
        && HAL::Get(HALData::EDisplayXPixels, pixelsX) == KErrNone
        && HAL::Get(HALData::EDisplayYPixels, pixelsY) == KErrNone) {
        dpi = 0.5 * ((pixelsX / (twipsX / DEFAULT_TWIPS_PER_INCH)) +
                     (pixelsY / (twipsY / DEFAULT_TWIPS_PER_INCH)));
        displaySize = QSize(pixelsX, pixelsY);
    }
#endif // __WINS__
#endif // Q_OS_SYMBIAN

    // Find the corresponding QDeclarativeView
    // First check only the top level widgets which is a fast and most likely
    gv = findDeclarativeView(QApplication::topLevelWidgets(), engine);
    if (!gv)
        // check all the widgets as fallback
        gv = findDeclarativeView(QApplication::allWidgets(), engine);

#ifdef Q_DEBUG_SCREEN
    if (!gv)
        qDebug() << "SDeclarativeScreen() cannot find QDeclarativeView";
#endif

    if (gv) {
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
        gv->setWindowState(gv->windowState() | Qt::WindowFullScreen);
#endif
        gv->installEventFilter(q);
    }
#if defined(Q_WS_SIMULATOR)
    q->connect(QApplication::desktop(), SIGNAL(resized(int)), SLOT(_q_desktopResized(int)), Qt::QueuedConnection);
#endif
    QSize initViewSize;
#if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
    initViewSize = displaySize;
#endif
    QMetaObject::invokeMethod(q, "_q_initView", Qt::QueuedConnection, Q_ARG(QSize, initViewSize));
}

SDeclarativeScreenPrivate::~SDeclarativeScreenPrivate()
{
}

void SDeclarativeScreenPrivate::updateOrientationAngle()
{
    Q_Q(SDeclarativeScreen);
    SDeclarativeScreen::Orientation oldOrientation = currentOrientation;
    SDeclarativeScreen::Orientation newOrientation(SDeclarativeScreen::Portrait);

    if (isLandscapeScreen())
        newOrientation = SDeclarativeScreen::Landscape;

#ifdef Q_DEBUG_SCREEN
    qDebug() << "SDeclarativeScreen::updateOrientationAngle():" << newOrientation;
#endif
    currentOrientation = newOrientation;
    if (currentOrientation != oldOrientation)
        emit q->currentOrientationChanged();
}

void SDeclarativeScreenPrivate::_q_updateScreenSize(const QSize &size)
{
    Q_Q(SDeclarativeScreen);
#ifdef Q_DEBUG_SCREEN
    qDebug() << "_q_updateScreenSize(): " << size;
#endif

    QSize newSize(size);
    if (newSize.isEmpty())
        newSize = currentScreenSize();

    if (newSize.isEmpty() || (screenSize == newSize && initDone))
        return;

    QSize newSizeTransposed = newSize;
    newSizeTransposed.transpose();
    if (displaySize != newSize && displaySize != newSizeTransposed) {
        displaySize = newSize;
        if (!settingDisplay)
            emit q->displayChanged();
    }
    if ((screenSize.width() <= screenSize.height() && newSize.width() > newSize.height())
        || (screenSize.width() > screenSize.height() && newSize.width() <= newSize.height()))
        emit q->privateAboutToChangeOrientation();

    QSize oldSize = screenSize;
    screenSize = newSize;
    if (oldSize.width() != newSize.width())
        emit q->widthChanged();
    if (oldSize.height() != newSize.height())
        emit q->heightChanged();

    updateOrientationAngle();

    initDone = true;
}

void SDeclarativeScreenPrivate::_q_initView(const QSize &size)
{
#ifdef Q_DEBUG_SCREEN
    qDebug() << "_q_initView() size" << size << " gv:" << gv;
#endif
    initCalled = true;
#if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
    // Emulate the resizing done by the system
    const QSize initialSize(adjustedSize(size));
    if (gv && gv->size() != initialSize)
        // Set the graphics view size to match the display size.
        // Resize event handling calls _q_updateScreenSize to
        // finalize the initialization.
        gv->resize(initialSize);
    else
        // Finalize the initialization directly.
        _q_updateScreenSize(initialSize);
#else
    Q_UNUSED(size);
    Q_Q(SDeclarativeScreen);
    if (allowedOrientations != SDeclarativeScreen::Default)
        q->setAllowedOrientations(allowedOrientations);
    else
        _q_updateScreenSize(currentScreenSize());
#endif
}

void SDeclarativeScreenPrivate::_q_desktopResized(int screen)
{
    Q_UNUSED(screen)
#if defined(Q_WS_SIMULATOR)
    if (screen == QApplication::desktop()->primaryScreen()) {
        QSize current = currentScreenSize();
        // Move view's scene rect to origin
        if (gv)
            gv->setSceneRect(0, 0, gv->width(), gv->height());
#ifdef Q_DEBUG_SCREEN
        qDebug() << "_q_desktopResized():" << current;
#endif
        _q_updateScreenSize(current);
    }
#endif
}

bool SDeclarativeScreenPrivate::isLandscapeScreen() const
{
    return (screenSize.width() > screenSize.height());
}

QSize SDeclarativeScreenPrivate::currentScreenSize() const
{
#if defined(Q_OS_SYMBIAN)
    TPixelsTwipsAndRotation params;
    CWsScreenDevice *screenDevice = CEikonEnv::Static()->ScreenDevice();
    int mode = screenDevice->CurrentScreenMode();
    screenDevice->GetScreenModeSizeAndRotation(mode, params);
    QSize nSize(params.iPixelSize.iWidth, params.iPixelSize.iHeight);
    return nSize;
#elif defined(Q_WS_SIMULATOR)
    return QApplication::desktop()->screenGeometry().size();
#else
    return screenSize;
#endif
}

QSize SDeclarativeScreenPrivate::adjustedSize(const QSize &size) const
{
    const bool inLandscape = screenSize.width() > screenSize.height();

    const int shortEdge = qMin(size.width(), size.height());
    const int longEdge = qMax(size.width(), size.height());
    QSize newSize(longEdge, shortEdge); // landscape
    if (!landscapeAllowed() || (!inLandscape && portraitAllowed()))
        newSize.transpose(); // portrait
#ifdef Q_DEBUG_SCREEN
    qDebug() << "adjustedSize(): " << newSize;
#endif
    return newSize;
}

bool SDeclarativeScreenPrivate::portraitAllowed() const
{
    return allowedOrientations == SDeclarativeScreen::Default
        || allowedOrientations & (SDeclarativeScreen::Portrait | SDeclarativeScreen::PortraitInverted);
}

bool SDeclarativeScreenPrivate::landscapeAllowed() const
{
    return allowedOrientations == SDeclarativeScreen::Default
        || allowedOrientations & (SDeclarativeScreen::Landscape | SDeclarativeScreen::LandscapeInverted);
}

SDeclarativeScreen::SDeclarativeScreen(QDeclarativeEngine *engine, QObject *parent)
    : QObject(parent), d_ptr(new SDeclarativeScreenPrivate(this, engine))
{
}

SDeclarativeScreen::~SDeclarativeScreen()
{
}

SDeclarativeScreen::Orientation SDeclarativeScreen::currentOrientation() const
{
    Q_D(const SDeclarativeScreen);
    return d->currentOrientation;
}

SDeclarativeScreen::Orientations SDeclarativeScreen::allowedOrientations() const
{
    Q_D(const SDeclarativeScreen);
    return d->allowedOrientations;
}

void SDeclarativeScreen::setAllowedOrientations(Orientations orientations)
{
    Q_D(SDeclarativeScreen);
#ifdef Q_DEBUG_SCREEN
    qDebug() << "SDeclarativeScreen::setAllowedOrientations():" << orientations
        << " current allowedOrientations:" << d->allowedOrientations
        << " current orientation:" << d->currentOrientation;
#endif

    if (d->allowedOrientations == orientations)
        return;

    d->allowedOrientations = orientations;
    emit allowedOrientationsChanged();

    if (!d->initCalled)
        return;

#ifdef Q_OS_SYMBIAN
    if (d->portraitAllowed() && d->landscapeAllowed()) {
        TRAP_IGNORE(iAvkonAppUi->SetOrientationL(CAknAppUiBase::EAppUiOrientationAutomatic));
    } else if (d->portraitAllowed()) {
        TRAP_IGNORE(iAvkonAppUi->SetOrientationL(CAknAppUiBase::EAppUiOrientationPortrait));
        if (!d->initDone)
            d->_q_updateScreenSize(d->currentScreenSize());
    } else {
        TRAP_IGNORE(iAvkonAppUi->SetOrientationL(CAknAppUiBase::EAppUiOrientationLandscape));
        if (!d->initDone)
            d->_q_updateScreenSize(d->currentScreenSize());
    }
#else
    QSize newSize(d->adjustedSize(d->screenSize));
    if (newSize != d->screenSize) {
        if (d->gv)
            // update screen size via resize event
            d->gv->resize(newSize); // emulate the resizing done by the system
        else
            // update screen size directly
            d->_q_updateScreenSize(newSize);
    }
#endif
}

bool SDeclarativeScreen::eventFilter(QObject *obj, QEvent *event)
{
    Q_D(SDeclarativeScreen);
    Q_UNUSED(obj);
    if (event->type() == QEvent::Resize) {
        QSize s = static_cast<QResizeEvent*>(event)->size();
#ifdef Q_DEBUG_SCREEN
        qDebug() << "SDeclarativeScreen::eventFilter() size:" << s << " begin screen update";
#endif
        d->_q_updateScreenSize(s);
        return false;
    }
    return QObject::eventFilter(obj, event);
}

int SDeclarativeScreen::rotation() const
{
    Q_D(const SDeclarativeScreen);
    int angle = 0;
    bool nativePortrait = d->displaySize.width() < d->displaySize.height();
    switch (d->currentOrientation) {
    case Portrait:
        angle = nativePortrait ? 0 : 90;
        break;
    case Landscape:
        angle = nativePortrait ? 270 : 0;
        break;
    case PortraitInverted:
        angle = nativePortrait ? 180 : 270;
        break;
    case LandscapeInverted:
        angle = nativePortrait ? 90 : 180;
        break;
    default:
        break;
    }
    return angle;
}

int SDeclarativeScreen::width() const
{
    Q_D(const SDeclarativeScreen);
    return d->screenSize.width();
}

int SDeclarativeScreen::height() const
{
    Q_D(const SDeclarativeScreen);
    return d->screenSize.height();
}

int SDeclarativeScreen::displayWidth() const
{
    Q_D(const SDeclarativeScreen);
    return d->displaySize.width();
}

int SDeclarativeScreen::displayHeight() const
{
    Q_D(const SDeclarativeScreen);
    return d->displaySize.height();
}

qreal SDeclarativeScreen::dpi() const
{
    Q_D(const SDeclarativeScreen);
    return d->dpi;
}

SDeclarativeScreen::DisplayCategory SDeclarativeScreen::displayCategory() const
{
    Q_D(const SDeclarativeScreen);
    const int w = d->displaySize.width();
    const int h = d->displaySize.height();
    const qreal diagonal = qSqrt(static_cast<qreal>(w * w + h * h)) / d->dpi;

    if (diagonal < CATEGORY_SMALL_LIMIT)
        return Small;
    else if (diagonal < CATEGORY_MEDIUM_LIMIT)
        return Normal;
    else if (diagonal < CATEGORY_LARGE_LIMIT)
        return Large;
    else
        return ExtraLarge;
}

SDeclarativeScreen::Density SDeclarativeScreen::density() const
{
    Q_D(const SDeclarativeScreen);
    if (d->dpi < DENSITY_SMALL_LIMIT)
        return Low;
    else if (d->dpi < DENSITY_MEDIUM_LIMIT)
        return Medium;
    else if (d->dpi < DENSITY_LARGE_LIMIT)
        return High;
    else
        return ExtraHigh;
}

void SDeclarativeScreen::privateSetDisplay(int width, int height, qreal dpi)
{
#ifdef Q_OS_SYMBIAN
    Q_UNUSED(width);
    Q_UNUSED(height);
    Q_UNUSED(dpi);
#else
    Q_D(SDeclarativeScreen);
    d->settingDisplay = true;

    qreal oldDpi = d->dpi;
    QSize oldDisplaySize = d->displaySize;

    d->dpi = dpi;
    if (d->gv)
        // update screen size via resize event
        d->gv->resize(width, height); // emulate the resizing done by the system
    else
        // update screen size directly
        d->_q_updateScreenSize(QSize(width, height));

    if (oldDpi != d->dpi || oldDisplaySize != d->displaySize)
        emit displayChanged();
    d->settingDisplay = false;
#endif
}
#include "moc_sdeclarativescreen.cpp"
