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

#ifndef MSTYLESHEET_H
#define MSTYLESHEET_H

#include <QMap>
#include <QString>
#include <QVariant>
#include "mstylesheetselector.h"
#include "mtheme.h"
#include "mstylesheetparser.h"
#include "mstylesheetattribute.h"

class MLogicalValues;
class MStyleSheetPrivate;

//! \internal
/*!
 * \brief This class represents all the selectors and attributes in a single CSS file.
 *
 * This class reads style definitions from .css file, manages them
 * and provides easy interface to create a style data object from them.
 */
class M_CORE_EXPORT MStyleSheet : public MStyleSheetParser
{
public:
    /*!
     * \brief MStyleSheet constructor
     */
    MStyleSheet(const MLogicalValues *values = NULL);

    /*!
     * \brief MStyleSheet destructor
     */
    virtual ~MStyleSheet();

    /*!
     * \internal
     *
     * \brief Creates and caches a new style object or retrieves existing from cache
     * \param sheets            A list of stylesheets
     * \param styleClassName    Name of the style class to be created
     * \param objectName        Name of the target object, can be empty
     * \param mode              Mode of the target object, can be empty
     * \param type              Type of the target object, can be empty
     * \param orientation       Orientation of this style
     * \param parent            Parent object
     */
    static MStyle *style(const QList<const MStyleSheet *>& sheets,
                           const QByteArray &styleClassName,
                           const QByteArray &objectName,
                           const QByteArray &mode,
                           const QByteArray &type,
                           M::Orientation orientation,
                           const MWidgetController *parent);

    /*!
     * \internal
     * \deprecated Please look for the MTheme::style() functions instead.
     *
     * \brief Creates and caches a new style object or retrieves existing from cache
     * \param sheets            A list of stylesheets
     * \param styleClassName    Name of the style class to be created
     * \param objectName        Name of the target object, can be empty
     * \param mode              Mode of the target object, can be empty
     * \param type              Type of the target object, can be empty
     * \param orientation       Orientation of this style
     * \param parent            Parent object
     */
    static MStyle *style(const QList<const MStyleSheet *>& sheets,
                           const QList<QPair<const QMetaObject*, QList<const MStyleSheet*> > >& parentsSheets,
                           const QByteArray &styleClassName,
                           const QByteArray &objectName,
                           const QByteArray &mode,
                           const QByteArray &type,
                           M::Orientation orientation,
                           const MWidgetController *parent);


    /*!
     * \brief Releases existing style object and removes it from cache if reference count drops to zero
     */
    static void releaseStyle(const MStyle *style);

    static void deleteStylesWithoutReference();
    static void deleteStylesFromStyleCreator(const QByteArray & styleClassName);

    /*!
     * \brief Cleans up the cache
     * \param outputAndDelete    If true, outputs warnings from remaining cached styles and deletes them
     */
    static void cleanup(bool outputAndDelete);
};
//! \internal_end

#endif // MSTYLESHEET_H

