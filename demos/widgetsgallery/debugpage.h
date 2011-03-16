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

#ifndef DEBUGPAGE_H
#define DEBUGPAGE_H

#include "templatepage.h"
#include <mgconfitem.h>

class MGConfItem;

class DebugPage: public TemplatePage
{
    Q_OBJECT

public:
    DebugPage();
    virtual ~DebugPage();
    virtual void createContent();
    virtual QString timedemoTitle();

private slots:

    void applyOptions();

private:

    MLinearLayoutPolicy *layoutPolicy;

    MGConfItem debugOptionsShortcuts;

    MLabel *label1;
    MLabel *labelNames;
    MLabel *labelObjects;
    MLabel *labelPosition;
    MLabel *labelSize;
    MLabel *labelBoundingRect;
    MLabel *labelMargins;
    MLabel *labelFps;

    MButton *checkBoxNames;
    MButton *checkBoxobjNames;
    MButton *checkBoxPosition;
    MButton *checkBoxSize;
    MButton *checkBoxBoundingRect;
    MButton *checkBoxMargins;
    MButton *checkBoxFps;
};

#endif
