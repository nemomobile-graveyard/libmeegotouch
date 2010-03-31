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

#include "duilabel.h"
#include "duilabel_p.h"
#include "duiwidgetview.h"
#include "duilabelhighlighter.h"

//#include "duilabelview.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiLabel)

DuiLabel::DuiLabel(QGraphicsItem *parent, DuiLabelModel *model) :
    DuiWidgetController(new DuiLabelPrivate, model == NULL ? new DuiLabelModel : model, parent)
{
}

DuiLabel::DuiLabel(QString const &text, QGraphicsItem *parent) :
    DuiWidgetController(new DuiLabelPrivate, new DuiLabelModel, parent)
{
    setText(text);
}

DuiLabel::~DuiLabel()
{
}

void DuiLabel::setupModel()
{
    DuiWidgetController::setupModel();
    connect(model(), SIGNAL(linkActivated(QString)), this, SIGNAL(linkActivated(QString)));
}

void DuiLabel::changeEvent(QEvent *event)
{
    //TODO Would probably be more proper to handle the changeEvent()
    //     in the view side because DuiWidgetController is
    //     forwarding the event there as well. It just means
    //     a bit more work due to WEIRD implementation of the label
    //     views. There is existing implementation for handling the
    //     changing of the direction through model so will use that
    //     for now.
    DuiWidgetController::changeEvent(event);
    if (event->type() == QEvent::LayoutDirectionChange)
        model()->setTextDirection(layoutDirection());
}

void DuiLabel::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    Q_UNUSED(event);
    //FIXME
    //Temporary remove this when proper longPressEvents are coming to view.
//    const DuiLabel *t = (const DuiLabel *) this;
//    DuiLabelView *labelView = (DuiLabelView *)(t->view());
//    labelView->longPressEvent(event);
}

void DuiLabel::setText(const QString &text)
{
    model()->setText(text);
}

void DuiLabel::setAlignment(Qt::Alignment alignment)
{
    model()->setAlignment(alignment);
}

Qt::Alignment DuiLabel::alignment() const
{
    return model()->alignment();
}

void DuiLabel::setWrapMode(QTextOption::WrapMode wrapMode)
{
    model()->setWrapMode(wrapMode);
}

void DuiLabel::setWordWrap(bool wrap)
{
    model()->setWordWrap(wrap);
}

QString DuiLabel::text() const
{
    return model()->text();
}

bool DuiLabel::wordWrap() const
{
    return model()->wordWrap();
}

QTextOption::WrapMode DuiLabel::wrapMode() const
{
    return model()->wrapMode();
}

void DuiLabel::setTextElide(bool elide)
{
    model()->setTextElide(elide);
}

bool DuiLabel::textElide() const
{
    return model()->textElide();
}

void DuiLabel::setFont(const QFont &font)
{
    model()->setUseModelFont(true);
    model()->setFont(font);
    QGraphicsWidget::setFont(font);
}

QFont DuiLabel::font() const
{
    if (!model()->useModelFont()) {
        if (view())
            return view()->font();
    }
    return model()->font();
}

void DuiLabel::setColor(const QColor &color)
{
    model()->setColor(color);
}

QColor DuiLabel::color() const
{
    /*if( !model()->color().isValid() ) {
        if( view() )
            return view()->color();
    }*/
    return model()->color();
}

void DuiLabel::addHighlighter(DuiLabelHighlighter *highlighter)
{
    model()->addHighlighter(highlighter);
}

void DuiLabel::removeHighlighter(DuiLabelHighlighter *highlighter)
{
    model()->removeHighlighter(highlighter);
}

void DuiLabel::removeAllHighlighters()
{
    model()->setHighlighters(DuiLabelHighlighterList());
}

