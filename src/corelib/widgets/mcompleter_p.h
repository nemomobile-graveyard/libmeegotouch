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

#ifndef MCOMPLETER_P_H
#define MCOMPLETER_P_H

#include <QString>
#include <QPoint>
#include <QRect>
#include <QList>
#include <QAbstractProxyModel>
#include "mcompleter.h"
#include "mscenewindow_p.h"

class QGraphicsItem;
class QStringListModel;
class MCompletionModel;
class QTimer;

class MCompleterPrivate : public MSceneWindowPrivate
{
    Q_DECLARE_PUBLIC(MCompleter)

public:

    MCompleterPrivate();

    virtual ~MCompleterPrivate();

    void _q_modelUpdate();

    void _q_complete();
    void _q_pollModel();
    void _q_modelAboutToBeReset();
    void _q_rowsRemoved(const QModelIndex &parent, int start, int end);

protected:
    void fetchPrefix();

    void init();

    void reset();

    void match(int);

    void setCompletionModel(QAbstractItemModel *, bool own = false);

    QString trim(const QString &);

    void resetFocus();

    void pollModel(bool isResetFocus);

    void updateScene(bool isResetFocus);

private:
    QString delimiters;
    bool hasOwnModel;
    QAbstractItemModel *completionModel;
    MCompletionModel *matchedModel;
    QList<int> matchedIndexList;
    QTimer *completerTimer;
#ifdef UNIT_TEST
    friend class Ut_MCompleter;
#endif
};

//!internal proxy model
class MCompletionModel : public QAbstractProxyModel
{
    Q_OBJECT

public:
    MCompletionModel(QObject *parent = 0);

    //! \reimp
    virtual void setSourceModel(QAbstractItemModel *source);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const;

    virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QModelIndex parent(const QModelIndex & = QModelIndex()) const;
    //! \reimp_end

    void setMatchedList(const QList<int>& indexList);

public slots:
    void modelDestroyed();

private:
    QList<int> indexList;

    friend class MCompleterPrivate;
};

#endif
