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

#ifndef DUICLASSFACTORY_H
#define DUICLASSFACTORY_H

#include "duiexport.h"
#include <duinamespace.h>

// forward declarations
class DuiViewCreatorBase;
class DuiStyleCreatorBase;
class DuiWidgetCreatorBase;
class DuiAnimationCreatorBase;
class DuiWidgetView;
class DuiStyle;
class DuiAnimation;
class QMetaObject;

class DuiWidgetController;

// DuiClassFactory
class DUI_EXPORT DuiClassFactory
{
public:
    static DuiClassFactory *instance();

    ////////////////////
    // widget classes //
    ////////////////////
    /*!
      Returns the name of the assembly where the given widget class was introduced.
      If the widget class was not found, returns NULL.
     */
    QString widgetAssemblyName(const QString &controllerClassName) const;

    /*!
      Returns the type of the assembly where the given widget class was introduced.
      If the widget class was not found, returns -1.
     */
    Dui::AssemblyType widgetAssemblyType(const QString &widgetClassName) const;

    //////////////////
    // view classes //
    //////////////////

    /*!
       Retrns a new instance of a view. Ownership is transferred to caller.
     */
    DuiWidgetView *createView(const char *viewClassName, const DuiWidgetController *controller) const;

    /*!
       Returns style type for a given view.
       If view is not registered return value is NULL.
     */
    const char *viewStyleType(const char *viewClassName) const;


    ///////////////////
    // style classes //
    ///////////////////

    /*!
       Returns a new instance of a style. Ownership is transferred to caller.
     */
    DuiStyle *createStyle(const char *styleClassName) const;

    /*!
       Returns the type name of given attribute.
       If attribute was not found from given styleClassName, returns NULL.
     */
    const char *styleAttributeType(const char *styleClassName, const char *attributeName) const;

    /*!
       Returns the meta object of given class type.
       If meta object was not found, returns NULL.
     */
    const QMetaObject *styleMetaObject(const char *styleClassName) const;

    /*!
      Returns the name of the assembly where the given style class was introduced.
      If the style class was not found, returns NULL.
     */
    QString styleAssemblyName(const char *styleClassName) const;

    /*!
      Returns the type of the assembly where the given style class was introduced.
      If the style class was not found, returns -1.
     */
    Dui::AssemblyType styleAssemblyType(const char *styleClassName) const;

    ///////////////////////
    // animation classes //
    ///////////////////////

    /*!
       Returns a new instance of an animation. Ownership is transferred to caller.
     */
    DuiAnimation *createAnimation(const char *animationClassName) const;

private:
    /////////
    // widgets
    friend class DuiWidgetCreatorBase;
    /*!
       Registers a new widget creator for this factory. This method gets called automatically
       from DuiWidgetCreatorBase constructor and should not be called from anywhere else.
     */
    void registerWidgetCreator(DuiWidgetCreatorBase *widgetCreator, const char *widgetClassName);
    /*!
       Unregisters a widget creator from this factory. This method gets called automatically
       from DuiWidgetCreatorBase destructor and should not be called from anywhere else.
     */
    void unregisterWidgetCreator(DuiWidgetCreatorBase *widgetCreator);

    /////////
    // views
    friend class DuiViewCreatorBase;
    /*!
       Registers a new view creator for this factory. This method gets called automatically
       from DuiViewCreatorBase constructor and should not be called from anywhere else.
     */
    void registerViewCreator(DuiViewCreatorBase *viewCreator, const char *viewClassName);
    /*!
       Unregisters a view creator from this factory. This method gets called automatically
       from DuiViewCreatorBase destructor and should not be called from anywhere else.
     */
    void unregisterViewCreator(DuiViewCreatorBase *viewCreator);

    //////////
    // styles
    friend class DuiStyleCreatorBase;
    /*!
       Registers a new style creator for this factory. This method gets called automatically
       from DuiStyleCreatorBase constructor and should not be called from anywhere else.
     */
    void registerStyleCreator(DuiStyleCreatorBase *styleCreator, const char *styleClassName);
    /*!
       Unregisters a style creator from this factory. This method gets called automatically
       from DuiStyleCreatorBase destructor and should not be called from anywhere else.
     */
    void unregisterStyleCreator(DuiStyleCreatorBase *styleCreator);

    //////////////
    // animations
    friend class DuiAnimationCreatorBase;
    /*!
       Registers a new animation creator for this factory. This method gets called automatically
       from DuiAnimationCreatorBase constructor and should not be called from anywhere else.
     */
    void registerAnimationCreator(DuiAnimationCreatorBase *animationCreator, const char *animationClassName);
    /*!
       Unregisters a animation creator from this factory. This method gets called automatically
       from DuiAnimationCreatorBase destructor and should not be called from anywhere else.
     */
    void unregisterAnimationCreator(DuiAnimationCreatorBase *animationCreator);

private:
    DuiClassFactory();
    ~DuiClassFactory();
    class DuiClassFactoryPrivate *d_ptr;
};

#endif

