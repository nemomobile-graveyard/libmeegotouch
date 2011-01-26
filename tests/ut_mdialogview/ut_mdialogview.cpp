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

#include <mdialogstyle.h>
#include <mlayout.h>
#include <mlinearlayoutpolicy.h>
#include <QGraphicsLinearLayout>
#include "mdialogview_p.h"
#include "mgridlayoutpolicy.h"
//#include "mbuttongrouplayoutpolicy.h"
#include "mbuttongrouplayoutpolicy_p.h"

#include <QGraphicsLayout>
#include "mlabel.h"

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
    MDialogStyle *dialogStyle = (MDialogStyle *)subject->style().operator->();

    dialogCloseButton = fetchWidget(*controller, "MDialogCloseButton");

    QVERIFY(dialogCloseButton != 0);

    dialogStyle->setHasCloseButton(true);
    subject->applyStyle();

    QCOMPARE(dialogCloseButton->isVisible(), true);

    dialogStyle->setHasCloseButton(false);
    subject->applyStyle();

    QCOMPARE(dialogCloseButton->isVisible(), false);
}

void Ut_MDialogView::titleBarVisibility()
{
    QGraphicsWidget *dialogTitleBar = 0;
    QGraphicsWidget *widget = 0;
    MDialogStyle *dialogStyle = (MDialogStyle *)subject->style().operator->();

    dialogTitleBar = fetchWidget(*controller, "MDialogTitleBar");

    QVERIFY(dialogTitleBar != 0);

    dialogStyle->setHasTitleBar(true);
    subject->applyStyle();

    QCOMPARE(dialogTitleBar->isVisible(), true);

    dialogStyle->setHasTitleBar(false);
    subject->applyStyle();

    QCOMPARE(dialogTitleBar->isVisible(), false);

    // title bar must also be removed from the dialog box layout,
    // otherwise it will still take space in the layout
    widget = fetchWidget(*controller, "MDialogTitleBar");
    QVERIFY(widget == 0);

    dialogStyle->setHasTitleBar(true);
    subject->applyStyle();

    QCOMPARE(dialogTitleBar->isVisible(), true);

    // title bar must be back to dialog's layout.
    widget = fetchWidget(*controller, "MDialogTitleBar");
    QVERIFY(widget == dialogTitleBar);
}

void Ut_MDialogView::spinnerVisibility()
{
    QGraphicsWidget *dialogSpinner = 0;
    QGraphicsWidget *titleBar = 0;

    dialogSpinner = fetchWidget(*controller, "MDialogProgressIndicator");

    // spinner shouldn't be created by default
    QVERIFY(dialogSpinner == 0);

    model->setProgressIndicatorVisible(true);

    dialogSpinner = fetchWidget(*controller, "MDialogProgressIndicator");

    // after changing visibility to true spinner should be
    // created and included in titleBar's layout
    QVERIFY(dialogSpinner != 0);
    titleBar = fetchWidget(*controller, "MDialogTitleBar");
    QVERIFY(titleBar != 0);
    MLayout *titleBarLayout = (MLayout*)titleBar->layout();
    QVERIFY(titleBarLayout->policy()->itemAt(0)==dialogSpinner);

    // after changing visibility to false spinner shouldn't be
    // present in titleBar layout
    model->setProgressIndicatorVisible(false);
    QVERIFY(titleBarLayout->policy()->itemAt(0)!=dialogSpinner);


}

void Ut_MDialogView::buttonBoxCentering()
{
    QGraphicsWidget *buttonBox = fetchWidget(*controller, "MDialogButtonBox");
    MDialogStyle *dialogStyle = (MDialogStyle *)subject->style().operator->();
    dialogStyle->setButtonBoxCentered(false);
    subject->applyStyle();
    QCOMPARE(buttonBox->layout()->count(), 1);
    dialogStyle->setButtonBoxCentered(true);
    dialogStyle->setDialogButtonFixedWidth(-1);
    subject->applyStyle();
    QCOMPARE(buttonBox->layout()->count(), 1);
    dialogStyle->setDialogButtonFixedWidth(40.0);
    subject->applyStyle();
    QCOMPARE(buttonBox->layout()->count(), 3);
    dialogStyle->setButtonBoxCentered(false);
    subject->applyStyle();
    QCOMPARE(buttonBox->layout()->count(), 1);
}

void Ut_MDialogView::verticalAlignment()
{
    MDialogStyle *dialogStyle = (MDialogStyle *)subject->style().operator->();
    dialogStyle->setDialogVerticalAlignment(dialogStyle->dialogVerticalAlignment() & !Qt::AlignVCenter);
    subject->applyStyle();
    QVERIFY(!subject->d_func()->bottomSpacer);
    dialogStyle->setDialogVerticalAlignment(dialogStyle->dialogVerticalAlignment() | Qt::AlignVCenter);
    subject->applyStyle();
    QVERIFY(subject->d_func()->bottomSpacer);
    dialogStyle->setDialogVerticalAlignment(dialogStyle->dialogVerticalAlignment() & !Qt::AlignVCenter);
    subject->applyStyle();
    QVERIFY(!subject->d_func()->bottomSpacer);
}

void Ut_MDialogView::buttonsFromModel()
{
    model->addButton(M::OkButton);
    model->addButton(M::CancelButton);
    QVERIFY(subject->d_func()->buttonBoxLayoutPolicy->count()==2);
    model->removeButton(model->button(M::CancelButton));
    QVERIFY(subject->d_func()->buttonBoxLayoutPolicy->count()==1);
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
