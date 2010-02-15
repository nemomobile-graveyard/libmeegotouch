/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include "duidialogmodel.h"
#include <duibuttonmodel.h>

namespace DuiDialogModelPrivate
{
    const QString ButtonNameOk = "DuiDialogButtonOk";
    const QString ButtonNameSave = "DuiDialogButtonSave";
    const QString ButtonNameSaveAll = "DuiDialogButtonSaveAll";
    const QString ButtonNameOpen = "DuiDialogButtonOpen";
    const QString ButtonNameYes = "DuiDialogButtonYes";
    const QString ButtonNameYesToAll = "DuiDialogButtonYesToAll";
    const QString ButtonNameNo = "DuiDialogButtonNo";
    const QString ButtonNameNoToAll = "DuiDialogButtonNoToAll";
    const QString ButtonNameAbort = "DuiDialogButtonAbort";
    const QString ButtonNameRetry = "DuiDialogButtonRetry";
    const QString ButtonNameIgnore = "DuiDialogButtonIgnore";
    const QString ButtonNameClose = "DuiDialogButtonClose";
    const QString ButtonNameCancel = "DuiDialogButtonCancel";
    const QString ButtonNameDiscard = "DuiDialogButtonDiscard";
    const QString ButtonNameHelp = "DuiDialogButtonHelp";
    const QString ButtonNameApply = "DuiDialogButtonApply";
    const QString ButtonNameReset = "DuiDialogButtonReset";
    const QString ButtonNameRestoreDefaults = "DuiDialogButtonRestoreDefaults";
    const QString ButtonNameDone = "DuiDialogButtonDone";

    static DuiButtonModel *createStandardButton(Dui::StandardButton buttonType);
}

using namespace DuiDialogModelPrivate;

DuiButtonModel *DuiDialogModelPrivate::createStandardButton(Dui::StandardButton buttonType)
{
    DuiButtonModel *button = new DuiButtonModel;

    switch (buttonType) {
    case Dui::OkButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.6.doc
        //: Saves changes and closes the dialog
        //% "OK"
        button->setText(qtTrId("qtn_comm_ok"));
        button->setObjectName(ButtonNameOk);
        break;
    case Dui::OpenButton:
        //~ uispec-document ??? FIXME
        //% "Open"
        button->setText(qtTrId("xx_open_button"));
        button->setObjectName(ButtonNameOpen);
        break;
    case Dui::SaveButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.6.doc
        //: For saving changes
        //% "Save"
        button->setText(qtTrId("qtn_comm_save"));
        button->setObjectName(ButtonNameSave);
        break;
    case Dui::CancelButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.6.doc
        //: Discards the changes and closes the dialog
        //% "Cancel"
        button->setText(qtTrId("qtn_comm_cancel"));
        button->setObjectName(ButtonNameCancel);
        break;
    case Dui::CloseButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.6.doc
        //: Command for closing e.g. dialogs
        //% "Close"
        button->setText(qtTrId("qtn_comm_command_close"));
        button->setObjectName(ButtonNameClose);
        break;
    case Dui::DiscardButton:
        //~ uispec-document ??? FIXME
        //% "Discard"
        button->setText(qtTrId("xx_discard_button"));
        button->setObjectName(ButtonNameDiscard);
        break;
    case Dui::ApplyButton:
        //~ uispec-document ??? FIXME
        //% "Apply"
        button->setText(qtTrId("xx_apply_button"));
        button->setObjectName(ButtonNameApply);
        break;
    case Dui::ResetButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.6.doc
        //% "Reset"
        button->setText(qtTrId("qtn_comm_settings_reset"));
        button->setObjectName(ButtonNameReset);
        break;
    case Dui::RestoreDefaultsButton:
        //~ uispec-document ??? FIXME
        //% "Restore defaults"
        button->setText(qtTrId("xx_restore-defaults_button"));
        button->setObjectName(ButtonNameRestoreDefaults);
        break;
    case Dui::HelpButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.6.doc
        //: Opens help for application
        //% "Help"
        button->setText(qtTrId("qtn_comm_help"));
        button->setObjectName(ButtonNameHelp);
        break;
    case Dui::SaveAllButton:
        //~ uispec-document ??? FIXME
        //% "Save all"
        button->setText(qtTrId("xx_save-all_button"));
        button->setObjectName(ButtonNameSaveAll);
        break;
    case Dui::YesButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.6.doc
        //: Command for confirming queries
        //% "Yes"
        button->setText(qtTrId("qtn_comm_command_yes"));
        button->setObjectName(ButtonNameYes);
        break;
    case Dui::YesToAllButton:
        //~ uispec-document ??? FIXME
        //% "Yes to all"
        button->setText(qtTrId("xx_yes-to-all_button"));
        button->setObjectName(ButtonNameYesToAll);
        break;
    case Dui::NoButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.6.doc
        //: Command for declining queries
        //% "No"
        button->setText(qtTrId("qtn_comm_command_no"));
        button->setObjectName(ButtonNameNo);
        break;
    case Dui::NoToAllButton:
        //~ uispec-document ??? FIXME
        //% "No to all"
        button->setText(qtTrId("xx_no-to-all_button"));
        button->setObjectName(ButtonNameNoToAll);
        break;
    case Dui::AbortButton:
        //~ uispec-document ??? FIXME
        //% "Abort"
        button->setText(qtTrId("xx_abort_button"));
        button->setObjectName(ButtonNameAbort);
        break;
    case Dui::RetryButton:
        // There is a “qtn_appl_retry” in
        // “DirectUI_AppletAndLibrary_ UISpecification_0.28.doc”
        // Description:
        // “Restart applet instantiation from the beginning.
        //  When this is tapped, the applet is re-instantiated
        //  to the previous location in the experience canvas.”
        // But this seems to not the correct one ...
        // There is another “Qtn_tui_button_retry” (why the capital ‘Q’?)
        // in “DirectUI_Transfer_and_Share_UI_Specification_0.13.doc”
        // but this seems to be not the right one either ...

        //~ uispec-document ??? FIXME
        //% "Retry"
        button->setText(qtTrId("xx_retry_button"));
        button->setObjectName(ButtonNameRetry);
        break;
    case Dui::IgnoreButton:
        //~ uispec-document ??? FIXME
        //% "Ignore"
        button->setText(qtTrId("xx_ignore_button"));
        button->setObjectName(ButtonNameIgnore);
        break;
    case Dui::DoneButton:
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.7.doc
        //: Command for confirmation
        //% "Done"
        button->setText(qtTrId("qtn_comm_command_done"));
        button->setObjectName(ButtonNameDone);
        break;
    case Dui::NoStandardButton:
    default:
        Q_ASSERT(false); // Should never be reached
        delete button;
        button = 0;
        break;
    }

    return button;
}

