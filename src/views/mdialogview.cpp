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

#include "mdialogview.h"
#include "mdialogview_p.h"

#include "mdialog.h"
#include "mdialog_p.h"
#include "mviewcreator.h"
#include "mpannableviewport.h"
#include "mscenemanager.h"
#include "mimagewidget.h"
#include <MLabel>
#include <MButton>
#include <MProgressIndicator>
#include <MLayout>
#include <MGridLayoutPolicy>
#include <MPositionIndicator>
#include "mbuttongrouplayoutpolicy_p.h"

#include <QFont>
#include <QGraphicsLinearLayout>

MDialogViewPrivate::MDialogViewPrivate()
    : q_ptr(0),
      controller(0),
      rootGrid(0),
      rootPolicy(0),
      topSpacer(0),
      bottomSpacer(0),
      leftSpacer(0),
      rightSpacer(0),
      leftButtonSpacer(0),
      rightButtonSpacer(0),
      buttonContainer(0),
      centerLayout(0),
      dialogBox(0),
      dialogBoxLayout(0),
      contents(0),
      contentsLayout(0),
      contentsViewport(0),
      centralWidget(0),
      titleBar(0),
      titleLabel(0),
      spinner(0),
      closeButton(0),
      buttonBox(0),
      buttonBoxLayout(0),
      buttonBoxLayoutPolicy(0),
      titleBarIconImage(0),
      policyTitle(0),
      policyTitleClose(0),
      policyIconTitleClose(0),
      policySpinnerTitleClose(0),
      policyIconTitle(0),
      policySpinnerTitle(0)
{
}

MDialogViewPrivate::~MDialogViewPrivate()
{
}

void MDialogViewPrivate::createWidgetHierarchy()
{
    // We don't require any painting on the controller.
    controller->setFlag(QGraphicsItem::ItemHasNoContents, true);

    rootGrid = new MLayout();
    rootGrid->setContentsMargins(0, 0, 0, 0);

    rootPolicy = new MGridLayoutPolicy(rootGrid);
    rootGrid->setPolicy(rootPolicy);
    // Cast here is needed to invoke method from QGraphicsWidget, since
    // overloaded method invokes dumbMode, preventing dialog from propper
    // styling.
    static_cast<QGraphicsWidget *>(controller)->setLayout(rootGrid);

    topSpacer = createSpacer();
    rootPolicy->addItem(topSpacer, 0, 1);

    leftSpacer = createSpacer();
    leftSpacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    rootPolicy->addItem(leftSpacer, 1, 0);
    createDialogBox();
    rootPolicy->addItem(dialogBox, 1, 1);
    rightSpacer = createSpacer();
    rightSpacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    rootPolicy->addItem(rightSpacer, 1, 2);
    createButtonBox();
    dialogBoxLayout->addItem(buttonBox);

}

QGraphicsWidget *MDialogViewPrivate::createSpacer()
{
    MWidgetController *spacer = new MTransparentWidget();

    spacer->hide();
    spacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    spacer->setMinimumSize(0, 0);
    spacer->setPreferredSize(0, 0);

    return spacer;
}

QGraphicsLinearLayout *MDialogViewPrivate::createLayout(Qt::Orientation orientation)
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(orientation);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    return layout;
}

void MDialogViewPrivate::createDialogBox()
{
    dialogBox = new MWidgetController;
    dialogBox->setObjectName("MDialogBox");
    dialogBoxLayout = createLayout(Qt::Vertical);
    dialogBox->setLayout(dialogBoxLayout);

    createTitleBar();
    dialogBoxLayout->addItem(titleBar);

    contentsViewport = new MPannableViewport;
    contentsViewport->setStyleName("MDialogContentsViewport");
    contentsViewport->positionIndicator()->setStyleName("CommonPositionIndicatorInverted");
    contentsViewport->setObjectName(contentsViewport->styleName());
    dialogBoxLayout->addItem(contentsViewport);

    contents = new MWidgetController();
    contents->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    contentsViewport->setWidget(contents);
    contentsLayout = createLayout(Qt::Vertical);
    contentsLayout->setContentsMargins(0, 0, 0, 0);
    contentsLayout->setSpacing(0);
    contents->setLayout(contentsLayout);
}

