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

#include "mlistfilter.h"
#include "mlistfilter_p.h"

#include <MList>
#include <MTextEdit>
#include <MPannableViewport>

#include <QKeyEvent>
#include <QPointF>
#include <MSortFilterProxyModel>

MListFilterPrivate::MListFilterPrivate() :
    viewportPos(QPointF(0,0)),
    panningStartPos(QPointF(0,0)),
    cachedPannableViewport(NULL),
    q_ptr(NULL)
{
}

MListFilterPrivate::~MListFilterPrivate()
{
}

void MListFilterPrivate::init()
{
    Q_Q(MListFilter);
    connect(q->list, SIGNAL(panningStarted()), this, SLOT(panningStarted()));
}

void MListFilterPrivate::panningStarted()
{
    // When list's pannable viewport can be resolved for the first time,
    // connect a signal to get viewport position changes
    if(!cachedPannableViewport && pannableViewport()) {
        MPannableViewport* viewport = pannableViewport();
        connect(viewport, SIGNAL(positionChanged(QPointF)), this, SLOT(viewportPositionChanged(QPointF)));
    }
    panningStartPos = viewportPos;
}

void MListFilterPrivate::viewportPositionChanged(const QPointF& pos)
{
    Q_Q(MListFilter);

    viewportPos = pos;
    if(q->filteringEnabled && viewportPos.y() < 0 && panningStartPos.y() == 0) {
        emit q->listPannedUpFromTop();
        panningStartPos = pos;
    }
}

MPannableViewport* MListFilterPrivate::pannableViewport()
{
    Q_Q(MListFilter);

    if(!cachedPannableViewport) {
        QGraphicsWidget* parentWidget = q->list->parentWidget();
        while(parentWidget && !cachedPannableViewport) {
            cachedPannableViewport = qobject_cast<MPannableViewport*>(parentWidget);
            parentWidget = parentWidget->parentWidget();
        }
    }
    return cachedPannableViewport;
}

MListFilter::MListFilter(MList *parent)
: list(parent), filterEditor(NULL), filterProxy(NULL), filteringEnabled(false), d_ptr(new MListFilterPrivate)
{
    Q_D(MListFilter);

    d->q_ptr = this;
    d->init();

    filterProxy = new MSortFilterProxyModel(parent);
    filterEditor = new MTextEdit(MTextEditModel::SingleLine, "", parent);
    filterEditor->setInputMethodAutoCapitalizationEnabled(false);
    filterEditor->setInputMethodCorrectionEnabled(false);
    filterEditor->setInputMethodPredictionEnabled(false);
    filterEditor->setVisible(false);

    connect(filterEditor, SIGNAL(textChanged()), this, SLOT(editorTextChanged()));
}

MListFilter::~MListFilter()
{
}

void MListFilter::setEnabled(bool enabled)
{
    filteringEnabled = enabled;

    if(enabled) {
        filterProxy->setSourceModel(list->itemModel());
        list->setItemModel(filterProxy);
        list->setFlag(QGraphicsItem::ItemIsFocusable, true);
        list->setFocusPolicy(Qt::StrongFocus);
        list->setFocus();
    } else {
        list->setFlag(QGraphicsItem::ItemIsFocusable, false);
        if(filterProxy->sourceModel())
            list->setItemModel(filterProxy->sourceModel());
        filterProxy->setSourceModel(NULL);
    }
}

QAbstractItemModel* MListFilter::updateItemModel(QAbstractItemModel *itemModel)
{
    QAbstractItemModel* proxyModel = itemModel;
    if(filteringEnabled && itemModel != filterProxy) {
        filterProxy->setSourceModel(itemModel);
        proxyModel = filterProxy;
    }
    return proxyModel;
}

bool MListFilter::enabled()
{
    return filteringEnabled;
}

MTextEdit *MListFilter::editor() const
{
    return filterEditor;
}

MSortFilterProxyModel *MListFilter::proxy() const
{
    return filterProxy;
}

void MListFilter::setFilterRole(int role)
{
    filterProxy->setFilterRole(role);
}

int MListFilter::filterRole() const
{
    return filterProxy->filterRole();
}

void MListFilter::keyPressEvent(QKeyEvent *event)
{
    if(filteringEnabled && event->text()[0].isPrint()) {
        filterEditor->insert(event->text());
        filterEditor->setFocus();
    }
}

void MListFilter::editorTextChanged()
{
    QRegExp::PatternSyntax syntax = QRegExp::RegExp;
    Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive;
    QRegExp regExp(filterEditor->text(), caseSensitivity, syntax);
    filterProxy->setFilterRegExp(regExp);
}
