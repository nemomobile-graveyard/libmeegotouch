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

#ifndef MWIDGETCREATOR_H
#define MWIDGETCREATOR_H

#include "mexport.h"
#include <QMetaProperty>
#include <mnamespace.h>

#if defined M_LIBRARY_NAME
#define M_REGISTER_WIDGET(WIDGET) \
    static const MWidgetCreator<WIDGET> g_WidgetCreator(#WIDGET, M_LIBRARY_NAME, M::Library);
#define M_REGISTER_WIDGET_NO_CREATE(WIDGET) \
    static const MWidgetNullCreator<WIDGET> g_WidgetCreator(#WIDGET, M_LIBRARY_NAME, M::Library);
#elif defined M_APPLICATION_NAME
#define M_REGISTER_WIDGET(WIDGET) \
    static const MWidgetCreator<WIDGET> g_WidgetCreator(#WIDGET, M_APPLICATION_NAME, M::Application);
#define M_REGISTER_WIDGET_NO_CREATE(WIDGET) \
    static const MWidgetNullCreator<WIDGET> g_WidgetCreator(#WIDGET, M_APPLICATION_NAME, M::Application);
#else
#error "You must define either M_LIBRARY_NAME or M_APPLICATION_NAME in your .pro file in order to register widgets!"
#endif

// forward declarations
class MWidgetController;
class MWidgetCreatorBasePrivate;

/*!
    \brief Interface for MWidgetGenerators, you can implement your own creator or use
           MWidgetCreator template class with M_REGISTER_WIDGET-macro.
 */
class M_CORE_EXPORT MWidgetCreatorBase
{
public:

    /*!
       Constructor will register this creator to MClassFactory.
     */
    MWidgetCreatorBase(const char *widgetClassName,
                         const char *widgetAssemblyName,
                         M::AssemblyType widgetAssemblyType);

    /*!
       Destructor will unregister this creator from MClassFactory.
     */
    virtual ~MWidgetCreatorBase();

    /*!
       Returns new widget instance.
       Ownership is transferred to caller.
     */
    virtual MWidgetController *create() const = 0;

    /*!
       Returns the assembly name in which the widget was introduced.
     */
    QString assemblyName() const;

    /*!
       Returns the type of the assembly in which the widget was introduced.
     */
    M::AssemblyType assemblyType() const;

    /*!
       Returns meta object of the widget.
     */
    virtual const QMetaObject *metaObject() const = 0;

private:
    MWidgetCreatorBasePrivate *const d_ptr;
};

template<class WIDGET>
class MWidgetCreator : public MWidgetCreatorBase
{
public:

    MWidgetCreator(const char *widgetClassName,
                     const char *widgetAssemblyName,
                     M::AssemblyType widgetAssemblyType) :
        MWidgetCreatorBase(widgetClassName, widgetAssemblyName, widgetAssemblyType)
    {}

    virtual ~MWidgetCreator()
    {}

    virtual MWidgetController *create() const {
        return new WIDGET();
    }

    virtual const QMetaObject *metaObject() const {
        return &WIDGET::staticMetaObject;
    }
};

template<class WIDGET>
class MWidgetNullCreator : public MWidgetCreatorBase
{
public:

    MWidgetNullCreator(const char *widgetClassName,
                         const char *widgetAssemblyName,
                         M::AssemblyType widgetAssemblyType) :
        MWidgetCreatorBase(widgetClassName, widgetAssemblyName, widgetAssemblyType)
    {}

    virtual ~MWidgetNullCreator()
    {}

    virtual MWidgetController *create() const {
        return NULL;
    }

    virtual const QMetaObject *metaObject() const {
        return &WIDGET::staticMetaObject;
    }
};

#endif

