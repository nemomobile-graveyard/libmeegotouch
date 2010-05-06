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

#ifndef MSETTINGSLANGUAGETEXT_H
#define MSETTINGSLANGUAGETEXT_H

#include <QString>
#include "msettingslanguagenode.h"

//! \internal
/*!
 * \brief A settings node representing text.
 */
class MSettingsLanguageText : public MSettingsLanguageNode
{
public:
    /*!
     * Constructs a new text settings node.
     * \param key the key (name) of the text item.
     * \param title the title of the text item.
     */
    MSettingsLanguageText(const QString &key, const QString &title);

    /*!
     * Destructor
     */
    virtual ~MSettingsLanguageText();

    /*!
     * Returns the key of this text node.
     */
    QString key() const;

    /*!
     * Returns the localized title of this text node.
     */
    QString title() const;

private:
    //! The key (name) of this object.
    QString key_;
    //! The title of this object.
    QString title_;
};
//! \internal_end

#endif // MSETTINGSLANGUAGETEXT_H
