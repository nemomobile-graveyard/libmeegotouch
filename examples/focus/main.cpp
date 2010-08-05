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
 * An example of managing focus.
 *
 * The example starts out with no widget being focused.
 * If the text entry on the page is clicked, it receives focus as expected.
 *
 * If the application menu or the example dialog is opened, the entry on the
 * page loses focus. The entry inside the dialog automatically gains focus when
 * it is opened. Closing the dialog or application menu automatically focuses
 * the text entry on the page.
 *
 */

#include <MApplication>
#include <MApplicationWindow>
#include <MScene>
#include <MTextEdit>
#include <MButton>
#include <QGraphicsLinearLayout>

#include "mypage.h"
#include "mydialog.h"

int main(int argc, char **argv)
{
    MApplication app(argc, argv);

    MApplicationWindow w;
    w.show();

    MyPage p;
    p.appear(&w);

    return app.exec();
}
