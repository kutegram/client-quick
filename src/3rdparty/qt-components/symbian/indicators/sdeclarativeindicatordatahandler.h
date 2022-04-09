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

#ifndef SDECLARATIVEINDICATORDATAHANDLER_H
#define SDECLARATIVEINDICATORDATAHANDLER_H

#include <sdeclarativestatuspanedatasubscriber.h>
#include <e32base.h>
#include <QList>
#include <QHash>

class CSDeclarativeStatusPaneSubscriber;
class CSDeclarativeIndicatorData;
class SDeclarativeIndicator;
class SDeclarativeIndicatorContainer;

class CSDeclarativeIndicatorDataHandler: public CBase, public MSDeclarativeStatusPaneSubscriberObverver
{
public:
    static CSDeclarativeIndicatorDataHandler* NewL( SDeclarativeIndicatorContainer* aContainer );
    ~CSDeclarativeIndicatorDataHandler();

private:
    CSDeclarativeIndicatorDataHandler( SDeclarativeIndicatorContainer* aContainer );
    void ConstructL();
    void LoadIndicatorDataL() ;
    void UpdateIndicators();
    CSDeclarativeIndicatorData* Data( TInt aUid ) const;
    SDeclarativeIndicator* Indicator( TInt aUid ) const;
    void PrioritizeIndicators();
    TRect Rect() const;
    static TInt InitializeCallBack( TAny* aAny );

private: // from MSDeclarativeStatusPaneSubscriberObverver
    void StatusPaneStateChanged( TStatusPaneChangeFlags aChangeFlags );

private:
    CSDeclarativeStatusPaneSubscriber* iSubscriber;
    SDeclarativeIndicatorContainer* iContainer;
    QHash<TInt, CSDeclarativeIndicatorData*> iIndicatorsData;
    QList<SDeclarativeIndicator*> iIndicatorItems;
    CAsyncCallBack* iInitializer;
};

#endif // SDECLARATIVEINDICATORDATAHANDLER_H
