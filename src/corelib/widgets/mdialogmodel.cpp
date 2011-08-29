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

#include "mdialogmodel.h"
#include <mbuttonmodel.h>

namespace MDialogModelPrivate
{
    const QString ButtonNameOk = "MDialogButtonOk";
    const QString ButtonNameSave = "MDialogButtonSave";
    const QString ButtonNameSaveAll = "MDialogButtonSaveAll";
    const QString ButtonNameOpen = "MDialogButtonOpen";
    const QString ButtonNameYes = "MDialogButtonYes";
    const QString ButtonNameYesToAll = "MDialogButtonYesToAll";
    const QString ButtonNameNo = "MDialogButtonNo";
    const QString ButtonNameNoToAll = "MDialogButtonNoToAll";
    const QString ButtonNameAbort = "MDialogButtonAbort";
    const QString ButtonNameRetry = "MDialogButtonRetry";
    const QString ButtonNameIgnore = "MDialogButtonIgnore";
    const QString ButtonNameClose = "MDialogButtonClose";
    const QString ButtonNameCancel = "MDialogButtonCancel";
    const QString ButtonNameDiscard = "MDialogButtonDiscard";
    const QString ButtonNameHelp = "MDialogButtonHelp";
    const QString ButtonNameApply = "MDialogButtonApply";
    const QString ButtonNameReset = "MDialogButtonReset";
    const QString ButtonNameRestoreDefaults = "MDialogButtonRestoreDefaults";
    const QString ButtonNameDone = "MDialogButtonDone";

    static MButtonModel *createStandardButton(M::StandardButton buttonType);
}

using namespace MDialogModelPrivate;

MButtonModel *MDialogModelPrivate::createStandardButton(M::StandardButton buttonType)
{
    MButtonModel *button = new MButtonModel;

    switch (buttonType) {
    case M::OkButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.21.doc
        //: Saves changes and closes the dialog
        //% "OK"
        button->setText(qtTrId("qtn_comm_ok"));
        button->setObjectName(ButtonNameOk);
        break;
    case M::OpenButton:
        //~ uispec-document ..._Common_Strings_UI_Specification_1.21.doc
        //: Command/Button. Opens a file.
        //% "Open"
        button->setText(qtTrId("qtn_comm_open"));
        button->setObjectName(ButtonNameOpen);
        break;
    case M::SaveButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.21.doc
        //: For saving changes
        //% "Save"
        button->setText(qtTrId("qtn_comm_save"));
        button->setObjectName(ButtonNameSave);
        break;
    case M::CancelButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.21.doc
        //: Discards the changes and closes the dialog
        //% "Cancel"
        button->setText(qtTrId("qtn_comm_cancel"));
        button->setObjectName(ButtonNameCancel);
        break;
    case M::CloseButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.21.doc
        //: Command for closing e.g. dialogs
        //% "Close"
        button->setText(qtTrId("qtn_comm_command_close"));
        button->setObjectName(ButtonNameClose);
        break;
    case M::DiscardButton:
        //~ uispec-document ??? FIXME
        //: Command/Button. Discards changes.
        //% "Discard"
        button->setText(qtTrId("qtn_comm_discard"));
        button->setObjectName(ButtonNameDiscard);
        break;
    case M::ApplyButton:
        //~ uispec-document ??? FIXME
        //: Command/Button. Applies a setting or changes.
        //% "Apply"
        button->setText(qtTrId("qtn_comm_apply"));
        button->setObjectName(ButtonNameApply);
        break;
    case M::ResetButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.21.doc
        //: Command for resetting to default settings values
        //% "Reset Settings"
        button->setText(qtTrId("qtn_comm_settings_reset"));
        button->setObjectName(ButtonNameReset);
        break;
    case M::RestoreDefaultsButton:
        //~ uispec-document ??? FIXME
        //: Command/Button. Puts something back to the former or original condition.
        //% "Restore"
        button->setText(qtTrId("qtn_comm_restore"));
        button->setObjectName(ButtonNameRestoreDefaults);
        break;
    case M::HelpButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.21.doc
        //: Opens the user guide for the application
        //% "User Guide"
        button->setText(qtTrId("qtn_comm_userguide"));
        button->setObjectName(ButtonNameHelp);
        break;
    case M::SaveAllButton:
        //~ uispec-document ??? FIXME
        //: Command/Button. Saves all modifications
        //% "Save all"
        button->setText(qtTrId("qtn_comm_save_all"));
        button->setObjectName(ButtonNameSaveAll);
        break;
    case M::YesButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.21.doc
        //: Command for confirming queries
        //% "Yes"
        button->setText(qtTrId("qtn_comm_command_yes"));
        button->setObjectName(ButtonNameYes);
        break;
    case M::YesToAllButton:
        //~ uispec-document ??? FIXME
        //: Command/Button. Confirms all queries at once.
        //% "Yes to all"
        button->setText(qtTrId("qtn_comm_yes_all"));
        button->setObjectName(ButtonNameYesToAll);
        break;
    case M::NoButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.21.doc
        //: Command for declining queries
        //% "No"
        button->setText(qtTrId("qtn_comm_command_no"));
        button->setObjectName(ButtonNameNo);
        break;
    case M::NoToAllButton:
        //~ uispec-document ??? FIXME
        //: Command/Button. Ignores all queries at once.
        //% "No to all"
        button->setText(qtTrId("qtn_comm_no_all"));
        button->setObjectName(ButtonNameNoToAll);
        break;
    case M::AbortButton:
        //~ uispec-document ??? FIXME
        //: Command/Button. Cancels or terminates an operation.
        //% "Abort"
        button->setText(qtTrId("qtn_comm_abort"));
        button->setObjectName(ButtonNameAbort);
        break;
    case M::RetryButton:
        //~ uispec-document ..._Common_Strings_UI_Specification_1.21.doc
        //: Command to retry something now
        //% "Retry"
        button->setText(qtTrId("qtn_comm_command_retry_now"));
        button->setObjectName(ButtonNameRetry);
        break;
    case M::IgnoreButton:
        //~ uispec-document ??? FIXME
        //: Command/Button. Rejects or dismisses something.
        //% "Ignore"
        button->setText(qtTrId("qtn_comm_ignore"));
        button->setObjectName(ButtonNameIgnore);
        break;
    case M::DoneButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.21.doc
        //: Command for confirmation
        //% "Done"
        button->setText(qtTrId("qtn_comm_command_done"));
        button->setObjectName(ButtonNameDone);
        break;
    case M::NoStandardButton:
    default:
        Q_ASSERT(false); // Should never be reached
        delete button;
        button = 0;
        break;
    }

    return button;
}

