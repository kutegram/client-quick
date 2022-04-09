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

#include "sdeclarativeindicatordata.h"

#include <aknconsts.h> // KAvkonBitmapFile
#include <barsread.h> // TResourceReader

CSDeclarativeIndicatorData* CSDeclarativeIndicatorData::NewL( TResourceReader& aReader )
    {
    CSDeclarativeIndicatorData* self = new (ELeave) CSDeclarativeIndicatorData();
    CleanupStack::PushL( self );
    self->ConstructFromResourceL( aReader );
    CleanupStack::Pop( self );
    return self;
    }

CSDeclarativeIndicatorData::CSDeclarativeIndicatorData() :
    iState( EAknIndicatorStateOff ),
    iFrameCount( 1 )
    {
    }

CSDeclarativeIndicatorData::~CSDeclarativeIndicatorData()
    {
    iIndicatorBitmapIndexes.Close();
    delete iBitmapFile;
    }

void CSDeclarativeIndicatorData::ConstructFromResourceL( TResourceReader& aReader )
    {
    iUid = aReader.ReadInt16();

    iMultiColorMode = (iUid == EAknIndicatorVtRecord);

    iPriority = aReader.ReadInt16(); // narrow priority
    aReader.ReadInt16(); // ignore the wide priority
    iBitmapFile = aReader.ReadHBufCL(); // bmp filename

    // If default icon file is used we delete filename to save some memory
   if ( iBitmapFile && !iBitmapFile->CompareF( KAvkonBitmapFile ) )
        {
        delete iBitmapFile;
        iBitmapFile = NULL;
        }

    TInt count = aReader.ReadInt16();  // Number of states

    for ( TInt i = 0; i < count; ++i )
        {
        TInt stateId = aReader.ReadInt16();
        if ( stateId != EAknIndicatorStateOff )
            {
            TInt iconCount = aReader.ReadInt16();
            if ( stateId == EAknIndicatorStateAnimate )
                iFrameCount = iconCount;

            ReadBitmapIndexesL( aReader, iconCount );
            }
        }
    }

TInt CSDeclarativeIndicatorData::Uid() const
    {
    return iUid;
    }

TInt CSDeclarativeIndicatorData::Priority() const
    {
    return iPriority;
    }

TInt CSDeclarativeIndicatorData::FrameCount() const
    {
    return iFrameCount;
    }

TBool CSDeclarativeIndicatorData::MultiColorMode() const
    {
    return iMultiColorMode;
    }

const TDesC& CSDeclarativeIndicatorData::BitmapFile() const
    {
    if ( iBitmapFile )
        return *iBitmapFile;

    return KNullDesC();
    }

TInt CSDeclarativeIndicatorData::State() const
    {
    return iState;
    }

void CSDeclarativeIndicatorData::SetState( TInt aState )
    {
    iState = aState;
    }

TInt CSDeclarativeIndicatorData::CurrentFrame() const
    {
    return iCurrentFrame;
    }

void CSDeclarativeIndicatorData::SetCurrentFrame( TInt aCurrentFrame )
    {
    iCurrentFrame = aCurrentFrame;
    }

TInt CSDeclarativeIndicatorData::GetIndicatorBitmapIndexes( RArray<TInt>& aIndexes ) const
    {
    TInt err = KErrNone;
    for ( TInt i = 0; i < iIndicatorBitmapIndexes.Count() && err == KErrNone; ++i )
        {
        err = aIndexes.Append( iIndicatorBitmapIndexes[i] );
        }
    return err;
    }

void CSDeclarativeIndicatorData::ReadBitmapIndexesL( TResourceReader& aReader, TInt aCount )
    {
    for ( TInt i = 0; i < aCount; ++i )
        {
        TInt bitmapId = aReader.ReadInt16(); // narrow bitmap id
        TInt maskId = aReader.ReadInt16(); // narrow mask id

        if ( bitmapId == KErrNotFound ) // no bitmap
            {
            bitmapId = 0;
            maskId = 0;
            }
        else if ( maskId == KErrNotFound ) // no mask, just bitmap
            {
            maskId = bitmapId;
            }

        User::LeaveIfError( iIndicatorBitmapIndexes.Append( bitmapId ) );
        User::LeaveIfError( iIndicatorBitmapIndexes.Append( maskId ) );


        // ignore wide bitmaps
        aReader.ReadInt16(); // wide bitmap id
        aReader.ReadInt16(); // wide mask id
        }
    }

//  End of File
