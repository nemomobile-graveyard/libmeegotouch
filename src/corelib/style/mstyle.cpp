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

#include "mdebug.h"
#include "mstyle.h"
#include "mstyle_p.h"
#include "mtheme.h"
#include "mtheme_p.h"
#include "mcomponentdata.h"
#include <mwindow.h>
#include <mscenemanager.h>

// TODO: get rid of this include, make MStyle not to use mgen
#include "gen_mstyledata.h"
#include <QMetaProperty>


///////////////////
// PRIVATE CLASS //
///////////////////
int MStyle::references() const
{
    return data->references;
}

int MStyle::addReference()
{
    data->references++;
    return data->references;
}

int MStyle::removeReference()
{
    --data->references;

    return data->references;
}

bool MStyle::isOrientationDependent() const
{
    return property("orientation_dependent").toBool();
}

void MStyle::setOrientationDependent(bool orientationDependent)
{
    // using a dymanic property is not the pretties thing on earth but
    // as we need to mainatin ABI compatibility we have no choice as
    // MStyle has not d-pointer
    setProperty("orientation_dependent", orientationDependent);
}

MStyleContainerPrivate::MStyleContainerPrivate() :
    currentMode("")
{
    cachedCurrentStyle[0] = 0;
    cachedCurrentStyle[1] = 0;
    activeStyle = 0;
    parent = NULL;
    q_ptr = NULL;
    cachedOrientationIndependentStyle = 0;
}

MStyleContainerPrivate::~MStyleContainerPrivate()
{
    releaseStyles();
}

void MStyleContainerPrivate::releaseStyles()
{
    for (int i = 0; i < 2; ++i) {
        QHashIterator<QString, const MStyle*> iter(cachedStyles[i]);
        while (iter.hasNext()) {
            iter.next();
            MTheme::releaseStyle(iter.value());
        }
        cachedStyles[i].clear();
        cachedCurrentStyle[i] = 0;
    }
    cachedOrientationIndependentStyle = 0;
    activeStyle = 0;
}

//////////////////
// PUBLIC CLASS //
//////////////////

// public constructor
MStyleContainer::MStyleContainer() :
    d_ptr(new MStyleContainerPrivate)
{
    d_ptr->q_ptr = this;
    MThemePrivate::registerStyleContainer(this);
}

// protected constructor
MStyleContainer::MStyleContainer(MStyleContainerPrivate *dd) :
    d_ptr(dd)
{
    d_ptr->q_ptr = this;
    MThemePrivate::registerStyleContainer(this);
}

// destructor
MStyleContainer::~MStyleContainer()
{
    MThemePrivate::unregisterStyleContainer(this);
    delete d_ptr;
}

void MStyleContainer::initialize(const QString &objectName, const QString &type, const MWidgetController *parent)
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
void MStyleContainer::setObjectName(const QString &objectName)
{
    if (objectName != d_ptr->objectName) {
        d_ptr->objectName = objectName;
        reloadStyles();
    }
}

// set the container typeName
void MStyleContainer::setType(const QString &type)
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
QString MStyleContainer::objectName() const
{
    return d_ptr->objectName;
}

// returns the type name of the style container
QString MStyleContainer::type() const
{
    return d_ptr->type;
}

void MStyleContainer::setParent(const MWidgetController *parent)
{
    if (d_ptr->parent != parent) {
        d_ptr->parent = parent;
        reloadStyles();
    }
}

const MWidgetController *MStyleContainer::parent() const
{
    return d_ptr->parent;
}

void MStyleContainer::setSceneManager(MSceneManager *sceneManager)
{
    d_ptr->sceneManager = sceneManager;
}

const MSceneManager *MStyleContainer::sceneManager() const
{
    return d_ptr->sceneManager.data();
}

// getter for derived classes
const MStyle *MStyleContainer::currentStyle() const
{
    if (!d_ptr->activeStyle) {
        updateCurrentStyle();
    }
    return d_ptr->activeStyle;
}

void MStyleContainer::updateCurrentStyle() const
{
    if (d_ptr->cachedOrientationIndependentStyle) {
        d_ptr->activeStyle = d_ptr->cachedOrientationIndependentStyle;
        return;
    }
    M::Orientation orientation = M::Landscape;

    if (!d_ptr->sceneManager.isNull()) {
        orientation = d_ptr->sceneManager.data()->orientation();
    } else {
        const MWindow *activeWindow = MComponentData::activeWindow();
        if (activeWindow)
            orientation = activeWindow->orientation();
    }

    if (d_ptr->cachedCurrentStyle[orientation] != 0) {
        d_ptr->activeStyle = d_ptr->cachedCurrentStyle[orientation];
        return;
    }

    const MStyle* style = 0;
    QHash<QString, const MStyle*>::iterator iter = d_ptr->cachedStyles[orientation].find(currentMode());
    if (iter != d_ptr->cachedStyles[orientation].end()) {
        style = *iter;
    } else {
        if (orientation == M::Portrait) {
            // check if we have an orientation independent style
            iter = d_ptr->cachedStyles[M::Landscape].find(currentMode());
            if (iter != d_ptr->cachedStyles[M::Landscape].end()) {
                if (!(*iter)->isOrientationDependent()) {
                    style = *iter;
                }
            }
        }
        if (!style) {
            style = MTheme::style(styleType(), objectName(), currentMode(), type(), orientation, parent());
            if (style->isOrientationDependent()) {
                d_ptr->cachedStyles[orientation].insert(currentMode(), style);
            } else {
                d_ptr->cachedStyles[M::Landscape].insert(currentMode(), style);
            }
        }
    }

    if (style->isOrientationDependent()) {
        d_ptr->cachedCurrentStyle[orientation] = style;
    } else {
        d_ptr->cachedOrientationIndependentStyle = style;
        // orientation independent styles are just saved in landscape mode but
        // resused if we request the same style in portrait
        d_ptr->cachedCurrentStyle[M::Landscape] = style;
    }
    d_ptr->activeStyle = style;
}

void MStyleContainer::setCurrentMode(const QString &mode)
{
    if (mode != d_ptr->currentMode) {
        d_ptr->cachedCurrentStyle[0] = 0;
        d_ptr->cachedCurrentStyle[1] = 0;
        d_ptr->currentMode = mode;
        d_ptr->cachedOrientationIndependentStyle = 0;
        d_ptr->activeStyle = 0;
    }
}

QString MStyleContainer::currentMode() const
{
    return d_ptr->currentMode;
}

// virtual method which returns the name of the style in this container instance
const char *MStyleContainer::styleType() const
{
    return "MStyle";
}

// virtual method which will reload all styles used in this container instance
// this method will be called when e.g. objectName changes
void MStyleContainer::reloadStyles()
{
    d_ptr->releaseStyles();
}

// sets the current style to default
void MStyleContainer::setModeDefault()
{
    setCurrentMode("");
}

MStyleContainer::dummyNeverToBeUsedPtr MStyleContainer::_dummyNeverToBeUsed()
{
    return &MStyleContainer::operator->;
}
