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

#include "duicompleter.h"
#include "duicompleter_p.h"
#include "duitextedit.h"
#include "duiapplicationwindow.h"
#include "duiscenemanager.h"
#include "duiapplication.h"

#include <QDebug>
#include <QString>
#include <QTimer>
#include <QTextDocument>
#include <QAbstractItemModel>
#include <QStringListModel>

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiCompleter)

namespace
{
    //! default delimiters are comma and semicolon.
    const QString DefaultDelimiters(",;");
    //! default match regular expression
    const QString DefaultMatchRegularExpression("(%1.*)||(.*\\W%1.*)");
    //! default maximum hits is 10
    const int DefaultMaximumHits = 10;
    //! time interval for completer
    const int CompleterTimeInterval = 500;
}

DuiCompleterPrivate::DuiCompleterPrivate()
    : delimiters(DefaultDelimiters),
      hasOwnModel(false),
      completionModel(0),
      matchedModel(new DuiCompletionModel),
      completerTimer(new QTimer)
{
}

DuiCompleterPrivate::~DuiCompleterPrivate()
{
    delete matchedModel;
    delete completerTimer;
}

void DuiCompleterPrivate::init()
{
    Q_Q(DuiCompleter);
    q->model()->setMatchedModel(matchedModel);
    completerTimer->setInterval(CompleterTimeInterval);
    completerTimer->setSingleShot(true);
    QObject::connect(completerTimer, SIGNAL(timeout()), q, SLOT(_q_complete()));
}

void DuiCompleterPrivate::reset()
{
    Q_Q(DuiCompleter);
    if (q->model()->popupActive())
        return;
    q->model()->setCompletionPrefix("");
    q->model()->setMatchedIndex(-1);
    matchedIndexList.clear();
    matchedModel->setMatchedList(matchedIndexList);
    completerTimer->stop();
}

void DuiCompleterPrivate::fetchPrefix()
{
    Q_Q(DuiCompleter);
    q->model()->setCompletionPrefix("");

    QGraphicsScene *scene = 0;
    if (!q->widget())
        return;
    else
        scene = q->widget()->scene();

    if (!scene || q->widget() != scene->focusItem())
        return;

    // get surrounding text and cursor position
    QString text;
    int cursorPosition = -1;

    Qt::InputMethodQuery query = Qt::ImSurroundingText;
    QVariant queryResult = scene->inputMethodQuery(query);
    if (queryResult.isValid())
        text = queryResult.toString();
    query = Qt::ImCursorPosition;
    queryResult = scene->inputMethodQuery(query);
    if (queryResult.isValid())
        cursorPosition = queryResult.toInt();

    if (text.isEmpty() || cursorPosition == -1)
        return;

    //if not acceptMultipleEntries(), then don't give suggestions by setting an empty prefix
    if (!q->model()->acceptMultipleEntries()) {
        bool multiEntries = false;
        foreach(const QChar & c, delimiters) {
            if (text.indexOf(c) != -1) {
                multiEntries = true;
                break;
            }
        }
        if (!multiEntries)
            q->model()->setCompletionPrefix(text);
    } else {
        // prefix is the text contains the cursor, and betweens delimiters.
        QString left, right;
        left = text.left(cursorPosition);
        right = text.mid(cursorPosition);
        int leftIndex = -1, rightIndex = right.length(), index = 0;
        foreach(const QChar & c, delimiters) {
            leftIndex = qMax(leftIndex, left.lastIndexOf(c));
            index = right.indexOf(c);
            if (index != -1)
                rightIndex = qMin(rightIndex, index);
        }
        leftIndex++;
        q->model()->setCompletionPrefix(QString(left.mid(leftIndex) + right.left(rightIndex)));
    }

    qDebug() << __PRETTY_FUNCTION__ << q->model()->completionPrefix();
}

void DuiCompleterPrivate::match(int maxHits)
{
    Q_Q(DuiCompleter);
    if (q->model()->completionPrefix().isEmpty())
        return;

    Qt::MatchFlags flags = Qt::MatchRegExp;
    //default match rule, the prefix should occur at the start, of just after one special character
    QString exp = QString(DefaultMatchRegularExpression).arg(q->model()->completionPrefix());
    QVariant var = QVariant(exp);

    matchedIndexList.clear();
    int row = 0;

    //all the columns are used to match
    QModelIndexList list;
    const int columnCount = completionModel->columnCount();
    for (int column = 0; column < columnCount; ++column) {
        list = completionModel->match(completionModel->index(row, column), Qt::DisplayRole, var, maxHits, flags);
        foreach(const QModelIndex & index, list) {
            if (!matchedIndexList.contains(index.row()))
                matchedIndexList << index.row();
        }
    }

    matchedModel->setMatchedList(matchedIndexList);
}

