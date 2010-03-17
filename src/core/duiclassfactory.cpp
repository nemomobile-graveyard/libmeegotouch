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

#include "duiclassfactory.h"
#include "duiviewcreator.h"
#include "duistylecreator.h"
#include "duiwidgetcreator.h"
#include "duianimationcreator.h"

#include <QHash>
#include <QString>
#include <DuiDebug>

class DuiClassFactoryPrivate
{
public:
    QMap<QString, DuiWidgetCreatorBase *> widgetCreators;
    QMap<QString, DuiViewCreatorBase *> viewCreators;
    QMap<QString, DuiStyleCreatorBase *> styleCreators;
    QMap<QString, DuiAnimationCreatorBase *> animationCreators;
};

DuiClassFactory::DuiClassFactory() :
    d_ptr(new DuiClassFactoryPrivate)
{
}

DuiClassFactory::~DuiClassFactory()
{
    delete d_ptr;
}

DuiClassFactory *DuiClassFactory::instance()
{
    static DuiClassFactory gFactory;
    return &gFactory;
}

////////////
// Widget //
////////////
void DuiClassFactory::registerWidgetCreator(DuiWidgetCreatorBase *creator, const char *widgetClassName)
{
    QString type(widgetClassName);
    if (type.isEmpty())
        qFatal("DuiClassFactory cannot register DuiWidgetCreator with empty type identifier");

    if (d_ptr->widgetCreators.contains(type)) {
        qWarning("DuiClassFactory already contains DuiWidgetCreator for %s", widgetClassName);
    } else {
        d_ptr->widgetCreators.insert(type, creator);
    }
}

void DuiClassFactory::unregisterWidgetCreator(DuiWidgetCreatorBase *creator)
{
    QString widgetClassName = d_ptr->widgetCreators.key(creator);
    if (widgetClassName.isEmpty()) {
        qWarning("DuiClassFactory cannot unregister DuiWidgetCreator which is not registered");
    } else {
        d_ptr->widgetCreators.remove(widgetClassName);
    }
}

QString DuiClassFactory::widgetAssemblyName(const QString &widgetClassName) const
{
    const DuiWidgetCreatorBase *widgetCreator = d_ptr->widgetCreators.value(widgetClassName, NULL);
    if (!widgetCreator) {
        duiWarning("DuiClassFactory") << "could not find assembly name for" << widgetClassName;
        return QString();
    }
    return widgetCreator->assemblyName();
}

Dui::AssemblyType DuiClassFactory::widgetAssemblyType(const QString &widgetClassName) const
{
    const DuiWidgetCreatorBase *widgetCreator = d_ptr->widgetCreators.value(widgetClassName, NULL);
    if (!widgetCreator) {
        duiWarning("DuiClassFactory") << "could not find assembly type for" << widgetClassName;
        return (Dui::AssemblyType) - 1;
    }
    return widgetCreator->assemblyType();
}

//////////
// View //
//////////
void DuiClassFactory::registerViewCreator(DuiViewCreatorBase *creator, const char *viewClassName)
{
    QString type(viewClassName);
    if (type.isEmpty())
        qFatal("DuiClassFactory cannot register DuiViewCreator with empty type identifier");

    if (d_ptr->viewCreators.contains(type)) {
        qWarning("DuiClassFactory already contains DuiViewCreator for %s", viewClassName);
    } else {
        d_ptr->viewCreators.insert(type, creator);
    }
}

void DuiClassFactory::unregisterViewCreator(DuiViewCreatorBase *creator)
{
    QString viewClassName = d_ptr->viewCreators.key(creator);
    if (viewClassName.isEmpty()) {
        qWarning("DuiClassFactory cannot unregister DuiViewCreator which is not registered");
    } else {
        d_ptr->viewCreators.remove(viewClassName);
    }
}

DuiWidgetView *DuiClassFactory::createView(const char *viewClassName, const DuiWidgetController *controller) const
{
    const DuiViewCreatorBase *viewCreator = d_ptr->viewCreators.value(QString(viewClassName), NULL);
    if (!viewCreator) {
        duiWarning("DuiClassFactory") << "could not create" << viewClassName;
        return NULL;
    }
    return viewCreator->create(controller);
}

const char *DuiClassFactory::viewStyleType(const char *viewClassName) const
{
    const DuiViewCreatorBase *viewCreator = d_ptr->viewCreators.value(QString(viewClassName), NULL);
    if (!viewCreator) {
        duiWarning("DuiClassFactory") << "could not find" << viewClassName;
        return NULL;
    }
    return viewCreator->styleType();
}

