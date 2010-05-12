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

#ifndef CHECKBOXPAGE_H
#define CHECKBOXPAGE_H

#include "mainpage.h"
#include "templatepage.h"
#include <MApplicationPage>
#include <MLayout>
#include <MGridLayoutPolicy>

class MButton;
class MButtonGroup;
class MWidget;
class MLabel;

class CheckboxPage : public TemplatePage
{
    Q_OBJECT
public:
    CheckboxPage();
    virtual ~CheckboxPage();
    virtual QString timedemoTitle();
    virtual void createContent();
protected:
    virtual void retranslateUi();
private:
    MButton *checkbox;
    MLabel *checkboxLabel;
};

#endif
