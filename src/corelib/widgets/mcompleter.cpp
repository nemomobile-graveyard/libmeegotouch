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

#include "mcompleter.h"
#include "mcompleter_p.h"
#include "mtextedit.h"
#include "mapplicationwindow.h"
#include "mscenemanager.h"
#include "mapplication.h"
#include "minputmethodstate.h"

#include <QDebug>
#include <QString>
#include <QTimer>
#include <QTextDocument>
#include <QAbstractItemModel>
#include <QStringListModel>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MCompleter)

namespace
{
    //! default delimiters are comma and semicolon.
    const QString DefaultDelimiters(QString(",;") + QString(QChar::ObjectReplacementCharacter));
    //! default match regular expression
    const QString DefaultMatchRegularExpression("(%1.*)||(.*\\W%1.*)");
    //! default maximum hits is 10
    const int DefaultMaximumHits = 10;
    //! time interval for completer
    const int CompleterTimeInterval = 500;
}

MCompleterPrivate::MCompleterPrivate()
    : delimiters(DefaultDelimiters),
      hasOwnModel(false),
      completionModel(0),
      matchedModel(new MCompletionModel),
      completerTimer(new QTimer)
{
}

MCompleterPrivate::~MCompleterPrivate()
{
    delete matchedModel;
    delete completerTimer;
}

void MCompleterPrivate::init()
{
    Q_Q(MCompleter);
    q->model()->setMatchedModel(matchedModel);
    completerTimer->setInterval(CompleterTimeInterval);
    completerTimer->setSingleShot(true);

    bool connected = true;
    connected = q->connect(completerTimer, SIGNAL(timeout()),
                           q, SLOT(_q_complete()),
                           Qt::UniqueConnection) && connected;

    connected = q->connect(q, SIGNAL(confirmed(QString,QModelIndex)),
                           q, SIGNAL(confirmed(QString)),
                           Qt::UniqueConnection) && connected;

    connected = q->connect(q, SIGNAL(startCompleting(QString,QModelIndex)),
                           q, SIGNAL(startCompleting(QString)),
                           Qt::UniqueConnection) && connected;

    if (!connected) {
        qWarning("MCompleterPrivate::init() - Failed to connect signals!");
    }
}

void MCompleterPrivate::reset()
{
    Q_Q(MCompleter);
    if (q->model()->popupActive())
        return;
    q->model()->setCompletionPrefix("");
    q->model()->setMatchedIndex(-1);
    matchedIndexList.clear();
    matchedModel->setMatchedList(matchedIndexList);
    completerTimer->stop();
}

void MCompleterPrivate::fetchPrefix()
{
    Q_Q(MCompleter);
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

    QString prefix;
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
            prefix = text;
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
        prefix = QString(left.mid(leftIndex) + right.left(rightIndex));
    }

    // trim for prefix
    QString trimSet = q->model()->charactersToTrimForCompletionPrefix();
    if (!prefix.isEmpty() && !trimSet.isEmpty()) {
        // trim left
        while (!prefix.isEmpty() && trimSet.contains(prefix.at(0))) {
            prefix = prefix.right(prefix.length() - 1);
        }
        // trim right
        while (!prefix.isEmpty() && trimSet.contains(prefix.at(prefix.length() - 1))) {
            prefix = prefix.left(prefix.length() - 1);
        }
    }

    q->model()->setCompletionPrefix(prefix);

    qDebug() << __PRETTY_FUNCTION__ << q->model()->completionPrefix();
}

