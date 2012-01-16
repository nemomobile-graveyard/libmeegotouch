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

#ifndef SPINNERPAGE_H
#define SPINNERPAGE_H

#include "templatepage.h"
#include <QAbstractTableModel>
#include <QTimer>

class MLabel;
class MCompleter;
class MContainer;
class MProgressIndicator;
class MButton;
class MFlowLayoutPolicy;

class SpinnerPage : public TemplatePage
{
    Q_OBJECT;

    enum View {
        Unknown,
        ApplicationMainArea,
        ContainerHeader,
        Dialog,
        Completer
    };

public:
    SpinnerPage();
    virtual ~SpinnerPage();
    virtual QString timedemoTitle();
    virtual void createContent();
public slots:
    void inApplicationMainArea();
    void inContainerHeader();
    void inDialog();
    void launchDialog();
    void inCompleter();

    void timeout();
protected:
    virtual void retranslateUi();

private:
    void reset();

    MContainer *container;
    MProgressIndicator *spinner;
    MLayout *spinnerLayout1;
    MLayout *spinnerLayout2;
    MLabel *header;
    MLabel *description;
    View view;
    QTimer timer;
    MFlowLayoutPolicy *imageContainerPolicy;
    MCompleter *completer;
    MAction *actionInMainArea;
    MAction *actionInContainerHeader;
    MAction *actionInDialog;
    MAction *actionInCompleter;
};

class IncrementalCompletionModel : public QAbstractTableModel
{
    Q_OBJECT
public :
    IncrementalCompletionModel(const QStringList &names, const QStringList &addresses, QObject *parent = 0);
    ~IncrementalCompletionModel();

    void setData(const QStringList &names, const QStringList &addresses);

    //! \reimp
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool canFetchMore(const QModelIndex &parent) const;
    //! \reimp_end

public slots:
    void startPopulating();
    void stopPopulating();
    void addOne();

private:
    QStringList column1, column2;
    QTimer populateTimer;
    int populateCounter;
};

#endif // SPINNERPAGE_H
