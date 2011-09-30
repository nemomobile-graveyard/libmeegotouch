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
#ifndef MBUTTON_IMPLODING_BACKGROUND_TRANSITION
#define MBUTTON_IMPLODING_BACKGROUND_TRANSITION

#include "mbuttontransition.h"
#include "mbuttonstyle.h"
#include <QTimeLine>
#include <QEasingCurve>


class MButtonModel;
class MWidgetController;
class MButtonViewPrivate;

class MButtonImplodingBackgroundTransition : public MButtonTransition
{
    Q_OBJECT
public:
    MButtonImplodingBackgroundTransition(MButtonStyleContainer& s,
					 MButtonModel* m,
					 MWidgetController* c,
					 MButtonViewPrivate* p);
    virtual ~MButtonImplodingBackgroundTransition();
    virtual void onPress();
    virtual void onRelease();
    virtual void onCancel();
    virtual void modifyBackgroundPainter(QPainter *painter) const;
    virtual void refreshStyle();
private:
    MButtonStyleContainer& style;
    MButtonModel* model;
    MWidgetController* controller;
    MButtonViewPrivate* priv;
    QTimeLine timeline;
    qreal scale;
    qreal opacity;
    QEasingCurve inScaleCurve;
    QEasingCurve inOpacityCurve;
    QEasingCurve outScaleCurve;
    QEasingCurve outOpacityCurve;
    bool released;
private slots:
    void valueChanged(qreal v);
    void finished();
};

#endif