void MDialogViewPrivate::createButtonBox()
{
    buttonBox = new MLabel;
    buttonBox->setStyleName("MDialogButtonBox");
    buttonBox->setObjectName(buttonBox->styleName());
    buttonBoxLayout = new MLayout();
    buttonBoxLayoutPolicy = new MButtonGroupLayoutPolicy(buttonBoxLayout, Qt::Horizontal);
    buttonBoxLayout->setPolicy(buttonBoxLayoutPolicy);
    buttonBoxLayout->setContentsMargins(0, 0, 0, 0);
    buttonBoxLayoutPolicy->setContentsMargins(0,0,0,0);
    buttonBoxLayoutPolicy->setSpacing(0);

    buttonBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    buttonBoxLayout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    centerLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    buttonBox->setLayout(centerLayout);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->setSpacing(0);
    buttonContainer = new MWidget;
    buttonContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    buttonContainer->setContentsMargins(0,0,0,0);
    buttonContainer->setLayout(buttonBoxLayout);
    centerLayout->addItem(buttonContainer);
}

void MDialogViewPrivate::realignButtonBox()
{
    if (leftButtonSpacer) {
        centerLayout->removeItem(leftButtonSpacer);
        delete leftButtonSpacer;
        leftButtonSpacer = 0;
    }
    if (rightButtonSpacer) {
        centerLayout->removeItem(rightButtonSpacer);
        delete rightButtonSpacer;
        rightButtonSpacer = 0;
    }
    if (q_func()->style()->buttonBoxCentered() && q_func()->style()->dialogButtonFixedWidth()>0) {
        leftButtonSpacer = createSpacer();
        centerLayout->insertItem(0, leftButtonSpacer);
        rightButtonSpacer = createSpacer();
        centerLayout->insertItem(2, rightButtonSpacer);
    }
}

void MDialogViewPrivate::createTitleBar()
{
    MLayout *layout = new MLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    // OBS: Need any simple class whose styling works.
    //      For some reason neither MWidgetController nor MStylableWidget get its
    //      style loaded.
    titleBar = new MDialogInternalBox;
    titleBar->setStyleName("MDialogTitleBar");
    titleBar->setObjectName(titleBar->styleName());
    titleBar->setLayout(layout);
    titleBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    titleLabel = new MLabel(titleBar);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setTextElide(true);
    titleLabel->setStyleName("CommonDialogHeader");

    closeButton = new MButton(titleBar);
    closeButton->setStyleName("MDialogCloseButton");
    closeButton->setObjectName(closeButton->styleName());
    closeButton->setViewType("icon");
    closeButton->setIconID("icon-m-framework-close");
    policyTitle = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    policyTitle->setSpacing(0);
    policyTitle->addItem(titleLabel);

    policyTitleClose = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    policyTitleClose->setSpacing(0);
    policyTitleClose->addItem(titleLabel);
    policyTitleClose->addItem(closeButton);

    layout->setPolicy(policyTitleClose);

    QObject::connect(closeButton, SIGNAL(clicked()), controller, SLOT(reject()));
}

MDialogView::MDialogView(MDialog *controller) :
    MSceneWindowView(controller),
    d_ptr(new MDialogViewPrivate)
{
    Q_D(MDialogView);
    d->q_ptr = this;
    d->controller = controller;
    d->createWidgetHierarchy();
}

MDialogView::MDialogView(MDialogViewPrivate &dd, MDialog *controller)
    : MSceneWindowView(controller),
      d_ptr(&dd)
{
    Q_D(MDialogView);
    d->q_ptr = this;
    d->controller = controller;
    d->createWidgetHierarchy();
}


MDialogView::~MDialogView()
{
    Q_D(MDialogView);

    // Make sure the model's centralWidget doesn't get deleted along with us.
    d->setCentralWidget(0);

    delete d_ptr;
}

