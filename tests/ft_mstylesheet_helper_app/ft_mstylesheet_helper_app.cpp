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

#include "mstylesheet.h"
#include "mapplication.h"
#include "testobjectstyle.h"

int main(int argc, char **argv)
{
    MStyleSheet sheet(0);

    if (argc < 2) {
	return 0;
    }

    QString css_filename;
    css_filename += QString(argv[1]);

    if (argc > 2 && QString(argv[2]).toLower() == "relaxed")
	sheet.setSyntaxMode(MStyleSheet::RelaxedSyntax);
    sheet.load(css_filename);

    QList<const MStyleSheet *> sheets;
    sheets.append(&sheet);

    TestObjectStyle *style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "", "", "", M::Landscape, NULL);

    MStyleSheet::releaseStyle(style);

    return 0;
}