///////////
// Style //
///////////
void DuiClassFactory::registerStyleCreator(DuiStyleCreatorBase *creator, const char *styleClassName)
{
    QString type(styleClassName);
    if (type.isEmpty())
        qFatal("DuiClassFactory cannot register DuiStyleCreator with empty type identifier");

    if (d_ptr->styleCreators.contains(type)) {
        qWarning("DuiClassFactory already contains DuiStyleCreator for %s", styleClassName);
    } else {
        d_ptr->styleCreators.insert(type, creator);
    }
}

void DuiClassFactory::unregisterStyleCreator(DuiStyleCreatorBase *creator)
{
    QString styleClassName = d_ptr->styleCreators.key(creator);
    if (styleClassName.isEmpty()) {
        qWarning("DuiClassFactory cannot unregister DuiStyleCreator which is not registered");
    } else {
        d_ptr->styleCreators.remove(styleClassName);
    }
}

DuiStyle *DuiClassFactory::createStyle(const char *styleClassName) const
{
    const DuiStyleCreatorBase *styleCreator = d_ptr->styleCreators.value(QString(styleClassName), NULL);
    if (!styleCreator) {
        duiWarning("DuiClassFactory") << "could not create" << styleClassName;
        return NULL;
    }
    return styleCreator->create();
}

const char *DuiClassFactory::styleAttributeType(const char *styleClassName, const char *attributeName) const
{
    const DuiStyleCreatorBase *styleCreator = d_ptr->styleCreators.value(QString(styleClassName), NULL);
    if (!styleCreator) {
        duiWarning("DuiClassFactory") << "could not find" << styleClassName;
        return NULL;
    }
    return styleCreator->attributeType(attributeName);
}

const QMetaObject *DuiClassFactory::styleMetaObject(const char *styleClassName) const
{
    const DuiStyleCreatorBase *styleCreator = d_ptr->styleCreators.value(QString(styleClassName), NULL);
    if (!styleCreator) {
        duiWarning("DuiClassFactory") << "could not find meta object for" << styleClassName;
        return NULL;
    }
    return styleCreator->metaObject();
}

QString DuiClassFactory::styleAssemblyName(const char *styleClassName) const
{
    const DuiStyleCreatorBase *styleCreator = d_ptr->styleCreators.value(QString(styleClassName), NULL);
    if (!styleCreator) {
        duiWarning("DuiClassFactory") << "could not find assembly name for" << styleClassName;
        return QString();
    }
    return styleCreator->assemblyName();
}

Dui::AssemblyType DuiClassFactory::styleAssemblyType(const char *styleClassName) const
{
    const DuiStyleCreatorBase *styleCreator = d_ptr->styleCreators.value(QString(styleClassName), NULL);
    if (!styleCreator) {
        duiWarning("DuiClassFactory") << "could not find assembly type for" << styleClassName;
        return (Dui::AssemblyType) - 1;
    }
    return styleCreator->assemblyType();
}

///////////////
// Animation //
///////////////

void DuiClassFactory::registerAnimationCreator(DuiAnimationCreatorBase *creator, const char *animationClassName)
{
    QString type(animationClassName);
    if (type.isEmpty())
        qFatal("DuiClassFactory cannot register DuiAnimationCreator with empty type identifier");

    if (d_ptr->animationCreators.contains(type)) {
        qWarning("DuiClassFactory already contains DuiAnimationCreator for %s", animationClassName);
    } else {
        d_ptr->animationCreators.insert(type, creator);
    }
}

void DuiClassFactory::unregisterAnimationCreator(DuiAnimationCreatorBase *creator)
{
    QString animationClassName = d_ptr->animationCreators.key(creator);
    if (animationClassName.isEmpty()) {
        qWarning("DuiClassFactory cannot unregister DuiAnimationCreator which is not registered");
    } else {
        d_ptr->animationCreators.remove(animationClassName);
    }
}

DuiAnimation *DuiClassFactory::createAnimation(const char *animationClassName) const
{
    const DuiAnimationCreatorBase *animationCreator = d_ptr->animationCreators.value(QString(animationClassName), NULL);
    if (!animationCreator) {
        duiWarning("DuiClassFactory") << "could not create" << animationClassName;
        return NULL;
    }
    return animationCreator->create();
}


