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

#ifndef MSETTINGSLANGUAGEBINARY_H
#define MSETTINGSLANGUAGEBINARY_H

#include <QList>
#include "mexport.h"
#include "msettingslanguagesettings.h"

/*!
 * \brief The root class for the settings binary interface.
 *
 * This is the central access point class for the settings binary
 * interface.
 */
class M_SETTINGS_EXPORT MSettingsLanguageBinary : public MSettingsLanguageSettings
{
public:
    /*!
     * Constructor.
     */
    MSettingsLanguageBinary();

    /*!
     * Destructor.
     */
    virtual ~MSettingsLanguageBinary();

    /*!
     * Returns the list of keys in this settings binary tree.
     *
     * \return a list of keys in this settings binary tree
     */
    QStringList keys() const;
};

#endif // MSETTINGSLANGUAGEBINARY_H
