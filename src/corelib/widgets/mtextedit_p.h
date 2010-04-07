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

#ifndef MTEXTEDIT_P_H
#define MTEXTEDIT_P_H

#include <QTextCursor>
#include <QString>
#include <QList>
#include <QInputMethodEvent>

class QGraphicsSceneMouseEvent;
class QValidator;
class MCompleter;

#include "private/mwidgetcontroller_p.h"
#include "mtextedit.h"

class MNavigationBar;

class MTextEditPrivate : public MWidgetControllerPrivate
{
    Q_DECLARE_PUBLIC(MTextEdit)

public:
    MTextEditPrivate();
    virtual ~MTextEditPrivate();

    QTextCursor *cursor() const;
    void moveCursor(QTextCursor::MoveOperation moveOp,
                    QTextCursor::MoveMode moveMode = QTextCursor::MoveAnchor, int n = 1);

    bool doBackspace();
    bool doDelete();
    bool doTab();
    bool doTextInsert(const QString &text);
    bool onReturnPressed(QKeyEvent *event);
    bool doSignCycle();

    bool validateCurrentBlock();

    bool setCursorPosition(int index);

    void setPreeditText(const QString &text,
                        const QList<QInputMethodEvent::Attribute> &attributes);
    void commitPreedit();
    void removePreedit();
    bool isPositionOnPreedit(int cursorPosition) const;
    bool isPreediting() const;

    void setMode(MTextEditModel::EditMode mode);

    void notifyInputContextMouseHandler(int position, QGraphicsSceneMouseEvent *event);

    void sendCopyAvailable(bool yes);

    static QEvent::Type translateGraphicsSceneMouseTypeToQMouse(QEvent::Type input);

    void _q_confirmCompletion(const QString &);

    bool copy();

private:
    bool pendingSoftwareInputPanelRequest;
    const QValidator *validator;
    bool ownValidator; // setting content type creates a validator that the widget owns

    MCompleter *completer;

    bool editActive; // true if editing started after getting focus
};

#endif
