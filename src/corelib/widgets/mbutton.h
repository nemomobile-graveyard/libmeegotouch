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

#ifndef MBUTTON_H
#define MBUTTON_H

#include <mwidgetcontroller.h>
#include <mbuttonmodel.h>
#include <mnamespace.h>

class MButtonPrivate;
class MButtonGroup;
class MWidget;

/*!
    \class MButton
    \brief MButton implementation of a common button widget.

    \ingroup widgets

    \section MButtonOverview Overview
        MButton provides functionality of check, toggle and push button types.
        A button can display a label containing text and an icon. setText() sets
        the text. setIconID() sets the logical icon identifier.

        MButton provides the following states for buttons:
        \li isDown() indicates whether the button is pressed down.
        \li isChecked() indicates whether the button is checked. Only checkable
            buttons can be checked and unchecked (see below).

        The difference between isDown() and isChecked() is as follows. When the
        user clicks a toggle button to check it, the button is first pressed
        then released into the checked state. When the user clicks it again
        (to uncheck it), the button moves first to the pressed state, then to
        the unchecked state (isChecked() and isDown() are both false).

        MButton provides the following signals:

        \li pressed() is emitted when the finger is inside the button.
        \li released() is emitted when the finger is lifted.
        \li clicked() is emitted when the button is tapped (pressed and then
            released), or when click() is called.
        \li toggled() is emitted when the state of a toggle button changes.

    \section MButtonUseGuidelines Usage guidelines
        - Direct UI has several different styles of buttons:
            - Push Buttons (with a "button style" borders around the button)
            - Icon Buttons (which in general behaves like a button, but is not
              visualized with a button-style border)
            - Switches / checkboxes (alters between "on" and "off" states of a
              particular item)
        - Push buttons are typically used with dialogs, notifications and main
          windows to invoke action. Push buttons are used to invoke action, for
          instance to have a "Call" button in a contact card or a "Send" button
          in a dialog.
        - Icon buttons are typically used in application toolbars, generally
          where space is at a premium.
        - Switches and checkboxes are typically used for functions that the user
          can set on and off.
        - Additionally, buttons can be presented by themselves, or linked together
          as a button group. See ::MButtonGroup for details.
        - Buttons do not support launching the object menu by long press.

    \section MButtonVariants Variants
        \li \link MButtonView Default button view. \endlink View is used
            to visualize standard push buttons and checkboxes.
        \li \link MButtonIconView Icon view. \endlink View is used to
            visualize icon buttons.

    \section MButtonOpenIssues Open issues
        - The Switch has (TBD: check how well this works in practice) its own
          haptic feedback: switching button from off to on feels different than
          when switching from on to off.
        - A new variant for switch should be implemented if some animation is
          needed between on and off states.

    \section MButtonExamples Examples
        Constructing different button variants:
        \code
            //push button with text
            MButton* pushButton = new MButton("Push Button");

            //checkbox
            MButton* checkbox = new MButton();
            checkbox->setViewType(MButton::checkboxType);
            checkbox->setCheckable(true);

            //toggle button
            MButton* toggleButton = new MButton("Toggle Button");
            toggleButton->setViewType(MButton::toggleType);
            toggleButton->setCheckable(true);

            //switch
            MButton* switch = new MButton();
            switch->setViewType(MButton::switchType);
            switch->setCheckable(true);

            //icon button
            MButton* iconButton = new MButton();
            iconButton->setViewType(MButton::iconType);
        \endcode

        Connecting to user input signals:
        \code
            //receive user input event from push button
            MButton* pushButton = new MButton("Push Button");
            connect(pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
            connect(pushButton, SIGNAL(pressed()), this, SLOT(buttonPressed()));
            connect(pushButton, SIGNAL(released()), this, SLOT(buttonReleased()));

            //receive user input event from checkable buttons
            //(checkbox, togglebutton and switch)
            MButton* checkable = new MButton();
            checkable->setViewType(MButton::checkboxType);
            checkable->setCheckable(true);
            connect(checkable, SIGNAL(clicked(bool)), this, SLOT(checkableClicked(bool)));
            connect(checkable, SIGNAL(toggled(bool)), this, SLOT(checkableToggled(bool)));
        \endcode

        Connecting to user input signals:
        \code
        \endcode

    \sa MButtonModel MButtonStyle MButtonGroup
*/
class M_CORE_EXPORT MButton : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MButton)

    /*!
        \property MButton::text
        \brief See MButtonModel::text
    */
    Q_PROPERTY(QString text READ text WRITE setText)

    /*!
        \property MButton::iconID
        \brief See MButtonModel::iconID
    */
    Q_PROPERTY(QString iconID READ iconID WRITE setIconID)

    /*!
        \property MButton::toggledIconID
        \brief See MButtonModel::toggledIconID
    */
    Q_PROPERTY(QString toggledIconID READ toggledIconID WRITE setToggledIconID)

    /*!
        \property MButton::icon
        \brief See MButtonModel::icon
    */
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon)

    /*!
        \property MButton::textVisible
        \brief See MButtonModel::textVisible
    */
    Q_PROPERTY(bool textVisible READ isTextVisible WRITE setTextVisible)

    /*!
        \property MButton::iconVisible
        \brief See MButtonModel::iconVisible
    */
    Q_PROPERTY(bool iconVisible READ isIconVisible WRITE setIconVisible)

    /*!
        \property MButton::checkable
        \brief See MButtonModel::checkable
    */
    Q_PROPERTY(bool checkable READ isCheckable WRITE setCheckable)

    /*!
        \property MButton::checked
        \brief See MButtonModel::checked
    */
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked DESIGNABLE isCheckable NOTIFY toggled USER true)

    /*!
        \property MButton::down
        \brief See MButtonModel::down
    */
    Q_PROPERTY(bool down READ isDown WRITE setDown DESIGNABLE false)