MDialogModel::~MDialogModel()
{
    foreach(MButtonModel *button, _buttons()) {
        button->decreaseReferenceCount();
    }
}
QGraphicsWidget *MDialogModel::centralWidget()
{
    const MDialogModel *constThis = this;
    return const_cast<QGraphicsWidget *>(constThis->centralWidget());
}

void MDialogModel::addButton(MButtonModel *button)
{
    MDialogButtonsList &list = _buttons();

    if (button && button->styleName().isEmpty()) {
        if (list.isEmpty())
            button->setStyleName("CommonQueryActionButton");
        else
            button->setStyleName("CommonQueryButton");
    }

    if (list.indexOf(button) == -1) {
        list.append(button);
        button->increaseReferenceCount();
        memberModified(Buttons);
    }
}

MButtonModel *MDialogModel::addButton(M::StandardButton buttonType)
{
    MButtonModel *buttonModel = 0;

    if (buttonType != M::NoStandardButton) {
        buttonModel = button(buttonType);

        if (!buttonModel) {
            buttonModel = createStandardButton(buttonType);
            buttonModel->setRole(roleFor(buttonType));
            addButton(buttonModel);
        }
    }

    return buttonModel;
}

void MDialogModel::removeButton(MButtonModel *button)
{
    Q_ASSERT(button);
    MDialogButtonsList &list = _buttons();

    if (list.removeOne(button)) {
        memberModified(Buttons);
        button->decreaseReferenceCount();
    }
}

MButtonModel *MDialogModel::button(M::StandardButton which)
{
    MDialogButtonsList &list = _buttons();
    MButtonModel *buttonModel = 0;
    int i = 0;

    const int listSize = list.size();
    while (buttonModel == 0 && i < listSize) {
        if (standardButton(list[i]) == which) {
            buttonModel = list[i];
        } else {
            ++i;
        }
    }

    return buttonModel;
}

MButtonModel *MDialogModel::button(int index)
{
    MDialogButtonsList &list = _buttons();
    if (index >= 0 && index < list.size()) {
        return list[index];
    } else {
        return 0;
    }
}

M::StandardButton MDialogModel::standardButton(const MButtonModel *button) const
{
    M::StandardButton result = M::NoStandardButton;

    if (button == NULL)
        return result;

    if (button->objectName() == MDialogModelPrivate::ButtonNameOk) {
        result = M::OkButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameOpen) {
        result = M::OpenButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameSave) {
        result = M::SaveButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameCancel) {
        result = M::CancelButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameClose) {
        result = M::CloseButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameDiscard) {
        result = M::DiscardButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameApply) {
        result = M::ApplyButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameReset) {
        result = M::ResetButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameRestoreDefaults) {
        result = M::RestoreDefaultsButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameHelp) {
        result = M::HelpButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameSaveAll) {
        result = M::SaveAllButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameYes) {
        result = M::YesButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameYesToAll) {
        result = M::YesToAllButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameNo) {
        result = M::NoButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameNoToAll) {
        result = M::NoToAllButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameAbort) {
        result = M::AbortButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameRetry) {
        result = M::RetryButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameIgnore) {
        result = M::IgnoreButton;
    } else if (button->objectName() == MDialogModelPrivate::ButtonNameDone) {
        result = M::DoneButton;
    }

    return result;
}

M::ButtonRole MDialogModel::roleFor(M::StandardButton button)
{
    switch (button) {
    case M::OkButton:
    case M::SaveButton:
    case M::OpenButton:
    case M::SaveAllButton:
    case M::RetryButton:
    case M::IgnoreButton:
    case M::DoneButton:
        return M::AcceptRole;

    case M::CancelButton:
    case M::CloseButton:
    case M::AbortButton:
        return M::RejectRole;

    case M::DiscardButton:
        return M::DestructiveRole;

    case M::HelpButton:
        return M::HelpRole;

    case M::ApplyButton:
        return M::ApplyRole;

    case M::YesButton:
    case M::YesToAllButton:
        return M::YesRole;

    case M::NoButton:
    case M::NoToAllButton:
        return M::NoRole;

    case M::RestoreDefaultsButton:
    case M::ResetButton:
        return M::ResetRole;

    default:
        return M::InvalidRole;
    }
}
