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

#ifndef DUISTYLECREATOR_H
#define DUISTYLECREATOR_H

#include "duiexport.h"
#include <QMetaProperty>
#include <duinamespace.h>

#if defined DUI_LIBRARY_NAME
#define DUI_REGISTER_STYLE(STYLE) \
    static const DuiStyleCreator<STYLE> g_StyleCreator(#STYLE, DUI_LIBRARY_NAME, Dui::Library);
#elif defined DUI_APPLICATION_NAME
#define DUI_REGISTER_STYLE(STYLE) \
    static const DuiStyleCreator<STYLE> gStyleCreator(#STYLE, DUI_APPLICATION_NAME, Dui::Application);
#else
#error "You must have CONFIG += dui in your .pro file in order to register styles, if you do not have any style classes, please do not include DuiStyleCreator header!"

#endif

// forward declarations
class DuiStyle;
class DuiStyleCreatorBasePrivate;

// interface for DuiStyleGenerators
// you can implement your own creator or use DuiStyleCreator template class with
// DUI_REGISTER_STYLE-macro.
class DUI_EXPORT DuiStyleCreatorBase
{
public:
    /*!
       Constructor will register this creator to DuiClassFactory.
     */
    DuiStyleCreatorBase(const char *styleClassName,
                        const char *styleAssemblyName,
                        Dui::AssemblyType styleAssemblyType);

    /*!
       Destructor will unregister this creator from DuiClassFactory.
     */
    virtual ~DuiStyleCreatorBase();

    /*!
       Returns new style instance.
       Ownership is transferred to caller.
     */
    virtual DuiStyle *create() const = 0;

    /*!
       Returns the assembly name in which the style was introduced.
     */
    QString assemblyName() const;

    /*!
       Returns the type of the assembly in which the style was introduced.
     */
    Dui::AssemblyType assemblyType() const;

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
    DuiStyleCreatorBasePrivate *const d_ptr;
};

template<class STYLE>
class DUI_EXPORT DuiStyleCreator : public DuiStyleCreatorBase
{
public:
    DuiStyleCreator(const char *styleClassName,
                    const char *styleAssemblyName,
                    Dui::AssemblyType styleAssemblyType) :
        DuiStyleCreatorBase(styleClassName, styleAssemblyName, styleAssemblyType)
    {}

    virtual ~DuiStyleCreator()
    {}

    virtual DuiStyle *create() const {
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