void MDialogView::applyStyle()
{
    Q_D(MDialogView);
    MDialogPrivate *dialogPrivate = d->controller->d_func();

    if (dialogPrivate->dumbMode) {
        return;
    }

    d->contentsViewport->setStyleName(style()->contentsViewportStyleName());
    d->buttonBox->setStyleName(style()->buttonBoxStyleName());
    d->titleBar->setStyleName(style()->titleBarStyleName());
    d->titleLabel->setStyleName(style()->titleLabelStyleName());

    MSceneWindowView::applyStyle();
    d->realignButtonBox();

    d->closeButton->setVisible(d->hasCloseButton());
    d->updateTitleBarVisibility();

    d->titleBar->setMinimumHeight(style()->titleBarHeight());
    d->titleBar->setPreferredHeight(style()->titleBarHeight());
    d->titleBar->setMaximumHeight(style()->titleBarHeight());

    d->titleLabel->setAlignment(style()->titleBarAlignment());
    if (style()->titleCapitalization()) {
        QFont labelFont = d->titleLabel->font();
        labelFont.setCapitalization(QFont::AllUppercase);
        d->titleLabel->setFont(labelFont);
    }

    // set dimensions separately to that we may have one dimensions set
    // and the other unset (-1).
    d->dialogBox->setMinimumWidth(style()->dialogMinimumSize().width());
    d->dialogBox->setMinimumHeight(style()->dialogMinimumSize().height());

    // set dimensions separately to that we may have one dimensions set
    // and the other unset (-1).
    d->dialogBox->setPreferredWidth(style()->dialogPreferredSize().width());
    d->dialogBox->setPreferredHeight(style()->dialogPreferredSize().height());
    d->buttonBox->setContentsMargins(0, 0, 0, 0);

   // d->buttonBox->setContentsMargins(0, style()->verticalSpacing(), 0, 0);

    d->buttonBoxLayoutPolicy->setButtonWidth(style()->dialogButtonFixedWidth());
    d->buttonBoxLayoutPolicy->setSpacing(style()->buttonSpacing());

    d->updateButtonBoxLayoutOrientation();

    d->rootPolicy->setContentsMargins(
        style()->dialogLeftMargin(), /* left */
        style()->dialogTopMargin(), /* top */
        style()->dialogRightMargin(), /* right */
        style()->dialogBottomMargin() /* bottom */);


    d->setupDialogVerticalAlignment();
    d->updateTitleBarLayoutPolicy();
}

void MDialogViewPrivate::setupDialogVerticalAlignment()
{
    Q_Q(MDialogView);

    if (q->style()->dialogVerticalAlignment() & Qt::AlignVCenter
            && bottomSpacer == 0) {

        // switch to center alignment
        bottomSpacer = createSpacer();
        rootPolicy->addItem(bottomSpacer, 2, 1);

    } else if (!(q->style()->dialogVerticalAlignment() & Qt::AlignVCenter)
               && bottomSpacer != 0) {

        // switch to bottom alignment.
        rootPolicy->removeItem(bottomSpacer);
        delete bottomSpacer;
        bottomSpacer = 0;
    }
}

void MDialogViewPrivate::updateTitleBarVisibility()
{
    Q_Q(MDialogView);
    bool visible = q->style()->titleBarHeight() > 0.0 && q->style()->hasTitleBar();

    if (titleBar->isVisible() == visible) {
        // nothing to do here
        return;
    }

    // invisible items still occupy space in layouts, therefore I manually have
    // to remove them.

    if (visible) {
        // I am currenly invisible and therefore not part of the layout.
        // To become visible I have to be added to the layout.
        dialogBoxLayout->insertItem(0, titleBar);
    } else {
        // I am currently visible and therefore part of the layout.
        // To become invisible I have to be removed from it.
        dialogBoxLayout->removeItem(titleBar);
    }

    // update visibility property
    titleBar->setVisible(visible);

    if (visible) {
        contentsViewport->setViewType(MPannableWidget::defaultType);
    } else {
        contentsViewport->setViewType("titlebarless");
    }
}

