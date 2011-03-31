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

#ifndef MSETTINGSLANGUAGEGROUP_H
#define MSETTINGSLANGUAGEGROUP_H

#include "msettingslanguagenode.h"
#include <QString>

//! \internal
/*!
 * \brief A settings node representing a group of setting items.
 *
 *  A group can contain one or more setting items.
 */
class MSettingsLanguageGroup : public MSettingsLanguageNode
{
public:
    /*!
     * Constructor.
     * \param title the title of the group.
     */
    MSettingsLanguageGroup(const QString &title);

    /*!
     * Destructor
     */
    virtual ~MSettingsLanguageGroup();

    /*!
     * Returns the localized title of this group.
     */
    QString title() const;

private:
    //! The title of this object.
    QString title_;
};
//! \internal_end

#endif // MSETTINGSLANGUAGEGROUP_H
