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

#include <QGraphicsLinearLayout>

#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MContainer>
#include <MSeparator>
#include <MLabel>

#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MFreestyleLayoutPolicy>
#include <MFlowLayoutPolicy>


// create container with some content
MContainer *createApplet()
{
    static int count = 1;

    MContainer *c = new MContainer();
    MWidget *w = new MWidget();

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
    layout->addItem(new MLabel(str));
    layout->addItem(new MSeparator());
}


int main(int argc, char **argv)
{

    // m skeleton
    MApplication app(argc, argv);
    MApplicationWindow w;
    w.show();
    MApplicationPage p;
    p.appear(&w);

    // main layout
    QGraphicsLinearLayout *main = new QGraphicsLinearLayout(Qt::Vertical, NULL);
    QGraphicsWidget *panel = p.centralWidget();
    panel->setLayout(main);

    // QT hbox
    separator(main, "Qt horizontal");
    QGraphicsLinearLayout *hbox = new QGraphicsLinearLayout(Qt::Horizontal, NULL);
    pack_QT_layout(hbox, 3);
    main->addItem(hbox);

    // M flow
    separator(main, "M flow");
    MLayout *dlayout3 = new MLayout();
    MFlowLayoutPolicy *flowpolicy = new MFlowLayoutPolicy(dlayout3);
    for (int i = 0; i < 6; ++i) flowpolicy->addItem(createApplet());
    main->addItem(dlayout3);

    // M hbox
    separator(main, "M horizontal");
    MLayout *dlayout1 = new MLayout();
    MLinearLayoutPolicy *lpolicy = new MLinearLayoutPolicy(dlayout1, Qt::Horizontal);
    for (int i = 0; i < 3; ++i) lpolicy->addItem(createApplet());
    main->addItem(dlayout1);

    // M freestyle
    separator(main, "M freestyle");
    MLayout *dlayout2 = new MLayout();
    MFreestyleLayoutPolicy *fpolicy = new MFreestyleLayoutPolicy(dlayout2);
    for (int i = 0; i < 3; ++i) fpolicy->addItem(createApplet());
    main->addItem(dlayout2);

    // M vbox
    separator(main, "M vertical");
    MLayout *dlayout4 = new MLayout();
    MLinearLayoutPolicy *vpolicy = new MLinearLayoutPolicy(dlayout4, Qt::Vertical);
    for (int i = 0; i < 3; ++i) vpolicy->addItem(createApplet());
    main->addItem(dlayout4);

    return app.exec();
}
