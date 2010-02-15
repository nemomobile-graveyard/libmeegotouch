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

#ifndef PROGRESSINDICATORPAGE_H
#define PROGRESSINDICATORPAGE_H

#include <DuiApplicationPage>

class DuiGridLayoutPolicy;
class DuiProgressIndicator;
class DuiButton;

class ProgressIndicatorPage : public DuiApplicationPage
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

    void pageRotated(const Dui::Orientation &orientation);

protected:
    DuiGridLayoutPolicy *l_policy;
    DuiGridLayoutPolicy *p_policy;

    DuiProgressIndicator *bar1;
    DuiProgressIndicator *bar2;
    DuiProgressIndicator *circ1;
    DuiProgressIndicator *circ2;

    DuiButton *buttonLeft;
};

#endif // PROGRESSINDICATORPAGE_H
