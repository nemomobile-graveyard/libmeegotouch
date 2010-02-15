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

#include <QMetaType>
#include <DuiApplication>

#include <DuiModalSceneWindow>
#include <DuiSceneWindow>

#include <DuiButton>
#include <QGraphicsLinearLayout>

#include "ut_duimodalscenewindow.h"

void Ut_DuiModalSceneWindow::init()
{
    app = buildApp(1, "./ut_duimodalscenewindow");
    modalscenewindow = new DuiModalSceneWindow();
}

void Ut_DuiModalSceneWindow::cleanup()
{
    delete modalscenewindow;
    delete app;
}

void Ut_DuiModalSceneWindow::populate()
{
    DuiButton *button1 = new DuiButton();
    DuiButton *button2 = new DuiButton();

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout();
    layout->addItem(button1);
    layout->addItem(button2);
    modalscenewindow->setLayout(layout);

    modalscenewindow->appearNow();
    modalscenewindow->disappearNow();
}

DuiApplication *Ut_DuiModalSceneWindow::buildApp(int count, const QString &params)
{
    QChar sep(' ');
    char *argv[MAX_PARAMS];
    int x = 0;

    QStringList list = params.split(sep);
    QStringListIterator it(list);
    while (it.hasNext() && x < MAX_PARAMS)  {
        argv[x++] = strdup(it.next().toLocal8Bit().constData());
    }
    return new DuiApplication(count, argv);
}

QTEST_APPLESS_MAIN(Ut_DuiModalSceneWindow);
