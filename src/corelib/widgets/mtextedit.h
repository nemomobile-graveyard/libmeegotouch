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

#ifndef MTEXTEDIT_H
#define MTEXTEDIT_H

#include <mwidgetcontroller.h>
#include <mtexteditmodel.h>
#include "mnamespace.h"

class MTextEditPrivate;
class QTextDocument;
class QTextCursor;
class QValidator;
class MCompleter;


/*!
 * \class MTextEdit
 * \brief MTextEdit is used to edit and display both plain and rich text.
 *
 * This widget can be used to show text and allow user to edit it.
 * Roughly corresponds Qt's QTextEdit and QLineEdit widgets, the LineMode
 * determines which. The mode can be set only at the constructor.
 *
 * MTextEdit can operate in single or multiline mode.
 * In single line mode no new lines can be added. If the text
 * is too long to fit inside the widget, it can be scrolled horizontally in the
 * basic MTextEditView, but
 * text will not be wrapped to new lines. In multi line mode new lines can be
 * added as necessary. The widget may also grow to make room for new lines, and
 * if that is not enough, contents can be scrolled horizontally.
 *
 * Caution: please do not use setInputMethodHints(Qt::InputMethodHints) with
 * objects of this class, you have to use setContentType, setInputMethodCorrectionEnabled,
 * setMaskedInput and other similar function instead.
 *
 * If you just need to display a small text snippet then you should use
 * MLabel instead.
 *
 */
