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
 * An example of using gestures in an application
 *
 * The application shows one image at a time, which can be pinched and
 * rotated. By swiping left or right (or using the arrow keys), the image
 * can be changed.
 *
 * You can simulate the pinch on your PC by holding down Ctrl on your keyboard
 * while pressing and dragging with the left mouse button.
 */

#include <MApplication>
#include <MApplicationWindow>

#include "mypage.h"

int main(int argc, char **argv)
{
    MApplication app(argc, argv);

    MApplicationWindow w;
    w.show();

    MyPage p;
    p.appear(&w);

    p.grabKeyboard(); // Forcing all keyboard events to the page

    return app.exec();
}