void DuiCompleterPrivate::_q_complete()
{
    Q_Q(DuiCompleter);
    if (!q->widget() || !q->widget()->scene())
        return;
    if (q->model()->popupActive() || (q->widget() != q->widget()->scene()->focusItem()))
        return;

    reset();
    fetchPrefix();
    if (q->model()->completionPrefix().isEmpty()) {
        //if no prefix, just reset and return
        if (q->isActive()) {
            reset();
            q->hideCompleter();
            resetFocus();
        }
        return;
    }

    emit q->startCompleting(q->model()->completionPrefix());

    if (q->receivers(SIGNAL(startCompleting(QString))) <= 0) {
        //if no customized match slot, then use default match rule
        //limit the max hits to DefaultMaximumHits + 1 (+1 allow it to be larger than DefaultMaximumHits)
        match(DefaultMaximumHits + 1);
    } else {
        //if customized, then think all items inside model are matched
        matchedIndexList.clear();
        const int rowCount = completionModel->rowCount();
        for (int i = 0; i < rowCount; ++i)
            matchedIndexList << i;
        matchedModel->setMatchedList(matchedIndexList);
    }

    if (matchedModel->rowCount() > 0) {
        q->model()->setMatchedIndex(0);
        q->model()->setActive(true);
        q->widget()->sceneManager()->appearSceneWindow(q);
        emit q->shown();
    } else if (q->isActive()) {
        reset();
        q->hideCompleter();
        resetFocus();
    }
}

QString DuiCompleterPrivate::trim(const QString &s)
{
    Q_Q(DuiCompleter);
    QString after = s;
    QString trimSet = q->model()->charactersToTrim();
    if (!trimSet.isEmpty()) {
        foreach(const QString & c, s) {
            if (trimSet.indexOf(c) == -1)
                break;
            else
                after = after.mid(1);
        }
        for (int i = after.length() - 1; i > 0; --i) {
            if (trimSet.indexOf(after[i]) == -1)
                break;
            else
                after = after.left(i);
        }
    }
    return after;
}

void DuiCompleterPrivate::setCompletionModel(QAbstractItemModel *m, bool own)
{
    Q_Q(DuiCompleter);
    if (m == completionModel)
        return;

    if (completionModel) {
        QObject::disconnect(completionModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
                            q, SLOT(_q_modelUpdate()));
    }
    if (hasOwnModel && completionModel) {
        matchedIndexList.clear();
        matchedModel->setSourceModel(0);
        delete completionModel;
    }

    completionModel = m;
    matchedModel->setSourceModel(completionModel);
    hasOwnModel = own;
    //default value column is the last column
    q->model()->setValueColumnIndex(completionModel->columnCount() - 1);
    QObject::connect(completionModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
                     q, SLOT(_q_modelUpdate()));
    _q_modelUpdate();
}

void DuiCompleterPrivate::_q_modelUpdate()
{
    if (!completionModel)
        return;
    matchedIndexList.clear();
    matchedModel->setMatchedList(matchedIndexList);
}

void DuiCompleterPrivate::resetFocus()
{
    Q_Q(DuiCompleter);
    if (!q->widget() || !q->widget()->scene())
        return;
    //set the focus back to textwidget
    q->widget()->scene()->setFocusItem(q->widget());
    DuiSceneManager *targetSceneManager = DuiApplication::activeWindow()->sceneManager();
    if (targetSceneManager)
        targetSceneManager->requestSoftwareInputPanel(q->widget());
}

DuiCompleter::DuiCompleter()
    : DuiSceneWindow(new DuiCompleterPrivate, new DuiCompleterModel,
                     DuiSceneWindow::Completer, "default")
{
    Q_D(DuiCompleter);
    d->init();
}

DuiCompleter::DuiCompleter(const QStringList &completionCandidates)
    : DuiSceneWindow(new DuiCompleterPrivate, new DuiCompleterModel,
                     DuiSceneWindow::Completer, "default")
{
    Q_D(DuiCompleter);
    d->setCompletionModel(new QStringListModel(completionCandidates), true);
    d->init();
}

