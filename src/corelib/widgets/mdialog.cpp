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

#include "mdialog.h"
#include "mdialog_p.h"

#include <mbuttonmodel.h>
#include <MDebug>
#include <MDismissEvent>
#include <MHomeButtonPanel>
#include <MLocale>
#include <MScene>
#include <MWindow>

#ifdef Q_WS_X11
#   include <QX11Info>
#   include <X11/Xlib.h>
#endif

#include <QSignalMapper>

#include <MApplication>
#include <MSceneManager>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MDialog)

MDialogPrivate::MDialogPrivate()
    :
    clickedButton(0),
    buttonClickedMapper(0),
    dumbMode(false),
    standAloneWindow(0),
    homeButtonPanel(0),
    suicideAfterDestroyingStandAloneWindow(false)
{
}

MDialogPrivate::~MDialogPrivate()
{
    if (buttonClickedMapper) {
        delete buttonClickedMapper;
        buttonClickedMapper = 0;
    }

    if (standAloneWindow) {
        Q_Q(MDialog);
        standAloneWindow->scene()->removeItem(q);
        delete standAloneWindow;
        standAloneWindow = 0;
    }
}

void MDialogPrivate::init()
{
    Q_Q(MDialog);

    clickedButton = 0;

    q->setFocusPolicy(Qt::ClickFocus);
}

void MDialogPrivate::appear(MSceneWindow::DeletionPolicy policy)
{
    Q_Q(MDialog);
    MWindow *window;

    if (q->isSystemModal()) {
        if (prepareStandAloneAppearance(policy)) {
            q->appear(standAloneWindow);
        }
    } else {
        window = MApplication::activeWindow();
        if (window) {
            q->appear(window, policy);
        } else {
            mWarning("MDialog") << "Cannot appear. No MWindow currently active.";
        }
    }
}

void MDialogPrivate::addStandardButtons(M::StandardButtons standardButtons)
{
    Q_Q(MDialog);

    uint buttonFlag = M::FirstButton;
    while (buttonFlag <= M::LastButton) {
        if (standardButtons.testFlag((M::StandardButton)buttonFlag)) {
            q->model()->addButton((M::StandardButton)buttonFlag);
        }
        buttonFlag = buttonFlag << 1;
    }
}

void MDialogPrivate::_q_buttonClicked(QObject *obj)
{
    Q_Q(MDialog);
    MButtonModel *buttonModel = 0;

    buttonModel = qobject_cast<MButtonModel *>(obj);
    if (buttonModel) {
        clickedButton = buttonModel;
        int result = resultFromStandardButtonId(q->model()->standardButton(buttonModel));
        q->done(result);
    }
}

void MDialogPrivate::updateStandAloneHomeButtonVisibility()
{
    Q_Q(MDialog);

    if (q->isSystemModal()) {
        // Remove the home button if it's there.
        if (homeButtonPanel) {
            if (homeButtonPanel->scene() != 0) {
                Q_ASSERT(homeButtonPanel->scene() == standAloneWindow->scene());
                standAloneWindow->sceneManager()->disappearSceneWindowNow(homeButtonPanel);
            }

            delete homeButtonPanel;
            homeButtonPanel = 0;
        }
    } else {
        // Put a home button on the system modal window
        homeButtonPanel = new MHomeButtonPanel();

        standAloneWindow->connect(homeButtonPanel,
                                  SIGNAL(buttonClicked()), SLOT(showMinimized()));

        standAloneWindow->sceneManager()->appearSceneWindowNow(homeButtonPanel);
    }
}

void MDialogPrivate::_q_onStandAloneDialogDisappeared()
{
    Q_Q(MDialog);
    Q_ASSERT(standAloneWindow != 0);

    q->disconnect(SIGNAL(disappeared()), q, SLOT(_q_onStandAloneDialogDisappeared()));

    standAloneWindow->setScene(0);
    standAloneWindow->deleteLater();
    standAloneWindow = 0;

    // Remove dialog from scene otherwise scene will delete dialog
    // on scene's destructor
    if (q->scene()) {
        q->scene()->removeItem(q);
    }

    if (homeButtonPanel) {
        if (homeButtonPanel->sceneManager())
            homeButtonPanel->sceneManager()->disappearSceneWindowNow(homeButtonPanel);
        delete homeButtonPanel;
        homeButtonPanel = 0;
    }

    if (suicideAfterDestroyingStandAloneWindow) {
        q->deleteLater();
    }
}

