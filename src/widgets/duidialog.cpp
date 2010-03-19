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

#include "duidialog.h"
#include "duidialog_p.h"

#include <duibuttonmodel.h>
#include <DuiDebug>
#include <DuiDismissEvent>
#include <DuiHomeButtonPanel>
#include <DuiLocale>
#include <DuiScene>
#include <DuiWindow>

#ifdef Q_WS_X11
#   include <QX11Info>
#   include <X11/Xlib.h>
#endif

#include <QSignalMapper>

#include <DuiApplication>
#include <DuiSceneManager>

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiDialog)

DuiDialogPrivate::DuiDialogPrivate()
    :
    clickedButton(0),
    buttonClickedMapper(0),
    dumbMode(false),
    standAloneWindow(0),
    homeButtonPanel(0),
    suicideAfterDestroyingStandAloneWindow(false)
{
}

DuiDialogPrivate::~DuiDialogPrivate()
{
    if (buttonClickedMapper) {
        delete buttonClickedMapper;
        buttonClickedMapper = 0;
    }

    if (standAloneWindow) {
        Q_Q(DuiDialog);
        standAloneWindow->scene()->removeItem(q);
        delete standAloneWindow;
        standAloneWindow = 0;
    }
}

void DuiDialogPrivate::init()
{
    Q_Q(DuiDialog);

    clickedButton = 0;

    q->setFocusPolicy(Qt::ClickFocus);
}

void DuiDialogPrivate::appear(bool now, DuiSceneWindow::DeletionPolicy policy)
{
    Q_Q(DuiDialog);
    DuiWindow *window;

    if (q->isWindowModal()) {
        if (prepareStandAloneAppearance(policy)) {
            if (now) {
                q->appearNow(standAloneWindow);
            } else {
                q->appear(standAloneWindow);
            }
        }
    } else {
        window = DuiApplication::activeWindow();
        if (window) {
            if (now) {
                q->appearNow(window, policy);
            } else {
                q->appear(window, policy);
            }
        } else {
            duiWarning("DuiDialog") << "Cannot appear. No DuiWindow currently active.";
        }
    }
}

void DuiDialogPrivate::addStandardButtons(Dui::StandardButtons standardButtons)
{
    Q_Q(DuiDialog);

    uint buttonFlag = Dui::FirstButton;
    while (buttonFlag <= Dui::LastButton) {
        if (standardButtons.testFlag((Dui::StandardButton)buttonFlag)) {
            q->model()->addButton((Dui::StandardButton)buttonFlag);
        }
        buttonFlag = buttonFlag << 1;
    }
}

void DuiDialogPrivate::_q_buttonClicked(QObject *obj)
{
    Q_Q(DuiDialog);
    DuiButtonModel *buttonModel = 0;

    buttonModel = qobject_cast<DuiButtonModel *>(obj);
    if (buttonModel) {
        clickedButton = buttonModel;
        int result = resultFromStandardButtonId(q->model()->standardButton(buttonModel));
        q->done(result);
    }
}

void DuiDialogPrivate::updateStandAloneHomeButtonVisibility()
{
    Q_Q(DuiDialog);

    if (q->isWindowModal()) {
        // Remove the home button if it's there.
        if (homeButtonPanel) {
            if (homeButtonPanel->scene() != 0) {
                Q_ASSERT(homeButtonPanel->scene() == standAloneWindow->scene());
                homeButtonPanel->disappearNow();
            }

            delete homeButtonPanel;
            homeButtonPanel = 0;
        }
    } else {
        // Put a home button on the system modal window
        homeButtonPanel = new DuiHomeButtonPanel();

        standAloneWindow->connect(homeButtonPanel,
                                  SIGNAL(buttonClicked()), SLOT(showMinimized()));

        homeButtonPanel->appearNow(standAloneWindow);
    }
}

void DuiDialogPrivate::_q_onStandAloneDialogDisappeared()
{
    Q_Q(DuiDialog);
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
        homeButtonPanel->disappearNow();
        delete homeButtonPanel;
        homeButtonPanel = 0;
    }

    if (suicideAfterDestroyingStandAloneWindow) {
        q->deleteLater();
    }
}

bool DuiDialogPrivate::prepareStandAloneAppearance(DuiSceneWindow::DeletionPolicy policy)
{
    Q_Q(DuiDialog);
    bool ok = true;

    if (standAloneWindow == 0) {
        standAloneWindow = new DuiWindow(new DuiSceneManager);
        standAloneWindow->setTranslucentBackground(true);
#ifdef Q_WS_X11
        XSetTransientForHint(QX11Info::display(), standAloneWindow->winId(), DuiApplication::activeWindow()->winId());
#endif
        q->connect(q, SIGNAL(disappeared()), SLOT(_q_onStandAloneDialogDisappeared()));
    }

    // We only have a home button in the stand-alone window if the
    // the dialog is supposed to be modeless.
    updateStandAloneHomeButtonVisibility();

    // Dialog will handle its own deletion policy instead of scene manager
    // since in this case the dialog owns the scene manager and not the other
    // way around
    suicideAfterDestroyingStandAloneWindow = (policy == DuiSceneWindow::DestroyWhenDone);

    standAloneWindow->show();

    // Check whether the dialog is already present in some scene manager
    if (shown) {
        if (q->sceneManager() != standAloneWindow->sceneManager()) {
            q->disappearNow();
        } else {
            ok = false;
        }
    }

    return ok;
}

