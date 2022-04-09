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

#include "qdatetimemodel.h"
#include "qdatetimemodel_p.h"

#include <math.h>
#include <QtCore/qdebug.h>
#include <QtCore/qlocale.h>

// ### FIX THIS INCLUDE ###
#include <QtCore/private/qdatetime_p.h>

/*!
  \internal
  Constructs a QDateTimeModelPrivate object
*/


QDateTimeModelPrivate::QDateTimeModelPrivate(QDateTimeModel *qq)
    : QDateTimeParser(QVariant::DateTime, QDateTimeParser::DateTimeEdit),
      q_ptr(qq)
{
    minimum = QDATETIMEEDIT_COMPAT_DATETIME_MIN;
    maximum = QDATETIMEEDIT_DATETIME_MAX;
}

void QDateTimeModelPrivate::setRange(const QDateTime &min, const QDateTime &max)
{
    minimum = min.toTimeSpec(spec);
    maximum = max.toTimeSpec(spec);
}

void QDateTimeModelPrivate::updateTimeSpec()
{
    minimum = minimum.toTimeSpec(spec);
    maximum = maximum.toTimeSpec(spec);
    value = value.toTimeSpec(spec);
}

QVariant QDateTimeModelPrivate::getZeroVariant() const
{
    return QDateTime(QDATETIMEEDIT_DATE_INITIAL, QTime(), spec);
}

// QDateTimeModel

QDateTimeModel::QDateTimeModel(QObject *parent)
    : QObject(parent), d_ptr(new QDateTimeModelPrivate(this))
{
}

QDateTimeModel::QDateTimeModel(QDateTimeModelPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
}

QDateTimeModel::~QDateTimeModel()
{
    delete d_ptr;
}

QDateTime QDateTimeModel::dateTime() const
{
    Q_D(const QDateTimeModel);
    return d->value;
}

void QDateTimeModel::setDateTime(const QDateTime &datetime)
{
    Q_D(QDateTimeModel);
    if (datetime.isValid() && datetime > d->minimum && datetime < d->maximum) {
        d->value.setDate(datetime.date());
        d->value.setTime(datetime.time());
        emit dateTimeChanged(d->value);
    }
}

QDate QDateTimeModel::date() const
{
    Q_D(const QDateTimeModel);
    return d->value.date();
}

void QDateTimeModel::setDate(const QDate &date)
{
    Q_D(QDateTimeModel);
    if (date.isValid() && date > d->minimum.date() && date < d->maximum.date()) {
        d->value.setDate(date);
        emit dateTimeChanged(d->value);
    }
}

QTime QDateTimeModel::time() const
{
    Q_D(const QDateTimeModel);
    return d->value.time();
}

void QDateTimeModel::setTime(const QTime &time)
{
    Q_D(QDateTimeModel);
    if (time.isValid() && time > d->minimum.time() && time < d->maximum.time()) {
        d->value.setTime(time);
        emit dateTimeChanged(d->value);
    }
}

int QDateTimeModel::weekNumber() const
{
    Q_D(const QDateTimeModel);
    return d->value.date().weekNumber();
}

void QDateTimeModel::setWeekNumber(int week)
{
    Q_D(QDateTimeModel);
    int diff = week - d->value.date().weekNumber();
    addDays(7 * diff);
}

int QDateTimeModel::dayOfWeek() const
{
    Q_D(const QDateTimeModel);
    return d->value.date().dayOfWeek();
}

int QDateTimeModel::dayOfYear() const
{
    Q_D(const QDateTimeModel);
    return d->value.date().dayOfYear();
}

int QDateTimeModel::daysInMonth() const
{
    Q_D(const QDateTimeModel);
    return d->value.date().daysInMonth();
}

int QDateTimeModel::daysInYear() const
{
    Q_D(const QDateTimeModel);
    return d->value.date().daysInYear();
}

QDateTime QDateTimeModel::toUTC() const
{
    Q_D(const QDateTimeModel);
    return d->value.toUTC();
}

QDate QDateTimeModel::firstDayOfWeek()
{
    Q_D(QDateTimeModel);
    QDate date(d->value.date());
    int dest = date.dayOfWeek() - 1;
    if (dest > 0) {
        return date.addDays(-1 * dest);
    }
    return date;
}

QString QDateTimeModel::longDayName() const
{
    return QDate::longDayName(dayOfWeek());
}

QString QDateTimeModel::longMonthName() const
{
    Q_D(const QDateTimeModel);
    return QDate::longMonthName(d->value.date().month());
}

QString QDateTimeModel::shortDayName() const
{
    return QDate::shortDayName(dayOfWeek());
}

