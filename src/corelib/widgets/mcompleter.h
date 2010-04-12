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
 * The MCompleter are used together with an valid \link MWidget \endlink (e.g. \link MTextEdit \endlink,
 * which supports input methods, with the \sa flags() \sa QGraphicsItem::ItemAcceptsInputMethod;
 * responds to inputMethodQuery() for Qt::ImCursorPosition and Qt::ImSurroundingText; and insert the
 * confirmed completion when receiving the signal confirmed(). ), to show completions which are filtered
 * from the source data model according the completion prefix.
 *
 * When the user starts typing a word, MCompleter suggests possible ways of completing the word,
 * based on a word list. The word list is provided as a QAbstractItemModel. (For simple applications,
 * where the word list is static, you can pass a QStringList to MCompleter's constructor.)
 * When the completions are shown, if user press return key or click mouse on a completion item,
 * that item will be confirmed, and inserted to the text entry.
 *
 * For example, if you want to show completions in a text entry with given model, the easiest way is using
 * QStringListModel like:
 * \code
 *   QStringList list;
 *   list << "apple" << "appreciate" << "orange" << "offset";
 *   QStringListModel testModel(list)
 *   MCompleter* completer = new MCompleter(testModel);
 *   MTextEdit * edit = new MTextEdit(MTextEditModel::MultiLine, "", 0);
 *   edit->setCompleter(completer);
 * \endcode
 *
 * The MCompleter supports customized match. If application connect a slot to the signal startCompleting(),
 * Example:
 * \code
 *   MCompleter* completer = new MCompleter();
 *   MTextEdit * edit = new MTextEdit(MTextEditModel::MultiLine, "", 0);
 *   edit->setCompleter(completer);
 *   connect(completer, SIGNAL(startCompleting(const QString&)), this, SLOT(customizedComplete(const QString&)));
 * \endcode
 * The application can update the model according the prefix in the slot, and then MCompleter will show
 * completion according the updated model(all the items in this updated model are thought to be matched).
 * \code
 *   void ExampleClass::customizedComplete(const QString& prefix)
 *   {
 *      //do customized match according prefix,
 *      completer->setCandidateSourceModel(model);
 *   }
 * \endcode
 *
 * MCompleter supports more than one dimension model, it regards the last column of the model as the value column.
 * All columns are used to match and display, but only the item belongs to value column will be inserted to text
 * entry when confirming an item. The default value column is the last column, and it could be changed by calling
 * setValueColumnIndex().
 * Application can declare its multiple dimension model by inherit QAbstractItemModel or QAbstractTableModel.
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
 * To trim some special characters before confirming an item to the text entry, call setCharactersToTrim() to set
 * the characters to be trimed, then these characters will be trimed from the start and the end of confirmed value.
 * Example, when the user confirms an item with value "<tom.w@example.com>", if the application wants to
 * remove '<' and '>', only insert "tome.w@example.com" to the text entry.
 * \code
 *   completer->setCharactersToTrim(QString("<>"));
 * \endcode
 */
class MWidget;
class MTextEdit;
class MCompleterPrivate;
class MApplicationWindow;
class QAbstractItemModel;

class M_EXPORT MCompleter : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MCompleter)

    Q_PROPERTY(bool acceptMultipleEntries READ acceptMultipleEntries WRITE setAcceptMultipleEntries)
public:

    /*!
     * \brief Default constructor. Create a MCompleter Object, without any completion candidates
     */
    MCompleter();

    /*!
     * \brief Constructor. Create a MCompleter Object, with the specified list \a completionCandidates as
     *  the source of possible completion candidates.
     * \param completionCandidates const QStringList & given completion candidate source.
     */
    explicit MCompleter(const QStringList &completionCandidates);

    /*!
     * \brief Constructor. Create a MCompleter Object, with the specified model \a completionCandidates as
     *  the source of possible completion candidate,
     * \a completionCandidates is a QAbstractItemModel. If completionCandidates has only one column,
     * then this column is the completion candidate list; if completionCandidates has more than one columns,
     * all the columns are used as possible completion candidate source, but when confirming,
     * only the item belongs to the value column will be inserted to text entry.
     * Default value column is the last column, and it could be changed by calling setValueColumnIndex().
     * \param completionCandidates QAbstractItemModel * given completion candidate source.
     *      MCompleter won't delete the model when destroying.
     */
    explicit MCompleter(QAbstractItemModel *completionCandidates);

    /*!
      \brief Destructor
     */
    virtual ~MCompleter();

    /*!
     *\brief Returns the widget for which the completer object is providing completions.
     */
    MWidget *widget();

    /*!
     *\brief Sets the widget for which completion are provided for to widget.
     * This function is automatically called when a MCompleter is set on a MWidget,
     * e.g. on MTextEdit using MTextEdit::setCompleter().
     * The widget needs to be set explicitly when providing completions for custom widgets.
     * The widget must support input methods, with the \sa flags() \sa QGraphicsItem::ItemAcceptsInputMethod;
     * respond to inputMethodQuery() for Qt::ImCursorPosition and Qt::ImSurroundingText; and insert the
     * confirmed completion when receiving the signal confirmed().
     * \sa confirmed().
     */
    void setWidget(MWidget *);

    /*!
     *\brief Call this function to set which column in the completion source model is the value column.
     * \sa valueColumnIndex().
     */
    void setValueColumnIndex(int);

    /*!
     *\brief Returns the column index in the completion source model which is the value column.
     * The value column contains the text which will be inserted to the text widget when confirming.
     * Default value column is the last column.
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
     *\brief Return the characters which will be trimmed when confirm(). \sa setCharactersToTrim()
     */
    QString charactersToTrim() const;

    /*!
     *\brief set the characters to be trimmed when confirm().
     * When confirming, the completion will remove the characters(belong to charactersToTrim()) from the start and the end. For example, to remove '<' and '>' from "<tom@example.com>", only insert "tom@example.com" to text entry.
     * \code
     *   completer->setCharactersToTrim(QString("<>"));
     * \endcode
     */
    void setCharactersToTrim(const QString &);

Q_SIGNALS:
    /*!
     *  This signal is emitted before starting matching the completion candidates.
     *  Application can customize its own completion rule, by connecting its customized match rule slot to this signal, and calling setCompletions, setMatchedCompletions inside the slot's implementation.
     */
    void startCompleting(const QString &);

    /*!
     * This signal is emitted when an item in the completion candidates is confirmed by the user.
     *  (by clicking or pressing return).
     * \sa widget(). The widget which is using the completer should insert the confirmed \a completion by itself.
     */
    void confirmed(const QString &completion);

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

#ifdef UNIT_TEST
    friend class Ut_MCompleter;
#endif
};

#endif
