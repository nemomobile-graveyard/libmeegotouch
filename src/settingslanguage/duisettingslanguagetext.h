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

#ifndef DUISETTINGSLANGUAGETEXT_H
#define DUISETTINGSLANGUAGETEXT_H

#include <QString>
#include "duisettingslanguagenode.h"
/*!
 * \brief A settings node representing text.
 */
class DuiSettingsLanguageText : public DuiSettingsLanguageNode
{
public:
    /*!
     * Constructs a new text settings node.
     * \param key the key (name) of the text item.
     * \param title the title of the text item.
     */
    DuiSettingsLanguageText(const QString &key, const QString &title);

    /*!
     * Destructor
     */
    virtual ~DuiSettingsLanguageText();

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


#endif // DUISETTINGSLANGUAGETEXT_H
