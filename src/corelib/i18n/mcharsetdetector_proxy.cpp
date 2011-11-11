/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mcharsetdetector.h"
#include "mlocale/mcharsetdetector.h"
#include "mcharsetmatch.h"
#include "mlocale/mcharsetmatch.h"
#include "mcharsetmatch_proxy_p.h"
#include "mlocale.h"

static bool g_mLocaleInited = false;

class MCharsetDetectorPrivate
{
public:
    MCharsetDetectorPrivate() : pDetector( 0 )
        {
            if ( ! g_mLocaleInited )
            {
                MLocale::initMLocale();
                g_mLocaleInited = true;
            }
        }
    ~MCharsetDetectorPrivate()
        {
            delete pDetector;
        }

    ML10N::MCharsetDetector *pDetector;
};

MCharsetDetector::MCharsetDetector()
    : d_ptr(new MCharsetDetectorPrivate)
{
    d_ptr->pDetector = new ML10N::MCharsetDetector;
}

MCharsetDetector::MCharsetDetector(const QByteArray &ba)
    : d_ptr (new MCharsetDetectorPrivate)
{
    d_ptr->pDetector = new ML10N::MCharsetDetector( ba );
}

MCharsetDetector::MCharsetDetector(const char *str)
    : d_ptr(new MCharsetDetectorPrivate)
{
    d_ptr->pDetector = new ML10N::MCharsetDetector( str );
}

MCharsetDetector::MCharsetDetector(const char *data, int size)
    : d_ptr(new MCharsetDetectorPrivate)
{
    d_ptr->pDetector = new ML10N::MCharsetDetector( data, size );
}

MCharsetDetector::~MCharsetDetector()
{
    delete d_ptr;
}

bool MCharsetDetector::hasError() const
{
    return d_ptr->pDetector->hasError();
}

void MCharsetDetector::clearError()
{
    d_ptr->pDetector->clearError();
}

QString MCharsetDetector::errorString() const
{
    return d_ptr->pDetector->errorString();
}

void MCharsetDetector::setText(const QByteArray &ba)
{
    d_ptr->pDetector->setText( ba );
}

MCharsetMatch MCharsetDetector::detect()
{
    MCharsetMatch match;
    *match.d_ptr->pMatch = d_ptr->pDetector->detect();
    return match;
}

QList<MCharsetMatch> MCharsetDetector::detectAll()
{
    QList<ML10N::MCharsetMatch> ml = d_ptr->pDetector->detectAll();

    QList<MCharsetMatch> matchList;

    foreach (const ML10N::MCharsetMatch &m, ml)
    {
        MCharsetMatch match;
        *match.d_ptr->pMatch = m;

        matchList.append( match );
    }

    return matchList;
}

QString MCharsetDetector::text(const MCharsetMatch &charsetMatch)
{
    return d_ptr->pDetector->text( *charsetMatch.d_ptr->pMatch );
}

void MCharsetDetector::setDeclaredLocale(const QString &locale)
{
    d_ptr->pDetector->setDeclaredLocale( locale );
}

void MCharsetDetector::setDeclaredEncoding(const QString &encoding)
{
    d_ptr->pDetector->setDeclaredEncoding( encoding );
}

QStringList MCharsetDetector::getAllDetectableCharsets()
{
    return d_ptr->pDetector->getAllDetectableCharsets();
}

void MCharsetDetector::enableInputFilter(bool enable)
{
    d_ptr->pDetector->enableInputFilter( enable );
}

bool MCharsetDetector::isInputFilterEnabled()
{
    return d_ptr->pDetector->isInputFilterEnabled();
}
