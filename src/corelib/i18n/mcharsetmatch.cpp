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

#include "mcharsetmatch.h"
#include "mcharsetmatch_p.h"

MCharsetMatchPrivate::MCharsetMatchPrivate()
    : _confidence(0),
      q_ptr(0)
{
}

MCharsetMatchPrivate::MCharsetMatchPrivate(const MCharsetMatchPrivate &other)
    : _name(other._name),
      _language(other._language),
      _confidence(other._confidence),
      q_ptr(0)
{
}

MCharsetMatchPrivate::~MCharsetMatchPrivate()
{
}

MCharsetMatchPrivate &MCharsetMatchPrivate::operator=(const MCharsetMatchPrivate &other)
{
    _name = other._name;
    _language = other._language;
    _confidence = other._confidence;
    return *this;
}

MCharsetMatch::MCharsetMatch()
    : d_ptr(new MCharsetMatchPrivate)
{
    Q_D(MCharsetMatch);
    d->q_ptr = this;
}

MCharsetMatch::MCharsetMatch(const QString name, const QString language, const qint32 confidence)
    : d_ptr(new MCharsetMatchPrivate)
{
    Q_D(MCharsetMatch);
    d->q_ptr = this;
    setName(name);
    setLanguage(language);
    setConfidence(confidence);
}

MCharsetMatch::MCharsetMatch(const MCharsetMatch &other)
    : d_ptr(new MCharsetMatchPrivate(*other.d_ptr))
{
    Q_D(MCharsetMatch);
    d->q_ptr = this;
}

MCharsetMatch::~MCharsetMatch()
{
    delete d_ptr;
}

MCharsetMatch &MCharsetMatch::operator=(const MCharsetMatch &other)
{
    if (this == &other) {
        return *this;
    }
    *d_ptr = *other.d_ptr;
    return *this;
}

bool MCharsetMatch::operator<(const MCharsetMatch &other) const
{
    if(this->confidence() < other.confidence())
        return true;
    else if(this->confidence() == other.confidence()
            && this->language().isEmpty()
            && !other.language().isEmpty())
        return true;
    else
        return false;
}

bool MCharsetMatch::operator>(const MCharsetMatch &other) const
{
    if(this->confidence() > other.confidence())
        return true;
    else if (this->confidence() == other.confidence()
             && !this->language().isEmpty()
             && other.language().isEmpty())
            return true;
    else
        return false;
}

QString MCharsetMatch::name() const
{
    Q_D(const MCharsetMatch);
    return d->_name;
}

void MCharsetMatch::setName(QString name)
{
    Q_D(MCharsetMatch);
    d->_name = name;
}

QString MCharsetMatch::language() const
{
    Q_D(const MCharsetMatch);
    return d->_language;
}

void MCharsetMatch::setLanguage(QString language)
{
    Q_D(MCharsetMatch);
    d->_language = language;
}

qint32 MCharsetMatch::confidence() const
{
    Q_D(const MCharsetMatch);
    return d->_confidence;
}

void MCharsetMatch::setConfidence(qint32 confidence)
{
    Q_D(MCharsetMatch);
    d->_confidence = confidence;
}
