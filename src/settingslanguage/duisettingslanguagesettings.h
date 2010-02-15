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

#ifndef DUISETTINGSLANGUAGESETTINGS_H
#define DUISETTINGSLANGUAGESETTINGS_H

#include <QString>
#include "duisettingslanguagenode.h"

/*!
 * \brief A settings node representing a "settings root node 'settings'".
 */
class DuiSettingsLanguageSettings : public DuiSettingsLanguageNode
{
public:
    /*!
     * Constructs a DuiSettingsLanguageSettings node.
     */
    DuiSettingsLanguageSettings();

    /*!
     * Destroys the DuiSettingsLanguageSettings node.
     */
    virtual ~DuiSettingsLanguageSettings();

private:
    //! The title of this settings.
    QString _title;

    //! Whether this settings should be represented as a group.
    bool _group;

    //! The IRI of the content this is linked to
    QString _content;
};

#endif // DUISETTINGSLANGUAGESETTINGS_H
