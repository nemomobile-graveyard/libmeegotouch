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

#ifndef MSETTINGSLANGUAGESELECTION_H
#define MSETTINGSLANGUAGESELECTION_H

#include <QList>
#include <QString>

#include "msettingslanguagenode.h"

class MSettingsLanguageOption;

//! \internal
/*!
 * \brief A settings node representing a selection.
 *
 * A selection has a predefined set of options.
 *
 * A selection contains \link MSettingsLanguageOption \endlink nodes.
 */
class MSettingsLanguageSelection : public MSettingsLanguageNode
{
public:
    /*!
     * Constructs a new selection with a given key.
     * \param key the key (name) of the selection.
     * \param title the title of the selection.
     */
    MSettingsLanguageSelection(const QString &key, const QString &title);

    /*!
     * Destructor.
     */
    virtual ~MSettingsLanguageSelection();

    /*!
     * Returns the key of this selection.
     */
    QString key() const;

    /*!
     * Returns the localized title of this text node.
     */
    QString title() const;

    /*!
     * Adds a new option to this selection.
     * \param title the title for the new option.
     * \param value the value for the new option.
     * \return the newly constructed option object.
     * \sa MSettingsLanguageOption
     */
    MSettingsLanguageOption *addOption(const QString &title, int value);

    /*!
     * Adds a new option to this selection. This version of the method
     * can be used if the option object is already constructed by some
     * other party.
     * \param option the option to be added.
     * \return the same pointer as was given as parameter if the addition
     * was succesfull. Returns \c NULL if the addition couldn't be done.
     * \sa MSettingsLanguageOption
     */
    MSettingsLanguageOption *addOption(MSettingsLanguageOption *option);

    /*!
     * Returns the number of child nodes of type MSettingsLanguageOption.
     */
    uint numOptions() const;

    /*!
     * Returns the MSettingsLanguageOption child nodes that this node has.
     */
    QList<const MSettingsLanguageOption *> options() const;

private:
    //! The key (name) of this object.
    QString _key;
    //! The title of this object.
    QString _title;
};
//! \internal_end

#endif // MSETTINGSLANGUAGESELECTION_H
