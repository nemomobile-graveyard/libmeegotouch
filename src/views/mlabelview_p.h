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

#ifndef MLABELVIEW_P_H
#define MLABELVIEW_P_H

#include <QTextOption>
#include <QTextDocument>
#include <QStaticText>
#include "private/mwidgetview_p.h"

class MLabel;
class MLabelView;
class QGraphicsSceneResizeEvent;
class QGestureEvent;
class QTapAndHoldGesture;

class MLabelViewSimple
{
public:
    MLabelViewSimple(MLabelViewPrivate *viewPrivate);
    virtual ~MLabelViewSimple();

    virtual void drawContents(QPainter *painter, const QSizeF &size);
    virtual bool resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void setupModel();
    virtual bool updateData(const QList<const char *>& modifications);
    virtual bool isRich();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(MCancelEvent *event);
    virtual void longPressEvent(QGestureEvent *event, QTapAndHoldGesture* gesture);
    virtual void applyStyle();
    Qt::TextFlag wrap() const;
    void initializeStaticText();
    
    // Helper method for initializeStaticText(): Adjusts the member variable
    // 'textOffset' dependent on the used alignment, as QStaticText does not support
    // text alignment within a bounding rectangle.
    void adjustTextOffset();

    void markDirty();

    MLabelViewPrivate *viewPrivate;

    QSizeF preferredSize;
    QPoint textOffset;
    QRectF paintingRect;
    bool dirty;
    QStaticText staticText;
};

class MLabelViewRich : public MLabelViewSimple
{

public:
    MLabelViewRich(MLabelViewPrivate *viewPrivate);
    virtual ~MLabelViewRich();

    QPixmap generatePixmap();
    virtual void drawContents(QPainter *painter, const QSizeF &size);
    virtual bool shouldElide() const;
    virtual bool resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void setupModel();
    virtual bool updateData(const QList<const char *>& modifications);
    virtual bool isRich();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(MCancelEvent *event);
    virtual void longPressEvent(QGestureEvent *event, QTapAndHoldGesture* gesture);

    virtual void applyStyle();

    void ensureDocumentIsReady();
    int cursorPositionOfLastVisibleCharacter();
    void updateRichTextEliding();
    void updateHighlighting();
    QString wrapTextWithSpanTag(const QString &text) const;

    mutable QTextDocument textDocument;
    bool textDocumentDirty;
    QPoint pixmapOffset;
    int mouseDownCursorPos;
    QPixmap pixmap;
};

class MLabelViewPrivate : public MWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(MLabelView)

public:
    MLabelViewPrivate();
    virtual ~MLabelViewPrivate();

    const MLabelModel *model() const;
    const MLabelStyle *style() const;
    const QRectF boundingRect() const;

    bool isRichText(QString text) const;

    // need define this for there are overload functions in controller
    MLabel *controller;

    QString cacheKey;
    MLabelViewSimple *impl;
    QTextOption textOptions;
};

#endif
