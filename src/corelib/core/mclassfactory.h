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

#ifndef MCLASSFACTORY_H
#define MCLASSFACTORY_H

#include "mexport.h"
#include <mnamespace.h>

// forward declarations
class MViewCreatorBase;
class MStyleCreatorBase;
class MWidgetCreatorBase;
class MAnimationCreatorBase;
class MEffectCreatorBase;
class MWidgetView;
class MStyle;
class QAbstractAnimation;
struct QMetaObject;

class MWidgetController;

//! \internal
// MClassFactory
class M_CORE_EXPORT MClassFactory
{
public:
    static MClassFactory *instance();

    ////////////////////
    // widget classes //
    ////////////////////
    /*!
      Returns the name of the assembly where the given widget class was introduced.
      If the widget class was not found, returns NULL.
     */
    QString widgetAssemblyName(const QByteArray &controllerClassName) const;

    /*!
      Returns the type of the assembly where the given widget class was introduced.
      If the widget class was not found, returns -1.
     */
    M::AssemblyType widgetAssemblyType(const QByteArray &widgetClassName) const;

    //////////////////
    // view classes //
    //////////////////

    /*!
       Retrns a new instance of a view. Ownership is transferred to caller.
     */
    MWidgetView *createView(const char *viewClassName, const MWidgetController *controller) const;

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
    MStyle *createStyle(const char *styleClassName) const;

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
    M::AssemblyType styleAssemblyType(const char *styleClassName) const;

    ///////////////////////
    // animation classes //
    ///////////////////////

    /*!
       Returns a new instance of an animation. Ownership is transferred to caller.
     */
    QAbstractAnimation *createAnimation(const QString &animationClassName) const;

    ///////////////////////
    // effect classes //
    ///////////////////////

    /*!
       Returns a new instance of an effect. Ownership is transferred to caller.
     */
    QGraphicsEffect *createEffect(const QString &effectClassName) const;

private:
    /////////
    // widgets
    friend class MWidgetCreatorBase;
    /*!
       Registers a new widget creator for this factory. This method gets called automatically
       from MWidgetCreatorBase constructor and should not be called from anywhere else.
     */
    void registerWidgetCreator(MWidgetCreatorBase *widgetCreator, const char *widgetClassName);
    /*!
       Unregisters a widget creator from this factory. This method gets called automatically
       from MWidgetCreatorBase destructor and should not be called from anywhere else.
     */
    void unregisterWidgetCreator(MWidgetCreatorBase *widgetCreator);

    /////////
    // views
    friend class MViewCreatorBase;
    /*!
       Registers a new view creator for this factory. This method gets called automatically
       from MViewCreatorBase constructor and should not be called from anywhere else.
     */
    void registerViewCreator(MViewCreatorBase *viewCreator, const char *viewClassName);
    /*!
       Unregisters a view creator from this factory. This method gets called automatically
       from MViewCreatorBase destructor and should not be called from anywhere else.
     */
    void unregisterViewCreator(MViewCreatorBase *viewCreator);

    //////////
    // styles
    friend class MStyleCreatorBase;
    /*!
       Registers a new style creator for this factory. This method gets called automatically
       from MStyleCreatorBase constructor and should not be called from anywhere else.
     */
    void registerStyleCreator(MStyleCreatorBase *styleCreator, const char *styleClassName);
    /*!
       Unregisters a style creator from this factory. This method gets called automatically
       from MStyleCreatorBase destructor and should not be called from anywhere else.
     */
    void unregisterStyleCreator(MStyleCreatorBase *styleCreator);

    //////////////
    // animations
    friend class MAnimationCreatorBase;
    /*!
       Registers a new animation creator for this factory. This method gets called automatically
       from MAnimationCreatorBase constructor and should not be called from anywhere else.
     */
    void registerAnimationCreator(MAnimationCreatorBase *animationCreator, const char *animationClassName);
    /*!
       Unregisters a animation creator from this factory. This method gets called automatically
       from MAnimationCreatorBase destructor and should not be called from anywhere else.
     */
    void unregisterAnimationCreator(MAnimationCreatorBase *animationCreator);

    //////////////
    // effects
    friend class MEffectCreatorBase;
    /*!
       Registers a new graphics effect creator for this factory. This method gets called automatically
       from MEffectCreatorBase constructor and should not be called from anywhere else.
     */
    void registerEffectCreator(MEffectCreatorBase *effectCreator, const char *effectClassName);
    /*!
       Unregisters a new graphics effect creator from this factory. This method gets called automatically
       from MEffectCreatorBase destructor and should not be called from anywhere else.
     */
    void unregisterEffectCreator(MEffectCreatorBase *effectCreator);

private:
    MClassFactory();
    ~MClassFactory();
    class MClassFactoryPrivate *d_ptr;
};
//! \internal_end

#endif

