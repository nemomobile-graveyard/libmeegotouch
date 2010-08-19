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

#ifndef MAPPLICATIONPAGE_P_H
#define MAPPLICATIONPAGE_P_H

#include <mapplicationpage.h>
#include <mscenewindow_p.h>
#include <QList>
#include <QRect>
#include <mnamespace.h>

class MScene;
class MAction;
class MWidget;
class MToolBar;
class MPannableViewport;
class QGraphicsLinearLayout;
class MApplicationWindow;
class MApplicationWindowPrivate;

class MApplicationPagePrivate : public MSceneWindowPrivate
{
    Q_DECLARE_PUBLIC(MApplicationPage)

public:

    MApplicationPagePrivate();

    void init();

    virtual void doEnterDisplayEvent();

    void deleteCurrentCentralWidget();
    void placeCentralWidget(QGraphicsWidget *widget);
    void setWidgetHeight(MWidget *w, qreal height);
    void updatePannableViewportPosition();

    void propagateOnDisplayChangeEvent(bool visible);

    // Note: Called by MApplicationWindow
    void setExposedContentRect(const QRectF &rect);
    void updateAutoMarginsForComponents();

private:
    QGraphicsLinearLayout *createLayout();
    MWidget *createSpacer(QGraphicsItem *parent);
    QRectF range;

public:
    bool rememberPosition;
    MWidget *topSpacer;
    MWidget *bottomSpacer;
    MWidget *mainWidget;
    QGraphicsLinearLayout *mainLayout;
    MPannableViewport *pannableViewPort;
    QGraphicsWidget *centralWidget;
    QRectF exposedContentRect;

    bool contentCreated;
    bool backEnabled;
};

#endif
