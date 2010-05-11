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
 * A helper applet runner tool.
 */

#include "mapplettester.h"

#include <MAppletMetaData>
#include <mappletid.h>
#include <QDebug>
#include <getopt.h>
#include <QString>
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MMashupCanvas>

void usage(const char *progName)
{
    qWarning("Usage: %s -i [OPTION] [METADATAFILENAME]", progName);
    qWarning("  -i, --instanceId=ID                       Defines the instance id of the applet");
    qWarning("  metadatafilename                   Defines the metadata file of the applet");
}

int main(int argc, char **argv)
{
    MAppletId::AppletInstanceID instanceId = 0;
    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            { "instanceid", 1, NULL, 'i'   },      /* --instanceid or -i  */
            { "help", 0, NULL, 'h' },  /* --help or -h  */
            { 0,    0,   0,   0 }
        };
        int c = getopt_long_only(argc, argv, "i:h", long_options, &option_index);
        if (c == -1)
            break;
        switch (c) {
        case 'i':
            instanceId = atoi(optarg);
            break;
        case 'h':
            usage(argv[0]);
            return -1;
        default:
            break;
        }
    }

    if (optind >= argc || QString(argv[optind]).isEmpty()) {
        // No arguments: use the mashup canvas mode
        MApplication application(argc, argv);
        MApplicationWindow applicationWindow;
        MApplicationPage *applicationPage = new MApplicationPage;
        applicationPage->setComponentsDisplayMode(MApplicationPage::AllComponents, MApplicationPageModel::Hide);
        applicationPage->setCentralWidget(new MMashupCanvas("appletcanvas"));
        applicationPage->appear(&applicationWindow);
        applicationWindow.show();
        return application.exec();
    } else {
        // Arguments given: load a single applet
        QString metaDataFileName = argv[optind];
        MAppletMetaData metadata(metaDataFileName);
        MApplication m_app(argc, argv, metadata.resourceIdentifier());
        MAppletId appletId("mapplettester", "default", instanceId);
        if (!metadata.isValid()) {
            qWarning() << "Applet metadata file" << metaDataFileName << "is not valid";
            return -1;
        }
        MAppletTester tester;
        if (!tester.init(metadata, appletId)) {
            qWarning() << "Applet initialization failed";
            return -1;
        }

        return tester.exec();
    }
}
