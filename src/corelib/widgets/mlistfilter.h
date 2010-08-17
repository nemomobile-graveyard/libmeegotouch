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

#ifndef MLISTFILTER_H
#define MLISTFILTER_H

#include <QObject>
#include "mexport.h"

class MListFilterPrivate;
class MList;
class MTextEdit;
class MSortFilterProxyModel;
class QAbstractItemModel;
class QKeyEvent;

/*!
 * \class MListFilter
 * \brief MListFilter implements live filtering, for searching through the list content.
 *
 * MListFilter creates a text editor and an item proxy model to be used to filter MList items.
 * Application is responsible of putting the text editor into layout.
 *
 * By default MListFilter uses regular expression with case insensitivity and text editor 
 * content as a pattern which works as substring search. If you want to use any other content 
 * in the pattern you can set your own regular expression when text editor content changes.
 *
 * See http://doc.trolltech.com/4.6/qsortfilterproxymodel.html for how to change case 
 * sensitivity and how to access regular expression.
 *
 * Example of using list filtering with a caret in the regular expression:
 * \code
 *
 * list->filtering()->setEnabled(true);
 * connect(list->filtering()->editor(), SIGNAL(textChanged()), this, SLOT(liveFilteringTextChanged()));
 * ...
 *
 * void MListPage::liveFilteringTextChanged()
 * {
 * QRegExp regExp(list->filtering()->proxy()->filterRegExp());
 * regExp.setPattern('^' + list->filtering()->editor()->text()); // matches in the beginning of line
 * list->filtering()->proxy()->setFilterRegExp(regExp);
 * ...
 *
 * \endcode
 *
 */
class M_EXPORT MListFilter : public QObject
{
    Q_OBJECT

public:

    /*!
     * \brief Default constructor. Constructs the list filter.
     * \param parent The list controller of the list filter content.
     */
    MListFilter(MList *parent = NULL);

    /*!
     * \brief Default destructor. Cleanups and destroys the list filter.
     */
    virtual ~MListFilter();

    /*!
     * \brief Sets live filtering to enabled or disabled. When
     * live filtering is enabled list uses QSortFilterProxyModel
     * as item model. By default live filtering is disabled.
     */
    void setEnabled(bool enabled);

    /*!
     * \return <code>true</code> if live filtering is enabled, otherwise false
     */
    bool enabled();

    /*!
     * \return text editor used in live filtering
     */
    MTextEdit *editor() const;

    /*!
     * \return proxy model used in live filtering
     */
    MSortFilterProxyModel *proxy() const;

    /*!
     * \brief Sets filter role to be used in live filtering proxy. Default is Qt::DisplayRole
     */
    void setFilterRole(int role);

    /*!
     * \return filter role used in live filtering proxy. Default is Qt::DisplayRole
     */
    int filterRole() const;

    /*!
     * \brief Handling of key events - used to pass key presses to text editor when
     * live filtering is enabled
     */
    void keyPressEvent(QKeyEvent *event);

public Q_SLOTS:

    /*!
     * \brief Handles text changes in live filtering text editor. Updates live filtering
     * model regular expression.
     */
    void editorTextChanged();

Q_SIGNALS:

    /*!
     * \brief Emitted when list is panned up starting from top position. This signal can
     * be used in applications as a starter for list filtering with virtual keyboard.
     */
    void listPannedUpFromTop();

protected:
    QAbstractItemModel *updateItemModel(QAbstractItemModel* itemModel);

private:
    Q_DISABLE_COPY(MListFilter)
    Q_DECLARE_PRIVATE(MListFilter)
    friend class MList;

    MList *list;
    MTextEdit *filterEditor;
    MSortFilterProxyModel *filterProxy;
    bool filteringEnabled;
    MListFilterPrivate *d_ptr;
};

#endif
