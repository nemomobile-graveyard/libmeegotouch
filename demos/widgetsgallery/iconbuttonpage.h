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

#ifndef ICONBUTTONPAGE_H
#define ICONBUTTONPAGE_H

#include "listpage.h"
#include "templatepage.h"
#include <DuiApplicationPage>
#include <DuiLayout>
#include <DuiGridLayoutPolicy>

class DuiButton;
class DuiButtonGroup;
class DuiWidget;
class DuiLabel;

class IconButtonPage : public TemplatePage
{
    Q_OBJECT
public:
    IconButtonPage();
    virtual ~IconButtonPage();
    virtual QString timedemoTitle();
    virtual void createContent();
protected:
    virtual void retranslateUi();
private:
    DuiButton *iconButton1;
    DuiButton *iconButton2;
};

#endif
