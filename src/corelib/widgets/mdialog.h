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

#ifndef MDIALOG_H
#define MDIALOG_H

#include "mscenewindow.h"
#include "mdialogmodel.h"
#include <mnamespace.h>

class MButton;
class MDialogPrivate;
class MDismissEvent;

/*!
    \class MDialog
    \brief The MDialog class is the base class of dialog windows.

    \ingroup widgets

    \section MDialogOverview Overview
    MDialog is a top-level scene window mostly used for short-term tasks and
    brief communications with the user. It can either be embedded in an existing MWindow
    (and therefore be application modal) or displayed in its own separate MWindow
    (which may be system modal). MDialogs are never modeless.


    A MDialog is comprised by three components:

    \li A title bar containig a title text, a progress indicator and a close button.
        The close button, the progress indicator or the entire title bar can be hidden.
        The progress indicator is hidden by default.
    \li A customizable central widget, that is meant to contain either the message or query
        for the user, or the content requesting user's selection.
    \li A button box, that can contain buttons for providing means of selection for simple queries.
        Can be hidden when central widget is customized.

    MDialog may be application modal or system modal. When displayed inside a given
    MWindow (by supplying a window parameter to appear() or exec())
    it will ignore the systemModal property and will be application modal. If
    no window is supplied and systemModal is set to true it will be displayed in its own
    separate top-level MWindow and will be system modal. The home button is not accessible
    when MDialog is system modal. MDialog is application modal by default.

    MDialog provides a return value, based on the user's choice. It also knows which button from
    the button box was pressed (if any).

    \section MDialogUseGuidelines Usage guidelines
    It is not always instantly clear whether to use a MDialog or MApplicationPage in your
    application, here are some characteristics and guidelines for making the decision:

    <table class="qt-style">
        <tr><th>Use an application page when...</th><th>Use a dialog when...</th></tr>
        <tr>
            <td>When you have a main task in the application; one page for each main task.</td>
            <td>When you are performing a secondary task related to the contents within the
                view.</td>
        </tr>
        <tr>
            <td>When you are staying inside the contents of particular application.</td>
            <td>When you are utilizing information from some other application source only as
                intermediate step, to avoid branching away.</td>
        </tr>
        <tr>
            <td>When you want to return to this step when pressing \em Back from following page.</td>
            <td>When it does not make sense to return to this step after pressing Back from
                following page.</td>
        </tr>
        <tr>
            <td>You need a complex UI for the selection; many levels, functionalities, different
                styles etc.</td>
            <td>A simple selection is enough, the action can be quickly finished, optionally with
                one click.</td>
        </tr>
        <tr>
            <td>When you need multiple instances of the same action (compose multiple separate
                emails at the same time, each in their own page.)</td>
            <td>When multiple instances do not make sense (for instance \em Select \em Connection
                is a dialog, since it does not make sense to have two of them open at the same
                time.)</td>
        </tr>
        </table>

    \section MDialogVariants Variants
        \li MMessageBox is designed for asking the user simple yes/no-type of questions.
            It contains text in its central area and has the title bar hidden.
        \li MPopupList is for picking one option from several alternatives.
            It has no button box, closes immediately after user makes selection.

    \section MDialogExamples Examples
    Constructing an entry dialog:
    \code
        MWidget *centralWidget = new MWidget;
        QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);

        MLabel *label = new MLabel("Name", centralWidget);
        MTextEdit *textEdit = new MTextEdit(MTextEditModel::SingleLine,
                                                 QString(),
                                                 centralWidget);
        centralWidget->setLayout(layout);

        layout->addItem(label);
        layout->addItem(textEdit);

        MDialog* dialog = new MDialog("Please enter your name",
                               centralWidget,
                               M::OkButton | M::ResetButton);

        dialog->exec();
    \endcode

    Constructing a question dialog, it is easier to use MMessageBox instead:
    \code
    MDialog* dialog = new MDialog("Question",
        new MLabel("Lorem ipsum dolor sit amet?"),
        M::YesButton | M::NoButton);
    dialog->exec();
    \endcode

    \sa MDialogView, MDialogStyle

 */
