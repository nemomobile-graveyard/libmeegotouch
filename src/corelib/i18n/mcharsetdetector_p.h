/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef MCHARSETDETECTOR_P_H
#define MCHARSETDETECTR_P_H

#include <unicode/utypes.h>

#include <QByteArray>

class UCharsetDetector;

class MCharsetDetectorPrivate
{
    Q_DECLARE_PUBLIC(MCharsetDetector)

public:
    MCharsetDetectorPrivate();

    virtual ~MCharsetDetectorPrivate();

    bool hasError() const;
    void clearError();
    QString errorString() const;

    QByteArray _ba;
    QByteArray _baExtended;

    UErrorCode _status;
    UCharsetDetector *_uCharsetDetector;

    QString _declaredLocale;
    QString _declaredEncoding;

    QStringList _allDetectableCharsets;

    MCharsetDetector *q_ptr;
private:
    Q_DISABLE_COPY(MCharsetDetectorPrivate)
};

#endif
