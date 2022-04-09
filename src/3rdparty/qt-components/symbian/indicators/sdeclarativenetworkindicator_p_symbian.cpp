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

#include "sdeclarativenetworkindicator_p.h"
#include "sdeclarativenetworkindicator.h"
#include "sdeclarativestatuspanedatasubscriber.h"

#include <e32base.h>
#include <AknsUtils.h> // AknsUtils
#include <avkon.mbg>   // bitmap enumerations

class SDeclarativeNetworkIndicatorPrivateImpl: public MSDeclarativeStatusPaneSubscriberObverver
{
public:
    SDeclarativeNetworkIndicatorPrivateImpl(
        SDeclarativeNetworkIndicator *qq,
        SDeclarativeNetworkIndicatorPrivate *dd);
    ~SDeclarativeNetworkIndicatorPrivateImpl();

    void StatusPaneStateChanged(TStatusPaneChangeFlags aChangeFlags);

    SDeclarativeNetworkIndicator *q_ptr;
    SDeclarativeNetworkIndicatorPrivate *d_ptr;

    CSDeclarativeStatusPaneSubscriber *subscriber;

    int currentState;
    QPixmap pixmap;
};

SDeclarativeNetworkIndicatorPrivateImpl::~SDeclarativeNetworkIndicatorPrivateImpl()
{
    delete subscriber;
}

SDeclarativeNetworkIndicatorPrivateImpl::SDeclarativeNetworkIndicatorPrivateImpl(
    SDeclarativeNetworkIndicator *qq, SDeclarativeNetworkIndicatorPrivate *dd)
    : q_ptr(qq), d_ptr(dd), subscriber(0), currentState(-1)
{
    QT_TRAP_THROWING(subscriber = CSDeclarativeStatusPaneSubscriber::NewL(*this));
    // Initialize network state and offline status
    const TAknSignalState &signalState = subscriber->SignalState();
    currentState = signalState.iIconState;
    dd->offline = (signalState.iSignalStrength == KAknSignalOffLineMode);
}

void SDeclarativeNetworkIndicatorPrivateImpl::StatusPaneStateChanged(TStatusPaneChangeFlags aChangeFlags)
{
    if (aChangeFlags & MSDeclarativeStatusPaneSubscriberObverver::EStatusPaneSignalState) {
        const TAknSignalState &signalState = subscriber->SignalState();
        bool newOffline(signalState.iSignalStrength == KAknSignalOffLineMode);
        bool offlineChanged(d_ptr->offline != newOffline);

        if (offlineChanged) {
            d_ptr->offline = newOffline;
            emit q_ptr->offlineChanged();
        }
        if (currentState != signalState.iIconState || offlineChanged) {
            currentState = signalState.iIconState;
            d_ptr->reset();
            q_ptr->update();
        }
    }
}

SDeclarativeNetworkIndicatorPrivate::SDeclarativeNetworkIndicatorPrivate(
    SDeclarativeNetworkIndicator *qq) : offline(false), q_ptr(qq)
{
    impl = new SDeclarativeNetworkIndicatorPrivateImpl(qq, this);
}

SDeclarativeNetworkIndicatorPrivate::~SDeclarativeNetworkIndicatorPrivate()
{
    delete impl;
}

void SDeclarativeNetworkIndicatorPrivate::reset()
{
    impl->pixmap = QPixmap();
}