class M_EXPORT MDialog : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MDialog)

    Q_PROPERTY(int result READ result WRITE setResult NOTIFY finished USER true)

    Q_PROPERTY(bool buttonBoxVisible READ isButtonBoxVisible WRITE setButtonBoxVisible)

    Q_PROPERTY(bool closeButtonVisible READ isCloseButtonVisible WRITE setCloseButtonVisible)
    Q_PROPERTY(bool titleBarVisible READ isTitleBarVisible WRITE setTitleBarVisible)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(bool systemModal READ isSystemModal WRITE setSystemModal)
    Q_PROPERTY(bool progressIndicatorVisible READ isProgressIndicatorVisible WRITE setProgressIndicatorVisible)

public:

    /*!
     * \brief This enum provides values that can be returned by a call to exec() method.
     *
     * When a dialog is closed with either accept() or reject(), the equivalent enum values
     * are returned by exec().
     *
     * \sa exec()
     */
    enum DialogCode {
        //! The dialog was accepted
        Accepted = QMessageBox::Ok,
        //! The dialog was rejected
        Rejected = QMessageBox::Cancel
    };

    /*!
     * \brief Constructs a dialog.
     */
    MDialog();

    /*!
     * \brief Constructs a dialog with a given \a title and a set of standard buttons
     *        specified by \a buttons.
     */
    MDialog(const QString &title, M::StandardButtons buttons);

protected:
    //! \internal
    MDialog(MDialogPrivate *dd, MDialogModel *model, MSceneWindow::WindowType windowType);

    MDialog(MDialogPrivate *dd, M::StandardButtons buttons,
              MDialogModel *model, MSceneWindow::WindowType windowType);
    //! \internal_end
