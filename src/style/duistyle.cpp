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
    currentMode("default")
{
    defaultStyle[Dui::Landscape] = NULL;
    defaultStyle[Dui::Portrait] = NULL;
    currentStyle[Dui::Landscape] = &defaultStyle[Dui::Landscape];
    currentStyle[Dui::Portrait] = &defaultStyle[Dui::Portrait];
    parent = NULL;
}

DuiStyleContainerPrivate::~DuiStyleContainerPrivate()
{
    DuiTheme::releaseStyle(defaultStyle[Dui::Landscape]);
    DuiTheme::releaseStyle(defaultStyle[Dui::Portrait]);
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
    if (!d_ptr->defaultStyle || objectName != d_ptr->objectName) {
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

    if (!d_ptr->defaultStyle || type != d_ptr->type) {
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
void DuiStyleContainer::setCurrentStyle(const DuiStyle*& landscapeStyle, const DuiStyle*& portraitStyle)
{
    Q_ASSERT(landscapeStyle);
    Q_ASSERT(portraitStyle);
    d_ptr->currentStyle[Dui::Landscape] = &landscapeStyle;
    d_ptr->currentStyle[Dui::Portrait] = &portraitStyle;
}

// getter for derived classes
const DuiStyle *DuiStyleContainer::currentStyle() const
{
    Dui::Orientation orientation = Dui::Landscape;

    if (DuiApplication::activeWindow())
        orientation = DuiApplication::activeWindow()->orientation();

    return *(d_ptr->currentStyle[orientation]);
}

void DuiStyleContainer::setCurrentMode(const QString &mode)
{
    d_ptr->currentMode = mode;
}

QString DuiStyleContainer::currentMode()
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
    DuiTheme::releaseStyle(d_ptr->defaultStyle[Dui::Landscape]);
    d_ptr->defaultStyle[Dui::Landscape] = NULL;

    DuiTheme::releaseStyle(d_ptr->defaultStyle[Dui::Portrait]);
    d_ptr->defaultStyle[Dui::Portrait] = NULL;

    if (d_ptr->currentMode == "default") {
        setModeDefault();
    }
}

// sets the current style to default
void DuiStyleContainer::setModeDefault()
{
    d_ptr->currentMode = "default";
    if (!d_ptr->defaultStyle[Dui::Landscape]) {
        const DuiStyle *tmp = d_ptr->defaultStyle[Dui::Landscape];
        d_ptr->defaultStyle[Dui::Landscape] = DuiTheme::style(styleType(), objectName(), "", type(), Dui::Landscape, parent());
        DuiTheme::releaseStyle(tmp);

    }
    if (!d_ptr->defaultStyle[Dui::Portrait]) {
        const DuiStyle *tmp = d_ptr->defaultStyle[Dui::Portrait];
        d_ptr->defaultStyle[Dui::Portrait] = DuiTheme::style(styleType(), objectName(), "", type(), Dui::Portrait, parent());
        DuiTheme::releaseStyle(tmp);
    }
    setCurrentStyle(d_ptr->defaultStyle[Dui::Landscape], d_ptr->defaultStyle[Dui::Portrait]);
}
