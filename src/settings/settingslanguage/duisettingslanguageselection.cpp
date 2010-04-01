/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include "duisettingslanguageselection.h"
#include "duisettingslanguageoption.h"

DuiSettingsLanguageSelection::DuiSettingsLanguageSelection(const QString &key) :
    _key(key)
{
}

DuiSettingsLanguageSelection::~DuiSettingsLanguageSelection()
{
}

QString DuiSettingsLanguageSelection::key() const
{
    return _key;
}

DuiSettingsLanguageOption *DuiSettingsLanguageSelection::addOption(const QString &title, int value)
{
    DuiSettingsLanguageOption *v = new DuiSettingsLanguageOption(title, value);
    addChild(v);
    return v;
}

DuiSettingsLanguageOption *DuiSettingsLanguageSelection::addOption(DuiSettingsLanguageOption *option)
{
    addChild(option);
    return option;
}

uint DuiSettingsLanguageSelection::numOptions() const
{
    uint n = 0;
    foreach(const DuiSettingsLanguageNode * child, children()) {
        if (dynamic_cast<const DuiSettingsLanguageOption *>(child)) {
            ++n;
        }
    }

    return n;
}

QList<const DuiSettingsLanguageOption *> DuiSettingsLanguageSelection::options() const
{
    QList<const DuiSettingsLanguageOption *> vs;
    foreach(const DuiSettingsLanguageNode * child, children()) {
        const DuiSettingsLanguageOption *optionChild;
        if ((optionChild = dynamic_cast<const DuiSettingsLanguageOption *>(child))) {
            vs.append(optionChild);
        }
    }

    return vs;
}