bool MDialogPrivate::prepareStandAloneAppearance(MSceneWindow::DeletionPolicy policy)
{
    Q_Q(MDialog);
    bool ok = true;

    if (standAloneWindow == 0) {
        standAloneWindow = new MWindow(new MSceneManager);
        standAloneWindow->setTranslucentBackground(true);
#ifdef Q_WS_X11
        standAloneWindow->setAttribute(Qt::WA_X11NetWmWindowTypeDialog, true);
        standAloneWindow->setWindowModality(Qt::WindowModal);
#endif
        q->connect(q, SIGNAL(disappeared()), SLOT(_q_onStandAloneDialogDisappeared()));
    }

    // We only have a home button in the stand-alone window if the
    // the dialog is supposed to be modeless.
    updateStandAloneHomeButtonVisibility();

    // Dialog will handle its own deletion policy instead of scene manager
    // since in this case the dialog owns the scene manager and not the other
    // way around
    suicideAfterDestroyingStandAloneWindow = (policy == MSceneWindow::DestroyWhenDone);

    standAloneWindow->show();

    // Check whether the dialog is already present in some scene manager
    if (shown) {
        if (q->sceneManager() != standAloneWindow->sceneManager()) {
            q->sceneManager()->disappearSceneWindowNow(q);
        } else {
            ok = false;
        }
    }

    return ok;
}

void MDialogPrivate::updateButtonClickedMappings()
{
    Q_Q(MDialog);

    if (buttonClickedMapper) {
        delete buttonClickedMapper;
        buttonClickedMapper = 0;
    }

    buttonClickedMapper = new QSignalMapper();
    q->connect(buttonClickedMapper, SIGNAL(mapped(QObject *)),
               SLOT(_q_buttonClicked(QObject *)));

    MButtonModel *buttonModel;
    MDialogButtonsList buttonModelsList = q->model()->buttons();

    const int buttonModelsSize = buttonModelsList.size();
    for (int i = 0; i < buttonModelsSize; ++i) {
        buttonModel = buttonModelsList[i];

        buttonClickedMapper->connect(buttonModel, SIGNAL(clicked()), SLOT(map()));
        buttonClickedMapper->setMapping(buttonModel, buttonModel);
    }
}

int MDialogPrivate::resultFromStandardButtonId(int buttonId)
{
    switch (buttonId) {
    case M::NoStandardButton:
    case M::OkButton:
    case M::SaveButton:
    case M::SaveAllButton:
    case M::OpenButton:
    case M::YesButton:
    case M::YesToAllButton:
    case M::RetryButton:
    case M::ApplyButton:
    case M::DoneButton:
        return MDialog::Accepted;
    case M::NoButton:
    case M::NoToAllButton:
    case M::AbortButton:
    case M::IgnoreButton:
    case M::CloseButton:
    case M::CancelButton:
    case M::DiscardButton:
    case M::HelpButton:
    case M::ResetButton:
    case M::RestoreDefaultsButton:
        return MDialog::Rejected;
    default:
        //Non-standard buttons should be handled by NoStandardButton case.
        //This should not happen.
        return MDialog::Accepted;
    };
}

MDialog::MDialog() :
    MSceneWindow(new MDialogPrivate(), new MDialogModel(), MSceneWindow::Dialog, QString())
{
    Q_D(MDialog);

    d->init();

    model()->setResultCode(MDialog::Rejected);
}

MDialog::MDialog(const QString &title, M::StandardButtons buttons) :
    MSceneWindow(new MDialogPrivate(), new MDialogModel(), MSceneWindow::Dialog, QString())
{
    Q_D(MDialog);

    d->init();

    model()->setResultCode(MDialog::Rejected);

    setTitle(title);

    d->addStandardButtons(buttons);
}

MDialog::MDialog(MDialogPrivate *dd, MDialogModel *model, MSceneWindow::WindowType windowType) :
    MSceneWindow(dd, model, windowType, QString())
{
    Q_D(MDialog);

    d->init();

    model->setResultCode(MDialog::Rejected);
}

MDialog::MDialog(MDialogPrivate *dd, M::StandardButtons buttons,
                     MDialogModel *model, MSceneWindow::WindowType windowType) :
    MSceneWindow(dd, model, windowType, QString())
{
    Q_D(MDialog);

    d->init();

    model->setResultCode(MDialog::Rejected);

    d->addStandardButtons(buttons);
}

MDialog::~MDialog()
{
}

QString MDialog::title() const
{
    return model()->title();
}

void MDialog::setTitle(const QString &title)
{
    model()->setTitle(title);
}

QGraphicsWidget *MDialog::centralWidget()
{
    return model()->centralWidget();
}

void MDialog::setCentralWidget(QGraphicsWidget *centralWidget)
{
    if (model()->centralWidget())
        delete model()->centralWidget();

    model()->setCentralWidget(centralWidget);
}

void MDialog::addButton(MButtonModel *buttonModel)
{
    model()->addButton(buttonModel);
}

MButtonModel *MDialog::addButton(const QString &text)
{
    MButtonModel *buttonModel = 0;

    buttonModel = new MButtonModel;
    buttonModel->setText(text);
    model()->addButton(buttonModel);

    return buttonModel;
}