void MDialogViewPrivate::updateWidgetVisibility(QGraphicsWidget *widget, bool visible, int index,
        QGraphicsLinearLayout *layout)
{
    if (widget->isVisible() == visible) {
        // nothing to do here
        return;
    }

    // invisible items still occupy space in layouts, therefore I manually have
    // to remove them.

    if (visible) {
        // I am currenly invisible and therefore not part of the layout.
        // To become visible I have to be added to the layout.
        layout->insertItem(index, widget);
    } else {
        // I am currently visible and therefore part of the layout.
        // To become invisible I have to be removed from it.
        layout->removeItem(widget);
    }

    // update visibility property
    widget->setVisible(visible);
}

void MDialogViewPrivate::repopulateButtonBox()
{
    Q_Q(MDialogView);
    MButton *button;

    while (buttonBoxLayout->count() > 0) {
        button = static_cast<MButton *>(buttonBoxLayout->itemAt(0));
        buttonBoxLayout->removeAt(0);
        delete button;
    }

    MDialogButtonsList buttonModelsList = q->model()->buttons();

    const int buttonModelsSize = buttonModelsList.size();
    for (int i = 0; i < buttonModelsSize; ++i) {
        addButton(q->model()->button(i));
    }
}

void MDialogViewPrivate::updateButtonBox()
{
    removeButtonsNotInDialogModel();
    addButtonsFromDialogModel();
    updateButtonBoxLayoutOrientation();
}

void MDialogViewPrivate::updateButtonBoxLayoutOrientation()
{
    Q_Q(MDialogView);
    buttonBoxLayoutPolicy->setOrientation(q->style()->buttonBoxOrientation());

    if (q->style()->maximumHorizontalButtons() < buttonBoxLayoutPolicy->count())
            buttonBoxLayoutPolicy->setOrientation(Qt::Vertical);

    if (q->model()->buttons().count() == 0 || buttonBoxLayoutPolicy->orientation() == Qt::Horizontal) {
        buttonBox->setPreferredHeight(buttonBox->minimumHeight() + q->style()->verticalSpacing());
    } else {
        buttonBox->setPreferredHeight((q->model()->buttons().count() * buttonBox->minimumHeight()) + q->style()->verticalSpacing());
    }
}

void MDialogViewPrivate::removeButtonsNotInDialogModel()
{
    Q_Q(MDialogView);
    MButton *button;
    MDialogButtonsList buttonModelsList = q->model()->buttons();
    int i = 0;

    while (i < buttonBoxLayout->count()) {
        button = static_cast<MButton *>(buttonBoxLayout->itemAt(i));
        if (!buttonModelsList.contains(button->model())) {
            buttonBoxLayout->removeAt(i);
            delete button;
        } else {
            ++i;
        }
    }
}

void MDialogViewPrivate::addButtonsFromDialogModel()
{
    Q_Q(MDialogView);
    MButtonModel *buttonModel;
    MDialogButtonsList buttonModelsList = q->model()->buttons();

    const int buttonModelsSize = buttonModelsList.size();
    for (int i = 0; i < buttonModelsSize; ++i) {
        buttonModel = buttonModelsList[i];

        if (!isButtonInButtonBox(buttonModel)) {
            addButton(buttonModel);
        }
    }
}

bool MDialogViewPrivate::isButtonInButtonBox(MButtonModel *buttonModel)
{
    int i = 0;
    bool foundButton = false;
    const int buttonBoxSize = buttonBoxLayout->count();

    while (i < buttonBoxSize && !foundButton) {
        foundButton = static_cast<MButton *>(buttonBoxLayout->itemAt(i))->model() == buttonModel;
        if (!foundButton) {
            ++i;
        }
    }

    return foundButton;
}

