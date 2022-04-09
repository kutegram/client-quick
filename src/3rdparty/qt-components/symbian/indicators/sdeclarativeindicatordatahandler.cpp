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

#include "sdeclarativeindicatordatahandler.h"
#include "sdeclarativestatuspanedatasubscriber.h"
#include "sdeclarativeindicatordata.h"
#include "sdeclarativeindicator.h"
#include "sdeclarativeindicatorcontainer.h"

#include <avkon.hrh> // EAknIndicatorStateOff
#include <avkon.rsg> // R_AVKON_STATUS_PANE_INDICATOR_DEFAULT
#include <barsread.h> // TResourceReader
#include <coemain.h> // CCoeEnv

CSDeclarativeIndicatorDataHandler* CSDeclarativeIndicatorDataHandler::NewL(
    SDeclarativeIndicatorContainer* aContainer )
    {
    CSDeclarativeIndicatorDataHandler* self = new CSDeclarativeIndicatorDataHandler( aContainer );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CSDeclarativeIndicatorDataHandler::ConstructL()
    {
    // subsciber reads the status pane data
    iSubscriber = CSDeclarativeStatusPaneSubscriber::NewL( *this );

    TCallBack callback( InitializeCallBack, this );
    iInitializer = new (ELeave) CAsyncCallBack( callback, CActive::EPriorityLow );
    iInitializer->CallBack(); // shoot
    }

CSDeclarativeIndicatorDataHandler::~CSDeclarativeIndicatorDataHandler()
    {
    delete iInitializer;
    delete iSubscriber;

    while (iIndicatorItems.count())
        delete iIndicatorItems.takeLast();

    while (iIndicatorsData.count())
        delete iIndicatorsData.take( iIndicatorsData.keys().at(0) );

    }

CSDeclarativeIndicatorDataHandler::CSDeclarativeIndicatorDataHandler(
    SDeclarativeIndicatorContainer* aContainer )
: iContainer( aContainer )
    {
    }

TInt CSDeclarativeIndicatorDataHandler::InitializeCallBack( TAny* aAny )
    {
    CSDeclarativeIndicatorDataHandler* thisPtr = static_cast<CSDeclarativeIndicatorDataHandler*>( aAny );
    if ( !thisPtr )
        {
        return KErrGeneral;
        }

    if ( !thisPtr->iIndicatorItems.count() )
        {
        TBool needsUpdating = EFalse;
        const TAknIndicatorState& indicatorState = thisPtr->iSubscriber->IndicatorState();
        for ( TInt i = 0; i < TAknIndicatorState::EMaxVisibleIndicators && !needsUpdating; ++i )
            {
            if ( indicatorState.visibleIndicators[i] != KIndicatorEmptySpot )
                {
                needsUpdating = ETrue;
                }
            }

        if ( needsUpdating )
            {
            thisPtr->UpdateIndicators();
            }
        }

    delete thisPtr->iInitializer;
    thisPtr->iInitializer = NULL;
    return KErrNone;
    }

void CSDeclarativeIndicatorDataHandler::StatusPaneStateChanged( TStatusPaneChangeFlags aChangeFlags )
    {
    if ( aChangeFlags&MSDeclarativeStatusPaneSubscriberObverver::EStatusPaneIndicatorsState )
        {
        UpdateIndicators();
        }
    }

void CSDeclarativeIndicatorDataHandler::LoadIndicatorDataL()
    {
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, R_AVKON_STATUS_PANE_INDICATOR_DEFAULT );

    TInt count = reader.ReadInt16();
    for ( TInt i = 0; i < count; ++i )
        {
        CSDeclarativeIndicatorData* indicatorData = CSDeclarativeIndicatorData::NewL( reader );
        iIndicatorsData.insert( indicatorData->Uid(), indicatorData );
        }

    CleanupStack::PopAndDestroy( 1 ); // Resource reader
    }

CSDeclarativeIndicatorData *CSDeclarativeIndicatorDataHandler::Data( TInt aUid ) const
    {
    if ( !iIndicatorsData.count() )
        {
        TRAP_IGNORE(const_cast<CSDeclarativeIndicatorDataHandler*>(this)->LoadIndicatorDataL());
        }

    if ( iIndicatorsData.contains( aUid ) )
        {
        return iIndicatorsData.value( aUid );
        }

    return 0;
    }

SDeclarativeIndicator *CSDeclarativeIndicatorDataHandler::Indicator( TInt aUid ) const
    {
    foreach ( SDeclarativeIndicator *indicator, iIndicatorItems )
        {
        if ( indicator->uid() == aUid )
            {
            return indicator;
            }
        }
    return 0;
    }

void CSDeclarativeIndicatorDataHandler::UpdateIndicators()
    {
    const TAknIndicatorState& indicatorState = iSubscriber->IndicatorState();
    bool changed = false;
    QList<SDeclarativeIndicator*> newIndicatorItems;

    // go through the visible ones
    for ( TInt i = 0; i < TAknIndicatorState::EMaxVisibleIndicators; ++i )
        {
        const TInt visibleUid = indicatorState.visibleIndicators[i];
        if ( visibleUid != KIndicatorEmptySpot )
            {
            // check if there is an existing istance
            SDeclarativeIndicator* indicator = Indicator( visibleUid );
            if ( indicator )
                {
                // remove from old list
                iIndicatorItems.removeOne( indicator );
                indicator->setParentItem( 0 ); // temporarily remove so we can re-arrange
                }
            else
                {
                // create new one
                CSDeclarativeIndicatorData *data = Data( visibleUid );
                indicator = new SDeclarativeIndicator( data, 0 );
                indicator->setColor( iContainer->indicatorColor() );
                QObject::connect( iContainer, SIGNAL(indicatorColorChanged(const QColor&)),
                                  indicator, SLOT(setColor(const QColor&)) );
                changed = true;
                }
            newIndicatorItems.append( indicator );
            changed |= indicator->setState( indicatorState.visibleIndicatorStates[i] );
            }
        }

    // delete old list
    while( iIndicatorItems.count() )
        {
        SDeclarativeIndicator *deletedIndicator = iIndicatorItems.at( iIndicatorItems.count() - 1 );
        deletedIndicator->setState( EAknIndicatorStateOff );
        iIndicatorItems.removeLast();
        delete deletedIndicator;
        changed = true;
        }

    // take the new one into use
    iIndicatorItems = newIndicatorItems;

    if ( changed )
        {
        PrioritizeIndicators();
        }

    // set parent in priority order
    foreach ( SDeclarativeIndicator *indicator, iIndicatorItems )
        {
        indicator->setParentItem( iContainer );
        }

    iContainer->layoutChildren();
    }

bool IndicatorPriorityLessThan( SDeclarativeIndicator *i1, SDeclarativeIndicator *i2 )
     {
     return i1->priority() < i2->priority();
     }

void CSDeclarativeIndicatorDataHandler::PrioritizeIndicators()
    {
    if ( iIndicatorItems.count() >= 2 )
        {
        qSort( iIndicatorItems.begin(), iIndicatorItems.end(), IndicatorPriorityLessThan );
        }
    }

//  End of File



