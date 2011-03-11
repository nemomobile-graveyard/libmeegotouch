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

#ifndef MCOMPLETER_H
#define MCOMPLETER_H

#include <QObject>
#include <QStringList>
#include <QMap>
#include "mwidgetcontroller.h"
#include "mscenewindow.h"
#include "mcompletermodel.h"

/*!
 * \class MCompleter
 * \brief MCompleter provides completion in MWidget.
 *
 * The MCompleter interacts with MWidgets that have input method support, e.g. MTextEdit.
 * Such widgets need to reimplement inputMethodQuery() for Qt::ImCursorPosition and Qt::ImSurroundingText.
 * \sa \link flags() \endlink, \link QGraphicsItem::ItemAcceptsInputMethod \endlink
 *
 * Once a completion candidate has been confirmed(), the widget is supposed to insert the received
 * candidate so that the completer can filter candidates according to the completion prefix.
 *
 * When the user starts typing a word, MCompleter suggests completion candidates, based on the contents
 * of its candidateSourceModel().
 * For simple use cases, a QStringList can be sufficient.
 * Candidates are confirmed (and inserted into the text edit) by pressing return or touching the
 * completion item.
 * \code
 *   QStringList list;
 *   list << "apple" << "appreciate" << "orange" << "offset";
 *   QStringListModel testModel(list)
 *   MCompleter* completer = new MCompleter(testModel);
 *   MTextEdit * edit = new MTextEdit(MTextEditModel::MultiLine, "", 0);
 *   edit->setCompleter(completer);
 * \endcode
 *
 * The MCompleter supports customized matching, through the startCompleting() signal:
 * \code
 *   MCompleter* completer = new MCompleter();
 *   MTextEdit * edit = new MTextEdit(MTextEditModel::MultiLine, "", 0);
 *   edit->setCompleter(completer);
 *   connect(completer, SIGNAL(startCompleting(QString, QModelIndex)), this, SLOT(customizedComplete(QString, QModelIndex)));
 * \endcode
 *
 * The application can update the model according to the prefix in the slot, and MCompleter will then show
 * completion candidates according to the updated model:
 * \code
 *   void ExampleClass::customizedComplete(const QString &prefix, const QModelIndex &index)
 *   {
 *      //do customized match according prefix,
 *      completer->setCandidateSourceModel(model);
 *   }
 * \endcode
 *
 * MCompleter supports more than one-dimensional models, in which case the last column is queried for values.
 * All columns are used to match and display a candidate, but only the item in that column will be inserted to the text
 * upon confirmation. The value column can be set via setValueColumnIndex().
 * \sa QAbstractItemModel, QAbstractTableModel
 * Example:
 * \code
 *   class TestModel : public  QAbstractTableModel
 *   {
 *      public :
 *          TestModel(const QStringList&, const QStringList&, QObject * parent = 0);
 *          //...
 *   };
 *
 *   QStringList modelColumn1;
 *   modelColumn1.append("Tom While");
 *   modelColumn1.append("John Frank");
 *   QStringList modelColumn2;
 *   modelColumn2.append("<tom.w@example.com>");
 *   modelColumn2.append("<john.f@example.com>");
 *   TestModel* testModel = new TestModel(modelColumn1, modelColumn2, this);
 *   MCompleter* completer = new MCompleter(testModel);
 *   MTextEdit * edit = new MTextEdit(MTextEditModel::MultiLine, "", 0);
 *   edit->setCompleter(completer);
 * \endcode
 *
 * To trim special characters before confirming an item to the text entry, call setCharactersToTrim().
 * For example, when the user confirms an item such as "<tom.w@example.com>", but the angle brackets
 * need to be removed before insertion:
 * \code
 *   completer->setCharactersToTrim(QString("<>"));
 * \endcode
 */
class MWidget;
class MTextEdit;
class MCompleterPrivate;
class MApplicationWindow;
class QAbstractItemModel;
class QModelIndex;

