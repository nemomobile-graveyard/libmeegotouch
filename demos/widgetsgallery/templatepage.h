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

#ifndef TEMPLATEPAGE_H
#define TEMPLATEPAGE_H

#include <DuiApplicationPage>

class DuiLayout;
class DuiLinearLayoutPolicy;
class DuiWidget;
class DuiButton;
class DuiSeparator;
class DuiLabel;
class QGraphicsLinearLayout;

class TemplatePage : public DuiApplicationPage
{
    Q_OBJECT

public:
    enum GroupID {
        ViewsAndDialogs = 0,
        ContainersAndApplets,
        LayoutsAndVisuals,
        Buttons,
        IndicatorsAndNotifications,
        ListsGridsAndMenus,
        UserInput,
        InputFeedback
    };

    TemplatePage();
    virtual ~TemplatePage();

    int groupID();
    static QStringList groupNames();

    virtual void createContent();

protected:
    virtual void createLayout();
    virtual void retranslateUi();

    int gid;

    DuiLayout              *layout;

    DuiLinearLayoutPolicy  *landscapePolicy;
    DuiLinearLayoutPolicy  *portraitPolicy;

    DuiWidget              *container;

    DuiSeparator           *separator;
    DuiLabel               *infoLabel;

    DuiLayout              *containerLayout;
    DuiLinearLayoutPolicy  *containerPolicy;
};

#endif
