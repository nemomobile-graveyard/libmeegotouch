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

#ifndef CLIPBOARDPAGE_H
#define CLIPBOARDPAGE_H

#include "templatepage.h"

#include <MListItem>
#include <MAbstractCellCreator>

#include <QAbstractItemModel>
#include <QDebug>
#include <QModelIndex>
#include <QMap>
#include <QPair>
#include <QPointer>

typedef QPair<QString, QString> CopyItemData;
typedef QList<CopyItemData> CopyItemDataList;

Q_DECLARE_METATYPE(CopyItemData)

class ClipboardPageListModel;
class QAbstractItemModel;
class QGraphicsGridLayout;
class QModelIndex;

class MBanner;
class MButton;
class MCellCreator;
class MLabel;
class MList;

class ClipboardPage: public TemplatePage
{
public:
    ClipboardPage();
    virtual ~ClipboardPage();

    virtual void createContent();
    virtual QString timedemoTitle();

protected:
    virtual void retranslateUi();

private:
    MList *createList(QAbstractItemModel *model, MCellCreator *creator);
    void populateListModel();

private:
    QPointer<ClipboardPageListModel> listModel;
};

class CopyListItem : public MListItem
{
    Q_OBJECT
public:
    CopyListItem(QGraphicsItem *parent = 0);
    void setTitle(const QString &title);
    void setCopyContent(const QString &content);
    void setHeaderAndItemIndex(int headerIndex, int itemIndex);

signals:
    void updateCopyContent(int headerIndex, int itemIndex, QString text);

protected:
    virtual void retranslateUi();

private slots:
    void handleClicked();
    void handleLineBreakSwitchToggled(bool checked);

private:
    void copyContentToClipboard(const QString &clipboardText,
                                const QString &contentDescription);
    void showBanner(const QString &text);
    void breakSubtitleIntoLines(bool breakIntoLines);

private:
    MButton *lineBreakSwitch;
    MLabel *titleLabel;
    MLabel *subtitleLabel;
    QGraphicsGridLayout *layoutGrid;

    QString bannerTitle;

    // For updating model
    int headerIndex;
    int itemIndex;
};

class CopyListItemCreator : public MAbstractCellCreator<CopyListItem>
{
public:
    MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const;
    void updateCell(const QModelIndex &index, MWidget *cell) const;
};

class ClipboardPageListModel : public QAbstractItemModel
{
    Q_OBJECT

    // Would otherwise use map here but row order cannot be allowed to change.
    // Therefore store data as list of QPair<header name, group item list>.
    typedef QPair<QString, CopyItemDataList> CopyItemGroup;
    typedef QList< QPair<QString, CopyItemDataList> > CopyItemGroups;

public:

    enum CopyItemRoles {
        DescriptionRole = Qt::UserRole,
        CopyContentRole
    };

    ClipboardPageListModel(QObject *parent = 0);
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    void clearCopyItems();
    void addCopyItems(QString headerText, QList<CopyItemData> items);

public slots:
    void updateCopyContent(int headerIndex, int itemIndex, QString text);

private:
    CopyItemGroups copyItemGroups;
};


#endif
