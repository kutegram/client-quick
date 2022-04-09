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

#ifndef QDATETIMEMODEL_H
#define QDATETIMEMODEL_H

#include <QtCore/qdatetime.h>
#include <QtCore/qvariant.h>
#include <QtCore/qscopedpointer.h>
#include <QtDeclarative/qdeclarative.h>

#include <kernel/common.h>

class QDateTimeModelPrivate;

class Q_COMPONENTS_EXPORT QDateTimeModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateTimeChanged)
    Q_PROPERTY(QTime time READ time WRITE setTime NOTIFY dateTimeChanged)
    Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime NOTIFY dateTimeChanged)
    Q_PROPERTY(int weekNumber READ weekNumber WRITE setWeekNumber NOTIFY dateTimeChanged)
    Q_PROPERTY(int dayOfWeek READ dayOfWeek NOTIFY dateTimeChanged)
    Q_PROPERTY(int dayOfYear READ dayOfYear NOTIFY dateTimeChanged)
    Q_PROPERTY(int daysInMonth READ daysInMonth NOTIFY dateTimeChanged)
    Q_PROPERTY(int daysInYear READ daysInYear NOTIFY dateTimeChanged)
    Q_PROPERTY(QDateTime utc READ toUTC NOTIFY dateTimeChanged)
    Q_PROPERTY(QDate firstDayOfWeek READ firstDayOfWeek NOTIFY dateTimeChanged)
    Q_PROPERTY(QString longDayName READ longDayName NOTIFY dateTimeChanged)
    Q_PROPERTY(QString longMonthName READ longMonthName NOTIFY dateTimeChanged)
    Q_PROPERTY(QString shortDayName READ shortDayName NOTIFY dateTimeChanged)
    Q_PROPERTY(QString shortMonthName READ shortMonthName NOTIFY dateTimeChanged)
    Q_PROPERTY(QDateTime maximumDateTime READ maximumDateTime WRITE setMaximumDateTime RESET clearMaximumDateTime)
    Q_PROPERTY(QDateTime minimumDateTime READ minimumDateTime WRITE setMinimumDateTime RESET clearMinimumDateTime)
    Q_PROPERTY(Qt::TimeSpec timeSpec READ timeSpec WRITE setTimeSpec)

public:
    QDateTimeModel(QObject *parent = 0);
    ~QDateTimeModel();

    QDate date() const;
    QTime time() const;
    QDateTime dateTime() const;

    int weekNumber() const;
    int dayOfWeek() const;
    int dayOfYear() const;
    int daysInMonth() const;
    int daysInYear() const;
    QDateTime toUTC() const;
    QDate firstDayOfWeek();

    QString longDayName() const;
    QString longMonthName() const;
    QString shortDayName() const;
    QString shortMonthName() const;

    QDateTime minimumDateTime() const;
    void clearMinimumDateTime();
    void setMinimumDateTime(const QDateTime &dateTime);

    QDateTime maximumDateTime() const;
    void clearMaximumDateTime();
    void setMaximumDateTime(const QDateTime &dateTime);

    void setDateTimeRange(const QDateTime &min, const QDateTime &max);

    Qt::TimeSpec timeSpec() const;
    void setTimeSpec(Qt::TimeSpec spec);

Q_SIGNALS:
    void dateTimeChanged(const QDateTime &dateTime);

public Q_SLOTS:
    QString localeFormat() const;
    QDateTime currentTime() const;

    void setDate(const QDate &date);
    void setTime(const QTime &time);
    void setDateTime(const QDateTime &dateTime);
    void setWeekNumber(int week);

    bool addDays(int ndays);
    bool addMonths(int nmonths);
    bool addYears(int nyears);
    bool addMSecs(qint64 ms);
    bool addSecs(int s);

    int daysTo(const QDateTime &date) const;
    int secsTo(const QDateTime &time) const;

protected:
    QDateTimeModel(const QVariant &val, QVariant::Type parserType, QObject *parent = 0);
    QDateTimeModel(QDateTimeModelPrivate &dd, QObject *parent = 0);
    QDateTimeModelPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(QDateTimeModel)
    Q_DISABLE_COPY(QDateTimeModel)
};

QML_DECLARE_TYPE(QDateTimeModel)

#endif // QDATETIMEMODEL_H
