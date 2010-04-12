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

#include "ut_mdialogview.h"

#include <QGraphicsLayout>

void Ut_MDialogView::init()
{
}

void Ut_MDialogView::cleanup()
{
}

void Ut_MDialogView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mdialogview" };
    app = new MApplication(argc, app_name);

    controller = new MDialog();
    subject = new MDialogView(controller);
    controller->setView(subject);
    model = controller->model();
}

void Ut_MDialogView::cleanupTestCase()
{
    delete controller;
    controller = 0;

    delete app;
    app = 0;
}

void Ut_MDialogView::closeButtonVisibility()
{
    QGraphicsWidget *dialogCloseButton;

    dialogCloseButton = fetchWidget(*controller, "MDialogCloseButton");

    QVERIFY(dialogCloseButton != 0);

    QCOMPARE(dialogCloseButton->isVisible(), true);

    model->setCloseButtonVisible(false);

    QCOMPARE(dialogCloseButton->isVisible(), false);

    model->setCloseButtonVisible(true);

    QCOMPARE(dialogCloseButton->isVisible(), true);
}

void Ut_MDialogView::titleBarVisibility()
{
    QGraphicsWidget *dialogTitleBar = 0;
    QGraphicsWidget *widget = 0;

    dialogTitleBar = fetchWidget(*controller, "MDialogTitleBar");

    QVERIFY(dialogTitleBar != 0);

    QCOMPARE(dialogTitleBar->isVisible(), true);

    model->setTitleBarVisible(false);

    QCOMPARE(dialogTitleBar->isVisible(), false);

    // title bar must also be removed from the dialog box layout,
    // otherwise it will still take space in the layout
    widget = fetchWidget(*controller, "MDialogTitleBar");
    QVERIFY(widget == 0);

    model->setTitleBarVisible(true);

    QCOMPARE(dialogTitleBar->isVisible(), true);

    // title bar must be back to dialog's layout.
    widget = fetchWidget(*controller, "MDialogTitleBar");
    QVERIFY(widget == dialogTitleBar);
}

QGraphicsWidget *Ut_MDialogView::fetchWidget(QGraphicsWidget &widget,
        const QString &objectName) const
{
    QGraphicsWidget *targetWidget = 0;

    if (widget.objectName() == objectName) {
        targetWidget = &widget;
    } else if (widget.layout() != 0) {
        targetWidget = fetchWidget(*(widget.layout()), objectName);
    }

    return targetWidget;
}

QGraphicsWidget *Ut_MDialogView::fetchWidget(QGraphicsLayout &layout,
        const QString &objectName) const
{
    QGraphicsWidget *targetWidget = 0;
    QGraphicsLayoutItem *layoutItem;
    QGraphicsItem *item;

    for (int i = 0; i < layout.count() && targetWidget == 0; i++) {
        layoutItem = layout.itemAt(i);

        if (layoutItem->isLayout()) {
            targetWidget = fetchWidget(*static_cast<QGraphicsLayout *>(layoutItem), objectName);
        } else {
            item = layoutItem->graphicsItem();

            if (item->isWidget()) {
                QGraphicsWidget *widget = static_cast<QGraphicsWidget *>(item);
                targetWidget = fetchWidget(*widget, objectName);
            }
        }
    }

    return targetWidget;
}

QTEST_APPLESS_MAIN(Ut_MDialogView)
