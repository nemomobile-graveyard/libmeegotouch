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

#include "msettingslanguageselection.h"
#include "msettingslanguageoption.h"

MSettingsLanguageSelection::MSettingsLanguageSelection(const QString &key, const QString &title) :
    _key(key),
    _title(title)
{
}

MSettingsLanguageSelection::~MSettingsLanguageSelection()
{
}

QString MSettingsLanguageSelection::key() const
{
    return _key;
}

QString MSettingsLanguageSelection::title() const
{
    return qtTrId(_title.toUtf8());;
}

MSettingsLanguageOption *MSettingsLanguageSelection::addOption(const QString &title, int value)
{
    MSettingsLanguageOption *v = new MSettingsLanguageOption(title, value);
    addChild(v);
    return v;
}

MSettingsLanguageOption *MSettingsLanguageSelection::addOption(MSettingsLanguageOption *option)
{
    addChild(option);
    return option;
}

uint MSettingsLanguageSelection::numOptions() const
{
    uint n = 0;
    foreach(const MSettingsLanguageNode * child, children()) {
        if (dynamic_cast<const MSettingsLanguageOption *>(child)) {
            ++n;
        }
    }

    return n;
}

QList<const MSettingsLanguageOption *> MSettingsLanguageSelection::options() const
{
    QList<const MSettingsLanguageOption *> vs;
    foreach(const MSettingsLanguageNode * child, children()) {
        const MSettingsLanguageOption *optionChild;
        if ((optionChild = dynamic_cast<const MSettingsLanguageOption *>(child))) {
            vs.append(optionChild);
        }
    }

    return vs;
}
