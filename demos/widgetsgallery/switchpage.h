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

#ifndef SWITCHPAGE_H
#define SWITCHPAGE_H

#include "listpage.h"
#include "templatepage.h"
#include <DuiApplicationPage>
#include <DuiLayout>
#include <DuiGridLayoutPolicy>

class DuiButton;
class DuiButtonGroup;
class DuiWidget;
class DuiLabel;

class SwitchPage : public TemplatePage
{
    Q_OBJECT
public:
    SwitchPage();
    virtual ~SwitchPage();
    virtual void createContent();
protected:
    virtual void retranslateUi();
private:
    DuiButton *switch1;
    DuiButton *switch2;
    DuiButton *switch3;
    DuiButton *switch4;
    DuiButton *switch5;

    DuiLabel *label1;
    DuiLabel *label2;
    DuiLabel *label3;
    DuiLabel *label4;
    DuiLabel *label5;

};

#endif