public:

    /*!
     * \brief Destructor for dialog class
     */
    virtual ~MDialog();

    /*!
     * \brief Returns the button that was clicked by the user, or 0 if the close button was clicked.
     *        If exec() hasn't been called yet, returns 0.
     */
    MButtonModel *clickedButton() const;

    /*!
     * \brief Returns the modal dialog's result code, Accepted or Rejected.
     * \sa DialogCode
     */
    int result() const;

    /*!
     * \brief Sets the modal dialog's result code to \a result.
     * \param result         Result code of this dialog.
     */
    void setResult(int result);

    /*!
     * \brief Returns true if the dialog's button box is visible, false otherwise.
     *
     * \sa setButtonBoxVisible()
     */
    bool isButtonBoxVisible() const;

    /*!
     * \brief Sets the visibility of the dialog's button box.
     *
     * It is possible to hide the button box because it doesn't necessarily have to be used for
     * making a selection/decision/response. Custom widgets that can be placed as a dialog's central
     * widget may contain input widget(s) sufficient for the dialog needs. The button box is
     * visible by default.
     *
     * \sa setCentralWidget(), isButtonBoxVisible()
     */
    void setButtonBoxVisible(bool visible);

    /*!
     * \brief Returns true if the dialog's close button is visible, false otherwise.
     *
     * \sa setCloseButtonVisible()
     */
    bool isCloseButtonVisible() const;

    /*!
     * \brief Sets the visibility of the dialog's close button.
     *
     * The dialog's optional close button is provided as a convenient way of
     * rejecting the dialog and discarding all changes. It's visible by default.
     *
     * Since the close button is part of the title bar, setting its visibility to
     * true when the title bar is invisible has no effect.
     *
     * \sa isCloseButtonVisible(), setTitleBarVisible()
     */
    void setCloseButtonVisible(bool visible);

    /*!
     * \brief Returns true if the dialog's progress indicator is visible,
     *        false otherwise.
     *
     * \sa setProgressIndicatorVisible()
     */
    bool isProgressIndicatorVisible() const;

    /*!
     * \brief Sets the visibility of the dialog's progress indicator.
     *
     * The progress indicator is used to indicate that some process is ongoing.
     * It sits on the title bar, next to the title text, and is hidden by default.
     *
     * Since the progress indicator is part of the title bar, setting its
     * visibility to true when the title bar is invisible has no effect.
     *
     * \sa isProgressIndicatorVisible(), setTitleBarVisible()
     */
    void setProgressIndicatorVisible(bool visible);

    /*!
     * \brief Returns true if the dialog's title bar is visible and false otherwise.
     *
     * \sa setTitleBarVisible()
     */
    bool isTitleBarVisible() const;

    /*!
     * \brief Sets the visibility of the dialog's title bar.
     *
     * The title bar sits on top of the dialog and contains a title text, an
     * optional close button and an optional progress indicator.
     *
     * Since both close button and progress indicator are part of the title bar,
     * setting their visibilities to true when the title bar itself is invisible
     * has no effect.
     *
     * The title bar is visible by default.
     *
     * \sa isTitleBarVisible(), setCloseButtonVisible(), setTitle()
     */
    void setTitleBarVisible(bool visible);

    /*!
     * \brief Returns whether the dialog should be displayed as system modal.
     *
     * If false, dialog will be application modal.
     * \sa setSystemModal()
     */
    bool isSystemModal() const;

    /*!
     * \brief Defines whether the dialog should be displayed as system modal.
     *
     * Changing this property will affect only subsequent calls to appear()
     * and exec(). I.e., if called between an appear() and a disappear() it won't affect
     * the modality of the ongoing appearance.
     *
     * \sa isSystemModal()
     */
    void setSystemModal(bool systemModal);

    /*!
     * \brief Adds a given \a button to the button box.
     *
     * \param button A button
     */
    void addButton(MButtonModel *button);

    /*!
     * \brief Creates a button with the given \a text and adds it to the button box.
     *
     * \sa removeButton()
     */
    MButtonModel *addButton(const QString &text);

    /*!
     * \brief Creates a standard button specified by the \a button value.
     *
     * The standard button has a predefined caption.
     *
     * Returns a pointer to the newly created button. If \a button was already
     * added, his model is returned instead and no new button is created.
     *
     * \sa StandardButton
     */
    MButtonModel *addButton(M::StandardButton button);

    /*!
     * \brief Removes \a button from the dialog without deleting it.
     *
     * \sa addButton()
     */
    void removeButton(MButtonModel *button);

    /*!
     * \brief Returns a pointer to the standard button specified by \a which.
     *
     * Returns a null pointer if there is no given standard button in the button box.
     */
    MButtonModel *button(M::StandardButton which);

    /*!
     * \brief Returns the standard button enum value corresponding to the given button,
     *        or NoButton if the given button isn't a standard button.
     *
     * \sa button(StandardButton)
     */
    M::StandardButton standardButton(MButtonModel *button) const;

    /*!
     * \brief Returns the title of the dialog.
     *
     * \sa setTitle()
     */
    QString title() const;

    /*!
     * \brief Sets the dialog's title to the given \a title.
     *
     * The MDialog class provides a header that contains the close button and
     * the label widget for displaying the title of the dialog window. By default
     * the title is an empty string.
     *
     * \sa title()
     */
    void setTitle(const QString &title);

    /**
     * \brief Returns the central widget for the dialog.
     *
     * By default dialog provides a widget (panel) on which other widgets can be placed. It's also
     * possible to set a central widget for a dialog with setCentralWidget() method.
     *
     * \return the pointer to the central widget.
     *
     * \sa setCentralWidget()
     */
    QGraphicsWidget *centralWidget();

    /**
     * \brief Sets the given widget to be the page's central widget.
     *
     * It's also possible to use the default widget, as a parent for all widgets using the
     * centralWidget() function.
     *
     * \b NOTE: MDialog takes ownership of the widget pointer and deletes it when needed.
     * \param  centralWidget the central widget.
     */
    void setCentralWidget(QGraphicsWidget *centralWidget);

