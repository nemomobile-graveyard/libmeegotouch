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

#include <MDebug>
#include "mnavigationbar.h"
#include "mnavigationbar_p.h"

#include "mnavigationbarmodel.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MNavigationBar)


/*
 * constructor of private class
 */
MNavigationBarPrivate::MNavigationBarPrivate()
    : homeButtonPanel(0)
{
}

/*
 * destructor of private class
 */
MNavigationBarPrivate::~MNavigationBarPrivate()
{
}

/*
 * general initialization function
 */
void MNavigationBarPrivate::init()
{
}

/*
 * constructor of public class
 */
MNavigationBar::MNavigationBar()
    : MSceneWindow(new MNavigationBarPrivate, new MNavigationBarModel, MSceneWindow::NavigationBar)
{
    Q_D(MNavigationBar);
    d->init();
}

MNavigationBar::MNavigationBar(QGraphicsItem *parent)
    : MSceneWindow(new MNavigationBarPrivate, new MNavigationBarModel, MSceneWindow::NavigationBar, QString(), parent)
{
    Q_D(MNavigationBar);
    d->init();
}

MNavigationBar::~MNavigationBar()
{
}

QString MNavigationBar::viewMenuIconID() const
{
    return model()->viewMenuIconID();
}

bool MNavigationBar::isProgressIndicatorVisible() const
{
    return model()->progressIndicatorVisible();
}

bool MNavigationBar::isArrowIconVisible() const
{
    return model()->arrowIconVisible();
}

void MNavigationBar::setViewMenuIconID(const QString &id)
{
    model()->setViewMenuIconID(id);
}

void MNavigationBar::setProgressIndicatorVisible(bool visible)
{
    model()->setProgressIndicatorVisible(visible);
}

void MNavigationBar::setArrowIconVisible(bool visible)
{
    model()->setArrowIconVisible(visible);
}

void MNavigationBar::notifyUser()
{
    model()->setNotifyUser(true);
}

void MNavigationBar::setViewMenuDescription(const QString &text)
{
    model()->setViewMenuDescription(text);
}

void MNavigationBar::dockToolBar(MToolBar *toolbar)
{
    model()->setToolBar(toolbar);
}

void MNavigationBar::undockToolBar()
{
    dockToolBar(NULL);
}

MNavigationBarModel::EscapeButtonModeEnum MNavigationBar::escapeButtonMode() const
{
    return model()->escapeButtonMode();
}

void MNavigationBar::setEscapeButtonMode(MNavigationBarModel::EscapeButtonModeEnum newMode)
{
    model()->setEscapeButtonMode(newMode);
}

bool MNavigationBar::escapeButtonEnabled() const
{
    return model()->escapeButtonEnabled();
}

void MNavigationBar::setEscapeButtonEnabled(bool enabled)
{
    model()->setEscapeButtonEnabled(enabled);
}

bool MNavigationBar::escapeButtonVisible() const
{
    return model()->escapeButtonVisible();
}

void  MNavigationBar::setEscapeButtonVisible(bool visible)
{
    model()->setEscapeButtonVisible(visible);
}

void MNavigationBar::setCustomContent(QGraphicsWidget *customContent)
{
    model()->setCustomContent(customContent);
}

QGraphicsWidget *MNavigationBar::customContent()
{
    return model()->customContent();
}

#include "moc_mnavigationbar.cpp"
