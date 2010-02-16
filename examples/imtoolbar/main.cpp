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

#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiTheme>
#include "imtoolbarpage.h"

namespace
{
    const QString InputMethodToolbars("/Dui/InputMethods/Toolbars");
};

int main(int argc, char **argv)
{
    // Create main dui application.
    DuiApplication application(argc, argv, "imtoolbar");

    // Create application window and make it visible.
    DuiApplicationWindow window;
    window.show();

    // Create application page and make it visible.
    ImToolbarPage page;
    page.appear();

    return application.exec();
}
