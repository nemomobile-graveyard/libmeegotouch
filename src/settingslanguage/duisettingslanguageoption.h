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

#ifndef DUISETTINGSLANGUAGEOPTION_H
#define DUISETTINGSLANGUAGEOPTION_H

#include "duisettingslanguagenode.h"
#include <QString>

//! \internal
/*!
 * \brief A settings node representing a selection option.
 *
 * A selection option has a title and a value.
 */
class DuiSettingsLanguageOption : public DuiSettingsLanguageNode
{
public:
    /*!
     * Constructs a new selection option with a given title and value.
     * \param title the title of the selection option.
     * \param value the value of the selection option.
     */
    DuiSettingsLanguageOption(const QString &title, int value);

    /*!
     * Destructor.
     */
    virtual ~DuiSettingsLanguageOption();

    /*!
     * Returns the localized title of this selection option.
     */
    QString title() const;

    /*!
     * Returns the value of this selection option.
     */
    int value() const;

private:
    //! The title of this selection option.
    QString title_;

    //! The value of this selection option.
    int value_;

};
//! \internal_end

#endif // DUISETTINGSLANGUAGEOPTION_H
