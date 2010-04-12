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

#include "msettingslanguageinteger.h"

MSettingsLanguageInteger::MSettingsLanguageInteger(const QString &key, const QString &title) :
    key_(key),
    title_(title),
    minValue_(),
    maxValue_()
{
}

MSettingsLanguageInteger::~MSettingsLanguageInteger()
{
}

QString MSettingsLanguageInteger::key() const
{
    return key_;
}

QString MSettingsLanguageInteger::title() const
{
    return qtTrId(title_.toUtf8());
}

bool MSettingsLanguageInteger::minValue(int &value) const
{
    if (minValue_.isSet) {
        value = minValue_.value;
    }
    return minValue_.isSet;
}

bool MSettingsLanguageInteger::maxValue(int &value) const
{
    if (maxValue_.isSet) {
        value = maxValue_.value;
    }
    return maxValue_.isSet;
}

void MSettingsLanguageInteger::setMinValue(const int newMin)
{
    minValue_.isSet = true;
    minValue_.value = newMin;
}
void MSettingsLanguageInteger::setMaxValue(const int newMax)
{
    maxValue_.isSet = true;
    maxValue_.value = newMax;
}