void DuiDialogPrivate::updateButtonClickedMappings()
{
    Q_Q(DuiDialog);

    if (buttonClickedMapper) {
        delete buttonClickedMapper;
        buttonClickedMapper = 0;
    }

    buttonClickedMapper = new QSignalMapper();
    q->connect(buttonClickedMapper, SIGNAL(mapped(QObject *)),
               SLOT(_q_buttonClicked(QObject *)));

    DuiButtonModel *buttonModel;
    DuiDialogButtonsList buttonModelsList = q->model()->buttons();

    const int buttonModelsSize = buttonModelsList.size();
    for (int i = 0; i < buttonModelsSize; ++i) {
        buttonModel = buttonModelsList[i];

        buttonClickedMapper->connect(buttonModel, SIGNAL(clicked()), SLOT(map()));
        buttonClickedMapper->setMapping(buttonModel, buttonModel);
    }
}

int DuiDialogPrivate::resultFromStandardButtonId(int buttonId)
{
    switch (buttonId) {
    case Dui::NoStandardButton:
    case Dui::OkButton:
    case Dui::SaveButton:
    case Dui::SaveAllButton:
    case Dui::OpenButton:
    case Dui::YesButton:
    case Dui::YesToAllButton:
    case Dui::RetryButton:
    case Dui::ApplyButton:
    case Dui::DoneButton:
        return DuiDialog::Accepted;
    case Dui::NoButton:
    case Dui::NoToAllButton:
    case Dui::AbortButton:
    case Dui::IgnoreButton:
    case Dui::CloseButton:
    case Dui::CancelButton:
    case Dui::DiscardButton:
    case Dui::HelpButton:
    case Dui::ResetButton:
    case Dui::RestoreDefaultsButton:
        return DuiDialog::Rejected;
    default:
        //Non-standard buttons should be handled by NoStandardButton case.
        //This should not happen.
        return DuiDialog::Accepted;
    };
}

DuiDialog::DuiDialog() :
    DuiSceneWindow(new DuiDialogPrivate(), new DuiDialogModel(), DuiSceneWindow::Dialog, QString())
{
    Q_D(DuiDialog);

    d->init();

    model()->setResultCode(DuiDialog::Rejected);
}

DuiDialog::DuiDialog(const QString &title, DuiWidget *centralWidget, Dui::StandardButtons buttons) :
    DuiSceneWindow(new DuiDialogPrivate(), new DuiDialogModel(), DuiSceneWindow::Dialog, QString())
{
    Q_D(DuiDialog);

    d->init();

    model()->setResultCode(DuiDialog::Rejected);

    setTitle(title);

    d->addStandardButtons(buttons);

    setCentralWidget(centralWidget);
}

DuiDialog::DuiDialog(const QString &title, Dui::StandardButtons buttons) :
    DuiSceneWindow(new DuiDialogPrivate(), new DuiDialogModel(), DuiSceneWindow::Dialog, QString())
{
    Q_D(DuiDialog);

    d->init();

    model()->setResultCode(DuiDialog::Rejected);

    setTitle(title);

    d->addStandardButtons(buttons);
}

DuiDialog::DuiDialog(DuiDialogPrivate *dd, DuiDialogModel *model, DuiSceneWindow::WindowType windowType) :
    DuiSceneWindow(dd, model, windowType, QString())
{
    Q_D(DuiDialog);

    d->init();

    model->setResultCode(DuiDialog::Rejected);
}

DuiDialog::DuiDialog(DuiDialogPrivate *dd, Dui::StandardButtons buttons,
                     DuiDialogModel *model, DuiSceneWindow::WindowType windowType) :
    DuiSceneWindow(dd, model, windowType, QString())
{
    Q_D(DuiDialog);

    d->init();

    model->setResultCode(DuiDialog::Rejected);

    d->addStandardButtons(buttons);
}

DuiDialog::~DuiDialog()
{
}

QString DuiDialog::title() const
{
    return model()->title();
}

void DuiDialog::setTitle(const QString &title)
{
    model()->setTitle(title);
}

DuiWidget *DuiDialog::centralWidget()
{
    return model()->centralWidget();
}

void DuiDialog::setCentralWidget(DuiWidget *centralWidget)
{
    if (model()->centralWidget())
        delete model()->centralWidget();

    model()->setCentralWidget(centralWidget);
}

void DuiDialog::addButton(DuiButtonModel *buttonModel)
{
    model()->addButton(buttonModel);
}

DuiButtonModel *DuiDialog::addButton(const QString &text)
{
    DuiButtonModel *buttonModel = 0;

    buttonModel = new DuiButtonModel;
    buttonModel->setText(text);
    model()->addButton(buttonModel);

    return buttonModel;
}

