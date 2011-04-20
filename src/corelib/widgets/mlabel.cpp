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

#include "mlabel.h"
#include "mlabel_p.h"
#include "mwidgetview.h"
#include "mlabelhighlighter.h"
#include <QGraphicsSceneContextMenuEvent>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MLabel)

MLabel::MLabel(QGraphicsItem *parent, MLabelModel *model) :
    MWidgetController(new MLabelPrivate, model == NULL ? new MLabelModel : model, parent)
{
    Q_D(MLabel);
    d->init();
}

MLabel::MLabel(QString const &text, QGraphicsItem *parent) :
    MWidgetController(new MLabelPrivate, new MLabelModel, parent)
{
    Q_D(MLabel);
    d->init();
    setText(text);
}

MLabel::~MLabel()
{
}

void MLabelPrivate::init()
{
    Q_Q(MLabel);
    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    policy.setHeightForWidth(true);
    q->setSizePolicy(policy);
    q->grabGestureWithCancelPolicy(Qt::TapAndHoldGesture, Qt::GestureFlags(), MWidget::MouseEventCancelOnGestureFinished);
}

void MLabel::setupModel()
{
    MWidgetController::setupModel();
    connect(model(), SIGNAL(linkActivated(QString)), this, SIGNAL(linkActivated(QString)));
}

void MLabel::changeEvent(QEvent *event)
{
    //TODO Would probably be more proper to handle the changeEvent()
    //     in the view side because MWidgetController is
    //     forwarding the event there as well. It just means
    //     a bit more work due to WEIRD implementation of the label
    //     views. There is existing implementation for handling the
    //     changing of the direction through model so will use that
    //     for now.
    MWidgetController::changeEvent(event);
}

void MLabel::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    Q_UNUSED(event);
    event->ignore();
}

void MLabel::setText(const QString &text)
{
    model()->setText(text);
}

void MLabel::setAlignment(Qt::Alignment alignment)
{
    model()->beginTransaction();
    model()->setAlignment(alignment);
    model()->setAlignmentFromStyle(false);
    model()->commitTransaction();
}

Qt::Alignment MLabel::alignment() const
{
    return model()->alignment();
}

void MLabel::setWrapMode(QTextOption::WrapMode wrapMode)
{
    model()->setWrapMode(wrapMode);
}

void MLabel::setWordWrap(bool wrap)
{
    model()->setWordWrap(wrap);
}

QString MLabel::text() const
{
    return model()->text();
}

bool MLabel::wordWrap() const
{
    return model()->wordWrap();
}

QTextOption::WrapMode MLabel::wrapMode() const
{
    return model()->wrapMode();
}

void MLabel::setTextElide(bool elide)
{
    model()->setTextElide(elide);
}

bool MLabel::textElide() const
{
    return model()->textElide();
}

void MLabel::setFont(const QFont &font)
{
    model()->setUseModelFont(true);
    model()->setFont(font);
    QGraphicsWidget::setFont(font);
}

QFont MLabel::font() const
{
    if (!model()->useModelFont()) {
        if (view())
            return view()->font();
    }
    return model()->font();
}

void MLabel::setColor(const QColor &color)
{
    model()->setColor(color);
}

QColor MLabel::color() const
{
    /*if( !model()->color().isValid() ) {
        if( view() )
            return view()->color();
    }*/
    return model()->color();
}

void MLabel::setTextFormat(Qt::TextFormat textFormat)
{
    model()->setTextFormat(textFormat);
}
 
Qt::TextFormat MLabel::textFormat() const
{
    return model()->textFormat();
}

void MLabel::addHighlighter(MLabelHighlighter *highlighter)
{
    model()->addHighlighter(highlighter);
}

void MLabel::removeHighlighter(MLabelHighlighter *highlighter)
{
    model()->removeHighlighter(highlighter);
}

void MLabel::removeAllHighlighters()
{
    model()->setHighlighters(QList<MLabelHighlighter *>());
}


#include "moc_mlabel.cpp"
