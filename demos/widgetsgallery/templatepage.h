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

#ifndef TEMPLATEPAGE_H
#define TEMPLATEPAGE_H

#include "timedemopage.h"

#include <MApplicationPage>

class MLayout;
class MLinearLayoutPolicy;
class MWidget;
class MButton;
class MSeparator;
class MLabel;
class QGraphicsLinearLayout;

class TemplatePage  : public TimedemoPage
{
    Q_OBJECT

public:
    enum Category {
        ApplicationView = 0,
        SimpleWidgets,
        Buttons,
        DialogsSheetsAndBanners,
        ListsGridsAndPopups,
        UserInput
    };

    TemplatePage(TemplatePage::Category category);
    virtual ~TemplatePage();

    TemplatePage::Category category();

    virtual void createContent();

protected:
    virtual void createLayout();
    virtual void retranslateUi();

    MLayout              *layout;

    MLinearLayoutPolicy  *landscapePolicy;
    MLinearLayoutPolicy  *portraitPolicy;

    MWidget              *container;

    MSeparator           *separator;
    MLabel               *infoLabel;

    MLayout              *containerLayout;
    MLinearLayoutPolicy  *containerPolicy;

private:
    TemplatePage::Category pageCategory;
};

#endif
