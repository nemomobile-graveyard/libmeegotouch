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

#include <QtGlobal>
#include <QString>
#include <QDebug>
#include <QDir>

#include "mgen_processor.h"

int main(int argc, const char *argv[])
{
    if (argc != 4 && argc != 3) {
        qWarning() << "usage: mgen option header [outdir]";
        qWarning() << " option:";
        qWarning() << "   --model";
        qWarning() << "   --style";
        qWarning() << "Examples:";
        qWarning() << "mgen --model mbuttonmodel.h ";
        qWarning() << "mgen --model mbuttonmodel.h gen_models/";
        return 1;
    }

    HeaderType type;
    if (QString(argv[1]) == QString("--model")) {
        type = Model;
    } else if (QString(argv[1]) == QString("--style")) {
        type = Style;
    } else {
        qWarning() << "unknown option \"" << argv[1] << "\"";
        return 1;
    }

    QString outdir;

    if (argc == 4) {
        outdir = argv[3];
    }

    if (!QDir(outdir).exists()) {
        QDir::current().mkdir(outdir);
    }

    if (type == Style) {
        parseAndGenerateStyleData(argv[2], outdir);
    } else if (type == Model) {
        parseAndGenerateModelData(argv[2], outdir);
    }

    return 0;
}