DuiCompleter::DuiCompleter(QAbstractItemModel *completionCandidates)
    : DuiSceneWindow(new DuiCompleterPrivate, new DuiCompleterModel,
                     DuiSceneWindow::Completer, "default")
{
    Q_D(DuiCompleter);
    d->setCompletionModel(completionCandidates);
    d->init();
}

DuiCompleter::~DuiCompleter()
{
}

QString DuiCompleter::completionPrefix() const
{
    return model()->completionPrefix();
}

void DuiCompleter::setCandidateSourceModel(QAbstractItemModel *model)
{
    Q_D(DuiCompleter);
    hideCompleter();
    d->setCompletionModel(model);
}

QAbstractItemModel *DuiCompleter::candidateSourceModel() const
{
    Q_D(const DuiCompleter);
    return d->completionModel;
}

QAbstractItemModel *DuiCompleter::matchedCandidateModel() const
{
    Q_D(const DuiCompleter);

    return d->matchedModel;
}

void DuiCompleter::hideCompleter()
{
    if (isActive()) {
        widget()->sceneManager()->disappearSceneWindow(this);
        model()->setActive(false);
        emit hidden();
    }
}

void DuiCompleter::setWidget(DuiWidget *widget)
{
    //verify whether the widget supports inputmethod
    if (!widget || !(widget->flags() && QGraphicsItem::ItemAcceptsInputMethod)) {
        qWarning() << __PRETTY_FUNCTION__ << ": invalid DuiWidget pointer!\nThe valid DuiWidget must \
support inputmethod (flags() with QGraphicsItem::ItemAcceptsInputMethod); \
respond to inputMethodQuery() for Qt::ImCursorPosition and Qt::ImSurroundingText; \
and insert the confirmed completion when receiving the signal confirmed().";
        return;
    }

    if (this->widget()) {
        this->widget()->removeEventFilter(this);
    }
    widget->installEventFilter(this);

    model()->setTextWidget(widget);
}

DuiWidget *DuiCompleter::widget()
{
    return model()->textWidget();
}

void DuiCompleter::setValueColumnIndex(int column)
{
    Q_D(DuiCompleter);
    if (column < 0 || column >= d->completionModel->columnCount())
        return;
    else
        model()->setValueColumnIndex(column);
}

int DuiCompleter::valueColumnIndex() const
{
    return model()->valueColumnIndex();
}

void DuiCompleter::complete()
{
    Q_D(DuiCompleter);
    d->completerTimer->start();
}

void DuiCompleter::confirm()
{
    Q_D(DuiCompleter);
    if (!widget() || (!isActive() && !model()->popupActive()))
        return;

    //fetch prefix again, ensure the prefix is not null
    d->fetchPrefix();
    if (model()->completionPrefix().isEmpty())
        return;

    if (model()->matchedIndex() >= 0 && model()->matchedIndex() < d->matchedModel->rowCount()) {
        //insert confirmed string
        int row = 0;
        int column = model()->valueColumnIndex();
        QString value;
        if (d->matchedIndexList.count() > model()->matchedIndex())
            row = d->matchedIndexList.at(model()->matchedIndex());
        QVariant var = d->completionModel->data(d->completionModel->index(row, column));
        if (var.isValid())
            value = var.toString();
        //trim before inserting
        value = d->trim(value);
        emit confirmed(value);

        d->reset();
        hideCompleter();
        d->resetFocus();
    }
}

bool DuiCompleter::acceptMultipleEntries() const
{
    return model()->acceptMultipleEntries();
}

void DuiCompleter::setAcceptMultipleEntries(bool enable)
{
    model()->setAcceptMultipleEntries(enable);
}

bool DuiCompleter::isActive() const
{
    return model()->active();
}

void DuiCompleter::queryAll()
{
    Q_D(DuiCompleter);
    if (!widget() || !widget()->scene())
        return;
    if (model()->popupActive() || (widget() != widget()->scene()->focusItem()))
        return;
    if (receivers(SIGNAL(startCompleting(QString))) > 0)
        return;

    d->reset();
    d->fetchPrefix();
    if (model()->completionPrefix().isEmpty())
        return;
    //if no customized match slot, then use default match rule. -1 to query all.
    d->match(-1);
}

void DuiCompleter::appear(DuiApplicationWindow *window, DuiSceneWindow::DeletionPolicy policy)
{
    DuiSceneWindow::appear(window, policy);
    model()->setActive(true);
}

void DuiCompleter::appearNow(DuiApplicationWindow *window, DuiSceneWindow::DeletionPolicy policy)
{
    DuiSceneWindow::appearNow(window, policy);
    model()->setActive(true);
}

