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

#include "duistyle.h"
#include "duistyle_p.h"
#include "duitheme.h"
#include "duitheme_p.h"
#include "duiapplication.h"
#include "duiapplicationwindow.h"

// TODO: get rid of this include, make DuiStyle not to use duigen
#include "gen_duistyledata.h"
#include <QMetaProperty>

/* TODO: Will be removed once all the pointers are removed from the styles
         and pixmaps & scalable images doesn't need release calls */
Q_DECLARE_METATYPE(const QPixmap *)
Q_DECLARE_METATYPE(const DuiScalableImage *)
void releaseAllocatedResourcesFromStyle(const DuiStyle *style)
{
    // TODO: Fix this in duitheme so, that the pixmaps are not pointers
    // and therefore we don't need to call release at all.
    const int propertyCount = style->metaObject()->propertyCount();
    for (int i = 0; i < propertyCount; ++i) {
        const QMetaProperty &property = style->metaObject()->property(i);
        if (property.isReadable()) {
            const char *type = property.typeName();
            if (strcmp(type, "const QPixmap*") == 0) {
                DuiTheme::releasePixmap(qvariant_cast<const QPixmap *>(property.read(style)));
            } else if (strcmp(type, "const DuiScalableImage*") == 0) {
                DuiTheme::releaseScalableImage(qvariant_cast<const DuiScalableImage *>(property.read(style)));
            }
        }
    }
}

///////////////////
// PRIVATE CLASS //
///////////////////
int DuiStyle::references() const
{
    return data->references;
}

int DuiStyle::addReference()
{
    data->references++;
    return data->references;
}

int DuiStyle::removeReference()
{
    data->references--;
    if (data->references <= 0) {
        releaseAllocatedResourcesFromStyle(this);
        deleteLater(); // Use deleteLater() for QObjects instead of "delete this"
        return 0;
    }
    return data->references;
}

DuiStyleContainerPrivate::DuiStyleContainerPrivate() :
    currentMode("")
{
    cachedCurrentStyle[0] = 0;
    cachedCurrentStyle[1] = 0;
    parent = NULL;
}

DuiStyleContainerPrivate::~DuiStyleContainerPrivate()
{
    releaseStyles();
}

void DuiStyleContainerPrivate::releaseStyles()
{
    for (int i = 0; i < 2; ++i) {
        QHashIterator<QString, const DuiStyle*> iter(cachedStyles[i]);
        while (iter.hasNext()) {
            iter.next();
            DuiTheme::releaseStyle(iter.value());
        }
        cachedStyles[i].clear();
        cachedCurrentStyle[i] = 0;
    }
}

//////////////////
// PUBLIC CLASS //
//////////////////

// public constructor
DuiStyleContainer::DuiStyleContainer() :
    d_ptr(new DuiStyleContainerPrivate)
{
    d_ptr->q_ptr = this;
    DuiThemePrivate::registerStyleContainer(this);
}

// protected constructor
DuiStyleContainer::DuiStyleContainer(DuiStyleContainerPrivate *dd) :
    d_ptr(dd)
{
    d_ptr->q_ptr = this;
    DuiThemePrivate::registerStyleContainer(this);
}

// destructor
DuiStyleContainer::~DuiStyleContainer()
{
    DuiThemePrivate::unregisterStyleContainer(this);
    delete d_ptr;
}

void DuiStyleContainer::initialize(const QString &objectName, const QString &type, const DuiWidgetController *parent)
{
    // The style type should never be "default" - that would probably be a view type
    // that's mistakenly being used as a style type.
    // The caller probably means "" instead.
    Q_ASSERT(type != "default");

    d_ptr->objectName = objectName;
    d_ptr->type = type;
    d_ptr->parent = parent;

    reloadStyles();
}

// set the container objectName
void DuiStyleContainer::setObjectName(const QString &objectName)
{
    if (objectName != d_ptr->objectName) {
        d_ptr->objectName = objectName;
        reloadStyles();
    }
}

// set the container typeName
void DuiStyleContainer::setType(const QString &type)
{
    // The style type should never be "default" - that would probably be a view type
    // that's mistakenly being used as a style type.
    // The caller probably means "" instead.
    Q_ASSERT(type != "default");

    if (type != d_ptr->type) {
        d_ptr->type = type;
        reloadStyles();
    }
}
// returns the object name of the style container
QString DuiStyleContainer::objectName() const
{
    return d_ptr->objectName;
}

// returns the type name of the style container
QString DuiStyleContainer::type() const
{
    return d_ptr->type;
}

void DuiStyleContainer::setParent(const DuiWidgetController *parent)
{
    if (d_ptr->parent != parent) {
        d_ptr->parent = parent;
        reloadStyles();
    }
}

const DuiWidgetController *DuiStyleContainer::parent() const
{
    return d_ptr->parent;
}


// this method is for this class and derived classes to set the current style
// changes the pointer reference of current style
void DuiStyleContainer::setCurrentStyle(const DuiStyle*&, const DuiStyle*&)
{
}

// getter for derived classes
const DuiStyle *DuiStyleContainer::currentStyle() const
{
    Dui::Orientation orientation = Dui::Landscape;

    if (DuiApplication::activeWindow())
        orientation = DuiApplication::activeWindow()->orientation();

    if (d_ptr->cachedCurrentStyle[orientation] != 0) {
        return d_ptr->cachedCurrentStyle[orientation];
    }

    const DuiStyle* style;
    QHash<QString, const DuiStyle*>::iterator iter = d_ptr->cachedStyles[orientation].find(currentMode());
    if (iter != d_ptr->cachedStyles[orientation].end()) {
        style = *iter;
    } else {
        style = DuiTheme::style(styleType(), objectName(), currentMode(), type(), orientation, parent());
        d_ptr->cachedStyles[orientation].insert(currentMode(), style);
    }
    d_ptr->cachedCurrentStyle[orientation] = style;
    return style;
}

void DuiStyleContainer::setCurrentMode(const QString &mode)
{
    if (mode != d_ptr->currentMode) {
        d_ptr->cachedCurrentStyle[0] = 0;
        d_ptr->cachedCurrentStyle[1] = 0;
        d_ptr->currentMode = mode;
    }
}

QString DuiStyleContainer::currentMode()
{
    return d_ptr->currentMode;
}

QString DuiStyleContainer::currentMode() const
{
    return d_ptr->currentMode;
}

// returns pointer to the current style
const DuiStyle *DuiStyleContainer::operator->() const
{
    return currentStyle();
}

// virtual method which returns the name of the style in this container instance
const char *DuiStyleContainer::styleType() const
{
    return "DuiStyle";
}

// virtual method which will reload all styles used in this container instance
// this method will be called when e.g. objectName changes
void DuiStyleContainer::reloadStyles()
{
    d_ptr->releaseStyles();
}

// sets the current style to default
void DuiStyleContainer::setModeDefault()
{
    setCurrentMode("");
}
