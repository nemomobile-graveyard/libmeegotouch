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

#include "mclassfactory.h"
#include "mviewcreator.h"
#include "mstylecreator.h"
#include "mwidgetcreator.h"
#include "manimationcreator.h"

#include <QHash>
#include <QString>
#include <MDebug>

class MClassFactoryPrivate
{
public:
    QMap<QString, MWidgetCreatorBase *> widgetCreators;
    QMap<QString, MViewCreatorBase *> viewCreators;
    QMap<QString, MStyleCreatorBase *> styleCreators;
    QMap<QString, MAnimationCreatorBase *> animationCreators;
};

MClassFactory::MClassFactory() :
    d_ptr(new MClassFactoryPrivate)
{
}

MClassFactory::~MClassFactory()
{
    delete d_ptr;
}

MClassFactory *MClassFactory::instance()
{
    static MClassFactory *gFactory = 0;

    if ( ! gFactory )
    {
        gFactory = new MClassFactory();
    }

    return gFactory;
}

////////////
// Widget //
////////////
void MClassFactory::registerWidgetCreator(MWidgetCreatorBase *creator, const char *widgetClassName)
{
    QString type(widgetClassName);
    if (type.isEmpty())
        qFatal("MClassFactory cannot register MWidgetCreator with empty type identifier");

    if (d_ptr->widgetCreators.contains(type)) {
        qWarning("MClassFactory already contains MWidgetCreator for %s", widgetClassName);
    } else {
        d_ptr->widgetCreators.insert(type, creator);
    }
}

void MClassFactory::unregisterWidgetCreator(MWidgetCreatorBase *creator)
{
    QString widgetClassName = d_ptr->widgetCreators.key(creator);
    if (widgetClassName.isEmpty()) {
        qWarning("MClassFactory cannot unregister MWidgetCreator which is not registered");
    } else {
        d_ptr->widgetCreators.remove(widgetClassName);
    }
}

QString MClassFactory::widgetAssemblyName(const QString &widgetClassName) const
{
    const MWidgetCreatorBase *widgetCreator = d_ptr->widgetCreators.value(widgetClassName, NULL);
    if (!widgetCreator) {
        mDebug("MClassFactory") << "could not find assembly name for" << widgetClassName;
        return QString();
    }
    return widgetCreator->assemblyName();
}

M::AssemblyType MClassFactory::widgetAssemblyType(const QString &widgetClassName) const
{
    const MWidgetCreatorBase *widgetCreator = d_ptr->widgetCreators.value(widgetClassName, NULL);
    if (!widgetCreator) {
        mDebug("MClassFactory") << "could not find assembly type for" << widgetClassName;
        return (M::AssemblyType) - 1;
    }
    return widgetCreator->assemblyType();
}

//////////
// View //
//////////
void MClassFactory::registerViewCreator(MViewCreatorBase *creator, const char *viewClassName)
{
    QString type(viewClassName);
    if (type.isEmpty())
        qFatal("MClassFactory cannot register MViewCreator with empty type identifier");

    if (d_ptr->viewCreators.contains(type)) {
        qWarning("MClassFactory already contains MViewCreator for %s", viewClassName);
    } else {
        d_ptr->viewCreators.insert(type, creator);
    }
}

void MClassFactory::unregisterViewCreator(MViewCreatorBase *creator)
{
    QString viewClassName = d_ptr->viewCreators.key(creator);
    if (viewClassName.isEmpty()) {
        qWarning("MClassFactory cannot unregister MViewCreator which is not registered");
    } else {
        d_ptr->viewCreators.remove(viewClassName);
    }
}

MWidgetView *MClassFactory::createView(const char *viewClassName, const MWidgetController *controller) const
{
    const MViewCreatorBase *viewCreator = d_ptr->viewCreators.value(QString(viewClassName), NULL);
    if (!viewCreator) {
        mWarning("MClassFactory") << "could not create" << viewClassName;
        return NULL;
    }
    return viewCreator->create(controller);
}

const char *MClassFactory::viewStyleType(const char *viewClassName) const
{
    const MViewCreatorBase *viewCreator = d_ptr->viewCreators.value(QString(viewClassName), NULL);
    if (!viewCreator) {
        mWarning("MClassFactory") << "could not find" << viewClassName;
        return NULL;
    }
    return viewCreator->styleType();
}