DuiWidget *DuiDialogModel::centralWidget()
{
    const DuiDialogModel *constThis = this;
    return const_cast<DuiWidget *>(constThis->centralWidget());
}

void DuiDialogModel::addButton(DuiButtonModel *button)
{
    DuiDialogButtonsList &list = _buttons();

    if (list.indexOf(button) == -1) {
        list.append(button);
        button->setParent(this);
        button->increaseReferenceCount();
        memberModified(Buttons);
    }
}

DuiButtonModel *DuiDialogModel::addButton(Dui::StandardButton buttonType)
{
    DuiButtonModel *buttonModel = 0;

    if (buttonType != Dui::NoStandardButton) {
        buttonModel = button(buttonType);

        if (!buttonModel) {
            buttonModel = createStandardButton(buttonType);
            addButton(buttonModel);
        }
    }

    return buttonModel;
}

void DuiDialogModel::removeButton(DuiButtonModel *button)
{
    DuiDialogButtonsList &list = _buttons();

    if (list.removeOne(button)) {
        memberModified(Buttons);
        button->setParent(0);
        button->decreaseReferenceCount();
    }
}

DuiButtonModel *DuiDialogModel::button(Dui::StandardButton which)
{
    DuiDialogButtonsList &list = _buttons();
    DuiButtonModel *buttonModel = 0;
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

DuiButtonModel *DuiDialogModel::button(int index)
{
    DuiDialogButtonsList &list = _buttons();
    if (index >= 0 && index < list.size()) {
        return list[index];
    } else {
        return 0;
    }
}

Dui::StandardButton DuiDialogModel::standardButton(const DuiButtonModel *button) const
{
    Dui::StandardButton result = Dui::NoStandardButton;

    if (button == NULL)
        return result;

    if (button->objectName() == DuiDialogModelPrivate::ButtonNameOk) {
        result = Dui::OkButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameOpen) {
        result = Dui::OpenButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameSave) {
        result = Dui::SaveButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameCancel) {
        result = Dui::CancelButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameClose) {
        result = Dui::CloseButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameDiscard) {
        result = Dui::DiscardButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameApply) {
        result = Dui::ApplyButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameReset) {
        result = Dui::ResetButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameRestoreDefaults) {
        result = Dui::RestoreDefaultsButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameHelp) {
        result = Dui::HelpButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameSaveAll) {
        result = Dui::SaveAllButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameYes) {
        result = Dui::YesButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameYesToAll) {
        result = Dui::YesToAllButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameNo) {
        result = Dui::NoButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameNoToAll) {
        result = Dui::NoToAllButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameAbort) {
        result = Dui::AbortButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameRetry) {
        result = Dui::RetryButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameIgnore) {
        result = Dui::IgnoreButton;
    } else if (button->objectName() == DuiDialogModelPrivate::ButtonNameDone) {
        result = Dui::DoneButton;
    }

    return result;
}
