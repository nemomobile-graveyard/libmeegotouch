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

class MEditorToolbar;
class MTextEdit;
class MTextMagnifier;
class QGraphicsSceneMouseEvent;
class QTextDocument;
class QTimer;
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
    int cursorPosition(const QPointF &point, Qt::HitTestAccuracy accuracy = Qt::FuzzyHit);

    QTextDocument *document() const;
    QTextDocument *promptDocument() const;
    void checkScroll();
    void setMouseTarget(const QPointF &point);
    bool isLayoutLeftToRight() const;

    // returns a paint context for drawing. Includes selection formatting
    QAbstractTextDocumentLayout::PaintContext paintContext() const;

    qreal cursorX() const;

    void initMaskedDocument();

    QTextDocument *activeDocument() const;

    qreal heightForWidth(qreal width) const;

    void showMagnifier();
    void hideMagnifier();

    void showEditorToolbar();
    void hideEditorToolbar();

protected slots:
    void scrolling();
    void hideUnmaskedText();
    void checkSize();
    void handleDocumentUpdate(int position, int charsRemoved, int charsAdded);
    void handleDocumentUpdated();
    void handleDocumentSizeChange(const QSizeF &newSize);
    void playTextFieldSelectionFeedback();
    void updateMagnifierPosition();
    void makeMagnifierDisappear();
    void makeMagnifierAppear();
    void restoreEditorToolbar();
    void updateEditorToolbarPosition();

private:
    void scrollingTestAndStart(QGraphicsSceneMouseEvent *event);
    void checkStartOfSelection(QGraphicsSceneMouseEvent *event);
    void startSelection(QGraphicsSceneMouseEvent *event);
    void updateSelection(const QPointF &pos);
    void updateSelection(QGraphicsSceneMouseEvent *event);

    //! Returns a rectangle covering preedit text in item space
    QRect preeditRectangle() const;

    //! Returns width of the cursor
    int cursorWidth() const;

    //! Returns a rectangle of the cursor
    QRect cursorRect() const;

    //! \return area covered by text starting from \a startPosition and ending
    //! to \a endPosition.  Both positions must be on the same line.
    QRect textRectangle(int startPosition, int endPosition);
    //! \return area covered by selection on its \a first or last line
    QRect selectionLineRectangle(bool first);

protected:
    MTextEditView *q_ptr;

    MTextEdit *controller;

    bool focused;

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

    QPointer<MInfoBanner> infoBanner;

    bool editActive; // true if editing started and having focus

    QTimer *hideInfoBannerTimer;

    QScopedPointer<MTextMagnifier> magnifier;
    QScopedPointer<MEditorToolbar> editorToolbar;
    bool oldItemSendsScenePositionChanges;

    bool focusReleaseExpected;

#ifdef UNIT_TEST
    friend class Ut_MTextEditView;
    friend class Ut_MTextEdit;
#endif
};
//! \internal_end

#endif