QPixmap SDeclarativeNetworkIndicatorPrivate::pixmap()
{
    if (impl->pixmap.isNull()) {
        TInt bitmapId = 0;
        TInt maskId = 0;
        CFbsBitmap *tempBitmap = 0;
        CFbsBitmap *tempMask = 0;
        if (offline) {
            bitmapId = EMbmAvkonQgn_indi_signal_offline;
            maskId = EMbmAvkonQgn_indi_signal_offline_mask;
        } else {
            switch (impl->currentState) {
                // Legacy GPRS indicators & common packet data indicators
                case EAknSignalGprsIndicatorAttached:
                case EAknSignalGprsIndicatorEstablishingContext: // fallthrough
                case EAknSignalCommonPacketDataIndicatorAttached: // fallthrough
                case EAknSignalCommonPacketDataIndicatorEstablishingContext: // fallthrough
                    bitmapId = EMbmAvkonQgn_indi_signal_pd_attach;
                    maskId = EMbmAvkonQgn_indi_signal_pd_attach_mask;
                    break;
                case EAknSignalGprsIndicatorContext:
                case EAknSignalCommonPacketDataIndicatorContext: // fallthrough
                    bitmapId = EMbmAvkonQgn_indi_signal_pd_context;
                    maskId = EMbmAvkonQgn_indi_signal_pd_context_mask;
                    break;
                case EAknSignalGprsIndicatorSuspended:
                case EAknSignalCommonPacketDataIndicatorSuspended: // fallthrough
                    bitmapId = EMbmAvkonQgn_indi_signal_pd_suspended;
                    maskId = EMbmAvkonQgn_indi_signal_pd_suspended_mask;
                    break;
                case EAknSignalGprsIndicatorMultipdp:
                case EAknSignalCommonPacketDataIndicatorMultipdp: // fallthrough
                    bitmapId = EMbmAvkonQgn_indi_signal_pd_multipdp;
                    maskId = EMbmAvkonQgn_indi_signal_pd_multipdp_mask;
                    break;

                // EDGE packet data indicator
                case EAknSignalEdgeIndicatorAvailable:
                    bitmapId = EMbmAvkonQgn_indi_signal_egprs_icon;
                    maskId = EMbmAvkonQgn_indi_signal_egprs_icon_mask;
                    break;
                case EAknSignalEdgeIndicatorAttached:
                case EAknSignalEdgeIndicatorEstablishingContext: // fallthrough
                    bitmapId = EMbmAvkonQgn_indi_signal_egprs_attach;
                    maskId = EMbmAvkonQgn_indi_signal_egprs_attach_mask;
                    break;
                case EAknSignalEdgeIndicatorContext:
                    bitmapId = EMbmAvkonQgn_indi_signal_egprs_context;
                    maskId = EMbmAvkonQgn_indi_signal_egprs_context_mask;
                    break;
                case EAknSignalEdgeIndicatorSuspended:
                    bitmapId = EMbmAvkonQgn_indi_signal_egprs_suspended;
                    maskId = EMbmAvkonQgn_indi_signal_egprs_suspended_mask;
                    break;
                case EAknSignalEdgeIndicatorMultipdp:
                    bitmapId = EMbmAvkonQgn_indi_signal_egprs_multipdp;
                    maskId = EMbmAvkonQgn_indi_signal_egprs_multipdp_mask;
                    break;

                // WCDMA indicators
                case EAknSignalWcdmaIndicatorAvailable:
                    bitmapId = EMbmAvkonQgn_indi_signal_wcdma_icon;
                    maskId = EMbmAvkonQgn_indi_signal_wcdma_icon_mask;
                    break;
                case EAknSignalWcdmaIndicatorAttached:
                case EAknSignalWcdmaIndicatorEstablishingContext: // fallthrough
                    bitmapId = EMbmAvkonQgn_indi_signal_wcdma_attach;
                    maskId = EMbmAvkonQgn_indi_signal_wcdma_attach_mask;
                    break;
                case EAknSignalWcdmaIndicatorContext:
                    bitmapId = EMbmAvkonQgn_indi_signal_wcdma_context;
                    maskId = EMbmAvkonQgn_indi_signal_wcdma_context_mask;
                    break;
                case EAknSignalWcdmaIndicatorMultipdp:
                    bitmapId = EMbmAvkonQgn_indi_signal_wcdma_multipdp;
                    maskId = EMbmAvkonQgn_indi_signal_wcdma_multipdp_mask;
                    break;
                case EAknSignalWcdmaIndicatorSuspended:
                    bitmapId = EMbmAvkonQgn_indi_signal_wcdma_suspended;
                    maskId = EMbmAvkonQgn_indi_signal_wcdma_suspended_mask;
                    break;

                // HSDPA indicators
                case EAknSignalHsdpaIndicatorAvailable:
                    bitmapId = EMbmAvkonQgn_indi_signal_hsdpa_icon;
                    maskId = EMbmAvkonQgn_indi_signal_hsdpa_icon_mask;
                    break;
                case EAknSignalHsdpaIndicatorAttached:
                case EAknSignalHsdpaIndicatorEstablishingContext: // fallthrough
                    bitmapId = EMbmAvkonQgn_indi_signal_hsdpa_attach;
                    maskId = EMbmAvkonQgn_indi_signal_hsdpa_attach_mask;
                    break;
                case EAknSignalHsdpaIndicatorContext:
                    bitmapId = EMbmAvkonQgn_indi_signal_hsdpa_context;
                    maskId = EMbmAvkonQgn_indi_signal_hsdpa_context_mask;
                    break;
                case EAknSignalHsdpaIndicatorSuspended:
                    bitmapId = EMbmAvkonQgn_indi_signal_hsdpa_suspended;
                    maskId = EMbmAvkonQgn_indi_signal_hsdpa_suspended_mask;
                    break;
                case EAknSignalHsdpaIndicatorMultipdp:
                    bitmapId = EMbmAvkonQgn_indi_signal_hsdpa_multipdp;
                    maskId = EMbmAvkonQgn_indi_signal_hsdpa_multipdp_mask;
                    break;

                // UMA indicators
                case EAknSignalUmaIndicatorAvailable:
                    bitmapId = EMbmAvkonQgn_indi_signal_uma_icon;
                    maskId = EMbmAvkonQgn_indi_signal_uma_icon_mask;
                    break;
                case EAknSignalUmaIndicatorAttached:
                case EAknSignalUmaIndicatorEstablishingContext: // fallthrough
                    bitmapId = EMbmAvkonQgn_indi_signal_uma_attach;
                    maskId = EMbmAvkonQgn_indi_signal_uma_attach_mask;
                    break;
                case EAknSignalUmaIndicatorContext:
                    bitmapId = EMbmAvkonQgn_indi_signal_uma_context;
                    maskId = EMbmAvkonQgn_indi_signal_uma_context_mask;
                    break;
                case EAknSignalUmaIndicatorSuspended:
                    bitmapId = EMbmAvkonQgn_indi_signal_uma_suspended;
                    maskId = EMbmAvkonQgn_indi_signal_uma_suspended_mask;
                    break;
                case EAknSignalUmaIndicatorMultipdp:
                    bitmapId = EMbmAvkonQgn_indi_signal_uma_multipdp;
                    maskId = EMbmAvkonQgn_indi_signal_uma_multipdp_mask;
                    break;

                // Default indicator
                case EAknSignalGprsIndicatorOff:
                case EAknSignalCommonPacketDataIndicatorOff: // fallthrough
                case EAknSignalEdgeIndicatorOff: // fallthrough
                case EAknSignalWcdmaIndicatorOff: // fallthrough
                case EAknSignalHsdpaIndicatorOff: // fallthrough
                case EAknSignalUmaIndicatorOff: // fallthrough
                case EAknSignalGprsIndicatorAvailable: // fallthrough
                case EAknSignalCommonPacketDataIndicatorAvailable: // fallthrough
                default:
                    bitmapId = EMbmAvkonQgn_prop_signal_icon;
                    maskId = EMbmAvkonQgn_prop_signal_icon_mask;
                    break;
            }
        }

        const TAknsItemID dummyId = {0, 0};
        const TAknsItemID dummyColorGroup = {0, 0};
        const TInt dummyColorIndex = 0;

        const TRgb symbianColor(color.red(), color.green(), color.blue(), color.alpha());

        QT_TRAP_THROWING({
            AknsUtils::CreateColorIconL(
                AknsUtils::SkinInstance(),
                dummyId,
                dummyColorGroup,
                dummyColorIndex,
                tempBitmap,
                tempMask,
                AknIconUtils::AvkonIconFileName(),
                bitmapId,
                maskId,
                symbianColor);
            });

        if (tempBitmap) {
            TSize symbianSize(int(q_ptr->width() + 0.5), int(q_ptr->height() + 0.5));
            AknIconUtils::SetSize(tempBitmap, symbianSize);
            impl->pixmap = QPixmap::fromSymbianCFbsBitmap(tempBitmap);
            if (tempMask) {
                AknIconUtils::SetSize(tempMask, symbianSize);
                QPixmap maskPixamp = QPixmap::fromSymbianCFbsBitmap(tempMask);
                impl->pixmap.setAlphaChannel(maskPixamp);
            }
        }
        delete tempBitmap;
        delete tempMask;
    }
    return impl->pixmap;
}

//  End of File
