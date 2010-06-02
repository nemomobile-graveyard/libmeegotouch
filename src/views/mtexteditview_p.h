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

#ifndef MTEXTEDITVIEW_P_H
#define MTEXTEDITVIEW_P_H

#include <QObject>
#include <QTextCharFormat>
#include <QAbstractTextDocumentLayout>
#include <QPointer>

#include "mtexteditview.h"

class MTextEdit;
class QGraphicsSceneMouseEvent;
class QColor;
class QTextDocument;
class QTimer;
class QTimeLine;
class MInfoBanner;


//! \internal
class MTextEditViewPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(MTextEditView)

public:
    MTextEditViewPrivate(MTextEdit *controller, MTextEditView *q);
    virtual ~MTextEditViewPrivate();

    int cursorPosition(QGraphicsSceneMouseEvent *event);
    int cursorPosition(QPointF event);

    const QTextDocument *document() const {
        return textDocument;
    }
    QTextDocument *promptDocument() const {
        return promptTextDocument;
    }

    void checkScroll();

    void setMouseTarget(const QPointF &point);

    // returns a paint context for drawing. Includes selection formatting
    QAbstractTextDocumentLayout::PaintContext paintContext() const;

    qreal cursorX() const;

    void initMaskedDocument();

    QTextDocument *activeDocument() const;

protected slots:
    void scrolling();
    void hideUnmaskedText();
    void checkSize();
    void handleDocumentUpdate(int position, int charsRemoved, int charsAdded);
    void handleDocumentSizeChange(const QSizeF &newSize);
    void playTextFieldSelectionFeedback();

private:
    void scrollingTestAndStart(QGraphicsSceneMouseEvent *event);
    void checkStartOfSelection(QGraphicsSceneMouseEvent *event);
    void startSelection(QGraphicsSceneMouseEvent *event);
    void updateSelection(const QPointF &pos);
    void updateSelection(QGraphicsSceneMouseEvent *event);

    //! Returns a rectangle covering preedit text in item space
    QRect preeditRectangle() const;

    //! Returns a rectangle of the cursor
    QRect cursorRect() const;

protected:
    MTextEditView *q_ptr;

    MTextEdit *controller;

    bool focused;

    QTextDocument *textDocument;
    QTextDocument *maskedTextDocument;
    QTextDocument *promptTextDocument;

    int unmaskPosition; // position where unmask in masked mode starts
    int unmaskLength; // lenght of unmasked text

    // styling
    qreal selectionThreshold;
    QChar maskCharacter; // what characters are replaced with in masked mode
    QTextCharFormat selectionFormat;

    qreal hscroll; // horizontal offset of text
    qreal vscroll; // vertical offset

    int scrollSpeedVertical;
    int scrollSpeedHorizontal;

    QTimer *longPressTimer;
    QTimer *scrollTimer;
    QTimer *maskTimer;

    bool selecting; // separate from controller mode because the selection there doesn't tell
    // this view initiated the selection by mouse press
    bool ignoreSelection;
    int startCursorPos;

    qreal documentHeight;
    QPointF mouseTarget; // known mouse position inside widget

    bool inAutoSelectionClick;

    MInfoBanner *infoBanner;

    bool editActive; // true if editing started and having focus

    QTimer *hideInfoBannerTimer;

#ifdef UNIT_TEST
    friend class Ut_MTextEditView;
    friend class Ut_MTextEdit;
#endif
};
//! \internal_end

#endif
