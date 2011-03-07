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
#include "muniquestringcache.h"

#include <QByteArray>
#include <QVariant>

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
     * \param parentObjectName Parent object (style) name
     * \param child A flag indicating whether this selector represents a descendant or
              direct child of the parent.
     * \param subclasses A flag indicating whether the su
     */

    explicit MStyleSheetSelector(
            const MStyleSheetAttribute *attributeList,
            const int attributeCount,
            const MUniqueStringCache::Index objectName,
            const MUniqueStringCache::Index className,
            const MUniqueStringCache::Index classType,
            const Orientation orientation,
            const MUniqueStringCache::Index mode,
            const MUniqueStringCache::Index parentName,
            const MUniqueStringCache::Index parentObjectName,
            const Flags flags,
            const MUniqueStringCache::Index filename
    );

    MUniqueStringCache::Index objectNameID() const;
    MUniqueStringCache::Index classNameID() const;
    MUniqueStringCache::Index classTypeID() const;
    MUniqueStringCache::Index modeID() const;
    MUniqueStringCache::Index parentNameID() const;
    MUniqueStringCache::Index parentObjectNameID() const;

    //! Destructor for MStyleSheetSelector class
    ~MStyleSheetSelector();

    /*!
     * \brief Provides access for the attributes of this selector.
     */
    const MStyleSheetAttribute* attributeList() const;

    /*!
     * \brief Return attribute with a given name.
     */
    const MStyleSheetAttribute* attributeByName(MUniqueStringCache::Index name) const;

    /*!
     * \brief Provides access for the attributes of this selector.
     */
    int attributeCount() const;

    /*!
     * \brief Returns the parent class name for this selector.
     */
    QLatin1String parentName() const;

    /*!
     * \brief Returns the parent class object name for this selector.
     */
    QLatin1String parentObjectName() const;

    /*!
     * \brief Returns the object name for this selector.
     */
    QLatin1String objectName() const;

    /*!
     * \brief Returns the class name for this selector.
     */
    QLatin1String className() const;

    /*!
     * \brief Returns the type for this selector.
     */
    QLatin1String classType() const;

    /*!
     * \brief Returns the screen orientation for this selector.
     */
    Orientation orientation() const;

    /*!
     * \brief Returns the object mode for this selector.
     */
    QLatin1String mode() const;

    /*!
     * \brief Returns flags of this selector.
     */
    Flags flags() const;

    QLatin1String filename() const;
    MUniqueStringCache::Index filenameID() const;

    bool fromMappedMemory;

private:
    Q_DISABLE_COPY(MStyleSheetSelector)

    //! Array containing a sorted list of MStyleSheetAttributes
    const MStyleSheetAttribute *_attributeList;
    //! The number of attributes in _attributeList
    const int _attributeCount;
    //! Object name, represented by this selector.
    const MUniqueStringCache::Index _objectName;
    //! Class name, represented by this selector.
    const MUniqueStringCache::Index _className;
    //! Type, represented by this selector.
    const MUniqueStringCache::Index _classType;
    //! Screen orientation type, represented by this selector.
    const MStyleSheetSelector::Orientation _orientation;
    //! Object mode, represented by this selector.
    const MUniqueStringCache::Index _mode;
    //! Parent class name, represented by this selector.
    const MUniqueStringCache::Index _parentName;
    //! Parent class object name, represented by this selector.
    const MUniqueStringCache::Index _parentObjectName;
    //! Flags indicating whether this is a special kind of selector.
    const MStyleSheetSelector::Flags _flags;
    //! Filename where this selector comes from.
    const MUniqueStringCache::Index _filename;
};

//! \internal_end

#endif
