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

#ifndef DUISTYLESHEET_H
#define DUISTYLESHEET_H

#include <QMap>
#include <QString>
#include <QVariant>
#include "duistylesheetselector.h"
#include "duitheme.h"
#include "duistylesheetparser.h"
#include "duistylesheetattribute.h"

class DuiLogicalValues;
class DuiStyleSheetPrivate;

/*!
 * \brief This class represents all the selectors and attributes in a single CSS file.
 *
 * This class reads style definitions from .css file, manages them
 * and provides easy interface to create a style data object from them.
 */
class DUI_EXPORT DuiStyleSheet : public DuiStyleSheetParser
{
public:
    /*!
     * \brief DuiStyleSheet constructor
     */
    DuiStyleSheet(const DuiLogicalValues *values = NULL);

    /*!
     * \brief DuiStyleSheet destructor
     */
    virtual ~DuiStyleSheet();

    /*!
     * \brief Creates and caches a new style object or retrieves existing from cache
     * \param sheets            A list of stylesheets
     * \param styleClassName    Name of the style class to be created
     * \param objectName        Name of the target object, can be empty
     * \param mode              Mode of the target object, can be empty
     * \param type              Type of the target object, can be empty
     * \param orientation       Orientation of this style
     * \param parent            Parent object
     */
    static DuiStyle *style(const QList<const DuiStyleSheet *>& sheets,
                           const QString &styleClassName,
                           const QString &objectName,
                           const QString &mode,
                           const QString &type,
                           const Dui::Orientation &orientation,
                           const DuiWidgetController *parent);

    /*!
     * \brief Releases existing style object and removes it from cache if reference count drops to zero
     */
    static void releaseStyle(const DuiStyle *style);

    /*!
     * \brief Cleans up the cache
     * \param output    If true, outputs warnings from remaining cached styles and deletes them
     */
    static void cleanup(bool outputAndDelete);
};

#endif // DUISTYLESHEET_H

