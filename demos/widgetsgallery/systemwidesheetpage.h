/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef SYSTEMWIDESHEETPAGE_H
#define SYSTEMWIDESHEETPAGE_H

#include <MApplicationPage>

class LabeledCheckbox;

class SystemwideSheetPage : public MApplicationPage
{
    Q_OBJECT
public:
    SystemwideSheetPage();
    virtual void createContent();

private Q_SLOTS:
    void openSystemwideSheet();

private:
    LabeledCheckbox *statusBarCheckbox;
    LabeledCheckbox *autoFocusCheckbox;
};

#endif