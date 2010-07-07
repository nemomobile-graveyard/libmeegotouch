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

#ifndef MDIALOGVIEW_P_H
#define MDIALOGVIEW_P_H

#include "mdialogview.h"
#include <mnamespace.h>
#include <QPointer>

class MDialog;
class MDialogPrivate;
class MButton;
class MLabel;
class MPannableViewport;
class MProgressIndicator;
class MLayout;
class MButtonGroupLayoutPolicy;

/*
Widget hierarchy:
==================

MDialog (MSceneWindow), rootLayout
 |
 |- top spacer
 |
 |- horizontal widget
 |      |
 |      |- left spacer
 |      |
 |      |- dialogBox (the actual graphical dialog)
 |      |      |
 |      |      |- title bar
 |      |      |     |
 |      |      |     |- spinner
 |      |      |     |
 |      |      |     |- title label
 |      |      |     |
 |      |      |     |- close button
 |      |      |
 |      |      |- contents viewport (a pannable viewport)
 |      |            |
 |      |            |- contents (inside a pannable viewport)
 |      |                  |
 |      |                  |- central widget
 |      |                  |
 |      |                  |- button box
 |      |
 |      | - right spacer
 |
 |- bottom spacer (only when dialog-vertical-alignment = center)
*/

class MDialogViewPrivate
{
    Q_DECLARE_PUBLIC(MDialogView)

protected:
    MDialogView *q_ptr;

public:
    MDialogViewPrivate();
    virtual ~MDialogViewPrivate();

    void updateWidgetVisibility(QGraphicsWidget *widget, bool visible, int index,
                                QGraphicsLinearLayout *layout);
    void setupDialogVerticalAlignment();

    void createWidgetHierarchy();
    void createHorizontalLayout();
    void createTitleBar();
    void createButtonBox();
    void createDialogBox();
    static QGraphicsWidget *createSpacer();
    static QGraphicsLinearLayout *createLayout(Qt::Orientation orientation);
    void repopulateButtonBox();
    static int buttonOrder(M::ButtonRole role);
    void removeButtonsNotInDialogModel();
    void addButtonsFromDialogModel();
    bool isButtonInButtonBox(MButtonModel *buttonModel);
    void setCentralWidget(QGraphicsWidget *newCentralWidget);
    void addButton(MButtonModel *buttonModel);
    void updateButtonBox();
    void setSpinnerVisibility(bool visibility);
    void _q_updatePanning();

    MDialog *controller;

    // Layout applied to the controller itself.
    QGraphicsLinearLayout *rootLayout;

    QGraphicsWidget *topSpacer;
    QGraphicsWidget *bottomSpacer;
    QGraphicsWidget *leftSpacer;
    QGraphicsWidget *rightSpacer;

    QGraphicsWidget *horizontalWidget;
    QGraphicsLinearLayout *horizontalLayout;

    // The dialog box itself.
    // Contains a title bar and a pannable viewport for the dialog's contents
    QGraphicsWidget *dialogBox;
    QGraphicsLinearLayout *dialogBoxLayout;

    // Widget that contains the contents of the dialog box.
    // It has the central widget and the button box.
    // Classes inheriting from MDialog may add new items to it.
    // We expect that specialized classes don't remove our items from
    // it by themselves.
    QGraphicsWidget *contents;
    QGraphicsLinearLayout *contentsLayout;

    MPannableViewport *contentsViewport;

    QPointer<QGraphicsWidget> centralWidget;

    MWidget *titleBar;
    MLabel *titleLabel;
    MProgressIndicator *spinner;
    MButton *closeButton;

    QGraphicsWidget *buttonBox;
    MLayout *buttonBoxLayout;
    MButtonGroupLayoutPolicy *buttonBoxLayoutPolicy;
};

#endif

