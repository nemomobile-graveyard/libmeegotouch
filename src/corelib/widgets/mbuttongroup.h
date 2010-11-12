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

#ifndef MBUTTONGROUP_H
#define MBUTTONGROUP_H

#include "mexport.h"

#include <QObject>

class MButtonGroupPrivate;
class MButton;

/*!
    \class MButtonGroup
    \brief MButtonGroup organizes groups of button widgets.

    \ingroup widgets

    \section MButtonGroupOverview Overview

        MButtonGroup does not provide a visual representation of this grouping,
        instead it manages the states of each of the buttons in the group. If
        visual representation for the buttons in a group is needed,
        MLinearLayoutPolicy::setNotifyWidgetsOfLayoutPositionEnabled() method
        can be used to enable the layout to automatically set background graphics
        for the buttons depending on their position.

        An exclusive button group switches off all checkable (toggle) buttons
        except the one that was clicked. By default, a button group is exclusive.
        The buttons in a button group are usually checkable buttons. Non-checkable
        buttons can be added into group as well but they don't affect the button
        selection policy at all, however the interaction signals are emitted normally
        for them too.

        If you create an exclusive button group, you should ensure that one of
        the buttons in the group is initially checked; otherwise, the group
        will initially be in a state where no buttons are checked.

        A button is added to the group with addButton(). It can be removed from
        the group with removeButton(). If the group is exclusive, the currently
        checked button is available as checkedButton(). If a button is clicked
        the buttonClicked() signal is emitted. For a checkable button in an
        exclusive group this means that the button was checked. The list of
        buttons in the group is returned by buttons().

        In addition, MButtonGroup can map between integers and buttons. You
        can assign an integer id to a button with setId(), and retrieve it with
        id(). The id of the currently checked button is available with checkedId(),
        and there is an overloaded signal buttonClicked() which emits the id of
        the button. The purpose of the mapping mechanism is to simplify the
        representation of enum values in a user interface.

    \section MButtonGroupUseGuidelines Usage guidelines
        - The button group can have a minimum of 2 buttons.
        - The button group component should try to take all the width that is
          available for the group if possible.
        - The button group should automatically wrap into multiple lines if not all
          components fit into one line.

    \section MButtonGroupInteractions Interactions
        - Tap down an unselected button
            - Highlights the tapped button, does not change currently selected
              button.
        - Single tap on unselected button
            - Selected state moves from previously selected button to tapped one.
              Performs possible additional action defined by application.
        - Single tap on already selected button
            - Button remains selected, nothing else happens.
        - Drag selected button onto adjacent button
            - Original button remains selected, adjacent button will not get
              selected.

    \section MButtonGroupOpenIssues Open issues
        - Should button group be always exclusive?

    \section MButtonGroupExamples Examples
        Adding buttons to group:
        \code
        //create button group
        MButtonGroup* buttonGroup = new MButtonGroup();

        //add buttons to group
        MButton* buttons[5];
        for( int i=0; i < 5; ++i) {
            buttons[i] = new MButton("Button" + QString::number(i));
            buttons[i]->setViewType(MButton::toggleType);
            buttons[i]->setCheckable(true);
            buttonGroup->addButton(buttons[i]);
        }

        //connect to button interaction signals
        connect(buttonGroup, SIGNAL(buttonPressed(MButton*)), this, SLOT(groupButtonPressed(MButton*)));
        connect(buttonGroup, SIGNAL(buttonReleased(MButton*)), this, SLOT(groupButtonReleased(MButton*)));
        connect(buttonGroup, SIGNAL(buttonClicked(MButton*)), this, SLOT(groupButtonClicked(MButton*)));

        //you can add non-checkable buttons in group as well
        MButton* button = new MButton();
        buttonGroup->addButton(button);

        //and maybe later set them as checkable
        button->setCheckable(true);

        //remove button
        buttonGroup->removeButton(buttons[0]);

        //the group does not free the added buttons they need to be manually deleted by the user
        //when deleting a button it is automatically removed from the group it belongs
        delete button;
        for( int i=0; i<5; ++i)
            delete buttons[i];
        \endcode

        User defined button ids in a group:
        \code
        //create button group
        MButtonGroup* buttonGroup = new MButtonGroup();

        //add buttons with user defined ids to group
        MButton* buttons[5];
        for( int i=0; i < 5; ++i) {
            buttons[i] = new MButton("Button" + QString::number(i));
            buttons[i]->setViewType(MButton::toggleType);
            buttons[i]->setCheckable(true);
            buttonGroup->addButton(buttons[i], i);
        }

        //connect to button interaction signals
        connect(buttonGroup, SIGNAL(buttonPressed(MButton*)), this, SLOT(groupButtonPressed(MButton*)));
        connect(buttonGroup, SIGNAL(buttonReleased(MButton*)), this, SLOT(groupButtonReleased(MButton*)));
        connect(buttonGroup, SIGNAL(buttonClicked(MButton*)), this, SLOT(groupButtonClicked(MButton*)));

        //when giving ids for buttons signals with button ids are emitted by buttongroup in
        //addition of the normal signals, when ids are not assigned these signals are not emitted
        connect(buttonGroup, SIGNAL(buttonPressed(int)), this, SLOT(groupButtonPressed(int)));
        connect(buttonGroup, SIGNAL(buttonReleased(int)), this, SLOT(groupButtonReleased(int)));
        connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(groupButtonClicked(int)));

        //fetch button by id
        MButton* button = buttonGroup->button(2);

        //fetch id of a button
        int id = buttonGroup->id(button);
        \endcode

        Group the buttons visually together:
        \code
        //create button group and horizontal layout for the buttons
        MButtonGroup* buttonGroup = new MButtonGroup();
        MLayout* layout = new MLayout;
        MLinearLayoutPolicy* policy = new MLinearLayoutPolicy(layout, Qt::Horizontal);

        //make the policy to set the backgrounds of the buttons
        policy->setNotifyWidgetsOfLayoutPositionEnabled(true);

        //add buttons to group and layout
        MButton* buttons[5];
        for( int i=0; i < 5; ++i) {
            buttons[i] = new MButton("Button" + QString::number(i));
            buttons[i]->setViewType(MButton::groupType); //optional, for creating buttons without margins
            buttonGroup->addButton(buttons[i]);
            policy->addItem(buttons[i]);
        }
        \endcode

    \sa MButton
*/

