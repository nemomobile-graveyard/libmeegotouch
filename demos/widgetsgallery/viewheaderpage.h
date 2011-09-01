/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef VIEWHEADERPAGE_H
#define VIEWHEADERPAGE_H

#include "templatepage.h"

#include <MPannableViewport>
#include <MContainer>
#include <MAction>
#include <MLinearLayoutPolicy>

class QStringListModel;

class ViewHeaderPage : public TemplatePage
{
    Q_OBJECT

public:
    ViewHeaderPage();
    virtual ~ViewHeaderPage();
    virtual QString timedemoTitle();

    virtual void createContent();

protected:
    MLinearLayoutPolicy *createNormalPolicy(MLayout *layout);
    MLinearLayoutPolicy *createFixedPolicy(MLayout *layout);
    QStringListModel *createListModel();

    virtual void retranslateUi();

private slots:
    void normal();
    void fixed();

private:
    MAction *actionNormal;
    MAction *actionFixed;

    MLinearLayoutPolicy *normalPolicy;
    MLinearLayoutPolicy *fixedPolicy;
};

#endif // VIEWHEADERPAGE_H
