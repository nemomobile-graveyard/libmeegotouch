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

#ifndef MBUBBLEITEM_P_H__
#define MBUBBLEITEM_P_H__

#include "mbubbleitem.h"
#include "core/mwidgetview_p.h"

class MLabel;
class MImageWidget;
class MScalableImage;
class MControllerWidget;
class QGraphicsGridLayout;
class MBubbleItemBackground;
class MSeparator;
class MImageWidget;

class MBubbleItemViewPrivate : public MWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(MBubbleItemView)

public:
    MBubbleItemViewPrivate();
    virtual ~MBubbleItemViewPrivate();

    void initLayout();
    void clearLayout();
    void replaceAvatar();
    void setupCentralWidget();

    void clearInformationWidgetsLayout();
    void updateInformationWidgetsLayout();

    void applyStyle();
    void refreshStyleMode();
    void updateMessageComposition(); 

    MImageWidget *avatar;
    MBubbleItemBackground* speechBubble;

    MLabel *timeStampLabel;
    MLabel *messageComposition;

    MSeparator *separator;

    QGraphicsWidget* currentCentralWidget;

private:
    MBubbleItem *controller;
    QGraphicsGridLayout *layout;
    QGraphicsLinearLayout *informationWidgetsLayout;
    QGraphicsWidget *informationWidgetsSpacer;
    QGraphicsLinearLayout *innerLayout;
};

#endif