class M_CORE_EXPORT MCompleter : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MCompleter)

    Q_PROPERTY(bool acceptMultipleEntries READ acceptMultipleEntries WRITE setAcceptMultipleEntries)
    Q_PROPERTY(QString charactersToTrimForCompletionPrefix READ charactersToTrimForCompletionPrefix WRITE setCharactersToTrimForCompletionPrefix)
    Q_PROPERTY(QString completionTitle READ completionTitle WRITE setCompletionTitle)

public:

    /*!
     * \brief Creates an instance without any completion candidates.
     */
    MCompleter();

    /*!
     * \brief Creates an instance with the specified list of possible completion candidates.
     * \param[in] completionCandidates list of completion candidates.
     */
    explicit MCompleter(const QStringList &completionCandidates);

    /*!
     * \brief Creates an instance with the specified model of possible completion candidates.
     * If \a completionCandidates has only one column,
     * then this column is the completion candidate list.
     * If completionCandidates has more than one column,
     * all columns are used as possible completion candidates, but only the items of the default
     * value column will be inserted into the text edit upon candidate confirmation.
     * The default value column is the last column. It can be changed through setValueColumnIndex().
     * \param[in] completionCandidates model of completion candidates, MCompleter does not take ownership.
     */
    explicit MCompleter(QAbstractItemModel *completionCandidates);

    /*!
      \brief Destructor
     */
    virtual ~MCompleter();

    /*!
     *\brief Returns the completer's widget which receives the completions.
     */
    MWidget *widget();

    /*!
     *\brief Sets the completer's widget which receives the completions.
     * This method is implicitely called when the MCompleter is bound to a MWidget,
     * e.g., \a MTextEdit::setCompleter().
     * For custom widgets, this method needs to be called explicitly.
     */
    void setWidget(MWidget *);

    /*!
     *\brief Changes the completer's default value column
     * \sa valueColumnIndex().
     */
    void setValueColumnIndex(int);

    /*!
     *\brief Returns the value column index of the completer's model.
     * The value column contains the text which will be inserted to the text edit upon candidate
     * confirmation.
     * Default value column is the last column of the model.
     */
    int valueColumnIndex() const;

    /*!
     *\brief Returns current completion prefix.
     * The prefix is the text which contains the cursor, and betweens two delimiters.
     * Default delimiters are comma and semicolon.
     */
    QString completionPrefix() const;

    /*!
     *\brief Sets the completion candidate source list with specified model.
     */
    void setCandidateSourceModel(QAbstractItemModel *);

    /*!
     *\brief Returns the completion candidates' source model.
     */
    QAbstractItemModel *candidateSourceModel() const;

    /*!
     *\brief Returns the matched completion candidates' model.
     */
    QAbstractItemModel *matchedCandidateModel() const;

    /*!
     *\brief Returns single/multiple entry property.
     If false, only single entry (e.g. single email address) is relevant, if true, then multiple entries can be confirmed in the same text field. Default delimiters between multiple entries are comma and semicolon.
     This means if there is already a confirmed completion in the text entry, then completer wouldn't give the next suggestion.
     */
    bool acceptMultipleEntries() const;

    /*!
     *\brief Sets single/multiple entry property. \sa acceptMultipleEntries().
     */
    void setAcceptMultipleEntries(bool);

    /*!
     *\brief Returns if current completer is active (there are some completions shown).
     */
    bool isActive() const;

    /*!
     *\brief Returns the characters which will be trimmed when confirm(). \sa setCharactersToTrim()
     */
    QString charactersToTrim() const;

    /*!
     *\brief Sets the characters to be trimmed when confirm().
     * When confirming, the completion will remove the characters(belong to charactersToTrim()) from the start and the end. For example, to remove '<' and '>' from "<tom@example.com>", only insert "tom@example.com" to text entry.
     * \code
     *   completer->setCharactersToTrim(QString("<>"));
     * \endcode
     */
    void setCharactersToTrim(const QString &str);

    /*!
     *\brief Returns the characters which will be trimmed from the start and the end of completionPrefix().
     * Default charactersToTrimForCompletionPrefix() is space.
     * \sa setCharactersToTrimForCompletionPrefix().
     */
    QString charactersToTrimForCompletionPrefix() const;

    /*!
     *\brief Sets the characters to be trimmed from the start and the end of completionPrefix().
     * All characters in \a str will be removed from the start and the end of completionPrefix().
     * Example:
     * \code
     *   // if the text which contains cursor and betweens two delimiters is "  text to be trimmed\t\r ".
     *   completer->setCharactersToTrimForCompletionPrefix(QString(" \t\r"));
     *   // completionPrefix() == "to be trimmed";
     * \endcode
     *
     * \sa charactersToTrimForCompletionPrefix().
     */
    void setCharactersToTrimForCompletionPrefix(const QString &str);

    /*!
     * \brief Set title for completions
     */
    void setCompletionTitle(const QString &title);

    /*!
     * \brief Returns title for completions
     */
    QString completionTitle() const;

