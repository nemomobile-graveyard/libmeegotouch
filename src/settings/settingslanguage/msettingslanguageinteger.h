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

#ifndef MSETTINGSLANGUAGEINTEGER_H
#define MSETTINGSLANGUAGEINTEGER_H

#include <QString>
#include "msettingslanguagenode.h"

//! \internal
/*!
 * \brief A settings node representing an integer within a given range.
 */
class MSettingsLanguageInteger : public MSettingsLanguageNode
{
public:
    struct MinMaxValue {
        MinMaxValue() : isSet(false), value(0) {}
        MinMaxValue(int v) : isSet(true), value(v) {}
        MinMaxValue(bool s, int v) : isSet(s), value(v) {}

        bool isSet;
        int value;
    };

    /*!
     * Constructs a new integer settings node.
     *
     * If maxValue is smaller than or equal to minValue, the integer will be
     * considered not to be constrained to any range.
     *
     * \param key the key (name) of the integer item.
     * \param title the title of the integer item.
     * \param min the minimum value for the item. Value can be set or unset.
     * \param max the maximum value for the item. Value can be set or unset.
     */
    MSettingsLanguageInteger(const QString &key, const QString &title);

    /*!
     * Destructor
     */
    virtual ~MSettingsLanguageInteger();

    /*!
     * Returns the key of this integer node.
     * \return key
     */
    QString key() const;

    /*!
     * Returns the localized title of this integer node.
     * \return title
     */
    QString title() const;

    /*!
     * Set parameter value to the minimum value, if minimum value is set.
     * \param value return param to be set to minimum value
     * \return true if minimum value is set
     */
    bool minValue(int &value) const;

    /*!
     * Set parameter value to the maximum value, if maximum value is set.
     * \param value return param to be set to maximum value
     * \return true if maximum value is set
     */
    bool maxValue(int &value) const;


    void setMinValue(int newMin);

    void setMaxValue(int newMax);

private:
    //! The key (name) of this object.
    QString key_;

    //! The title of this object.
    QString title_;

    //! The minimum value.
    MinMaxValue minValue_;

    //! The maximum value.
    MinMaxValue maxValue_;
};
//! \internal_end

#endif // MSETTINGSLANGUAGEINTEGER_H