public Q_SLOTS:
    /*!
     * Shows the dialog on the currently active window and registers it in the
     * associated MSceneManager. Uses an animation to show the window.
     *
     * If systemModal property is true, it will be displayed as a separate top level
     * MWindow regardless of whether there's an active window and the user
     * won't be able to switch to any other application or to the home screen until
     * the dialog is closed (the home button won't be accessible).
     *
     * \param policy    Deletion policy, defines the ownership for this window
     *
     * \sa setSystemModal()
     */
    virtual void appear(MSceneWindow::DeletionPolicy policy = KeepWhenDone);

    /*!
     * Shows the dialog on the window specified by \a window and registers
     * it in the associated MSceneManager. Uses animation to show the dialog.
     *
     * If \a window is 0, it functions just like appear() (without window parameter).
     * Otherwise it ignores the systemModal property and is displayed as application
     * modal.
     *
     * \param window    The window on which the dialog is going to be shown.
     * \param policy    Deletion policy, defines the ownership for this dialog
     *
     * \sa setSystemModal()
     */
    virtual void appear(MWindow *window, MSceneWindow::DeletionPolicy policy = KeepWhenDone);

    /*!
     * \brief Dismisses the modal dialog and sets the result code to Accepted.
     *
     * Equals to done(Accepted).
     *
     * \sa accept()
     */
    virtual void accept();

    /*!
     * Dismisses the dialog and sets its result code to \a result. If the dialog
     * is shown with exec(), done() causes the local event loop to quit, and
     * exec() to return result.
     *
     * \param result         Result code of this dialog.
     *
     * \sa exec()
     */
    virtual void done(int result);

    /*!
     * \brief Dismisses the modal dialog and sets the result code to Rejected.
     *
     * Equals to done(Rejected).
     *
     * \sa reject()
     */
    virtual void reject();

    /*!
     * \brief Makes the dialog appear and returns only when the user dismisses it.
     *
     * If the \a window is specified, the dialog appears on the specified window
     * and will be application modal, ignoring the value of systemModal property.
     *
     * If no \a window is specified and systemModal property is false, the dialog
     * will appear in the currently active window. If no window is currently
     * active, the method will fail.
     *
     * If no \a window is specified and systemModal property is true, it will be
     * displayed as a separate top level MWindow regardless of whether there's
     * an active window and the user won't be able to switch to any other
     * application or to the home screen until the dialog is dismissed (the home
     * button won't be accessible).
     *
     * When using a MDialog with standard buttons, this method returns a StandardButton
     * value indicating the standard button that was clicked. When using MDialog with custom
     * buttons, this function returns an opaque value; use clickedButton() to determine which
     * button was clicked.
     *
     * If the dialog was dismissed with either accept() or reject() (or the equivalent done()
     * calls), a DialogCode result is returned instead.
     *
     * \sa StandardButton, setSystemModal()
     */
    int exec(MWindow *window = 0);

    /*!
     * Users of MDialog shouldn't fiddle with its layout.
     * Be advised that if you directly manipulate the layout the dialog won't
     * work as it's supposed to anymore.
     */
    void setLayout(QGraphicsLayout *layout);

    /*!
     * Users of MDialog shouldn't fiddle with its layout.
     * Be advised that if you directly manipulate the layout the dialog won't
     * work as it's supposed to anymore.
     */
    QGraphicsLayout *layout();

Q_SIGNALS:

    /*!
     * \brief This signal is emitted when the dialog has been accepted either by the user
     * or by calling accept() or done() with the MDialog::Accepted argument.
     *
     * Note that this signal is not emitted when hiding the dialog with hide() or
     * setVisible(false). This includes deleting the dialog while it is visible.
     *
     * \sa accept()
     */
    void accepted();

    /*!
     * \brief This signal is emitted when the dialog's result code has been set, either by the user
     * or by calling done(), accept(), or reject().
     *
     * Note that this signal is not emitted when hiding the dialog with hide() or setVisible(false).
     * This includes deleting the dialog while it is visible.
     *
     * \param result         Result code of this dialog.
     * \sa done()
     */
    void finished(int result);

    /*!
     * \brief This signal is emitted when the dialog has been rejected either by the user
     * or by calling reject() or done() with the MDialog::Rejected argument.
     *
     * Note that this signal is not emitted when hiding the dialog with hide() or
     * setVisible(false). This includes deleting the dialog while it is visible.
     *
     * \sa reject()
     */
    void rejected();

protected Q_SLOTS:

    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    virtual void setupModel();
    //! \reimp_end

protected:
    //! \reimp
    virtual void dismissEvent(MDismissEvent *event);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(MDialog)
    Q_DISABLE_COPY(MDialog)
    Q_PRIVATE_SLOT(d_func(), void _q_buttonClicked(QObject *))
    Q_PRIVATE_SLOT(d_func(), void _q_onStandAloneDialogDisappeared())

    friend class MDialogView;
    friend class MDialogViewPrivate;
};

#endif

