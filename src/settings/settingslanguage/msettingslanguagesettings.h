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

#ifndef MSETTINGSLANGUAGESETTINGS_H
#define MSETTINGSLANGUAGESETTINGS_H

#include <QString>
#include "msettingslanguagenode.h"

//! \internal
/*!
 * \brief A settings node representing a "settings root node 'settings'".
 */
class MSettingsLanguageSettings : public MSettingsLanguageNode
{
public:
    /*!
     * Constructs a MSettingsLanguageSettings node.
     */
    MSettingsLanguageSettings();

    /*!
     * Destroys the MSettingsLanguageSettings node.
     */
    virtual ~MSettingsLanguageSettings();

private:
    //! The title of this settings.
    QString _title;

    //! Whether this settings should be represented as a group.
    bool _group;

    //! The IRI of the content this is linked to
    QString _content;
};
//! \internal_end

#endif // MSETTINGSLANGUAGESETTINGS_H
