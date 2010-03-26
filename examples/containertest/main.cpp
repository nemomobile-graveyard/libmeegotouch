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

#include <QGraphicsLinearLayout>

#include <DuiApplication>
#include <DuiApplicationPage>
#include <DuiApplicationWindow>
#include <DuiContainer>
#include <DuiSeparator>
#include <DuiLabel>

#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiFreestyleLayoutPolicy>
#include <DuiFlowLayoutPolicy>


// create container with some content
DuiContainer *createApplet()
{
    static int count = 1;

    DuiContainer *c = new DuiContainer();
    DuiWidget *w = new DuiWidget();

    w->setObjectName("object");
    w->setMinimumSize(128, 128);

    c->setTitle(QString("Container %1").arg(count++));
    c->setCentralWidget(w);

    return c;
}

// pack stuff to qt layout
void pack_QT_layout(QGraphicsLinearLayout *layout, int items)
{
    for (int i = 0; i < items; ++i) {
        layout->addItem(createApplet());
    }
}

// helper
void separator(QGraphicsLinearLayout *layout, const QString &str = QString())
{
    layout->addItem(new DuiLabel(str));
    layout->addItem(new DuiSeparator());
}


int main(int argc, char **argv)
{

    // dui skeleton
    DuiApplication app(argc, argv);
    DuiApplicationWindow w;
    w.show();
    DuiApplicationPage p;
    p.appearNow();

    // main layout
    QGraphicsLinearLayout *main = new QGraphicsLinearLayout(Qt::Vertical, NULL);
    QGraphicsWidget *panel = p.centralWidget();
    panel->setLayout(main);

    // QT hbox
    separator(main, "Qt horizontal");
    QGraphicsLinearLayout *hbox = new QGraphicsLinearLayout(Qt::Horizontal, NULL);
    pack_QT_layout(hbox, 3);
    main->addItem(hbox);

    // DUI flow
    separator(main, "DUI flow");
    DuiLayout *dlayout3 = new DuiLayout();
    DuiFlowLayoutPolicy *flowpolicy = new DuiFlowLayoutPolicy(dlayout3);
    for (int i = 0; i < 6; ++i) flowpolicy->addItem(createApplet());
    main->addItem(dlayout3);

    // DUI hbox
    separator(main, "DUI horizontal");
    DuiLayout *dlayout1 = new DuiLayout();
    DuiLinearLayoutPolicy *lpolicy = new DuiLinearLayoutPolicy(dlayout1, Qt::Horizontal);
    for (int i = 0; i < 3; ++i) lpolicy->addItem(createApplet());
    main->addItem(dlayout1);

    // DUI freestyle
    separator(main, "DUI freestyle");
    DuiLayout *dlayout2 = new DuiLayout();
    DuiFreestyleLayoutPolicy *fpolicy = new DuiFreestyleLayoutPolicy(dlayout2);
    for (int i = 0; i < 3; ++i) fpolicy->addItem(createApplet());
    main->addItem(dlayout2);

    // DUI vbox
    separator(main, "DUI vertical");
    DuiLayout *dlayout4 = new DuiLayout();
    DuiLinearLayoutPolicy *vpolicy = new DuiLinearLayoutPolicy(dlayout4, Qt::Vertical);
    for (int i = 0; i < 3; ++i) vpolicy->addItem(createApplet());
    main->addItem(dlayout4);

    return app.exec();
}