class M_CORE_EXPORT MTextEdit : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MTextEdit)

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged USER true)
    // TODO: following type should be M::TextContentType, unfortunately due to a moc bug,
    // such code doesn't compile with the current Qt. Need to change this when it works.
    Q_PROPERTY(TextContentType contentType READ contentType WRITE setContentType)
    Q_PROPERTY(Qt::TextInteractionFlags textInteractionFlags READ textInteractionFlags WRITE setTextInteractionFlags)
    Q_PROPERTY(QString prompt READ prompt WRITE setPrompt)

    Q_PROPERTY(bool inputMethodCorrectionEnabled READ inputMethodCorrectionEnabled WRITE setInputMethodCorrectionEnabled)
    Q_PROPERTY(bool isReadOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(bool inputMethodAutoCapitalizationEnabled READ inputMethodAutoCapitalizationEnabled WRITE setInputMethodAutoCapitalizationEnabled)
    Q_PROPERTY(QString selectedText READ selectedText) //TODO: not usable from outside at the moment, M_MODEL_PROPERTY always need set and get function.
    Q_PROPERTY(bool autoSelectionEnabled READ isAutoSelectionEnabled WRITE setAutoSelectionEnabled)
    Q_PROPERTY(bool inputMethodPredictionEnabled READ inputMethodPredictionEnabled WRITE setInputMethodPredictionEnabled)
    Q_PROPERTY(int maxLength READ maxLength WRITE setMaxLength)
    Q_PROPERTY(MTextEditModel::EchoMode echoMode READ echoMode WRITE setEchoMode)
    Q_PROPERTY(bool autoSipEnabled READ isAutoSipEnabled WRITE setAutoSipEnabled)
    Q_PROPERTY(bool errorHighlight READ errorHighlight WRITE setErrorHighlight)

public:
    typedef M::TextContentType TextContentType; // workaround for moc bug

    //! What is under the press or release location
    enum TextFieldLocationType {
        //! Pressed on top of a word
        Word,

        //! Pressed between words or empty space
        WordBoundary
    };

    /*!
     * \brief Default constructor. Creates a textedit field with a specified text and line mode.
     * \param type widget type (single line or multiline).
     * \param text optional text.
     * \param parent optional parent.
     */
    explicit MTextEdit(MTextEditModel::LineMode type = MTextEditModel::SingleLine,
                         const QString &text = QString(), QGraphicsItem *parent = 0);
    /*!
     * \brief Creates a textedit field with a specified line mode, model and parent.
     * \param type widget type (single line or multiline).
     * \param model the model to be used.
     * \param parent the parent of the text edit.
     */
    MTextEdit(MTextEditModel *model, QGraphicsItem *parent);

    //! \internal
    /*!
     * \brief Creates a textedit field with a specified line mode, private instance, model and parent.
     * \param type widget type (single line or multiline).
     * \param dd private instance to be used.
     * \param model the model to be used.
     * \param parent the parent of the text edit.
     */
    MTextEdit(MTextEditPrivate *dd, MTextEditModel *model,
              QGraphicsItem *parent);
    //! \internal_end

    /*!
      \brief Destructor
     */
    virtual ~MTextEdit();

    /**
     * \brief Returns cursor position inside the text edit field.
     * \return cursor position of the text edit widget.
     */
    int cursorPosition() const;

    /**
     * \brief Returns the edit mode of the text edit widget.
     * \return the edit mode.
     */
    MTextEditModel::EditMode mode() const;

    /**
     * \brief Returns the line mode of the text edit widget.
     * \return the line mode.
     */
    MTextEditModel::LineMode lineMode() const;

    /**
     * \brief Returns the content type of the text edit widget.
     */
    M::TextContentType contentType() const;

    /*!
     * \brief Returns current text
     * \return QString containing current widget text
     */
    QString text() const;

    /*!
     * \brief Returns the current document
     * \return pointer to the document
     * \note Caution advised if changes are done using this interface. Change signals 
     * are not necessarily emitted and care is needed not to break MTextEdit state.
     */
    QTextDocument *document() const;

    /*!
     * \brief Returns a copy of the currently visible cursor.
     * \note Caution advised if changes are done using this interface. Change signals 
     * are not necessarily emitted and care is needed not to break MTextEdit state.
     */
    QTextCursor textCursor() const;

    /*!
     * \brief Sets the currently visible cursor.
     */
    void setTextCursor(const QTextCursor &cursor);

    /*!
     * \brief set content type
     */
    void setContentType(M::TextContentType type);

    /*!
     * \brief Set text interaction flags
     * \param flags @see Qt::TextInteractionFlags
     */
    void setTextInteractionFlags(Qt::TextInteractionFlags);

    /*!
     * \brief Text interaction flags @see Qt::TextInteractionFlags
     * \return Text interaction flags
     */
    Qt::TextInteractionFlags textInteractionFlags() const;

    // The mouse handling is a bit complex. The controller receives the events first and
    // passes them to view. However, the controller has to notify input context on events
    // happening on top of preedit, so views should use the following methods back.
    // Right now release handler does lot of state changes. Mostly because it involves
    // input context which is kept out of view. This results in quite tightly coupled
    // controller and view. Might be better to later move some more mouse logic to the view.

    /*!
     * \brief Handle mouse press event on the widget. THIS IS PURELY FOR THE VIEW.
     * View should call this when receiving mouse presses that may happen on preedit (active mode).
     * The implementation will notify possible input context.
     * \param cursorPosition position of the click within characters
     * \param event event the view received
     * \param location assigned by the method either as Word or  as WordBoundary depending on where in text mouse is pressed
     * \note Location parameter is assigned by the method only if Qt::TextInteractionFlags is something other than Qt::NoTextInteraction
     */
    void handleMousePress(int cursorPosition, QGraphicsSceneMouseEvent *event, TextFieldLocationType *location);

    /*!
     * \overload handleMousePress()
     */
    void handleMousePress(int cursorPosition, QGraphicsSceneMouseEvent *event);

    /*!
     * \brief Handle mouse release event on the widget. THIS IS PURELY FOR THE VIEW.
     * View should call this on mouse releases, except when release ends a selection the view
     * has not started.
     * The implementation will notify possible input context, relocate the cursor or make
     * clicked word as preedit if possible.
     * \param cursorPosition position of the click within characters
     * \param event event the view received
     * \param location assigned by the method either as Word or  as WordBoundary depending on where in text mouse is released
     * \note Location parameter is assigned by the method only if Qt::TextInteractionFlags is something other than Qt::NoTextInteraction
     */
    void handleMouseRelease(int cursorPosition, QGraphicsSceneMouseEvent *event, TextFieldLocationType *location);

    /*!
     * \overload handleMouseRelease()
     */
    void handleMouseRelease(int cursorPosition, QGraphicsSceneMouseEvent *event);

    /*!
     * \brief Handle mouse move event on the widget. THIS IS PURELY FOR THE VIEW.
     * View should call this when receiving mouse moves that may happen on preedit (active mode).
     * The implementation will notify possible input context.
     * \param cursorPosition position of the click within characters
     * \param event event the view received
     */
    void handleMouseMove(int cursorPosition, QGraphicsSceneMouseEvent *event);

    /*!
     * \brief Returns whether the text entry is read only
     */
    bool isReadOnly() const;

    /*!
     * \brief If readOnly equal true, then set the text entry to read only, vice versa
     */
    void setReadOnly(bool);

    /*!
     * \brief Returns the current echo mode
     */
    MTextEditModel::EchoMode echoMode() const;

    /*!
     * \brief Sets the echo mode which determines how input is made visible.
     */
    void setEchoMode(MTextEditModel::EchoMode echoMode);

    /*!
     * \brief removes any selection and changes mode to basic
     */
    void deselect();

    /*!
     * \brief Selects text from word boundary to another so that start and start+length are
     included in the selection
    */
    void setSelection(int start, int length, bool useBoundaries = false);

    /*
     * \brief returns the index of first selected character in the widget or -1 if no text is
     * selected
     */
    int selectionStart() const;

    /*
     * \brief returns the selected text or empty string if no text is selected
     */
    QString selectedText() const;

    //! Returns true if some or all of the text has been selected
    bool hasSelectedText() const;

    /*!
     * \brief tell whether text is allowed to be selected
     * \return true if text selection is allowed, otherwise false
     */
    bool isSelectionEnabled() const;

    /*!
     * \brief Returns whether text should be selected when focus is gained
     */
    bool isAutoSelectionEnabled() const;

    /*!
     * \brief Enables or disables text auto selection
     * if enabled, all text will be selected when focus is gained
     */
    void setAutoSelectionEnabled(bool enable);

    /**
     * \brief sets correction on the input method server side to be enabled or disabled
     */
    void setInputMethodCorrectionEnabled(bool enabled);

    /**
     * \brief returns if correction support is enabled for input method servers
     */
    bool inputMethodCorrectionEnabled() const;

    /**
     * \brief sets word prediction on the input method server side to be enabled or disabled
     */
    void setInputMethodPredictionEnabled(bool enabled);

    /**
     * \brief returns if word prediction support is enabled for input method servers
     */
    bool inputMethodPredictionEnabled() const;

    /**
     * \brief Returns maximum allowed length for text in the text edit.
     */
    int maxLength() const;

    /**
     * \brief Sets the maximum allowed length for text in the text edit.
     *
     * It is still possible, however, to exceed the maximum by inserting text
     * with QTextCursor::insertText().
     *
     * If truncation occurs any selected text will be unselected.
     *
     * \param[in] numChars The number of characters to set as maximum length.
     */
    void setMaxLength(int numChars);

    /*
     * \brief sets auto-capitalization  on the input method server side to be enabled or disabled
     */
    void setInputMethodAutoCapitalizationEnabled(bool enabled);

    /*
     * \brief returns if auto-capitalization support is enabled for input method servers
     */
    bool inputMethodAutoCapitalizationEnabled() const;

    /*!
     * \brief sets prompt value
     * \param prompt prompt text
     * Prompt is placeholder text for describing a text enty.
     */
    void setPrompt(const QString &prompt);

    /*!
     * \brief returns current prompt text
     */
    QString prompt() const;

    /*!
     * \brief sets the MTextEdit only to accept input that the validator accepts.
     * \param validator the validator for input text
     * Validator will check that current paragraph validates after text insertion, and
     * disallows insertion on validation failure. The whole content is used to check
     * on hasAcceptableInput(). Note: Checking per paragraph doesn not include the possible
     * paragraph ending newline.
     */
    void setValidator(const QValidator *validator);

    /*!
     * \brief returns a pointer to the current validator or 0 if no validator is used
     */
    const QValidator *validator() const;

    /*!
     * \brief returns boolean value that indicates if input satisfies the validator
     */
    bool hasAcceptableInput() const;

    /*!
     * \brief Sets this text edit to provide auto completions from the completer.
     */
    void setCompleter(MCompleter *completer);

    /*!
     * \brief Returns the completer pointer, which is used by this text edit.
     */
    MCompleter *completer();

    /*!
     * \brief Allows control over SIP handling
     *
     * When a text edit is focused and this property is disabled, no software
     * input panel will be requested.
     */
    void setAutoSipEnabled(bool enabled);

    //! \sa setAutoSipEnabled
    bool isAutoSipEnabled() const;

    /*!
     * \brief Attaches a custom toolbar named \a name.
     *  \param name is the absolute file name of a custom toolbar, or the relative file under
     *      /usr/share/meegoimframework/imtoolbars/ (relative path deprecated).
     * Note if another toolbar is already attached, detachToolbar() will be automatically called.
     *
     * This method is provided for convenience. Equals with widget uses MInputMethodState::registerToolbar()
     * to register a toolbar and obtain its identifier, then calls attachToolbar(int) to attach.
     * The registered toolbar is valid until this widget attaches to another toolbar, or calls detachToolbar(),
     * or widget is deleted.
     * \sa attachToolbar(int), MInputMethodState::registerToolbar()
     */
    void attachToolbar(const QString &name);

    /*!
     * \brief Attaches a custom toolbar which identifier is \a id.
     * \param id should be a valid toolbar identifier which is registered by other widget or
     *      MInputMethodState::registerToolbar().
     * Note if another toolbar is already attached, detachToolbar() will be automatically called.
     */
    void attachToolbar(int id);

    /*!
     * \brief Returns the name of current attached custom toolbar.
     * Note the toolbar name could be empty if the toolbar is attached by attachToolbar(int).
     */
    QString attachedToolbar() const;

    /*!
     * \brief Returns the unique identifier of current attached custom toolbar.
     * Note if the toolbar is registered by attachToolbar(const QString &), then this toolbar identifier
     * is valid until the widget attaches to another toolbar, or calls detachToolbar(), or widget is deleted.
     * \sa attachToolbar(const QString&), attachToolbar(int).
     */
    int attachedToolbarId() const;

    /*!
     * \brief Detaches the custom toolbar which is already attached before.
     * detachToolbar() is automatically called in destructor.
     * \sa attachToolbar(const QString &).
     */
    void detachToolbar();

    /*!
     * \brief Returns true if the textedit has error highlighting enabled.
     */
    bool errorHighlight() const;

    /*!
     * \brief Show error highlighting.
     */
    void setErrorHighlight(bool showErrorHighlight);

public Q_SLOTS:
    /**
     * \brief Set text for this widget. This replaces the existing text.
     * \param text New text for this text edit widget.
     * \return false if \a text is not allowed to be set.
     * On successful insertion, the cursor is moved to the end of the text
     */
    bool setText(const QString &text);

    /**
     * \brief deletes any selection or preedit, and inserts text to the cursor position
     * \param text text to insert
     * \return true if \a text was accepted
     */
    bool insert(const QString &text);

    /*!
     * \brief Sets the cursor position of the text edit field. Moves to basic mode by committing
     * preedit and removing selection.
     * \param index The index at which the cursor appears.
     * \return false if \a index is an invalid cursor position.
     */
    bool setCursorPosition(int index);

    /*!
     * \brief makes the whole document text selected. No-op if there is no content.
     */
    void selectAll();

    /*!
     * \brief deletes all content in the widget
     */
    void clear();

    /*!
     * \brief Copies the selected text to the clipboard.
     * If there is no selected text nothing happens.
     */
    virtual void copy();

    /*!
     * \brief Pastes the plain text from the clipboard into the text edit at the current cursor position.
     * If there is no text in the clipboard nothing happens.
     * If clipboard content is rejected by assigned validator then signal pasteFailed() is emitted.
     */
    virtual void paste();

    /*!
     * \brief Copies the selected text to the clipboard and deletes it, if there is any, and if echoMode() is Normal.
     *
     * If the current validator disallows deleting the selected text, cut() will copy without deleting.
     * If text could not be changed (isReadOnly() return true) then cut() do nothing.
     */
    virtual void cut();

Q_SIGNALS:
    /**
     *  \brief A signal which is emitted whenever the widget gets the focus.
     */
    void gainedFocus(Qt::FocusReason);

    /**
     *  \brief A signal which is emitted whenever the widget lost the focus.
     */
    void lostFocus(Qt::FocusReason);

    /**
     *  \brief A signal which is emitted whenever the text has been changed.
     */
    void textChanged();

    //! \brief This signal is emitted whenever the selection changes
    void selectionChanged();

    //! \brief A signal to be emitted when the cursor position changes.
    //!
    //! Preedit cursor (as defined by the MTextEditModel) changes also
    //! cause this signal to be emitted.
    void cursorPositionChanged();

    //! \brief This signal is emitted when text is selected or de-selected in the text edit.
    // When text is selected this signal will be emitted with yes set to true.
    // If no text has been selected or if the selected text is de-selected this signal is emitted with yes set to false.
    void copyAvailable(bool yes);

    /*!
     * \brief This signal is emitted when paste is failed and nothing was inserted
     */
    void pasteFailed();

    /*!
     * \brief This signal is emitted when the Enter key is pressed in single line editor.
     * Note that if there is a validator() set on the line edit, the returnPressed()
     * signal will only be emitted if the validator() returns QValidator::Acceptable.
     */
    void returnPressed();

protected:
    /*! \reimp */
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);
    virtual void inputMethodEvent(QInputMethodEvent *);
    virtual void changeEvent(QEvent *event);
    virtual void retranslateUi();
    virtual QVariant inputMethodQuery(Qt::InputMethodQuery query) const;
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    /*! \reimp_end */

private:
    Q_DECLARE_PRIVATE(MTextEdit)
    Q_DISABLE_COPY(MTextEdit)

    friend class MCompleterViewPrivate;
    friend class MTextEditViewPrivate;
#ifdef UNIT_TEST
    friend class Ut_MTextEdit;
    friend class Ut_MTextEditView;
#endif
    Q_PRIVATE_SLOT(d_func(), void _q_confirmCompletion(const QString &))
    Q_PRIVATE_SLOT(d_func(), void _q_updatePasteActionState())
};

#endif
