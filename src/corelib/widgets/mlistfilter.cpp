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

#include "mlistfilter.h"
#include "mlistfilter_p.h"

#include <MList>
#include <MTextEdit>
#include <MPannableViewport>

#include <QKeyEvent>
#include <QPointF>
#include <MSortFilterProxyModel>

MListFilterPrivate::MListFilterPrivate() :
    pullStartPos(QPointF(0,0)),
    pullStarted(false),
    cachedPannableViewport(NULL),
    q_ptr(NULL)
{
}

MListFilterPrivate::~MListFilterPrivate()
{
    Q_Q(MListFilter);

    disconnect(q->list, SIGNAL(geometryChanged()), this, SLOT(updatePannableViewport()));
}

void MListFilterPrivate::init()
{
    Q_Q(MListFilter);

    updatePannableViewport();
    connect(q->list, SIGNAL(geometryChanged()), this, SLOT(updatePannableViewport()));
}

void MListFilterPrivate::_q_moveFocusToList()
{
    Q_Q(MListFilter);

    if (q->filterEditor)
        q->filterEditor->clearFocus();

    if (q->list)
        q->list->setFocus();
}

void MListFilterPrivate::updatePannableViewport()
{
    MPannableViewport *viewPort = pannableViewport();
    if (viewPort != cachedPannableViewport) {
        if (cachedPannableViewport) {
            disconnect(cachedPannableViewport->physics(),
                       SIGNAL(pointerPressed()),
                       this,
                       SLOT(startEditorPullDown()));
            disconnect(cachedPannableViewport->physics(),
                       SIGNAL(pointerReleased()),
                       this,
                       SLOT(stopEditorPullDown()));

            disconnect(cachedPannableViewport,
                       SIGNAL(positionChanged(QPointF)),
                       this,
                       SLOT(checkEditorPullDistance(QPointF)));
        }

        if (viewPort) {
            connect(viewPort->physics(),
                    SIGNAL(pointerPressed()),
                    this,
                    SLOT(startEditorPullDown()),
                    Qt::UniqueConnection);
            connect(viewPort->physics(),
                    SIGNAL(pointerReleased()),
                    this,
                    SLOT(stopEditorPullDown()),
                    Qt::UniqueConnection);

            connect(viewPort,
                    SIGNAL(positionChanged(QPointF)),
                    this,
                    SLOT(checkEditorPullDistance(QPointF)),
                    Qt::UniqueConnection);
        }
    }
    cachedPannableViewport = viewPort;
}

void MListFilterPrivate::startEditorPullDown()
{
    Q_Q(MListFilter);

    pullStartPos = cachedPannableViewport->position();
    if (q->filteringEnabled && !q->editor()->isVisible() && pullStartPos.y() <= 0)
        pullStarted = true;
}

void MListFilterPrivate::checkEditorPullDistance(const QPointF& pos)
{
    Q_Q(MListFilter);

    if (!pullStarted)
        return;

    qreal distance = pullStartPos.y() - pos.y();
    if (distance >= q->filterEditor->preferredHeight()) {
         emit q->listPannedUpFromTop();
         stopEditorPullDown();
     } else if (distance < 0)
         stopEditorPullDown(); // wrong direction
}

void MListFilterPrivate::stopEditorPullDown()
{
    pullStarted = false;
}

MPannableViewport* MListFilterPrivate::pannableViewport()
{
    Q_Q(MListFilter);

    MPannableViewport *pannableViewport = NULL;
    QGraphicsWidget *parentWidget = q->list->parentWidget();
    while (parentWidget && !pannableViewport) {
        pannableViewport = qobject_cast<MPannableViewport*>(parentWidget);
        parentWidget = parentWidget->parentWidget();
    }
    return pannableViewport;
}

MListFilter::MListFilter(MList *parent)
    : list(parent),
      filterEditor(NULL),
      filterProxy(NULL),
      filteringEnabled(false),
      d_ptr(new MListFilterPrivate),
      filteringMode(MListFilter::FilterAsSubstring)
{
    Q_D(MListFilter);

    d->q_ptr = this;
    d->init();

    filterEditor = new MTextEdit(MTextEditModel::SingleLine, "", parent);
    filterEditor->setInputMethodAutoCapitalizationEnabled(false);
    filterEditor->setInputMethodCorrectionEnabled(false);
    filterEditor->setInputMethodPredictionEnabled(false);
    filterEditor->setVisible(false);

    filterProxy = new MSortFilterProxyModel(parent);
    QRegExp::PatternSyntax syntax = QRegExp::RegExp;
    Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive;
    QRegExp regExp(filterEditor->text(), caseSensitivity, syntax);
    filterProxy->setFilterRegExp(regExp);

    connect(filterEditor, SIGNAL(textChanged()), this, SLOT(editorTextChanged()));
    connect(list, SIGNAL(indexScrollStarted()), this, SLOT(_q_moveFocusToList()));
}

MListFilter::~MListFilter()
{
    delete d_ptr;
}

void MListFilter::setEnabled(bool enabled)
{
    filteringEnabled = enabled;

    if (enabled) {
        filterProxy->setSourceModel(list->itemModel());
        list->setItemModel(filterProxy);
        list->setFlag(QGraphicsItem::ItemIsFocusable, true);
        list->setFocusPolicy(Qt::StrongFocus);
        list->setFocus();
    } else {
        list->setFlag(QGraphicsItem::ItemIsFocusable, false);
        if (filterProxy->sourceModel())
            list->setItemModel(filterProxy->sourceModel());
        filterProxy->setSourceModel(NULL);
    }
}

QAbstractItemModel* MListFilter::updateItemModel(QAbstractItemModel *itemModel)
{
    QAbstractItemModel* proxyModel = itemModel;
    if (filteringEnabled && itemModel != filterProxy) {
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
    if (filteringEnabled && event->text()[0].isPrint()) {
        filterEditor->insert(event->text());
        filterEditor->setFocus();
    }
}

void MListFilter::editorTextChanged()
{
    if (filteringMode != MListFilter::FilterByApplication) {

        // Split the sentence in filter editor
        QStringList keywords(filterEditor->text().split(" "));

        // Escape each keyword
        QStringList escapedKeywords;
        foreach (QString keyword, keywords)
            escapedKeywords << QRegExp::escape(keyword);

        // Join keywords into regular expression pattern
        QString pattern(escapedKeywords.join(".* .*") + ".*");
        if(filteringMode == MListFilter::FilterAsSubstring)
            pattern.prepend(".*");
        else if(filteringMode == MListFilter::FilterAsBeginningOfLine)
            pattern.prepend("^");

        // Set regular expression with the new pattern
        QRegExp regExp(filterProxy->filterRegExp());
        regExp.setPattern(pattern);
        filterProxy->setFilterRegExp(regExp);
    }
}

void MListFilter::setFilterMode(MListFilter::FilterMode mode)
{
    filteringMode = mode;
}

MListFilter::FilterMode MListFilter::filterMode() const
{
    return filteringMode;
}

#include "moc_mlistfilter.cpp"