void MCompleterPrivate::match(int maxHits)
{
    Q_Q(MCompleter);
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

void MCompleterPrivate::_q_complete()
{
    Q_Q(MCompleter);
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

    emit q->startCompleting(q->model()->completionPrefix(),
                            completionModel->index(q->model()->matchedIndex(), q->valueColumnIndex()));
    pollModel(true);
}

QString MCompleterPrivate::trim(const QString &s)
{
    Q_Q(MCompleter);
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

void MCompleterPrivate::setCompletionModel(QAbstractItemModel *m, bool own)
{
    Q_Q(MCompleter);
    if (m == completionModel)
        return;

    if (completionModel) {
        QObject::disconnect(completionModel, 0, q, 0);
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
                     q, SLOT(_q_pollModel()));
    QObject::connect(completionModel, SIGNAL(modelAboutToBeReset()),
                     q, SLOT(_q_modelAboutToBeReset()));
    QObject::connect(completionModel, SIGNAL(modelReset()),
                     q, SLOT(_q_pollModel()));
    QObject::connect(completionModel, SIGNAL(rowsInserted(QModelIndex, int, int)),
                     q, SLOT(_q_pollModel()));
    QObject::connect(completionModel, SIGNAL(rowsRemoved(QModelIndex, int, int)),
                     q, SLOT(_q_rowsRemoved(QModelIndex, int, int)));

    _q_modelUpdate();
}

void MCompleterPrivate::pollModel(bool isResetFocus)
{
    Q_Q(MCompleter);

    // TODO #1: remove check for one of those signals once they have been merged.
    // TODO #2: Change the "<= 1" back to "<= 0", because this is only needed for
    //          the current signal forwarding!
    if ((q->receivers(SIGNAL(startCompleting(QString))) <= 0) &&
        (q->receivers(SIGNAL(startCompleting(QString, QModelIndex))) <= 1)) {
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
    updateScene(isResetFocus);
}

void MCompleterPrivate::updateScene(bool isResetFocus)
{
    Q_Q(MCompleter);

    if (matchedModel->rowCount() > 0) {
        q->model()->setMatchedIndex(0);
        q->model()->setActive(true);
        q->widget()->sceneManager()->appearSceneWindowNow(q);
        emit q->shown();
    } else if (q->isActive()) {
        reset();
        q->hideCompleter();
        if (isResetFocus)
            resetFocus();
    }
}

void MCompleterPrivate::_q_modelUpdate()
{
    Q_Q(MCompleter);
    if (!completionModel)
        return;
    matchedIndexList.clear();
    matchedModel->setMatchedList(matchedIndexList);
    if (q->isActive())
        pollModel(false);
}

void MCompleterPrivate::_q_pollModel()
{
    pollModel(false);
}

void MCompleterPrivate::_q_modelAboutToBeReset()
{
    Q_Q(MCompleter);
    reset();
    q->hideCompleter();
}

void MCompleterPrivate::_q_rowsRemoved(const QModelIndex& parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_Q(MCompleter);
    if (start < 0 || end < 0 || start > end) {
        qDebug() << "[MCompleterPrivate::onRowsRemoved] Invalid parameters!";
        return;
    }

    for (int i = start; i <= end; i++) {
        for (QList<int>::iterator it = matchedIndexList.begin(); it != matchedIndexList.end(); ++it) {
            if (*it == i) {
                matchedIndexList.erase(it);
                break;
            }
        }
    }
    matchedModel->setMatchedList(matchedIndexList);
    if (q->isActive())
        updateScene(true);
}

void MCompleterPrivate::resetFocus()
{
    Q_Q(MCompleter);

    if (!q->widget() || !q->widget()->scene()) {
        return;
    }

    // set the focus back to text widget if really lose focus
    if (q->widget()->scene()->focusItem() != q->widget()) {
        q->widget()->scene()->setFocusItem(q->widget());
        MInputMethodState::requestSoftwareInputPanel();
    }
}

MCompleter::MCompleter()
    : MSceneWindow(new MCompleterPrivate, new MCompleterModel,
                     MSceneWindow::Completer, "default")
{
    Q_D(MCompleter);
    d->init();
}

MCompleter::MCompleter(const QStringList &completionCandidates)
    : MSceneWindow(new MCompleterPrivate, new MCompleterModel,
                     MSceneWindow::Completer, "default")
{
    Q_D(MCompleter);
    d->setCompletionModel(new QStringListModel(completionCandidates), true);
    d->init();
}

MCompleter::MCompleter(QAbstractItemModel *completionCandidates)
    : MSceneWindow(new MCompleterPrivate, new MCompleterModel,
                     MSceneWindow::Completer, "default")
{
    Q_D(MCompleter);
    d->setCompletionModel(completionCandidates);
    d->init();
}

MCompleter::~MCompleter()
{
}

QString MCompleter::completionPrefix() const
{
    return model()->completionPrefix();
}

void MCompleter::setCandidateSourceModel(QAbstractItemModel *model)
{
    Q_D(MCompleter);
    hideCompleter();
    d->setCompletionModel(model);
}

QAbstractItemModel *MCompleter::candidateSourceModel() const
{
    Q_D(const MCompleter);
    return d->completionModel;
}

QAbstractItemModel *MCompleter::matchedCandidateModel() const
{
    Q_D(const MCompleter);

    return d->matchedModel;
}

void MCompleter::hideCompleter()
{
    if (isActive()) {
        // change active state to false before really hidden to notify view
        // to do necessary clear.
        model()->setActive(false);
        widget()->sceneManager()->disappearSceneWindowNow(this);
        emit hidden();
    }
}

void MCompleter::setWidget(MWidget *widget)
{
    if (widget == this->widget())
        return;
    //verify whether the widget supports inputmethod
    if (!widget || !(widget->flags() && QGraphicsItem::ItemAcceptsInputMethod)) {
        qWarning() << __PRETTY_FUNCTION__ << ": invalid MWidget pointer!\nThe valid MWidget must \
support inputmethod (flags() with QGraphicsItem::ItemAcceptsInputMethod); \
respond to inputMethodQuery() for Qt::ImCursorPosition and Qt::ImSurroundingText; \
and insert the confirmed completion when receiving the signal confirmed().";
        if (this->widget()) {
            this->widget()->removeEventFilter(this);
            hideCompleter();
        }
        model()->setTextWidget(0);
        return;
    }

    if (this->widget()) {
        this->widget()->removeEventFilter(this);
    }
    widget->installEventFilter(this);

    model()->setTextWidget(widget);
}

MWidget *MCompleter::widget()
{
    return model()->textWidget();
}

void MCompleter::setValueColumnIndex(int column)
{
    Q_D(MCompleter);
    if (column < 0 || column >= d->completionModel->columnCount())
        return;
    else
        model()->setValueColumnIndex(column);
}

int MCompleter::valueColumnIndex() const
{
    return model()->valueColumnIndex();
}

void MCompleter::complete()
{
    Q_D(MCompleter);
    d->completerTimer->start();
}

void MCompleter::confirm()
{
    Q_D(MCompleter);
    if (!widget() || !isActive())
        return;

    //fetch prefix again, ensure the prefix is not null
    d->fetchPrefix();
    if (model()->completionPrefix().isEmpty())
        return;

    if (model()->matchedIndex() >= 0 && model()->matchedIndex() < d->matchedModel->rowCount()) {
        //inform widget about confirmed string
        int row = 0;
        const int column = model()->valueColumnIndex();
        QString value;
        if (d->matchedIndexList.count() > model()->matchedIndex())
            row = d->matchedIndexList.at(model()->matchedIndex());
        QVariant var = d->completionModel->data(d->completionModel->index(row, column));
        if (var.isValid())
            value = var.toString();

        emit confirmed(d->trim(value), d->completionModel->index(row, column));

        d->reset();
        hideCompleter();
        d->resetFocus();
    }
}

bool MCompleter::acceptMultipleEntries() const
{
    return model()->acceptMultipleEntries();
}

void MCompleter::setAcceptMultipleEntries(bool enable)
{
    model()->setAcceptMultipleEntries(enable);
}

bool MCompleter::isActive() const
{
    return model()->active() || model()->popupActive();
}

void MCompleter::queryAll()
{
    Q_D(MCompleter);

    if (!widget() || !widget()->scene()) {
        return;
    }

    if (model()->popupActive() || (widget() != widget()->scene()->focusItem())) {
        return;
    }

    // TODO #1: remove check for one of those signals once they have been merged.
    // TODO #2: Change the "> 1" back to "> 0", because this is only needed for
    //          the current signal forwarding!
    if ((receivers(SIGNAL(startCompleting(QString))) > 0) ||
        (receivers(SIGNAL(startCompleting(QString, QModelIndex))) > 1)) {
        return;
    }

    d->reset();
    d->fetchPrefix();

    if (model()->completionPrefix().isEmpty()) {
        return;
    }

    //if no customized match slot, then use default match rule. -1 to query all.
    d->match(-1);
}

void MCompleter::appear(MWindow *window, MSceneWindow::DeletionPolicy policy)
{
    MSceneWindow::appear(window, policy);
    model()->setActive(true);
}

void MCompleter::appear(MSceneWindow::DeletionPolicy policy)
{
    appear(0, policy);
}

void MCompleter::disappear()
{
    MSceneWindow::disappear();
    model()->setActive(false);
}

bool MCompleter::eventFilter(QObject *object, QEvent *e)
{
    Q_D(MCompleter);
    bool eaten = false;
    if (model()->active() && object == widget()) {
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

QString MCompleter::charactersToTrim() const
{
    return model()->charactersToTrim();
}

void MCompleter::setCharactersToTrim(const QString &set)
{
    model()->setCharactersToTrim(set);
}

QString MCompleter::charactersToTrimForCompletionPrefix() const
{
    return model()->charactersToTrimForCompletionPrefix();
}

void MCompleter::setCharactersToTrimForCompletionPrefix(const QString &str)
{
    model()->setCharactersToTrimForCompletionPrefix(str);
}

void MCompleter::setCompletionTitle(const QString &title)
{
    model()->setCompletionTitle(title);
}

QString MCompleter::completionTitle() const
{
    return model()->completionTitle();
}



MCompletionModel::MCompletionModel(QObject *parent)
    : QAbstractProxyModel(parent)
{
}

void MCompletionModel::setMatchedList(const QList<int>& indexList)
{
    if (QAbstractProxyModel::sourceModel() && (indexList.count() <= QAbstractProxyModel::sourceModel()->rowCount())) {
        this->indexList = indexList;
        QAbstractProxyModel::endResetModel();
    }
}

void MCompletionModel::setSourceModel(QAbstractItemModel *source)
{
    bool hadModel = (QAbstractProxyModel::sourceModel() != 0);

    if (hadModel)
        QObject::disconnect(QAbstractProxyModel::sourceModel(), 0, this, 0);

    QAbstractProxyModel::setSourceModel(source);

    if (source)
        connect(source, SIGNAL(destroyed()), this, SLOT(modelDestroyed()));
}

QModelIndex MCompletionModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column, 0) : QModelIndex();
}

QModelIndex MCompletionModel::mapToSource(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() >= indexList.count())
        return QModelIndex();

    return createIndex(indexList.at(index.row()), index.column());
}

QModelIndex MCompletionModel::mapFromSource(const QModelIndex &index) const
{
    if (!index.isValid() || !indexList.contains(index.row()))
        return QModelIndex();

    return createIndex(indexList.indexOf(index.row()), 0);
}

QVariant MCompletionModel::data(const QModelIndex &index, int role) const
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

int MCompletionModel::columnCount(const QModelIndex &) const
{
    //FIXME: Do we need to display more columns? Now MPopupList can only display one column.
    if (QAbstractProxyModel::sourceModel())
        return 1;
    else
        return 0;
}

int MCompletionModel::rowCount(const QModelIndex &) const
{
    if (QAbstractProxyModel::sourceModel())
        return indexList.count();
    else
        return 0;
}

QModelIndex MCompletionModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

void MCompletionModel::modelDestroyed()
{
    // switch to static empty model
    QAbstractProxyModel::setSourceModel(0);
}

#include "moc_mcompleter.cpp"
