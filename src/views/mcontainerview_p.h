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

#ifndef MMASHUPCONTAINERVIEW_P_H
#define MMASHUPCONTAINERVIEW_P_H

#include "private/mwidgetview_p.h"

class MContainer;
class QGraphicsLinearLayout;

class MButton;
class MLabel;
class MImageWidget;
class MSeparator;
class QPixmap;
class MScalableImage;
class MContainerHeader;
class MProgressIndicator;

class MContainerViewPrivate : public MWidgetViewPrivate
{
public:
    MContainerViewPrivate();
    virtual ~MContainerViewPrivate();

    MContainer *controller;

    void init();

    void createHeader();
    void removeHeader();
    void setupIcon(const QSize &size);

    void createProgressIndicator();
    void layoutProgressIndicator();

    void setupTextLabel();

    // layout for the header component of the container
    QGraphicsLinearLayout *headerLayout;

    // widgets for the header
    MContainerHeader *header;
    MSeparator *separator;
    MImageWidget *icon;
    MLabel  *title;
    MLabel  *text;

    const MScalableImage *background;

    //! Whether the header is in "pressed" state
    bool headerPressed;

    MProgressIndicator *progressIndicator;

    MContainerView *q_ptr;
    Q_DECLARE_PUBLIC(MContainerView)
};


#endif
