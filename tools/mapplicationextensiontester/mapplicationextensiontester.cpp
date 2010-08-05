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

/*
 * Application extension testing tool.
 */

#include "mapplicationextensiontester.h"

#include <QDebug>
#include <getopt.h>
#include <QString>
#include <QGraphicsLinearLayout>
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MApplicationExtensionArea>
#include <MMessageBox>
#include <QDir>
#include <QFile>
#include <QFileInfo>


ApplicationExtensionPage::ApplicationExtensionPage(const QString &interface) :
    interface(interface)
{
}

void ApplicationExtensionPage::createContent()
{
    QGraphicsWidget *panel = centralWidget();
    QGraphicsLinearLayout *vbox = new QGraphicsLinearLayout(Qt::Vertical);
    panel->setLayout(vbox);

    MApplicationExtensionArea *extArea = new MApplicationExtensionArea(interface);
    vbox->addItem(extArea);
}

void usage(const char *progName)
{
    qWarning("Usage: %s INTERFACE", progName);
    qWarning("  INTERFACE                          Defines which interface the extensions to be loaded must implement");
}

int main(int argc, char **argv)
{
    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            { "help", 0, NULL, 'h' },  /* --help or -h  */
            { 0,    0,   0,   0 }
        };
        int c = getopt_long_only(argc, argv, ":h", long_options, &option_index);
        if (c == -1)
            break;
        switch (c) {
        case 'h':
            usage(argv[0]);
            return -1;
        default:
            break;
        }
    }

    if (optind >= argc || QString(argv[optind]).isEmpty()) {
        usage(argv[0]);
        return -1;
    }

    MApplication m_app(argc, argv);
    MApplicationWindow window;
    ApplicationExtensionPage page(argv[optind]);
    window.show();
    page.appear(&window);

    return m_app.exec();
}
