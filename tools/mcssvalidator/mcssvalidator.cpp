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

//==============================================================
// Description : Tool to check if CSS files have a valid syntax
//==============================================================

#include <QFile>
#include <mstylesheetparser.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    bool valid = true;
    if (argc < 2) {
	cout <<
	    "MeeGo CSS Validator, usage:\n  " <<
	    argv[0] <<
	    " PATH_TO_CSS_FILE [PATH_TO_CSS_FILE_2…]\n";
	return 0;
    }
    for (int i = 1; i < argc; i++) {
	if (!QFile::exists(argv[i])) {
	    cout << "Skipping \"" << argv[i] << "\", does not exist\n";
	    continue;
	}
	cout << "Validating \"" << argv[i] << "\"\n";
	bool passedTest = MStyleSheetParser::validate(argv[i]);
	cout << argv[i] << " : " << (passedTest ? "VALID" : "INVALID") << "\n";
	valid = valid && passedTest;
    }
    return valid ? 0 : 1;
}
