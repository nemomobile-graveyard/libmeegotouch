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

#include <duiscenemanager.h>
#include "duiviewcreator.h"
#include "duiscenewindow.h"
#include "duiscenewindow_p.h"
#include "duiscenewindowview.h"
#include "duiscenewindowview_p.h"
#include "duiabstractwidgetanimation.h"
#include "duiclassfactory.h"

DuiSceneWindowViewPrivate::DuiSceneWindowViewPrivate() :
    controller(0),
    dismissAnimation(0)
{
}

DuiSceneWindowViewPrivate::~DuiSceneWindowViewPrivate()
{
    delete dismissAnimation;
}


DuiSceneWindowView::DuiSceneWindowView(DuiSceneWindow *controller) :
    DuiWidgetView(*new DuiSceneWindowViewPrivate, controller)
{
    Q_D(DuiSceneWindowView);

    d->controller = controller;
    connect(this, SIGNAL(geometryAttributesChanged()), controller, SIGNAL(repositionNeeded()));
}

DuiSceneWindowView::DuiSceneWindowView(DuiSceneWindowViewPrivate &dd, DuiSceneWindow *controller) :
    DuiWidgetView(dd, controller)
{
    Q_D(DuiSceneWindowView);

    d->controller = controller;
    connect(this, SIGNAL(geometryAttributesChanged()), controller, SIGNAL(repositionNeeded()));
}

DuiSceneWindowView::~DuiSceneWindowView()
{
}

void DuiSceneWindowView::applyStyle()
{
    Q_D(DuiSceneWindowView);

    if (showAnimation()) {
        disconnect(showAnimation(), SIGNAL(finished()), d->controller, SIGNAL(appeared()));
    }

    if (hideAnimation()) {
        disconnect(hideAnimation(), SIGNAL(finished()), d->controller, SIGNAL(disappeared()));
    }

    DuiWidgetView::applyStyle();

    if (style()->dismissAnimation() == "none") {
        delete d->dismissAnimation;
        d->dismissAnimation = NULL;
    } else {
        if (!d->dismissAnimation || style()->dismissAnimation() != d->dismissAnimation->metaObject()->className()) {
            delete d->dismissAnimation;
            d->dismissAnimation = dynamic_cast<DuiAbstractWidgetAnimation *>(DuiClassFactory::instance()->createAnimation(style()->dismissAnimation().toStdString().c_str()));
            if (d->dismissAnimation)
                d->dismissAnimation->setView(this);
        }
    }


    if (showAnimation()) {
        connect(showAnimation(), SIGNAL(finished()), d->controller, SIGNAL(appeared()));
    }

    if (hideAnimation()) {
        connect(hideAnimation(), SIGNAL(finished()), d->controller, SIGNAL(disappeared()));
    }

    emit geometryAttributesChanged();
}

void DuiSceneWindowView::setupModel()
{
    DuiWidgetView::setupModel();
    emit geometryAttributesChanged();
}

Qt::Alignment DuiSceneWindowView::alignment() const
{
    Qt::Alignment verticalAlign = style()->verticalAlign() & Qt::AlignVertical_Mask;
    Qt::Alignment horizontalAlign = style()->horizontalAlign() & Qt::AlignHorizontal_Mask;

    return verticalAlign | horizontalAlign;
}

QPointF DuiSceneWindowView::offset() const
{
    return style()->offset();
}

DUI_REGISTER_VIEW_NEW(DuiSceneWindowView, DuiSceneWindow)
