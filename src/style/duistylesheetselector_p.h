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

#ifndef DUISTYLESHEETSELECTOR_P_H
#define DUISTYLESHEETSELECTOR_P_H

#include "duistylesheetselector.h"
#include "duistylesheetattribute.h"

class DuiStyleSheetSelectorPrivate
{
public:
    //! Parent class name, represented by this selector.
    QString parentName;
    //! Object name, represented by this selector.
    QString objName;
    //! Class name, represented by this selector.
    QString clName;
    //! Type, represented by this selector.
    QString clType;
    //! Screen orientation type, represented by this selector.
    QString screenOrientation;
    //! Object mode, represented by this selector.
    QString objectMode;
    //! Attribute list of this selector (ClassName#ObjectName.Orientation:Mode).
    DuiAttributeList data;
    //! Name of the css file where the selector was created.
    QString filename;
    //! Flags indicating whether this is a special kind of selector.
    DuiStyleSheetSelector::Flags flags;

    void operator=(const DuiStyleSheetSelectorPrivate &);
};

#endif //DUISTYLESHEETSELECTOR_P_H