//not properly supported yet
/*
        Multiselection group:
        \code
            //create non-exclusive buttongroup
            MButtonGroup* buttonGroup = new MButtonGroup();
            buttonGroup->setExclusive(false);
        \endcode
*/

class M_CORE_EXPORT MButtonGroup : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool exclusive READ exclusive WRITE setExclusive)

public:

    /*!
        \brief Constructs a new, empty button group with the given \a parent.
    */
    MButtonGroup(QObject *parent = 0);

    /*!
        \brief Destroys the button group.
    */
    virtual ~MButtonGroup();

    /*!
        \brief Adds the given \a button to the end of the group's internal list
               of buttons.

         The group does not take the ownership of the button, the caller of the
         method is still responsible for releasing the button.
    */
    void addButton(MButton *button);

    /*!
        \brief Adds the given \a button to the button group, with the given
               \a id.

         The group does not take the ownership of the button, the caller of the
         method is still responsible for releasing the button.

         \a id can not be -1.
    */
    void addButton(MButton *button, int id);

    /*!
        \brief Returns the button with the specified \a id, or 0 if no such
               button exists.
    */
    MButton *button(int id) const;

    /*!
        \brief Returns the list of this group's buttons. This may be empty.
    */
    QList<MButton *> buttons() const;

    /*!
        \brief Returns the button group's checked button, or 0 if no buttons
               are checked.
    */
    MButton *checkedButton() const;

    /*!
        \brief Returns the id of the checkedButton(), or -1 if no button is
               checked or button without id is checked.
    */
    int checkedId() const;

    /*!
        \brief Returns true if the group is exclusive
     */
    bool exclusive(void) const;

    /*!
        \brief Set whether the button group is exclusive.

        If the group is exclusive, then only one button in the group can be
        checked at any given time. The user can click on any button to check it,
        and that button will replace the existing one as the checked button in
        the group.
    */
    void setExclusive(bool);

    /*!
        \brief Returns the id for the specified \a button, or -1 if no such
               button exists.
    */
    int id(MButton *button) const;

    /*!
        \brief Removes the given \a button from the button group.
    */
    void removeButton(MButton *button);

    /*!
        \brief Sets the \a id for the specified \a button.

        \a id can not be -1.
    */
    void setId(MButton *button, int id);

Q_SIGNALS:

    /*!
        \brief This signal is emitted when the given button is clicked.

        A button is clicked when it is first pressed and then released, or
        programmatically when MButton::click() is called.

        \see MButton::clicked().
    */
    void buttonClicked(MButton *button);

    /*!
        \brief This signal is emitted when a button with the given \a id is
               clicked

               Buttons without id does not emit this signal.
    */
    void buttonClicked(int id);

    /*!
        \brief This signal is emitted when the given \a button is pressed down.
    */
    void buttonPressed(MButton *button);

    /*!
        \brief This signal is emitted when a button with the given \a id is
               pressed down.

               Buttons without id does not emit this signal.
    */
    void buttonPressed(int id);

    /*!
        \brief This signal is emitted when the given \a button is released.
    */
    void buttonReleased(MButton *button);

    /*!
        \brief This signal is emitted when a button with the given \a id is
               released.

               Buttons without id does not emit this signal.
    */
    void buttonReleased(int id);

protected:
    //! \internal
    MButtonGroupPrivate *const d_ptr;
    //! \internal_end

private:

    Q_DISABLE_COPY(MButtonGroup)
    Q_DECLARE_PRIVATE(MButtonGroup)
};

#endif