void MDialogViewPrivate::addButton(MButtonModel *buttonModel)
{
    M::ButtonRole currentButtonRole;
    MButton *newButton = new MButton(0, buttonModel);
    newButton->setObjectName(buttonModel->objectName());
    newButton->setStyleName("CommonQueryButton");
    MButton *currButton = 0;
    bool buttonAdded = false;
    int i = 0;
    M::ButtonRole buttonRole = buttonModel->role();

    /* The order of buttons should be arranged so, that from left-to-right,
       top-down, the positive/confirming button should be the first one, and
       the negative/canceling action should be the last.

       OBS: buttons should be also mirrored in right-to-left-layouts, e.g. Arabic.
    */
    do {
        if (i < buttonBoxLayout->count()) {
            currButton = static_cast<MButton *>(buttonBoxLayout->itemAt(i));

            currentButtonRole = currButton->model()->role();

            if (buttonOrder(buttonRole) < buttonOrder(currentButtonRole)) {
                buttonBoxLayoutPolicy->insertItem(i, newButton);
                buttonAdded = true;
            }

            ++i;
        } else {
            // just add it to the end of the layout.
            buttonBoxLayoutPolicy->addItem(newButton);
            buttonAdded = true;
        }
    } while (!buttonAdded);
    for (i = 0; i < buttonBoxLayout->count(); i++) {
        buttonBoxLayoutPolicy->itemAt(i)->setPreferredWidth(
                buttonBox->preferredWidth() / buttonBoxLayout->count());
    }
}

int MDialogViewPrivate::buttonOrder(M::ButtonRole role)
{
    int order;

    switch (role) {
        // Positive actions
    case M::AcceptRole:
    case M::ActionRole:
    case M::HelpRole:
    case M::YesRole:
    case M::ApplyRole:
        order = 1;
        break;

        // Negative actions
    case M::RejectRole:
    case M::DestructiveRole:
    case M::NoRole:
    case M::ResetRole:
        order = 2;
        break;

    default:
        order = 3;
        break;
    };

    return order;
}

void MDialogViewPrivate::setCentralWidget(QGraphicsWidget *newCentralWidget)
{
    if (centralWidget)
        contentsLayout->removeItem(centralWidget);

    // Place the new one
    if (newCentralWidget) {
        newCentralWidget->setObjectName("MDialogContents");
        contentsLayout->insertItem(0, newCentralWidget);
        centralWidget = newCentralWidget;
    }
}

void MDialogViewPrivate::setSpinnerVisibility(bool visibility)
{
    MLayout *layout = static_cast<MLayout *>(titleBar->layout());
    if (!spinner) {
        if (!visibility) return;
        spinner = new MProgressIndicator(titleBar, MProgressIndicator::spinnerType);
        spinner->setUnknownDuration(true);
        spinner->setStyleName("MDialogProgressIndicator");
        spinner->setObjectName(spinner->styleName());
    }

    if (!policySpinnerTitle) {
        policySpinnerTitle = new MLinearLayoutPolicy(layout, Qt::Horizontal);
        policySpinnerTitle->addItem(spinner);
        policySpinnerTitle->addItem(titleLabel);
    }

    if (!policySpinnerTitleClose) {
        policySpinnerTitleClose = new MLinearLayoutPolicy(layout, Qt::Horizontal);
        policySpinnerTitleClose->addItem(spinner);
        policySpinnerTitleClose->addItem(titleLabel);
        policySpinnerTitleClose->addItem(closeButton);
    }
    updateTitleBarLayoutPolicy();
}

void MDialogViewPrivate::updateTitleBarIconVisibility()
{
    Q_Q(MDialogView);
    MLayout *layout = static_cast<MLayout *>(titleBar->layout());

    if(!titleBarIconImage)
        titleBarIconImage = new MImageWidget(titleBar);
    titleBarIconImage->setStyleName("CommonTitleIcon");
    titleBarIconImage->setImage(q->model()->titleBarIconId());

    if (!policyIconTitle) {
        policyIconTitle = new MLinearLayoutPolicy(layout, Qt::Horizontal);
        policyIconTitle->addItem(titleBarIconImage);
        policyIconTitle->addItem(titleLabel);
    }

    if (!policyIconTitleClose) {
        policyIconTitleClose = new MLinearLayoutPolicy(layout, Qt::Horizontal);
        policyIconTitleClose->addItem(titleBarIconImage);
        policyIconTitleClose->addItem(titleLabel);
        policyIconTitleClose->addItem(closeButton);
    }

    updateTitleBarLayoutPolicy();
}

