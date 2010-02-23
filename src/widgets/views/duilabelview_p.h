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

#ifndef DUILABELVIEW_P_H
#define DUILABELVIEW_P_H

#include <QTextOption>
#include <QTextDocument>
#include "private/duiwidgetview_p.h"

class DuiLabel;
class DuiLabelView;
class QGraphicsSceneResizeEvent;


class DuiLabelViewSimple
{
public:
    DuiLabelViewSimple(DuiLabelViewPrivate *viewPrivate);
    virtual ~DuiLabelViewSimple();

    virtual QPixmap generatePixmap();
    virtual void drawContents(QPainter *painter, const QSizeF &size);
    virtual bool resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void setupModel();
    virtual bool updateData(const QList<const char *>& modifications);
    virtual bool isRich();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(DuiCancelEvent *event);
    virtual void longPressEvent(QGraphicsSceneContextMenuEvent *event);
    virtual void applyStyle();

    DuiLabelViewPrivate *viewPrivate;

    QSizeF preferredSize;
    QPoint textOffset;

    QPixmap pixmap;
};

class DuiLabelViewRich : public DuiLabelViewSimple
{

public:
    DuiLabelViewRich(DuiLabelViewPrivate *viewPrivate);
    virtual ~DuiLabelViewRich();

    virtual QPixmap generatePixmap();
    virtual void drawContents(QPainter *painter, const QSizeF &size);
    virtual bool shouldElide() const;
    virtual bool resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void setupModel();
    virtual bool updateData(const QList<const char *>& modifications);
    virtual bool isRich();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(DuiCancelEvent *event);
    virtual void longPressEvent(QGraphicsSceneContextMenuEvent *event);

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
};

class DuiLabelViewPrivate : public DuiWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(DuiLabelView)

public:
    DuiLabelViewPrivate();
    virtual ~DuiLabelViewPrivate();

    const DuiLabelModel *model() const;
    const DuiLabelStyle *style() const;
    const QRectF boundingRect() const;

    // need define this for there are overload functions in controller
    DuiLabel *controller;

    QString cacheKey;
    DuiLabelViewSimple *impl;
    QTextOption textOptions;
};

#endif
