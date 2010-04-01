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

#ifndef DUISETTINGSLANGUAGEBINARY_H
#define DUISETTINGSLANGUAGEBINARY_H

#include <QList>
#include "duiexport.h"
#include "duisettingslanguagesettings.h"

/*!
 * \brief The root class for the settings binary interface.
 *
 * This is the central access point class for the settings binary
 * interface.
 */
class DUI_EXPORT DuiSettingsLanguageBinary : public DuiSettingsLanguageSettings
{
public:
    /*!
     * Constructor.
     */
    DuiSettingsLanguageBinary();

    /*!
     * Destructor.
     */
    virtual ~DuiSettingsLanguageBinary();

    /*!
     * Returns the list of keys in this settings binary tree.
     *
     * \return a list of keys in this settings binary tree
     */
    QStringList keys() const;
};

#endif // DUISETTINGSLANGUAGEBINARY_H
