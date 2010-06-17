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

#ifndef BUTTONGROUPPAGE_H
#define BUTTONGROUPPAGE_H

#include "mainpage.h"
#include "templatepage.h"
#include <MApplicationPage>

class MButton;
class MLabel;

class ButtonGroupPage : public TemplatePage
{
    Q_OBJECT
public:
    ButtonGroupPage();
    virtual ~ButtonGroupPage();
    virtual QString timedemoTitle();
    virtual void createContent();

protected:
    virtual void retranslateUi();

private:
    MButton *pushButton1;
    MButton *pushButton2;
    MButton *pushButton3;
    MButton *pushButton4;
    MButton *pushButton5;
    MButton *pushButton6;
    MButton *pushButton7;
    MButton *pushButton8;
    MButton *pushButton9;

    MLabel* hLabel1;
    MLabel* hLabel2;
    MLabel* vLabel;
};

#endif