public:
    /*!
        \brief Variable that defines id for toggle button variant.
    */
    static const MTheme::ViewType toggleType;

    /*!
        \brief Variable that defines id for checkbox button variant.
    */
    static const MTheme::ViewType checkboxType;

    /*!
        \brief Variable that defines id for icon button variant.
    */
    static const MTheme::ViewType iconType;

    /*!
        \brief Variable that defines id for switch button variant.
    */
    static const MTheme::ViewType switchType;

    /*!
        \brief Variable that defines id for a buttons inside button groups.
    */
    static const MTheme::ViewType groupType;

    /*!
      \brief Constructs a button without text.
     */
    explicit MButton(QGraphicsItem *parent = 0, MButtonModel *model = 0);

    /*!
      \overload
      \brief Constructs a button with text.
     */
    explicit MButton(const QString &text, QGraphicsItem *parent = 0, MButtonModel *model = 0);

    /*!
      \overload
      \brief Constructs a button with icon and text.
     */
    MButton(const QString &iconID, const QString &text, QGraphicsItem *parent = 0, MButtonModel *model = 0);

    /*!
      \brief Destroys the button.
     */
    virtual ~MButton();

    /*!
     \brief Sets the logical ID of the icon of the button.
     */
    void setIconID(const QString &iconID);

    /*!
      \brief Returns the logical ID of the icon of the button.
     */
    QString iconID() const;

    /*!
      \brief Sets the logical ID of the toggled icon of the button.
     */
    void setToggledIconID(const QString &toggledIconID);

    /*!
      \brief Returns the logical ID of the toggled icon of the button.
    */
    QString toggledIconID() const;

    /*!
     \brief Sets the \a icon of the button.

     \sa icon(), setIconVisible(), setIconID()
     */
    void setIcon(const QIcon &icon);

    /*!
      \brief Returns the icon of the button.

      \sa setIcon()
     */
    QIcon icon() const;

    /*!
      \brief Returns the text of the button.

      If the button has no text, the text() function will return an empty string.

      There is no default text.
     */
    QString text() const;

    /*!
      \brief Set the text of the button.
    */
    void setText(const QString &text);

    /*!
      \brief Returns true if the text of the button is visible.
     */
    bool isTextVisible() const;

    /*!
      \brief Returns true if the icon of the button is visible.
    */
    bool isIconVisible() const;

    /*!
      \brief Returns true if the button is checkable.
     */
    bool isCheckable() const;

    /*!
      \brief Set the button to be checkable.

      By default, the button is not checkable.
     */
    void setCheckable(bool);

    /*!
      \brief Set the button down state programmatically.

      Unless you are implementing your own button type, you should not need to call this.
    */
    void setDown(bool);

    /*!
      \brief Returns true if the button is pressed down.
    */
    bool isDown() const;

    /*!
      \brief Returns true if the button is checked.
     */
    bool isChecked() const;

    /*!
      \brief Sets the checked state of the button.

      Only checkable buttons can be checked. By default, the button is unchecked.
    */
    void setChecked(bool);

    /*!
      \brief Returns the group that this button belongs to.

      If the button is not a member of any MButtonGroup, this function returns 0.

      \see MButtonGroup.
     */
    MButtonGroup *group() const;

Q_SIGNALS:
    /*!
      \brief This signal is emitted when the button is pressed down.
     */
    void pressed();

    /*!
      \brief This signal is emitted when the button is released.
     */
    void released();

    /*!
      \brief This signal is emitted when the button is activated.

      Notably, this signal is \e not emitted if you call setDown(),
      setChecked() or toggle().

      If the button is checkable, \a checked is true if the button is
      checked, or false if the button is unchecked.
     */
    void clicked(bool checked = false);

    /*!
      \brief This signal is emitted whenever a checkable button changes its state.

      \a checked is true if the button is checked, or false if the button is unchecked.

      This may be the result of a user action, click() slot activation, or because setChecked() was called.
    */
    void toggled(bool checked);

public Q_SLOTS:

    /*!
      \brief Performs a click.

      All the usual signals associated with a click are emitted as
      appropriate. If the button is checkable, the state of the button is
      toggled.
    */
    void click();

    /*!
      \brief Toggles the state of a checkable button.
     */
    void toggle();

    /*!
      \brief Set the visibility of the icon of the button.
    */
    void setIconVisible(bool);

    /*!
      \brief Set the visibility of the text of the button.
     */
    void setTextVisible(bool);

protected:
    //! \internal
    MButton(MButtonPrivate *dd, MButtonModel *model, QGraphicsItem *parent);
    void setGroup(MButtonGroup *group);
    //! \internal_end

    //! \reimp
    virtual void setupModel();
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    //! \reimp_end

protected Q_SLOTS:

    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

    void modelClick();

private:
    Q_DECLARE_PRIVATE(MButton)
    Q_DISABLE_COPY(MButton)

    friend class MButtonGroup;
};

#endif