QString QDateTimeModel::shortMonthName() const
{
    Q_D(const QDateTimeModel);
    return QDate::shortMonthName(d->value.date().month());
}

void QDateTimeModel::clearMinimumDateTime()
{
    setMinimumDateTime(QDateTime(QDATETIMEEDIT_DATE_MIN));
}

void QDateTimeModel::setMinimumDateTime(const QDateTime &dateTime)
{
    Q_D(QDateTimeModel);
    if (dateTime.isValid() && dateTime.date() >= QDATETIMEEDIT_DATE_MIN) {
        const QDateTime minimum = dateTime.toTimeSpec(d->spec);
        const QDateTime maximum = d->maximum;
        d->setRange(minimum, (maximum > minimum ? maximum : minimum));
    }
}

QDateTime QDateTimeModel::maximumDateTime() const
{
    Q_D(const QDateTimeModel);
    return d->maximum;
}

void QDateTimeModel::clearMaximumDateTime()
{
    setMaximumDateTime(QDATETIMEEDIT_DATETIME_MAX);
}

void QDateTimeModel::setMaximumDateTime(const QDateTime &dateTime)
{
    Q_D(QDateTimeModel);
    if (dateTime.isValid() && dateTime.date() <= QDATETIMEEDIT_DATE_MAX) {
        const QDateTime maximum = dateTime.toTimeSpec(d->spec);
        const QDateTime minimum = d->minimum;
        d->setRange((minimum < maximum ? minimum : maximum), maximum);
    }
}

void QDateTimeModel::setDateTimeRange(const QDateTime &min, const QDateTime &max)
{
    Q_D(QDateTimeModel);
    const QDateTime minimum = min.toTimeSpec(d->spec);
    QDateTime maximum = max.toTimeSpec(d->spec);
    if (min > max) {
        maximum = minimum;
    }
    d->setRange(minimum, maximum);
}

Qt::TimeSpec QDateTimeModel::timeSpec() const
{
    Q_D(const QDateTimeModel);
    return d->spec;
}

void QDateTimeModel::setTimeSpec(Qt::TimeSpec spec)
{
    Q_D(QDateTimeModel);
    if (spec != d->spec) {
        d->spec = spec;
        d->updateTimeSpec();
    }
}

QString QDateTimeModel::localeFormat() const
{
    const QLocale loc;
    return loc.dateTimeFormat(QLocale::ShortFormat);
}

QDateTime QDateTimeModel::currentTime() const
{
    return QDateTime::currentDateTime();
}

QDateTime QDateTimeModel::minimumDateTime() const
{
    Q_D(const QDateTimeModel);
    return d->minimum;
}

bool QDateTimeModel::addDays(int ndays)
{
    Q_D(QDateTimeModel);
    QDateTime ndate(d->value.addDays(ndays));
    if (ndate > d->minimum && ndate < d->maximum) {
        d->value.setDate(ndate.date());
        emit dateTimeChanged(d->value);
        return true;
    }
    return false;
}

bool QDateTimeModel::addMonths(int nmonths)
{
    Q_D(QDateTimeModel);
    QDateTime ndate(d->value.addMonths(nmonths));
    if (ndate > d->minimum && ndate < d->maximum) {
        d->value.setDate(ndate.date());
        emit dateTimeChanged(d->value);
        return true;
    }
    return false;
}

bool QDateTimeModel::addYears(int nyears)
{
    Q_D(QDateTimeModel);
    QDateTime ndate(d->value.addYears(nyears));
    if (ndate > d->minimum && ndate < d->maximum) {
        d->value.setDate(ndate.date());
        emit dateTimeChanged(d->value);
        return true;
    }
    return false;
}

bool QDateTimeModel::addMSecs(qint64 ms)
{
    Q_D(QDateTimeModel);
    QDateTime ndate(d->value.addMSecs(ms));
    if (ndate > d->minimum && ndate < d->maximum) {
        d->value.setTime(ndate.time());
        emit dateTimeChanged(d->value);
        return true;
    }
    return false;
}

bool QDateTimeModel::addSecs(int s)
{
    Q_D(QDateTimeModel);
    QDateTime ndate(d->value.addSecs(s));
    if (ndate > d->minimum && ndate < d->maximum) {
        d->value.setTime(ndate.time());
        emit dateTimeChanged(d->value);
        return true;
    }
    return false;
}

int QDateTimeModel::daysTo(const QDateTime &date) const
{
    Q_D(const QDateTimeModel);
    return d->value.daysTo(date);
}

int QDateTimeModel::secsTo(const QDateTime &time) const
{
    Q_D(const QDateTimeModel);
    return d->value.secsTo(time);
}