Q_SIGNALS:
    /*!
     * This signal is emitted before starting matching the completion candidates.
     * Application can customize its own completion rule, by connecting its
     * customized match rule slot to this signal, and calling setCompletions,
     * setMatchedCompletions inside the slot's implementation.
     */
    void startCompleting(const QString &prefix);

    /*!
     * This is an overloaded version of startCompleting().
     */
    // TODO: merge with prefix-only version and use a default parameter for the index on next ABI break.
    void startCompleting(const QString &prefix, const QModelIndex &index);

    /*!
     * This signal is emitted when an item in the completion candidates is confirmed by the user.
     * The widget() using the completer is supposed to insert the confirmed completion by itself.
     */
    void confirmed(const QString &candidate);

    /*!
     * This is an overloaded version of confirmed().
     */
    // TODO: merge with candidate-only version and use a default parameter for the index on next ABI break.
    void confirmed(const QString &candidate, const QModelIndex &index);

    /*!
     * This signal is emitted just after showing the completer.
     */
    void shown();

    /*!
     * This signal is emitted just after hiding the completer.
     */
    void hidden();

public Q_SLOTS:

    /*!
     *\brief hides the completer.
     * This function should be called when the MWidget wants to hide completer when losing focus.
     * The application can not call show() or appear() to show completer, but can call complete() to start to match,
     * and the completer will be visible automatically if there are some matched completion candidates.
     */
    void hideCompleter();

    /*!
     * Matches completion candidates, and display the completer if there are some matched completion candidates.
     * If there is no customized match rule slot connected, completer searches the completion candidate source model,
     * and continues until reach the end or the hints number equal default maximum hits: 10.
     * If you want to get all matched items, use queryAll().
     * /sa queryAll()
     */
    void complete();

    /*!
     * Confirms current selected completion item, will insert the item's (belongs to the value column) display
     * string to the text entry.
     */
    void confirm();

    /*!
     * Queries the completion candidate source model, to get all matched items.
     * It is always called after complete(), but it does nothing if there is a customized match rule slot connected.
     * /sa complete()
     */
    void queryAll();

protected Q_SLOTS:

    //! \reimp
    virtual void appear(MWindow *window, MSceneWindow::DeletionPolicy policy = KeepWhenDone);
    virtual void appear(MSceneWindow::DeletionPolicy policy = KeepWhenDone);
    virtual void disappear();
    virtual bool eventFilter(QObject *, QEvent *);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(MCompleter)
    Q_DISABLE_COPY(MCompleter)

    Q_PRIVATE_SLOT(d_func(), void _q_modelUpdate())
    Q_PRIVATE_SLOT(d_func(), void _q_complete())
    Q_PRIVATE_SLOT(d_func(), void _q_pollModel())
    Q_PRIVATE_SLOT(d_func(), void _q_modelAboutToBeReset())
    Q_PRIVATE_SLOT(d_func(), void _q_rowsRemoved(const QModelIndex &parent, int start, int end))

#ifdef UNIT_TEST
    friend class Ut_MCompleter;
#endif
};

#endif
