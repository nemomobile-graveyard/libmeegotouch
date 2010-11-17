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

#ifndef SWITCHPAGE_H
#define SWITCHPAGE_H

#include "mainpage.h"
#include "templatepage.h"
#include <MApplicationPage>
#include <MLayout>
#include <MGridLayoutPolicy>

class MButton;
class MButtonGroup;
class MWidget;
class MLabel;

class SwitchPage : public TemplatePage
{
    Q_OBJECT
public:
    SwitchPage();
    virtual ~SwitchPage();
    virtual QString timedemoTitle();
    virtual void createContent();

protected:
    virtual void retranslateUi();

protected Q_SLOTS:
    void switchToggled(bool toggle);

private:
    MButton *switch1;
    MButton *switch2;
    MButton *switch3;
    MButton *switch4;
    MButton *switch5;

    MLabel *label1;
    MLabel *label2;
    MLabel *label3;
    MLabel *label4;
    MLabel *label5;

};

#endif
