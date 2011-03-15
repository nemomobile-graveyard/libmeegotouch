/***************************************************************************
**
** Copyright (C) 2010-2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MTEXTEDIT_P_H
#define MTEXTEDIT_P_H

#include <QTextCursor>
#include <QString>
#include <QList>
#include <QInputMethodEvent>
#include <QTime>
#include <QAction>

class QGraphicsSceneMouseEvent;
class QValidator;
class MCompleter;

#include "private/mwidgetcontroller_p.h"
#include "mtextedit.h"

class MNavigationBar;

class MTextEditSignalEmitter : public QObject
{
    Q_OBJECT
public:
    MTextEditSignalEmitter(MTextEditPrivate &p);
    virtual ~MTextEditSignalEmitter();

Q_SIGNALS:
    /*!
     * \brief This signal is emitted when the scene position of MTextEdit changes.
     *
     * \Note This signal is only emitted when there's at least one object connected to it.
     * If no one is connected to this signal it won'tt be emitted. Connecting to this signal
     * will result in the flag QGraphicsItem::ItemSendsScenePositionChanges being set for
     * MTextEdit and as a consequence some performance penalty.
     * This signal is useful for widgets like completer and magnifier, who want to listen to
     * the movement of text edit and follow it.
     */
    void scenePositionChanged();

protected:
    /*! \reimp */
    virtual void connectNotify(const char *signal);
    virtual void disconnectNotify(const char *signal);
    /*! \reimp_end */

private:
    MTextEditPrivate &editPtr;
    int scenePositionChangedConnections;
    bool oldItemSendsScenePositionChanges;
    friend class MTextEditPrivate;
    friend class MTextEdit;
};

class MTextEditPrivate : public MWidgetControllerPrivate
{
    Q_DECLARE_PUBLIC(MTextEdit)

public:
    MTextEditPrivate();
    virtual ~MTextEditPrivate();

    void init();

    QTextCursor *cursor() const;
    bool moveCursor(QTextCursor::MoveOperation moveOp,
                    QTextCursor::MoveMode moveMode = QTextCursor::MoveAnchor, int n = 1);

    bool smartMoveCursor(QTextCursor::MoveOperation moveOp,
                         QTextCursor::MoveMode moveMode = QTextCursor::MoveAnchor, int n = 1);

    bool doBackspace(bool deleteStartOfWord);
    bool doDelete(bool deleteEndOfWord);
    bool doTab();
    bool doTextInsert(const QString &text, bool usePreeditStyling = false);
    bool onReturnPressed(QKeyEvent *event);
    bool doSignCycle();

    bool validateCurrentBlock();

    bool validate();

    bool setCursorPosition(int index);

    void setPreeditText(const QString &text,
                        const QList<QInputMethodEvent::Attribute> &attributes);
    void commitPreedit(bool keepPreeditCursorPosition = false);
    void removePreedit();
    bool isPositionOnPreedit(int cursorPosition) const;
    bool isPreediting() const;

    void requestSip();
    void requestAutoSip();
    void closeAutoSip();

    void setMode(MTextEditModel::EditMode mode);

    void notifyInputContextMouseHandler(int position, QGraphicsSceneMouseEvent *event);

    void sendCopyAvailable(bool yes);

    static QEvent::Type translateGraphicsSceneMouseTypeToQMouse(QEvent::Type input);

    enum StyleType
    {
        Bold,
        Italic,
        Underline
    };

    struct styleData
    {
        QTextCharFormat charFormat;
        int count;
    };
    QList<styleData> preeditStyling;

    void storePreeditTextStyling(int start, int end);
    void addStyleToPreeditStyling(StyleType currentStyleType, bool setValue);
    QTextCharFormat currentPreeditCharFormat() const;
    void insertTextWithPreeditStyling(const QString &text, int &currentListIndex, int &currentCount);
    void clearUnusedPreeditStyling(int currentListIndex, int currentCount);
    int currentPreeditStylingIndex(int &currentStyleLastCharIndex) const;
    virtual bool copy();
    QString replaceLineBreaks(QString text, QString replacement);
    void _q_confirmCompletion(const QString &);
    void _q_updatePasteActionState();

    //! \brief Disable MTextEdit::updateMicroFocus().
    //!
    //! Can be called multiple times without calling \a enableUpdateMicroFocus in between.  In such a case
    //! \a enableUpdateMicroFocus must be called equally many times.
    void disableUpdateMicroFocus();
    //! \param flush call updateMicroFocus() automatically if true and updateMicroFocus was called
    //! while being disabled
    void enableUpdateMicroFocus(bool flush = false);
    //! \brief updateMicroFocus() wrapper that can be disabled
    //! \sa disableUpdateMicroFocus
    //! \sa enableUpdateMicroFocus
    void updateMicroFocus();

    //! Call QInputContext::update() if input context exists
    //! and widget has focus
    void icUpdate();

    //! \brief Reset IC if we have focus.  Disable handling of input method events during reset().
    void safeReset();

    enum FocusEventStateType {
        NoFocusEventReceivedYet,
        FocusInEventReceived,
        FocusOutEventReceived
    };

    //! Used to keep track of focus events, ie., to determine whether a
    //! focusIn event got its corresponding focusOut event.
    //! Works around NB#186087 - QGraphicsItem never gets focusOutEvent on hide.
    FocusEventStateType focusEventState;

    // get the characters really in qtextdocument
    int realCharacterCount() const;

    //! connect to completer.
    void  connectCompleter();
    //! disconnect from completer.
    void disconnectCompleter();

private:
    const QValidator *validator;
    bool ownValidator; // setting content type creates a validator that the widget owns

    MCompleter *completer;
    int registeredToolbarId;

    bool editActive; // true if editing started after getting focus

    // protection from input methods that send something on QInputContext::reset()
    bool omitInputMethodEvents;

    int updateMicroFocusDisabled;
    bool pendingMicroFocusUpdate;
    QTime lastMousePressTime;
    // was the last mouse press event a double click event?
    bool doubleClick;
    // start and end indices of the word over which the mouse was released the last time
    int previousReleaseWordStart;
    int previousReleaseWordEnd;
    // the last time when double click selection was done
    QTime doubleClickSelectionTime;
    MTextEditSignalEmitter signalEmitter;
    bool preeditInjectionInProgress;
    QAction cutAction;
    QAction copyAction;
    QAction pasteAction;
    bool programmaticalDocumentChange; // true while text changed using public interface

    friend class MTextEditSignalEmitter;
    friend class MCompleterViewPrivate;
    friend class MTextEditViewPrivate;
};

#endif
