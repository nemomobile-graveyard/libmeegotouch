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

#ifndef CUSTOMSELECTABLELABEL_H
#define CUSTOMSELECTABLELABEL_H

#include <MRichTextEdit>
#include <MWidget>

class MLabel;
class MTextEditModel;
class MLayout;
class MLinearLayoutPolicy;
class MLabelHighlighter;
class QTimer;

class CustomEdit : public MRichTextEdit
{
    Q_OBJECT

public:
    CustomEdit(QGraphicsItem *parent = 0) : MRichTextEdit(MTextEditModel::MultiLine, QString(), parent) {}
    virtual ~CustomEdit() {}

    bool startSelection(const QPointF &hitPoint);
};

class CustomSelectableLabel : public MWidget
{
    Q_OBJECT

public:
    CustomSelectableLabel(QString const &text = "", QGraphicsItem *parent = 0);
    virtual ~CustomSelectableLabel();

    void setStyleName(const QString &name);

    void setText(const QString &text);

    void addHighlighter(MLabelHighlighter *highlighter);
    void removeHighlighter(MLabelHighlighter *highlighter);
    void removeAllHighlighters();

protected:
    virtual void tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture* gesture);

    bool selecting;
    MLabel *label;
    CustomEdit *edit;
    const MTextEditModel *editModel;
    MLayout *layout;
    MLinearLayoutPolicy *labelPolicy;
    MLinearLayoutPolicy *editPolicy;
    QTimer *hitTimer;
    QPointF hitPoint;

protected Q_SLOTS:
    void switchToEdit();
    void switchToLabel();

    void hitTimeout();
    void onEditModelModified(const QList<const char *>& modifications);
};


#endif // CUSTOMSELECTABLELABEL_H
