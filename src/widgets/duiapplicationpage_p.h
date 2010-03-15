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

#ifndef DUIAPPLICATIONPAGE_P_H
#define DUIAPPLICATIONPAGE_P_H

#include <duiapplicationpage.h>
#include <duiscenewindow_p.h>
#include <QList>
#include <duinamespace.h>

class DuiScene;
class DuiAction;
class DuiWidget;
class DuiToolBar;
class DuiPannableViewport;
class QGraphicsLinearLayout;
class DuiApplicationWindow;
class DuiApplicationWindowPrivate;

class DuiApplicationPagePrivate : public DuiSceneWindowPrivate
{
    Q_DECLARE_PUBLIC(DuiApplicationPage)

public:

    DuiApplicationPagePrivate();

    void init();

    void deleteCurrentCentralWidget();
    void placeCentralWidget(DuiWidget *widget);
    void setSpacerHeight(DuiWidget *spacer, qreal height);
    void updatePannableViewportPosition();

    void propagateOnDisplayChangeEvent(bool visible);

    // Note: Called by DuiApplicationWindow
    void updateAutoMarginsForComponents(const Dui::Orientation &orientation,
                                        qreal navigationBarHeight,
                                        qreal dockWidgetHeight,
                                        bool dockWidgetVisible);

    // FIXME: After API freeze move that code to enterDisplayEvent().
    // Didn't enterDisplayEvent() make createContent() obsolete/redundant altogether?
    // Note: Called by DuiApplicationWindow
    void prepareForAppearance();

private:
    QGraphicsLinearLayout *createLayout();
    DuiWidget *createSpacer(QGraphicsItem *parent);
    QRectF range;

public:
    DuiEscapeButtonPanelModel::EscapeMode escapeButtonMode;
    bool rememberPosition;
    DuiWidget *topSpacer;
    DuiWidget *bottomSpacer;
    DuiWidget *mainWidget;
    QGraphicsLinearLayout *mainLayout;
    DuiPannableViewport *pannableViewPort;
    DuiWidget *centralWidget;

    QString title;
    bool contentCreated;
    bool backEnabled;
};

#endif
