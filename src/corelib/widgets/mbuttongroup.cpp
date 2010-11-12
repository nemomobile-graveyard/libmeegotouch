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

#include <MDebug>
#include <QList>
#include <QTimer>

#include "mbutton.h"
#include "mbuttongroup.h"
#include "mbuttongroup_p.h"


MButtonGroupPrivate::MButtonGroupPrivate()
    : checkedButton(0),
      exclusive(true),
      q_ptr(0)
{
}

MButtonGroupPrivate::~MButtonGroupPrivate()
{
}

void MButtonGroupPrivate::buttonPress()
{
    Q_Q(MButtonGroup);

    MButton *button = static_cast<MButton *>(this->sender());
    emit q->buttonPressed(button);
    int i = q->id(button);
    if (i != -1)
        emit q->buttonPressed(i);
}

void MButtonGroupPrivate::buttonRelease()
{
    Q_Q(MButtonGroup);

    MButton *button = static_cast<MButton *>(this->sender());
    emit q->buttonReleased(button);
    int i = q->id(button);
    if (i != -1)
        emit q->buttonReleased(i);
}

void MButtonGroupPrivate::buttonClick()
{
    Q_Q(MButtonGroup);

    MButton *button = static_cast<MButton *>(this->sender());
    emit q->buttonClicked(button);
    int i = q->id(button);
    if (i != -1)
        emit q->buttonClicked(i);
}

void MButtonGroupPrivate::buttonToggle(bool checked)
{
    Q_Q(MButtonGroup);

    MButton *button = static_cast<MButton *>(this->sender());
    if (!button) {
        mWarning("MButtonGroup") << "buttonToggle(): slot unexpectedly called by someone else than MButton";
        return;
    }

    if (!button->isCheckable()) {
        mWarning("MButtonGroup") << "buttonToggle(): Cannot check non-checkable button.";
        return;
    }

    if (checked && q->exclusive() && checkedButton) {
        exclusive = false;
        checkedButton->setChecked(false);
        exclusive = true;
    }

    if (checked) {
        checkedButton = button;
    } else if (checkedButton == button) {
        /* If a button is unchecked and its same as checkedButton, then
           checkedButton should be reset as well */
        checkedButton = 0;
    }
}

void MButtonGroupPrivate::enforceExclusivity()
{
    if (!exclusive)
        return;

    /* In case of exclusive mode:
    case 1: If there is no button checked, find the first checkable
    button check it.
    case 2: If there is more than one button checked,
    uncheck all buttons except the first checked one
    */
    MButton *checkableButton = 0;
    MButton *newCheckedButton = 0;
    QList<MButton *>::iterator i = buttonList.begin();
    while (i != buttonList.end()) {
        MButton *b = *i;
        /* Get the first checkable button */
        if (b->isCheckable() && checkableButton == 0) {
            checkableButton = b;
        }
        /* Set the first checked button and uncheck the rest */
        if (b->isChecked()) {
            if (newCheckedButton == 0) {
                newCheckedButton = b;
                checkedButton = b;
            } else {
                b->setChecked(false);
            }
        }
        ++i;
    }
    if (checkedButton == 0 && checkableButton != 0) {
        /* Make sure that exactly one button is checked */
        checkableButton->setChecked(true);
        checkedButton = checkableButton;
    }
}

MButtonGroup::MButtonGroup(QObject *parent)
    : QObject(parent),
      d_ptr(new MButtonGroupPrivate)
{
    d_ptr->q_ptr = this;
}

MButtonGroup::~MButtonGroup()
{
    delete d_ptr;
}

void MButtonGroup::addButton(MButton *button)
{
    Q_D(MButtonGroup);

    if (d->buttonList.contains(button)) {
        return;
    }

    d->buttonList.append(button);

    button->setGroup(this);
    connect(button, SIGNAL(pressed()), d, SLOT(buttonPress()));
    connect(button, SIGNAL(released()), d, SLOT(buttonRelease()));
    connect(button, SIGNAL(clicked()), d, SLOT(buttonClick()));
    connect(button, SIGNAL(toggled(bool)), d, SLOT(buttonToggle(bool)));

    /* Adding a checked button to a group makes it the currently checked button */
    if (button->isCheckable() && button->isChecked()) {
        if (exclusive() && d->checkedButton) {
            //TODO do this somehow differently if possible.
            d->exclusive = false;
            d->checkedButton->setChecked(false);
            d->exclusive = true;
        }
        d->checkedButton = button;
    }
}

void MButtonGroup::addButton(MButton *button, int id)
{
    Q_D(MButtonGroup);

    if (id == -1)
        mWarning("MButtonGroup") << "addButton(): Ignoring invalid id.";
    else
        d->buttonIdMap.insert(button, id);

    addButton(button);
}

MButton *MButtonGroup::button(int id) const
{
    Q_D(const MButtonGroup);
    return d->buttonIdMap.key(id);
}

QList<MButton *> MButtonGroup::buttons() const
{
    Q_D(const MButtonGroup);

    return d->buttonList;
}

MButton *MButtonGroup::checkedButton() const
{
    Q_D(const MButtonGroup);
    return d->checkedButton;
}

int MButtonGroup::checkedId() const
{
    Q_D(const MButtonGroup);
    return d->buttonIdMap.value(d->checkedButton, -1);
}

bool MButtonGroup::exclusive(void) const
{
    Q_D(const MButtonGroup);
    return d->exclusive;
}

void MButtonGroup::setExclusive(bool exclusive)
{
    Q_D(MButtonGroup);
    d->exclusive = exclusive;
    d->enforceExclusivity();
}

int MButtonGroup::id(MButton *button) const
{
    Q_D(const MButtonGroup);
    return d->buttonIdMap.value(button, -1);
}

void MButtonGroup::removeButton(MButton *button)
{
    Q_D(MButtonGroup);

    if (button->group() == this) {
        d->buttonList.removeAll(button);
        d->buttonIdMap.remove(button);
        button->setGroup(0);

        disconnect(button, 0, d, 0);

        if (d->checkedButton == button) {
            d->checkedButton = 0;
            // Wait until we reach the event loop to do the validation
            // as removeButton() could have been called because the parent
            // widget controller is being destroyed and, therefore, it's
            // taking all its children (our buttons) along with him.
            // If we validate immediatelly it will trigger the styling system
            // needlessly for every single button removal.
            QTimer::singleShot(0, d, SLOT(enforceExclusivity()));
        }
    }
}

void MButtonGroup::setId(MButton *button, int id)
{
    Q_D(MButtonGroup);
    if (d->buttonList.contains(button)) {
        if (id == -1)
            mWarning("MButtonGroup") << "setId(): Ignoring invalid id.";
        else
            d->buttonIdMap[button] = id;
    } else {
        mWarning("MButtonGroup") << "setId(): button(" << (void *)button << ") is not in the group.";
    }
}