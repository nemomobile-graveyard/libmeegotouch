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

#ifndef MCOMPLETERVIEW_P_H
#define MCOMPLETERVIEW_P_H

#include "mcompleter.h"
#include "mcompleterview.h"
#include "private/mwidgetview_p.h"
#include <MButton>
#include <MProgressIndicator>

class MCompleter;
class MWidgetController;
class MLabel;
class MButton;
class MPopupList;
class QGraphicsLinearLayout;
class CompletionCountButton;

//! \internal
class MCompleterViewPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(MCompleterView)

public:
    MCompleterViewPrivate(MCompleter *controller, MCompleterView *q);
    virtual ~MCompleterViewPrivate();

protected slots:

    void createContents();

    void clear();

    void showPopup();

    void refocusPopup();

    void updatePosition();
    QRect cursorRectangle() const;
    QRect widgetRectangle() const;
    QRectF mapRectToRoot(const QGraphicsItem *item, const QRectF &itemRect) const;
    QRectF mapSceneRectToRoot(const QRectF &sceneRect) const;

    void handlePopupAppearing();
    void handlePopupDisappearing();
    void handlePopupDisappeared();

    void handleCompleterShown();

    void handleCompleterHidden();

    void showCompletionsButton();
    void hideCompletionsButton();

protected:
    void init();

    void updateCompletionLabelText();
    void updateCompletionsButton();

    //! The MCompleter controller
    MCompleter *controller;
    MCompleterView *q_ptr;

    MLabel *completionLabel;
    CompletionCountButton *completionsButton;
    QGraphicsLinearLayout *layout;
    MPopupList *popup;

    int popupSelectedIndex;
};

//! CompletionCountButton is a button used to show matching completions.
//! It contains a spinner which can be enabled and disabled. When shown,
//! the spinner is replacing button borders.
class CompletionCountButton : public MButton
{
    Q_OBJECT
public:
    CompletionCountButton(QGraphicsItem *parent = 0);
    virtual ~CompletionCountButton();

    void setProgressIndicatorVisible(bool visible);
    bool progressIndicatorVisible() const;

    void setAllowMouseInteraction(bool allow);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    MProgressIndicator spinner;
    bool allowMouseInteraction;
};

//! \internal_end

#endif