MButtonModel *MDialog::addButton(M::StandardButton buttonType)
{
    return model()->addButton(buttonType);
}

void MDialog::removeButton(MButtonModel *buttonModel)
{
    model()->removeButton(buttonModel);
}

MButtonModel *MDialog::button(M::StandardButton which)
{
    return model()->button(which);
}

M::StandardButton MDialog::standardButton(MButtonModel *buttonModel) const
{
    return model()->standardButton(buttonModel);
}

void MDialog::appear(MSceneWindow::DeletionPolicy policy)
{
    Q_D(MDialog);
    d->appear(policy);
}

void MDialog::appear(MWindow *window, MSceneWindow::DeletionPolicy policy)
{
    Q_D(MDialog);

    if (window) {
        MSceneWindow::appear(window, policy);
    } else {
        d->appear(policy);
    }
}

void MDialog::accept()
{
    done(Accepted);
}

void MDialog::reject()
{
    Q_D(MDialog);

    QObject *sender= QObject::sender();
    if (sender != 0 && sender->objectName() == "MDialogCloseButton") {
        d->clickedButton = 0;
    }
    done(Rejected);
}

void MDialog::done(int result)
{
    setResult(result);

    emit finished(result);
    if (result == MDialog::Accepted)
        emit accepted();
    else if (result == MDialog::Rejected)
        emit rejected();

    if (sceneManager())
        sceneManager()->disappearSceneWindow(this);
}

int MDialog::exec(MWindow *window)
{
    Q_D(MDialog);
    MSceneManager *targetSceneManager = 0;
    int result;

    d->clickedButton = 0;
    d->policy = MSceneWindow::KeepWhenDone;

    if (window) {
        targetSceneManager = window->sceneManager();
    } else {
        if (isSystemModal()) {
            d->prepareStandAloneAppearance(KeepWhenDone);
            targetSceneManager = d->standAloneWindow->sceneManager();
        } else {
            window = MApplication::activeWindow();
            if (window) {
                targetSceneManager = window->sceneManager();
            } else {
                mWarning("MDialog") << "Cannot appear. No MWindow currently active.";
            }
        }
    }

    if (targetSceneManager) {
        result = targetSceneManager->execDialog(this);
    } else {
        result = Rejected;
    }

    return result;
}

MButtonModel *MDialog::clickedButton() const
{
    Q_D(const MDialog);

    return d->clickedButton;
}

int MDialog::result() const
{
    return model()->resultCode();
}

void MDialog::setResult(int result)
{
    model()->setResultCode(result);
}

bool MDialog::isButtonBoxVisible() const
{
    return model()->buttonBoxVisible();
}

void MDialog::setButtonBoxVisible(bool visible)
{
    model()->setButtonBoxVisible(visible);
}

bool MDialog::isCloseButtonVisible() const
{
    return model()->closeButtonVisible();
}

void MDialog::setCloseButtonVisible(bool visible)
{
    model()->setCloseButtonVisible(visible);
}

bool MDialog::isProgressIndicatorVisible() const
{
    return model()->progressIndicatorVisible();
}

void MDialog::setProgressIndicatorVisible(bool visible)
{
    model()->setProgressIndicatorVisible(visible);
}

bool MDialog::isTitleBarVisible() const
{
    return model()->titleBarVisible();
}

void MDialog::setTitleBarVisible(bool visible)
{
    model()->setTitleBarVisible(visible);
}

bool MDialog::isSystemModal() const
{
    return model()->systemModal();
}

void MDialog::setSystemModal(bool systemModal)
{
    model()->setSystemModal(systemModal);
}

void MDialog::setLayout(QGraphicsLayout *layout)
{
    Q_D(MDialog);
    mWarning("MDialog:") << "Please don't change the layout directly.";
    d->dumbMode = true;
    QGraphicsWidget::setLayout(layout);
}

QGraphicsLayout *MDialog::layout()
{
    Q_D(MDialog);
    mWarning("MDialog:") << "Please don't change the layout directly.";
    d->dumbMode = true;
    return QGraphicsWidget::layout();
}

void MDialog::updateData(const QList<const char *>& modifications)
{
    MSceneWindow::updateData(modifications);
    Q_D(MDialog);

    if (modifications.contains(MDialogModel::Buttons)) {
        d->updateButtonClickedMappings();
        d->clickedButton = 0;
    }
}

void MDialog::setupModel()
{
    MSceneWindow::setupModel();
    Q_D(MDialog);
    d->updateButtonClickedMappings();
    d->clickedButton = 0;
}

void MDialog::dismissEvent(MDismissEvent *event)
{
    setResult(Rejected);
    emit finished(result());
    emit rejected();

    event->accept();
}

#include "moc_mdialog.cpp"
