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

#ifndef DUISETTINGSLANGUAGESELECTION_H
#define DUISETTINGSLANGUAGESELECTION_H

#include <QList>
#include <QString>

#include "duisettingslanguagenode.h"

class DuiSettingsLanguageOption;

//! \internal
/*!
 * \brief A settings node representing a selection.
 *
 * A selection has a predefined set of options.
 *
 * A selection contains \link DuiSettingsLanguageOption \endlink nodes.
 */
class DuiSettingsLanguageSelection : public DuiSettingsLanguageNode
{
public:
    /*!
     * Constructs a new selection with a given key.
     * \param key the key (name) of the selection.
     */
    DuiSettingsLanguageSelection(const QString &key);

    /*!
     * Destructor.
     */
    virtual ~DuiSettingsLanguageSelection();

    /*!
     * Returns the key of this selection.
     */
    QString key() const;

    /*!
     * Adds a new option to this selection.
     * \param title the title for the new option.
     * \param value the value for the new option.
     * \return the newly constructed option object.
     * \sa DuiSettingsLanguageOption
     */
    DuiSettingsLanguageOption *addOption(const QString &title, int value);

    /*!
     * Adds a new option to this selection. This version of the method
     * can be used if the option object is already constructed by some
     * other party.
     * \param option the option to be added.
     * \return the same pointer as was given as parameter if the addition
     * was succesfull. Returns \c NULL if the addition couldn't be done.
     * \sa DuiSettingsLanguageOption
     */
    DuiSettingsLanguageOption *addOption(DuiSettingsLanguageOption *option);

    /*!
     * Returns the number of child nodes of type DuiSettingsLanguageOption.
     */
    uint numOptions() const;

    /*!
     * Returns the DuiSettingsLanguageOption child nodes that this node has.
     */
    QList<const DuiSettingsLanguageOption *> options() const;

private:
    //! The key (name) of this object.
    QString _key;

};
//! \internal_end

#endif // DUISETTINGSLANGUAGESELECTION_H