///////////
// Style //
///////////
void MClassFactory::registerStyleCreator(MStyleCreatorBase *creator, const char *styleClassName)
{
    QString type(styleClassName);
    if (type.isEmpty())
        qFatal("MClassFactory cannot register MStyleCreator with empty type identifier");

    if (d_ptr->styleCreators.contains(type)) {
        qWarning("MClassFactory already contains MStyleCreator for %s", styleClassName);
    } else {
        d_ptr->styleCreators.insert(type, creator);
    }
}

void MClassFactory::unregisterStyleCreator(MStyleCreatorBase *creator)
{
    QString styleClassName = d_ptr->styleCreators.key(creator);
    if (styleClassName.isEmpty()) {
        qWarning("MClassFactory cannot unregister MStyleCreator which is not registered");
    } else {
        d_ptr->styleCreators.remove(styleClassName);
    }
}

MStyle *MClassFactory::createStyle(const char *styleClassName) const
{
    const MStyleCreatorBase *styleCreator = d_ptr->styleCreators.value(QString(styleClassName), NULL);
    if (!styleCreator) {
        mWarning("MClassFactory") << "could not create" << styleClassName;
        return NULL;
    }
    return styleCreator->create();
}

const char *MClassFactory::styleAttributeType(const char *styleClassName, const char *attributeName) const
{
    const MStyleCreatorBase *styleCreator = d_ptr->styleCreators.value(QString(styleClassName), NULL);
    if (!styleCreator) {
        mWarning("MClassFactory") << "could not find" << styleClassName;
        return NULL;
    }
    return styleCreator->attributeType(attributeName);
}

const QMetaObject *MClassFactory::styleMetaObject(const char *styleClassName) const
{
    const MStyleCreatorBase *styleCreator = d_ptr->styleCreators.value(QString(styleClassName), NULL);
    if (!styleCreator) {
        mWarning("MClassFactory") << "could not find meta object for" << styleClassName;
        return NULL;
    }
    return styleCreator->metaObject();
}

QString MClassFactory::styleAssemblyName(const char *styleClassName) const
{
    const MStyleCreatorBase *styleCreator = d_ptr->styleCreators.value(QString(styleClassName), NULL);
    if (!styleCreator) {
        mWarning("MClassFactory") << "could not find assembly name for" << styleClassName;
        return QString();
    }
    return styleCreator->assemblyName();
}

M::AssemblyType MClassFactory::styleAssemblyType(const char *styleClassName) const
{
    const MStyleCreatorBase *styleCreator = d_ptr->styleCreators.value(QString(styleClassName), NULL);
    if (!styleCreator) {
        mWarning("MClassFactory") << "could not find assembly type for" << styleClassName;
        return (M::AssemblyType) - 1;
    }
    return styleCreator->assemblyType();
}

///////////////
// Animation //
///////////////

void MClassFactory::registerAnimationCreator(MAnimationCreatorBase *creator, const char *animationClassName)
{
    QString type(animationClassName);
    if (type.isEmpty())
        qFatal("MClassFactory cannot register MAnimationCreator with empty type identifier");

    if (d_ptr->animationCreators.contains(type)) {
        qWarning("MClassFactory already contains MAnimationCreator for %s", animationClassName);
    } else {
        d_ptr->animationCreators.insert(type, creator);
    }
}

void MClassFactory::unregisterAnimationCreator(MAnimationCreatorBase *creator)
{
    QString animationClassName = d_ptr->animationCreators.key(creator);
    if (animationClassName.isEmpty()) {
        qWarning("MClassFactory cannot unregister MAnimationCreator which is not registered");
    } else {
        d_ptr->animationCreators.remove(animationClassName);
    }
}

MAnimation *MClassFactory::createAnimation(const char *animationClassName) const
{
    const MAnimationCreatorBase *animationCreator = d_ptr->animationCreators.value(QString(animationClassName), NULL);
    if (!animationCreator) {
        mWarning("MClassFactory") << "could not create" << animationClassName;
        return NULL;
    }
    return animationCreator->create();
}