void DuiCompleter::appear(DuiSceneWindow::DeletionPolicy policy)
{
    appear(0, policy);
}

void DuiCompleter::appearNow(DuiSceneWindow::DeletionPolicy policy)
{
    appearNow(0, policy);
}

void DuiCompleter::disappear()
{
    DuiSceneWindow::disappear();
    model()->setActive(false);
}

void DuiCompleter::disappearNow()
{
    DuiSceneWindow::disappearNow();
    model()->setActive(false);
}

bool DuiCompleter::eventFilter(QObject *object, QEvent *e)
{
    Q_D(DuiCompleter);
    bool eaten = false;
    if (isActive() && object == widget()) {
        switch (e->type()) {
        case QEvent::GraphicsSceneMouseRelease:
            //User relocates the cursor by tapping the text entry will hide completer
            d->reset();
            hideCompleter();
            d->resetFocus();
            break;
        case QEvent::KeyPress: {
            QKeyEvent *key = static_cast<QKeyEvent *>(e);
            switch (key->key()) {
            case Qt::Key_Return:
                //Enter key will insert the top hit
                confirm();
                eaten = true;
                break;
            default:
                break;
            }
            break;
        }
        default:
            break;
        }
    }
    return eaten;
}

QString DuiCompleter::charactersToTrim() const
{
    return model()->charactersToTrim();
}

void DuiCompleter::setCharactersToTrim(const QString &set)
{
    return model()->setCharactersToTrim(set);
}

DuiCompletionModel::DuiCompletionModel(QObject *parent)
    : QAbstractProxyModel(parent)
{
}

void DuiCompletionModel::setMatchedList(const QList<int>& indexList)
{
    if (QAbstractProxyModel::sourceModel() && (indexList.count() <= QAbstractProxyModel::sourceModel()->rowCount())) {
        this->indexList = indexList;
        QAbstractProxyModel::endResetModel();
    }
}

void DuiCompletionModel::setSourceModel(QAbstractItemModel *source)
{
    bool hadModel = (QAbstractProxyModel::sourceModel() != 0);

    if (hadModel)
        QObject::disconnect(QAbstractProxyModel::sourceModel(), 0, this, 0);

    QAbstractProxyModel::setSourceModel(source);

    if (source)
        connect(source, SIGNAL(destroyed()), this, SLOT(modelDestroyed()));
}

QModelIndex DuiCompletionModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column, 0) : QModelIndex();
}

QModelIndex DuiCompletionModel::mapToSource(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() >= indexList.count())
        return QModelIndex();

    return createIndex(indexList.at(index.row()), index.column());
}

QModelIndex DuiCompletionModel::mapFromSource(const QModelIndex &index) const
{
    if (!index.isValid() || !indexList.contains(index.row()))
        return QModelIndex();

    return createIndex(indexList.indexOf(index.row()), 0);
}

QVariant DuiCompletionModel::data(const QModelIndex &index, int role) const
{
    QVariant var;
    int row = index.row();
    int column = index.column();
    if (!QAbstractProxyModel::sourceModel() || (row < 0)
            || (row >= indexList.count()) || (column != 0))
        return var;

    var = QAbstractProxyModel::sourceModel()->data(mapToSource(index), role);
    if (role == Qt::DisplayRole) {
        QString value;
        const int columnCount = QAbstractProxyModel::sourceModel()->columnCount();
        for (column = 0; column < columnCount; ++column) {
            var = QAbstractProxyModel::sourceModel()->data(mapToSource(createIndex(row, column)), role);
            if (var.isValid()) {
                //space between columns
                if (column > 0)
                    value += ' ';
                value += var.toString();
            }
        }
        if (!value.isEmpty())
            var = QVariant(value);
    }
    return var;
}

int DuiCompletionModel::columnCount(const QModelIndex &) const
{
    //FIXME: Do we need to display more columns? Now DuiPopupList can only display one column.
    if (QAbstractProxyModel::sourceModel())
        return 1;
    else
        return 0;
}

int DuiCompletionModel::rowCount(const QModelIndex &) const
{
    if (QAbstractProxyModel::sourceModel())
        return indexList.count();
    else
        return 0;
}

QModelIndex DuiCompletionModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

void DuiCompletionModel::modelDestroyed()
{
    // switch to static empty model
    QAbstractProxyModel::setSourceModel(0);
}

#include "moc_duicompleter.cpp"
