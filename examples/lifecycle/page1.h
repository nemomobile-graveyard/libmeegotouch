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

#ifndef PROGRESSINDICATORPAGE_H
#define PROGRESSINDICATORPAGE_H

#include <MApplicationPage>

class MGridLayoutPolicy;
class MProgressIndicator;
class MButton;

class ProgressIndicatorPage : public MApplicationPage
{
    Q_OBJECT;

public:
    ProgressIndicatorPage();
    virtual ~ProgressIndicatorPage();

    virtual void createContent();

public slots:
    void rotate(int d);
    void rotatel();
    void rotater();

    void buttonLeftSlot();
    void buttonRightSlot();

    void pageRotated(const M::Orientation &orientation);

protected:
    MGridLayoutPolicy *l_policy;
    MGridLayoutPolicy *p_policy;

    MProgressIndicator *bar1;
    MProgressIndicator *bar2;
    MProgressIndicator *circ1;
    MProgressIndicator *circ2;

    MButton *buttonLeft;
};

#endif // PROGRESSINDICATORPAGE_H
