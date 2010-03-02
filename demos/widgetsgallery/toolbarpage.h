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

#ifndef TOOLBARPAGE_H
#define TOOLBARPAGE_H

#include "templatepage.h"

class DuiToolBar;
class DuiGridLayoutPolicy;

class ToolBarPage : public TemplatePage
{
    Q_OBJECT

public:
    ToolBarPage();
    virtual ~ToolBarPage();
    virtual QString timedemoTitle();

    virtual void createContent();

protected:
    virtual void createLayout();
    virtual void retranslateUi();

private slots:
    void fourButtons();
    void textEntryWithTwoButtons();

private:
    void addTextEntry();
    void clearToolbarActions();
};

#endif
