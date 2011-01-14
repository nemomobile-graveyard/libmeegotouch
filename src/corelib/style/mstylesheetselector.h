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

#ifndef MSTYLESHEETSELECTOR_H
#define MSTYLESHEETSELECTOR_H

#include "mexport.h"
#include "mstylesheetattribute.h"

#include <QByteArray>
#include <QVariant>

class MStyleSheetSelectorPrivate;

//! \internal
/*!
 * Style sheet selector class.
 *
 * This class represents a set of properties for a single selector statement in CSS file.
 * Later, when the style for the target object is requested, all known selectors are compared to
 * target object and the matching settings are stored to target object's style object.
 */
class M_CORE_EXPORT MStyleSheetSelector
{
    //friend class MStyleSheetParser;

public:

    enum Flags {
        Child           = 0x1,
        ExactClassMatch = 0x2
    };

    enum Orientation {
	LandscapeOrientation = M::Landscape,
	PortraitOrientation = M::Portrait,
	UndefinedOrientation = 3
    };
    /*!
     * MStyleSheetSelector constructor
     * \param objectName target object name of this selector, can be empty
     * \param className target class name for this selector, can be empty
     * \param classType class type
     * \param orientation target orientation for this selector, can be empty
     * \param mode target mode for this selector, can be empty
     * \param filename Name of the css file where this selector was parsed, can be empty.
              Parameter is used for providing detailed css parsing errors.
     * \param parentName Name of parent
     * \param child A flag indicating whether this selector represents a descendant or
              direct child of the parent.
     * \param subclasses A flag indicating whether the su
     * \deprecated Please use the new constructor
     */

    explicit MStyleSheetSelector(const QByteArray &objectName = "",
                                   const QByteArray &className = "",
                                   const QByteArray &classType = "",
                                   const Orientation orientation = UndefinedOrientation,
                                   const QByteArray &mode = "",
                                   const QByteArray &filename = "",
                                   const QByteArray &parentName = "",
                                   Flags flags = (Flags) 0);

    /*!
     * MStyleSheetSelector constructor
     * \param objectName target object name of this selector, can be empty
     * \param className target class name for this selector, can be empty
     * \param classType class type
     * \param orientation target orientation for this selector, can be empty
     * \param mode target mode for this selector, can be empty
     * \param filename Name of the css file where this selector was parsed, can be empty.
              Parameter is used for providing detailed css parsing errors.
     * \param parentName Name of parent
     * \param parentObjectName Parent object (style) name
     * \param child A flag indicating whether this selector represents a descendant or
              direct child of the parent.
     * \param subclasses A flag indicating whether the su
     */

    explicit MStyleSheetSelector(const QByteArray &objectName,
                                   const QByteArray &className,
                                   const QByteArray &classType,
                                   const Orientation orientation,
                                   const QByteArray &mode,
                                   const QByteArray &filename,
                                   const QByteArray &parentName,
                                   const QByteArray &parentObjectName,
                                   Flags flags = (Flags) 0);


    /*!
     * Copy constructor
     * \param selector style sheet selector to clone
     */
    MStyleSheetSelector(const MStyleSheetSelector &selector);

    //! Destructor for MStyleSheetSelector class
    virtual ~MStyleSheetSelector();

    /*!
     * \brief Provides access for the attributes of this selector.
     */
    MAttributeList *attributes();

    /*!
     * \brief Returns the parent class name for this selector.
     */
    QByteArray parentName() const;

    /*!
     * \brief Returns the parent class object name for this selector.
     */
    QByteArray parentObjectName() const;

    /*!
     * \brief Returns the object name for this selector.
     */
    QByteArray objectName() const;

    /*!
     * \brief Returns the class name for this selector.
     */
    QByteArray className() const;

    /*!
     * \brief Returns the type for this selector.
     */
    QByteArray classType() const;

    /*!
     * \brief Returns the screen orientation for this selector.
     */
    Orientation orientation() const;

    /*!
     * \brief Returns the object mode for this selector.
     */
    QByteArray mode() const;

    /*!
     * \brief Returns flags of this selector.
     */
    Flags flags() const;

private:
    Q_DECLARE_PRIVATE(MStyleSheetSelector)
    MStyleSheetSelectorPrivate *const d_ptr;
};
//! \internal_end

#endif
