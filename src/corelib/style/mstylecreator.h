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

#ifndef MSTYLECREATOR_H
#define MSTYLECREATOR_H

#include "mexport.h"
#include <QMetaProperty>
#include <mnamespace.h>

#if defined M_LIBRARY_NAME
#define M_REGISTER_STYLE(STYLE) \
    static const MStyleCreator<STYLE> g_StyleCreator(#STYLE, M_LIBRARY_NAME, M::Library);
#elif defined M_APPLICATION_NAME
#define M_REGISTER_STYLE(STYLE) \
    static const MStyleCreator<STYLE> gStyleCreator(#STYLE, M_APPLICATION_NAME, M::Application);
#else
#error "You must have CONFIG += meegotouch in your .pro file in order to register styles, if you do not have any style classes, please do not include MStyleCreator header!"

#endif

// forward declarations
class MStyle;
class MStyleCreatorBasePrivate;

// interface for MStyleGenerators
// you can implement your own creator or use MStyleCreator template class with
// M_REGISTER_STYLE-macro.
class M_CORE_EXPORT MStyleCreatorBase
{
public:
    /*!
       Constructor will register this creator to MClassFactory.
     */
    MStyleCreatorBase(const char *styleClassName,
                        const char *styleAssemblyName,
                        M::AssemblyType styleAssemblyType);

    /*!
       Destructor will unregister this creator from MClassFactory.
     */
    virtual ~MStyleCreatorBase();

    /*!
       Returns new style instance.
       Ownership is transferred to caller.
     */
    virtual MStyle *create() const = 0;

    /*!
       Returns the assembly name in which the style was introduced.
     */
    QString assemblyName() const;

    /*!
       Returns the type of the assembly in which the style was introduced.
     */
    M::AssemblyType assemblyType() const;

    /*!
       Returns type name of the given attribute.
       Returns NULL if attribute was not found.
     */
    virtual const char *attributeType(const char *attributeName) const = 0;

    /*!
       Returns meta object of the style.
     */
    virtual const QMetaObject *metaObject() const = 0;
private:
    MStyleCreatorBasePrivate *const d_ptr;
};

template<class STYLE>
class MStyleCreator : public MStyleCreatorBase
{
public:
    MStyleCreator(const char *styleClassName,
                    const char *styleAssemblyName,
                    M::AssemblyType styleAssemblyType) :
        MStyleCreatorBase(styleClassName, styleAssemblyName, styleAssemblyType)
    {}

    virtual ~MStyleCreator()
    {}

    virtual MStyle *create() const {
        return new STYLE();
    }

    virtual const char *attributeType(const char *attributeName) const {
        int propertyIndex = STYLE::staticMetaObject.indexOfProperty(attributeName);

        // leave the QObject properties untouched
        if (propertyIndex < QObject::staticMetaObject.propertyOffset()) {
            return NULL;
        }

        return STYLE::staticMetaObject.property(propertyIndex).typeName();
    }

    virtual const QMetaObject *metaObject() const {
        return &STYLE::staticMetaObject;
    }
};

#endif

