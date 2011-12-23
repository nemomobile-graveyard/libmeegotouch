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

#include "customselectablelabel.h"
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MLabel>
#include <MLabelHighlighter>
#include <MBreakIterator>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QTapAndHoldGesture>

bool CustomEdit::startSelection(const QPointF &hitPoint)
{
    QPointF startSelectionPoint = startPoint(hitPoint);

    //fake double click to activate selection on text edit
    QGraphicsSceneMouseEvent event;
    event.setPos(startSelectionPoint);
    event.setButton(Qt::LeftButton);
    mousePressEvent(&event);
    event.setPos(startSelectionPoint);
    mouseReleaseEvent(&event);

    event.setPos(startSelectionPoint);
    mousePressEvent(&event);
    event.setPos(startSelectionPoint);
    mouseReleaseEvent(&event);

    return hasSelectedText();
}

QPointF CustomEdit::startPoint(const QPointF &hitPoint) const
{
    // Prefer to select something even though the tap hit between words

    MBreakIterator breakIterator(text());
    if (breakIterator.isBoundary(cursorPosition(hitPoint))) {
        // Hit the boundary -> try to find a word nearby
        QList<QPoint> baseOffsets;
        baseOffsets << QPoint(3,0) << QPoint(-3,0) << QPoint(0,-3) << QPoint(0,3);
        for (int i=1; i<=4; i++) { // Check each direction at 3,6,9 and 12 pixel distances -> a change to get the closest one
            Q_FOREACH(QPoint offset, baseOffsets) {
                QPointF checkPoint = hitPoint + i*offset;
                int checkPos = cursorPosition(checkPoint);
                if (checkPos >= 0 && !breakIterator.isBoundary(checkPos)) {
                    return checkPoint;
                }
            }
        }
    }

    return hitPoint;
}

int CustomEdit::cursorPosition(const QPointF &hitPoint) const
{
    const CustomEditView *editView = qobject_cast<const CustomEditView *>(view());
    if (editView)
        return const_cast<CustomEditView *>(editView)->cursorPosition(hitPoint);

    return -1;
}

CustomSelectableLabel::CustomSelectableLabel(QString const &text, QGraphicsItem *parent)
    : MWidget(parent),
      selecting(false),
      label(0),
      edit(0),
      editModel(0),
      layout(0),
      labelPolicy(0),
      editPolicy(0),
      hitTimer(0)
{
    label = new MLabel(this);
    label->setWordWrap(true);
    label->setWrapMode(QTextOption::WordWrap);

    edit = new CustomEdit(this);
    edit->setReadOnly(true);

    QTextDocument *document = edit->document();
    if (document) {
        QTextOption textOptions;

        if (label->wordWrap())
            textOptions.setWrapMode(label->wrapMode());
        else
            textOptions.setWrapMode(QTextOption::ManualWrap);
        textOptions.setAlignment(label->alignment());

        document->setDefaultTextOption(textOptions);
        document->setDefaultFont(label->font());
    }

    edit->setSizePolicy(label->sizePolicy());

    editModel = dynamic_cast<MTextEditModel*>(edit->model());
    connect(editModel, SIGNAL(modified(QList<const char*>)), this, SLOT(onEditModelModified(QList<const char *>)));

    layout = new MLayout;
    setLayout(layout);

    labelPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    labelPolicy->setContentsMargins(0,0,0,0);
    labelPolicy->addItem(label);
    labelPolicy->activate();

    editPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    editPolicy->setContentsMargins(0,0,0,0);
    editPolicy->addItem(edit);

    grabGestureWithCancelPolicy(Qt::TapAndHoldGesture, Qt::GestureFlags(), MWidget::MouseEventCancelOnGestureFinished);

    setText(text);
}

CustomSelectableLabel::~CustomSelectableLabel()
{
    delete hitTimer;
}

void CustomSelectableLabel::setStyleName(const QString &name)
{
    label->setStyleName(name);
    edit->setStyleName(QString("LikeLabel") + name);
}

void CustomSelectableLabel::setText(const QString &text)
{
    label->setText(text);
}

void CustomSelectableLabel::addHighlighter(MLabelHighlighter *highlighter)
{
    label->addHighlighter(highlighter);
}

void CustomSelectableLabel::removeHighlighter(MLabelHighlighter *highlighter)
{
    label->removeHighlighter(highlighter);
}

void CustomSelectableLabel::removeAllHighlighters()
{
    label->removeAllHighlighters();
}

void CustomSelectableLabel::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
    if (selecting) {
        event->ignore(gesture);
        return;
    }

    event->accept(gesture);

    if (gesture->state() == Qt::GestureFinished) {
        switchToEdit();

        hitPoint = gesture->position();

        // Use timer to postpone startSelection() call to subsequent event loop
        // because MTextEdit has not handled focusInEvent yet and won't start selection
        if (!hitTimer) {
            hitTimer = new QTimer(this);
            hitTimer->setSingleShot(true);
            connect(hitTimer, SIGNAL(timeout()), this, SLOT(hitTimeout()));
        }
        hitTimer->start(0);
    }
}

void CustomSelectableLabel::hitTimeout()
{
    if (!edit->startSelection(edit->mapFromScene(hitPoint))) {
        switchToLabel();
    }
}

void CustomSelectableLabel::switchToEdit()
{
    if (selecting)
        return;

    if (label->textElide())
        edit->setText(label->renderedText()); //Elided text can be retrieved using renderedText() but it looses HTML-formatting
    else
        edit->setHtml(label->text()); // Does not give elided text but retains original HTML-format (note! highlighter texts are not colored)

    selecting = true;
    editPolicy->activate();
    edit->setFocus(Qt::MouseFocusReason);
}

void CustomSelectableLabel::switchToLabel()
{
    if (!selecting)
        return;

    selecting = false;
    labelPolicy->activate();
}

void CustomSelectableLabel::onEditModelModified(const QList<const char *>& modifications)
{
    if (selecting && modifications.contains(MTextEditModel::IsSelecting)
        && editModel && !editModel->isSelecting()) {
        switchToLabel();
    }
}