DuiButtonModel *DuiDialog::addButton(Dui::StandardButton buttonType)
{
    return model()->addButton(buttonType);
}

void DuiDialog::removeButton(DuiButtonModel *buttonModel)
{
    model()->removeButton(buttonModel);
}

DuiButtonModel *DuiDialog::button(Dui::StandardButton which)
{
    return model()->button(which);
}

Dui::StandardButton DuiDialog::standardButton(DuiButtonModel *buttonModel) const
{
    return model()->standardButton(buttonModel);
}

void DuiDialog::appear(DuiSceneWindow::DeletionPolicy policy)
{
    Q_D(DuiDialog);
    d->appear(false, policy);
}

void DuiDialog::appear(DuiWindow *window, DuiSceneWindow::DeletionPolicy policy)
{
    Q_D(DuiDialog);

    if (window) {
        DuiSceneWindow::appear(window, policy);
    } else {
        d->appear(false, policy);
    }
}

void DuiDialog::appearNow(DuiSceneWindow::DeletionPolicy policy)
{
    Q_D(DuiDialog);
    d->appear(true, policy);
}

void DuiDialog::appearNow(DuiWindow *window, DuiSceneWindow::DeletionPolicy policy)
{
    Q_D(DuiDialog);

    if (window) {
        DuiSceneWindow::appearNow(window, policy);
    } else {
        d->appear(true, policy);
    }
}

void DuiDialog::accept()
{
    done(Accepted);
}

void DuiDialog::reject()
{
    done(Rejected);
}

void DuiDialog::done(int result)
{
    setResult(result);

    emit finished(result);
    if (result == DuiDialog::Accepted)
        emit accepted();
    else if (result == DuiDialog::Rejected)
        emit rejected();

    if (sceneManager())
        sceneManager()->hideWindow(this);
}

int DuiDialog::exec(DuiWindow *window)
{
    Q_D(DuiDialog);
    DuiSceneManager *targetSceneManager = 0;
    int result;

    d->clickedButton = 0;
    d->policy = DuiSceneWindow::KeepWhenDone;

    if (window) {
        targetSceneManager = window->sceneManager();
    } else {
        if (isWindowModal()) {
            d->prepareStandAloneAppearance(KeepWhenDone);
            targetSceneManager = d->standAloneWindow->sceneManager();
        } else {
            window = DuiApplication::activeWindow();
            if (window) {
                targetSceneManager = window->sceneManager();
            } else {
                duiWarning("DuiDialog") << "Cannot appear. No DuiWindow currently active.";
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

DuiButtonModel *DuiDialog::clickedButton() const
{
    Q_D(const DuiDialog);

    return d->clickedButton;
}

int DuiDialog::result() const
{
    return model()->resultCode();
}

void DuiDialog::setResult(int result)
{
    model()->setResultCode(result);
}

bool DuiDialog::isButtonBoxVisible() const
{
    return model()->buttonBoxVisible();
}

void DuiDialog::setButtonBoxVisible(bool visible)
{
    model()->setButtonBoxVisible(visible);
}

bool DuiDialog::isCloseButtonVisible() const
{
    return model()->closeButtonVisible();
}

void DuiDialog::setCloseButtonVisible(bool visible)
{
    model()->setCloseButtonVisible(visible);
}

bool DuiDialog::isProgressIndicatorVisible() const
{
    return model()->progressIndicatorVisible();
}

void DuiDialog::setProgressIndicatorVisible(bool visible)
{
    model()->setProgressIndicatorVisible(visible);
}

bool DuiDialog::isTitleBarVisible() const
{
    return model()->titleBarVisible();
}

void DuiDialog::setTitleBarVisible(bool visible)
{
    model()->setTitleBarVisible(visible);
}

bool DuiDialog::isWindowModal() const
{
    return model()->windowModal();
}

void DuiDialog::setWindowModal(bool windowModal)
{
    model()->setWindowModal(windowModal);
}

void DuiDialog::setLayout(QGraphicsLayout *layout)
{
    Q_D(DuiDialog);
    duiWarning("DuiDialog:") << "Please don't change the layout directly.";
    d->dumbMode = true;
    QGraphicsWidget::setLayout(layout);
}

QGraphicsLayout *DuiDialog::layout()
{
    Q_D(DuiDialog);
    duiWarning("DuiDialog:") << "Please don't change the layout directly.";
    d->dumbMode = true;
    return QGraphicsWidget::layout();
}

void DuiDialog::updateData(const QList<const char *>& modifications)
{
    DuiSceneWindow::updateData(modifications);
    Q_D(DuiDialog);

    if (modifications.contains(DuiDialogModel::Buttons)) {
        d->updateButtonClickedMappings();
        d->clickedButton = 0;
    }
}

void DuiDialog::setupModel()
{
    DuiSceneWindow::setupModel();
    Q_D(DuiDialog);
    d->updateButtonClickedMappings();
    d->clickedButton = 0;
}

void DuiDialog::dismissEvent(DuiDismissEvent *event)
{
    setResult(Rejected);
    emit finished(result());
    emit rejected();

    event->accept();
}

void DuiDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
    reject();
}

#include "moc_duidialog.cpp"
