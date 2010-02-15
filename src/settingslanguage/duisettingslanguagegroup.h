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

#ifndef DUISETTINGSLANGUAGEGROUP_H
#define DUISETTINGSLANGUAGEGROUP_H

#include "duisettingslanguagenode.h"

/*!
 * \brief A settings node representing a group of setting items.
 *
 *  A group can contain one or more setting items.
 */
class DuiSettingsLanguageGroup : public DuiSettingsLanguageNode
{
public:
    /*!
     * Constructor.
     */
    DuiSettingsLanguageGroup() {}

    virtual ~DuiSettingsLanguageGroup() {}
};

#endif // DUISETTINGSLANGUAGEGROUP_H
