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

#ifndef SDECLARATIVE_H
#define SDECLARATIVE_H

#include <QtCore/qobject.h>
#include <QtDeclarative/qdeclarative.h>

class SDeclarativePrivate;

class SDeclarative : public QObject
{
    Q_OBJECT
    Q_PROPERTY(InteractionMode listInteractionMode READ listInteractionMode WRITE setListInteractionMode NOTIFY listInteractionModeChanged FINAL)
    Q_PROPERTY(QString currentTime READ currentTime NOTIFY currentTimeChanged)
    Q_PROPERTY(bool foreground READ isForeground NOTIFY foregroundChanged)
    Q_PROPERTY(S60Version s60Version READ s60Version CONSTANT)

    Q_ENUMS(InteractionMode S60Version ScrollBarVisibility SourceSize EffectType Feedback)

public:
    explicit SDeclarative(QObject *parent = 0);
    virtual ~SDeclarative();

    enum InteractionMode {
        TouchInteraction,
        KeyNavigation
    };

    enum S60Version {
        SV_S60_5_2 = 0, // Symbian Anna
        SV_S60_5_3,
        SV_S60_5_4,
        SV_S60_UNKNOWN
    };

    // No duplicate prefix naming because enum values are only visible via qml
    enum ScrollBarVisibility {
        ScrollBarWhenNeeded = 0,
        ScrollBarWhenScrolling
    };

    enum SourceSize {
        UndefinedSourceDimension = -2
    };

    enum EffectType {
        FadeOut,
        FadeInFadeOut
    };

    enum Feedback {
        Basic,
        Sensitive,
        BasicButton,
        SensitiveButton,
        BasicKeypad,
        SensitiveKeypad,
        BasicSlider,
        SensitiveSlider,
        BasicItem,
        SensitiveItem,
        ItemScroll,
        ItemPick,
        ItemDrop,
        ItemMoveOver,
        BounceEffect,
        CheckBox,
        MultipleCheckBox,
        Editor,
        TextSelection,
        BlankSelection,
        LineSelection,
        EmptyLineSelection,
        PopUp,
        PopupOpen,
        PopupClose,
        Flick,
        StopFlick,
        MultiPointTouchActivate,
        RotateStep,
        LongPress,
        PositiveTacticon,
        NeutralTacticon,
        NegativeTacticon
    };

    static inline QString resolveIconFileName(const QString &iconName)
    {
        QString fileName = iconName;
        if (fileName.startsWith(QLatin1String("qtg")) ||
                fileName.startsWith(QLatin1String("toolbar-")))
            fileName.prepend(QLatin1String(":/graphics/"));

        if (fileName.lastIndexOf(QLatin1Char('.')) == -1)
            fileName.append(QLatin1String(".svg"));

        return fileName;
    }

    InteractionMode listInteractionMode() const;
    void setListInteractionMode(InteractionMode mode);

    static QString currentTime();
    bool isForeground();
    S60Version s60Version() const;

    Q_INVOKABLE int privateAllocatedMemory() const;
    Q_INVOKABLE void privateClearIconCaches();
    Q_INVOKABLE void privateClearComponentCache();

Q_SIGNALS:
    void listInteractionModeChanged();
    void currentTimeChanged();
    void foregroundChanged();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Q_DISABLE_COPY(SDeclarative)
    QScopedPointer<SDeclarativePrivate> d_ptr;
};

class SDialogStatus : public QObject
{
    Q_OBJECT
    Q_ENUMS(Status)

public:
    enum Status {
        Opening,
        Open,
        Closing,
        Closed
    };
};

class SPageOrientation : public QObject
{
    Q_OBJECT
    Q_ENUMS(PageOrientation)

public:
    enum PageOrientation {
        Automatic,
        LockPortrait,
        LockLandscape,
        LockPrevious,
        Manual
    };
};

class SPageStatus : public QObject
{
    Q_OBJECT
    Q_ENUMS(Status)

public:
    enum Status {
        Inactive,
        Activating,
        Active,
        Deactivating
    };
};

QML_DECLARE_TYPE(SDeclarative)

#endif // SDECLARATIVE_H
