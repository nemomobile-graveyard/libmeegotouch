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

#ifndef DUIMASHUPCONTAINERVIEW_P_H
#define DUIMASHUPCONTAINERVIEW_P_H

#include "private/duiwidgetview_p.h"

class DuiContainer;
class QGraphicsLinearLayout;

class DuiButton;
class DuiLabel;
class DuiImageWidget;
class DuiSeparator;
class QPixmap;
class DuiScalableImage;
class DuiContainerHeader;
class DuiProgressIndicator;

class DuiContainerViewPrivate : public DuiWidgetViewPrivate
{
public:
    DuiContainerViewPrivate();
    virtual ~DuiContainerViewPrivate();

    DuiContainer *controller;

    void init();

    void createHeader();
    void removeHeader();
    void setupIcon(const QSize &size);

    void createProgressIndicator();
    void layoutProgressIndicator();

    // vertical main layout
    QGraphicsLinearLayout *mainLayout;

    // layout for the header component of the container
    QGraphicsLinearLayout *headerLayout;

    // widgets for the header
    DuiContainerHeader *header;
    DuiImageWidget *icon;
    DuiLabel  *title;
    DuiLabel  *text;

    const DuiScalableImage *background;

    //! Whether the header is in "pressed" state
    bool headerPressed;

    DuiProgressIndicator *progressIndicator;

    DuiContainerView *q_ptr;
    Q_DECLARE_PUBLIC(DuiContainerView)
};


#endif
