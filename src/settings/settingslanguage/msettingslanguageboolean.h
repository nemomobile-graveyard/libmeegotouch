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

#ifndef MSETTINGSLANGUAGEBOOLEAN_H
#define MSETTINGSLANGUAGEBOOLEAN_H

#include <QList>
#include <QString>

#include "msettingslanguagenode.h"

//! \internal
/*!
 * \brief A settings node representing a boolean.
 */
class MSettingsLanguageBoolean : public MSettingsLanguageNode
{
public:
    /*!
     * Constructs a new boolean with a given key.
     * \param key the key (name) of the boolean.
     */
    MSettingsLanguageBoolean(const QString &key, const QString &title);

    /*!
     * Destructor.
     */
    virtual ~MSettingsLanguageBoolean();

    /*!
     * Returns the key of this boolean.
     */
    QString key() const;

    /*!
     * Returns the localized title of this node.
     */
    QString title() const;

private:
    //! The key (name) of this boolean.
    QString key_;

    //! The title of this boolean.
    QString title_;
};
//! \internal_end

#endif // MSETTINGSLANGUAGEBOOLEAN_H
