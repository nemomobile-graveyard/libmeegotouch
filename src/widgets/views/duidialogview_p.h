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

#ifndef DUIDIALOGVIEW_P_H
#define DUIDIALOGVIEW_P_H

#include "duidialogview.h"
#include "duiscenewindowview_p.h"
#include <duinamespace.h>
#include <QPointer>

class DuiDialog;
class DuiDialogPrivate;
class DuiButton;
class DuiLabel;
class DuiPannableViewport;
class DuiProgressIndicator;
class DuiLayout;
class DuiButtonGroupLayoutPolicy;

/*
Widget hierarchy:
==================

DuiDialog (DuiSceneWindow), rootLayout
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

class DuiDialogViewPrivate : public DuiSceneWindowViewPrivate
{
    Q_DECLARE_PUBLIC(DuiDialogView)

public:
    DuiDialogViewPrivate();
    virtual ~DuiDialogViewPrivate();

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
    static int stdButtonOrder(Dui::StandardButton buttonType);
    void removeButtonsNotInDialogModel();
    void addButtonsFromDialogModel();
    bool isButtonInButtonBox(DuiButtonModel *buttonModel);
    void setCentralWidget(DuiWidget *newCentralWidget);
    void addButton(DuiButtonModel *buttonModel);
    void updateButtonBox();

    DuiDialog *controller;

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
    // Classes inheriting from DuiDialog may add new items to it.
    // We expect that specialized classes don't remove our items from
    // it by themselves.
    QGraphicsWidget *contents;
    QGraphicsLinearLayout *contentsLayout;

    DuiPannableViewport *contentsViewport;

    QPointer<DuiWidget> centralWidget;

    DuiWidget *titleBar;
    DuiLabel *titleLabel;
    DuiProgressIndicator *spinner;
    DuiButton *closeButton;

    QGraphicsWidget *buttonBox;
    DuiLayout *buttonBoxLayout;
    DuiButtonGroupLayoutPolicy *buttonBoxLayoutPolicy;
};

#endif

