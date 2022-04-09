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

#include "sdeclarativestatuspanedatasubscriber.h"

// #define Q_DEBUG_SUBSCRIBER

#ifdef Q_DEBUG_SUBSCRIBER
#include <QDebug>
#endif // Q_DEBUG_SUBSCRIBER

const TUint32 KAknStatusPaneSystemData = 0x10000000;
const TUid KPSUidAvkonInternal = { 0x10207218 };


CSDeclarativeStatusPaneSubscriber::CSDeclarativeStatusPaneSubscriber(
    MSDeclarativeStatusPaneSubscriberObverver& aObserver )
: CActive( CActive::EPriorityStandard ), iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    iInitialized = EFalse;

    // initialize the array to 'empty'
    for ( TInt i = 0; i < TAknIndicatorState::EMaxVisibleIndicators; ++i )
        {
        iIndicatorState.visibleIndicators[i] = KIndicatorEmptySpot;
        }
    }

CSDeclarativeStatusPaneSubscriber* CSDeclarativeStatusPaneSubscriber::NewL(
    MSDeclarativeStatusPaneSubscriberObverver& aObserver )
    {
    CSDeclarativeStatusPaneSubscriber* self =
        new ( ELeave ) CSDeclarativeStatusPaneSubscriber( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CSDeclarativeStatusPaneSubscriber::ConstructL()
    {
    User::LeaveIfError( iProperty.Attach( KPSUidAvkonInternal, KAknStatusPaneSystemData ) );
    DoSubscribe();
    }

CSDeclarativeStatusPaneSubscriber::~CSDeclarativeStatusPaneSubscriber()
    {
    Cancel();
    iProperty.Close();
    }

void CSDeclarativeStatusPaneSubscriber::EnsureData()
    {
    if ( !iInitialized )
        {
        TAknStatusPaneStateData::TAknStatusPaneStateDataPckg statusPaneStateDataPckg( iStateData );
        TInt err = iProperty.Get( KPSUidAvkonInternal, KAknStatusPaneSystemData, statusPaneStateDataPckg );
        if ( err == KErrNone )
            {
            iIndicatorState = iStateData.iIndicatorState;
            iSignalState = iStateData.iSignalState;
            iInitialized = ETrue;
            }
#ifdef Q_DEBUG_SUBSCRIBER
        else
            {
            qDebug() << "CSDeclarativeStatusPaneSubscriber::IndicatorState() cannot read status pane data";
            }
#endif // Q_DEBUG_SUBSCRIBER
        }
    }

TAknIndicatorState CSDeclarativeStatusPaneSubscriber::IndicatorState() const
    {
    const_cast<CSDeclarativeStatusPaneSubscriber*>(this)->EnsureData();
    return iIndicatorState;
    }

TAknSignalState CSDeclarativeStatusPaneSubscriber::SignalState() const
    {
    const_cast<CSDeclarativeStatusPaneSubscriber*>(this)->EnsureData();
    return iSignalState;
    }

void CSDeclarativeStatusPaneSubscriber::DoCancel()
    {
    iProperty.Cancel();
    }


void CSDeclarativeStatusPaneSubscriber::DoSubscribe()
    {
    if ( !IsActive() )
        {
        iProperty.Subscribe( iStatus );
        SetActive();
        }
    }

TBool IndicatorStatesEqual( const TAknIndicatorState& state1, const TAknIndicatorState& state2 )
    {
    TBool same = ETrue;
    for ( TInt i = 0; i < TAknIndicatorState::EMaxVisibleIndicators && same; ++i ) {
        if ( state1.visibleIndicators[i] != state2.visibleIndicators[i] )
            {
            same = EFalse;
            }
        else if ( state1.visibleIndicators[i] != KIndicatorEmptySpot )
            {
            if ( state1.visibleIndicatorStates[i] != state2.visibleIndicatorStates[i] )
                {
                same = EFalse;
                }
            }
        }
    return same;
    }

TBool SignalStatesEqual( const TAknSignalState& state1, const TAknSignalState& state2 )
    {
    return state1.iIconState == state2.iIconState && state1.iSignalStrength == state2.iSignalStrength;
    }

void CSDeclarativeStatusPaneSubscriber::RunL()
    {
    if ( iStatus != KErrNone )
        {
#ifdef Q_DEBUG_SUBSCRIBER
        qDebug() << "CSDeclarativeStatusPaneSubscriber::RunL error in status pane data change, iStatus: " << iStatus;
#endif // Q_DEBUG_SUBSCRIBER
        return;
        }

    // resubscribe before processing new value to prevent missing updates
    DoSubscribe();

    TAknStatusPaneStateData::TAknStatusPaneStateDataPckg statusPaneStateDataPckg( iStateData );
    iProperty.Get( KPSUidAvkonInternal, KAknStatusPaneSystemData, statusPaneStateDataPckg );

    int changeFlags = MSDeclarativeStatusPaneSubscriberObverver::EStatusPaneUndefined;
    if ( !iInitialized || !IndicatorStatesEqual( iStateData.iIndicatorState, iIndicatorState ) )
        {
        iIndicatorState = iStateData.iIndicatorState;
        changeFlags += MSDeclarativeStatusPaneSubscriberObverver::EStatusPaneIndicatorsState;
        }

    if ( !iInitialized || !SignalStatesEqual( iStateData.iSignalState, iSignalState ) )
        {
        iSignalState = iStateData.iSignalState;
        changeFlags |= MSDeclarativeStatusPaneSubscriberObverver::EStatusPaneSignalState;
        }

    iInitialized = ETrue;

    if ( changeFlags != MSDeclarativeStatusPaneSubscriberObverver::EStatusPaneUndefined ) {
        iObserver.StatusPaneStateChanged(
            MSDeclarativeStatusPaneSubscriberObverver::TStatusPaneChangeFlags( changeFlags ) );
        }
    }

//  End of File
