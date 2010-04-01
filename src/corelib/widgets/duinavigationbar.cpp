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

#include <DuiDebug>
#include "duinavigationbar.h"
#include "duinavigationbar_p.h"

#include "duinavigationbarmodel.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiNavigationBar)


/*
 * constructor of private class
 */
DuiNavigationBarPrivate::DuiNavigationBarPrivate()
    : homeButtonPanel(0)
{
}

/*
 * destructor of private class
 */
DuiNavigationBarPrivate::~DuiNavigationBarPrivate()
{
}

/*
 * general initialization function
 */
void DuiNavigationBarPrivate::init()
{
}

/*
 * constructor of public class
 */
DuiNavigationBar::DuiNavigationBar()
    : DuiSceneWindow(new DuiNavigationBarPrivate, new DuiNavigationBarModel, DuiSceneWindow::NavigationBar)
{
    Q_D(DuiNavigationBar);
    d->init();
}

DuiNavigationBar::~DuiNavigationBar()
{
}

QString DuiNavigationBar::viewMenuIconID() const
{
    return model()->viewMenuIconID();
}

bool DuiNavigationBar::isProgressIndicatorVisible() const
{
    return model()->progressIndicatorVisible();
}

bool DuiNavigationBar::isArrowIconVisible() const
{
    return model()->arrowIconVisible();
}

void DuiNavigationBar::setViewMenuIconID(const QString &id)
{
    model()->setViewMenuIconID(id);
}

void DuiNavigationBar::setProgressIndicatorVisible(bool visible)
{
    model()->setProgressIndicatorVisible(visible);
}

void DuiNavigationBar::setArrowIconVisible(bool visible)
{
    model()->setArrowIconVisible(visible);
}

void DuiNavigationBar::notifyUser()
{
    model()->setNotifyUser(true);
}

void DuiNavigationBar::setViewMenuDescription(const QString &text)
{
    model()->setViewMenuDescription(text);
}

void DuiNavigationBar::dockToolBar(DuiToolBar *toolbar)
{
    model()->setToolBar(toolbar);
}

void DuiNavigationBar::undockToolBar()
{
    dockToolBar(NULL);
}

#include "moc_duinavigationbar.cpp"