void MDialogViewPrivate::updateTitleBarLayoutPolicy()
{
    Q_Q(MDialogView);
    MLayout *layout = static_cast<MLayout *>(titleBar->layout());
    if (q->model()->progressIndicatorVisible()) {
        if (hasCloseButton()) {
            layout->setPolicy(policySpinnerTitleClose);
        } else {
            layout->setPolicy(policySpinnerTitle);
        }

    } else if (!q->model()->titleBarIconId().isEmpty()) {
        if(hasCloseButton()) {
            layout->setPolicy(policyIconTitleClose);
        } else {
            layout->setPolicy(policyIconTitle);
        }
    } else {
        if (hasCloseButton()) {
            layout->setPolicy(policyTitleClose);
        } else {
            layout->setPolicy(policyTitle);
        }
    }
}


void MDialogView::setupModel()
{
    Q_D(MDialogView);

    MDialogPrivate *dialogPrivate = d->controller->d_func();

    MSceneWindowView::setupModel();

    if (dialogPrivate->dumbMode) {
        return;
    }

    d->titleLabel->setText(model()->title());
    d->setSpinnerVisibility(model()->progressIndicatorVisible());

    d->updateWidgetVisibility(d->buttonBox,
                              model()->buttonBoxVisible(),
                              2,
                              d->dialogBoxLayout);

    d->repopulateButtonBox();

    d->setCentralWidget(model()->centralWidget());
    d->updateButtonBoxLayoutOrientation();
    d->contentsViewport->setVerticalPanningPolicy(model()->contentsVerticalPanningPolicy());
    d->updateTitleBarIconVisibility();
}

void MDialogView::updateData(const QList<const char *> &modifications)
{
    Q_D(MDialogView);

    MSceneWindowView::updateData(modifications);

    MDialogPrivate *dialogPrivate = d->controller->d_func();
    const char *member;

    if (dialogPrivate->dumbMode) {
        return;
    }

    foreach(member, modifications) {
        if (member == MDialogModel::ProgressIndicatorVisible) {
            d->setSpinnerVisibility(model()->progressIndicatorVisible());
        } else if (member == MDialogModel::Title) {
            d->titleLabel->setText(model()->title());
        } else if (member == MDialogModel::ButtonBoxVisible) {
            d->updateWidgetVisibility(d->buttonBox,
                                      model()->buttonBoxVisible(),
                                      2,
                                      d->dialogBoxLayout);
        } else if (member == MDialogModel::CentralWidget) {
            d->setCentralWidget(model()->centralWidget());
        } else if (member == MDialogModel::Buttons) {
            d->updateButtonBox();
        } else if (member == MDialogModel::ContentsVerticalPanningPolicy) {
            d->contentsViewport->setVerticalPanningPolicy(model()->contentsVerticalPanningPolicy());
        } else if (member == MDialogModel::TitleBarIconId) {
            d->updateTitleBarIconVisibility();
        }
    }
}

QPainterPath MDialogView::shape() const
{
    Q_D(const MDialogView);
    QPainterPath path;

    path = d->dialogBox->shape();
    path.translate(d->dialogBox->geometry().x(),
                   d->dialogBox->geometry().y());

    return path;
}

QGraphicsLinearLayout *MDialogView::contentsLayout()
{
    Q_D(MDialogView);
    return d->contentsLayout;
}

MPannableViewport *MDialogView::contentsViewport()
{
    Q_D(MDialogView);
    return d->contentsViewport;
}

bool MDialogViewPrivate::hasCloseButton() {
    Q_Q(MDialogView);
    if (q->model()->system() && q->model()->modal())
        return false;
    else
        return q->style()->hasCloseButton();
}

#include "moc_mdialogview.cpp"

M_REGISTER_VIEW_NEW(MDialogView, MDialog)
