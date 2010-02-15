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

#ifndef DUIWIDGETCREATOR_H
#define DUIWIDGETCREATOR_H

#include "duiexport.h"
#include <QMetaProperty>
#include <duinamespace.h>

#if defined DUI_LIBRARY_NAME
#define DUI_REGISTER_WIDGET(WIDGET) \
    static const DuiWidgetCreator<WIDGET> g_WidgetCreator(#WIDGET, DUI_LIBRARY_NAME, Dui::Library);
#define DUI_REGISTER_WIDGET_NO_CREATE(WIDGET) \
    static const DuiWidgetNullCreator<WIDGET> g_WidgetCreator(#WIDGET, DUI_LIBRARY_NAME, Dui::Library);
#elif defined DUI_APPLICATION_NAME
#define DUI_REGISTER_WIDGET(WIDGET) \
    static const DuiWidgetCreator<WIDGET> g_WidgetCreator(#WIDGET, DUI_APPLICATION_NAME, Dui::Application);
#define DUI_REGISTER_WIDGET_NO_CREATE(WIDGET) \
    static const DuiWidgetNullCreator<WIDGET> g_WidgetCreator(#WIDGET, DUI_APPLICATION_NAME, Dui::Application);
#else
#error "You must define either DUI_LIBRARY_NAME or DUI_APPLICATION_NAME in your .pro file in order to register widgets!"
#endif

// forward declarations
class DuiWidgetController;
class DuiWidgetCreatorBasePrivate;

/*!
    \brief Interface for DuiWidgetGenerators, you can implement your own creator or use
           DuiWidgetCreator template class with DUI_REGISTER_WIDGET-macro.
 */
class DUI_EXPORT DuiWidgetCreatorBase
{
public:

    /*!
       Constructor will register this creator to DuiClassFactory.
     */
    DuiWidgetCreatorBase(const char *widgetClassName,
                         const char *widgetAssemblyName,
                         Dui::AssemblyType widgetAssemblyType);

    /*!
       Destructor will unregister this creator from DuiClassFactory.
     */
    virtual ~DuiWidgetCreatorBase();

    /*!
       Returns new widget instance.
       Ownership is transferred to caller.
     */
    virtual DuiWidgetController *create() const = 0;

    /*!
       Returns the assembly name in which the widget was introduced.
     */
    QString assemblyName() const;

    /*!
       Returns the type of the assembly in which the widget was introduced.
     */
    Dui::AssemblyType assemblyType() const;

    /*!
       Returns meta object of the widget.
     */
    virtual const QMetaObject *metaObject() const = 0;

private:
    DuiWidgetCreatorBasePrivate *const d_ptr;
};

template<class WIDGET>
class DUI_EXPORT DuiWidgetCreator : public DuiWidgetCreatorBase
{
public:

    DuiWidgetCreator(const char *widgetClassName,
                     const char *widgetAssemblyName,
                     Dui::AssemblyType widgetAssemblyType) :
        DuiWidgetCreatorBase(widgetClassName, widgetAssemblyName, widgetAssemblyType)
    {}

    virtual ~DuiWidgetCreator()
    {}

    virtual DuiWidgetController *create() const {
        return new WIDGET();
    }

    virtual const QMetaObject *metaObject() const {
        return &WIDGET::staticMetaObject;
    }
};

template<class WIDGET>
class DUI_EXPORT DuiWidgetNullCreator : public DuiWidgetCreatorBase
{
public:

    DuiWidgetNullCreator(const char *widgetClassName,
                         const char *widgetAssemblyName,
                         Dui::AssemblyType widgetAssemblyType) :
        DuiWidgetCreatorBase(widgetClassName, widgetAssemblyName, widgetAssemblyType)
    {}

    virtual ~DuiWidgetNullCreator()
    {}

    virtual DuiWidgetController *create() const {
        return NULL;
    }

    virtual const QMetaObject *metaObject() const {
        return &WIDGET::staticMetaObject;
    }
};

#endif

