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

#include <QObject>
#include <QGraphicsLinearLayout>

#include <MTheme>
#include <MLabel>
#include <MButton>

#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>

#include <QStaticText>

#include "pt_mwidgetcontroller.h"

#define MAX_WIDGETS 1000

Pt_MWidgetControllerPage::Pt_MWidgetControllerPage()
{
    connect(this, SIGNAL(appeared()), this, SLOT(pageAppeared()));
}

Pt_MWidgetControllerPage::~Pt_MWidgetControllerPage()
{
}

void Pt_MWidgetControllerPage::pageAppeared()
{
    Pt_MWidgetController test;
    exit(QTest::qExec(&test, qApp->argc(), qApp->argv()));
}

void Pt_MWidgetController::initTestCase()
{

}

void Pt_MWidgetController::cleanupTestCase()
{

}

void Pt_MWidgetController::init()
{
}

void Pt_MWidgetController::cleanup()
{
    QList<QGraphicsItem *> items = MApplication::instance()->activeApplicationWindow()->currentPage()->centralWidget()->childItems();
    while (items.count() > 0)
        delete items.takeLast();
}

void Pt_MWidgetController::test_qGraphicsWidgets()
{
    QBENCHMARK {
        for (int i=0; i<MAX_WIDGETS;i++) {
            QGraphicsWidget *widget = new QGraphicsWidget(MApplication::instance()->activeApplicationWindow()->currentPage()->centralWidget());
            widget->effectiveSizeHint(Qt::PreferredSize);
        }
    }
}

void Pt_MWidgetController::test_mWidgetController()
{
    QBENCHMARK {
        for (int i=0; i<MAX_WIDGETS;i++) {
            MWidgetController *widget = new MWidgetController(MApplication::instance()->activeApplicationWindow()->currentPage()->centralWidget());
            widget->effectiveSizeHint(Qt::PreferredSize);
        }
     }
}

void Pt_MWidgetController::test_mLabel()
{
    QBENCHMARK {
        for (int i=0; i<MAX_WIDGETS;i++) {
            MLabel *widget = new MLabel(MApplication::instance()->activeApplicationWindow()->currentPage()->centralWidget());
            widget->setText("Text");
            widget->effectiveSizeHint(Qt::PreferredSize);
        }
     }
}

void Pt_MWidgetController::test_mButton()
{
    QBENCHMARK {
        for (int i=0; i<MAX_WIDGETS;i++) {
            MButton *widget = new MButton(MApplication::instance()->activeApplicationWindow()->currentPage()->centralWidget());
            widget->setText("Text");
            widget->effectiveSizeHint(Qt::PreferredSize);
        }
     }
}

int main(int argc, char *argv[])
{
    MApplication app(argc, argv);

    MApplicationWindow window;
    window.show();

    Pt_MWidgetControllerPage page;
    page.appear(&window);

    return app.exec();
}
