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
#include <MLabel>
#include <MButton>
#include <MProgressIndicator>
#include <MStylableWidget>
#include <MLayout>
#include "mbuttongrouplayoutpolicy_p.h"

#include <QGraphicsLinearLayout>

MDialogViewPrivate::MDialogViewPrivate()
    : q_ptr(0),
      controller(0),
      rootLayout(0),
      topSpacer(0),
      bottomSpacer(0),
      leftSpacer(0),
      rightSpacer(0),
      horizontalWidget(0),
      horizontalLayout(0),
      dialogBox(0),
      contentsLayout(0),
      centralWidget(0),
      titleBar(0),
      titleLabel(0),
      spinner(0),
      closeButton(0),
      buttonBox(0),
      buttonBoxLayout(0),
      buttonBoxLayoutPolicy(0)
{
}

MDialogViewPrivate::~MDialogViewPrivate()
{
}

void MDialogViewPrivate::createWidgetHierarchy()
{
    rootLayout = createLayout(Qt::Vertical);
    static_cast<QGraphicsWidget *>(controller)->setLayout(rootLayout);

    topSpacer = createSpacer();
    rootLayout->addItem(topSpacer);

    createHorizontalLayout();
    // If I directly add horizontalLayout to rootLayout (instead
    // of assigning it to a widget and then adding that widget)
    // the sizes go nuts.
    // Yet another bug in QGraphicsLayout...
    // Widget with overloaded shape added to ensure that 
    // horizontalWidget won't intercept mouse events
    class MTransparentWidget : public QGraphicsWidget {
	QGraphicsWidget *box;
        public:
	MTransparentWidget(QGraphicsWidget *widget){
	    box = widget;
	}
	QPainterPath shape() const {
	    QPainterPath path;
	    path = box->shape();
	    path.translate(box->pos());
	    return path;
	}
    };
    horizontalWidget = new MTransparentWidget(dialogBox);
    horizontalWidget->setLayout(horizontalLayout);
    rootLayout->addItem(horizontalWidget);
    //rootLayout->addItem(horizontalLayout);
}

QGraphicsWidget *MDialogViewPrivate::createSpacer()
{
    QGraphicsWidget *spacer = new QGraphicsWidget();

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

void MDialogViewPrivate::createHorizontalLayout()
{
    horizontalLayout = createLayout(Qt::Horizontal);

    leftSpacer = createSpacer();
    leftSpacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    horizontalLayout->addItem(leftSpacer);

    createDialogBox();
    horizontalLayout->addItem(dialogBox);

    rightSpacer = createSpacer();
    rightSpacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    horizontalLayout->addItem(rightSpacer);
}

void MDialogViewPrivate::createDialogBox()
{
    dialogBox = new QGraphicsWidget;
    dialogBox->setObjectName("MDialogBox");
    dialogBoxLayout = createLayout(Qt::Vertical);
    dialogBox->setLayout(dialogBoxLayout);

    createTitleBar();
    dialogBoxLayout->addItem(titleBar);

    contentsViewport = new MPannableViewport;
    contentsViewport->setObjectName("MDialogContentsViewport");
    dialogBoxLayout->addItem(contentsViewport);

    contents = new QGraphicsWidget();
    contents->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    contentsViewport->setWidget(contents);
    contentsLayout = createLayout(Qt::Vertical);
    contentsLayout->setContentsMargins(0, 0, 0, 0);
    contents->setLayout(contentsLayout);

    createButtonBox();
    contentsLayout->addItem(buttonBox);
}

void MDialogViewPrivate::createButtonBox()
{
    buttonBox = new QGraphicsWidget;
    buttonBoxLayout = new MLayout();
    buttonBoxLayoutPolicy = new MButtonGroupLayoutPolicy(buttonBoxLayout, Qt::Horizontal);
    buttonBoxLayout->setPolicy(buttonBoxLayoutPolicy);
    buttonBoxLayout->setContentsMargins(0, 0, 0, 0);
    buttonBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    buttonBox->setLayout(buttonBoxLayout);
}

void MDialogViewPrivate::createTitleBar()
{
    QGraphicsLinearLayout *layout = createLayout(Qt::Horizontal);
    titleBar = new MStylableWidget;
    titleBar->setObjectName("MDialogTitleBar");
    titleBar->setLayout(layout);
    titleBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    spinner = new MProgressIndicator(titleBar, MProgressIndicator::spinnerType);
    spinner->setUnknownDuration(true);
    spinner->setObjectName("MDialogProgressIndicator");
    layout->addItem(spinner);
    spinner->setVisible(false); // hidden by default

    titleLabel = new MLabel(titleBar);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setTextElide(true);
    titleLabel->setObjectName("MDialogTitleLabel");
    layout->addItem(titleLabel);
    layout->setAlignment(titleLabel, Qt::AlignCenter);

    closeButton = new MButton(titleBar);
    closeButton->setObjectName("MDialogCloseButton");
    closeButton->setViewType("icon");
    closeButton->setIconID("icon-m-framework-close");
    layout->addItem(closeButton);
    layout->setAlignment(closeButton, Qt::AlignVCenter | Qt::AlignHCenter);
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

    MSceneWindowView::applyStyle();

    // update title bar height
    d->titleBar->setMinimumHeight(style()->titleBarHeight());
    d->titleBar->setPreferredHeight(style()->titleBarHeight());
    d->titleBar->setMaximumHeight(style()->titleBarHeight());

    // set dimensions separately to that we may have one dimensions set
    // and the other unset (-1).
    d->dialogBox->setMinimumWidth(style()->dialogMinimumSize().width());
    d->dialogBox->setMinimumHeight(style()->dialogMinimumSize().height());

    // set dimensions separately to that we may have one dimensions set
    // and the other unset (-1).
    d->dialogBox->setPreferredWidth(style()->dialogPreferredSize().width());
    d->dialogBox->setPreferredHeight(style()->dialogPreferredSize().height());

    d->buttonBox->setContentsMargins(
        0, style()->verticalSpacing(), 0, 0);

    d->buttonBoxLayoutPolicy->setOrientation(style()->buttonBoxOrientation());
    d->buttonBoxLayoutPolicy->setSpacing(style()->buttonSpacing());

    d->rootLayout->setContentsMargins(
        style()->dialogLeftMargin(), /* left */
        style()->dialogTopMargin(), /* top */
        style()->dialogRightMargin(), /* right */
        style()->dialogBottomMargin() /* bottom */);

    d->setupDialogVerticalAlignment();
}

void MDialogViewPrivate::setupDialogVerticalAlignment()
{
    Q_Q(MDialogView);

    if (q->style()->dialogVerticalAlignment() & Qt::AlignVCenter
            && bottomSpacer == 0) {

        // switch to center alignment

        bottomSpacer = createSpacer();
        rootLayout->addItem(bottomSpacer);

    } else if (!(q->style()->dialogVerticalAlignment() & Qt::AlignVCenter)
               && bottomSpacer != 0) {

        // switch to bottom alignment.

        rootLayout->removeItem(bottomSpacer);
        delete bottomSpacer;
        bottomSpacer = 0;
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

    if (widget == titleBar) {
        if (visible) {
            contentsViewport->setViewType(MPannableWidget::defaultType);
        } else {
            contentsViewport->setViewType("titlebarless");
        }
    }
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
    Q_Q(MDialogView);
    M::StandardButton currButtonType;
    MButton *newButton = new MButton(0, buttonModel);
    newButton->setObjectName(buttonModel->objectName());
    MButton *currButton = 0;
    bool buttonAdded = false;
    int i = 0;
    M::StandardButton buttonType = q->model()->standardButton(buttonModel);

    /* The order of buttons should be arranged so, that from left-to-right,
       top-down, the positive/confirming button should be the first one, and
       the negative/canceling action should be the last.

       OBS: buttons should be also mirrored in right-to-left-layouts, e.g. Arabic.
    */
    do {
        if (i < buttonBoxLayout->count()) {
            currButton = static_cast<MButton *>(buttonBoxLayout->itemAt(i));

            currButtonType = q->model()->standardButton(currButton->model());

            if (stdButtonOrder(buttonType) < stdButtonOrder(currButtonType)) {
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
}

int MDialogViewPrivate::stdButtonOrder(M::StandardButton buttonType)
{
    int order;

    switch (buttonType) {
        // Positive actions
    case M::OkButton:
    case M::SaveButton:
    case M::SaveAllButton:
    case M::OpenButton:
    case M::YesButton:
    case M::YesToAllButton:
    case M::RetryButton:
    case M::ApplyButton:
    case M::DoneButton:
        order = 1;
        break;

        // Negative actions
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
        contentsLayout->insertItem(0, newCentralWidget);
        centralWidget = newCentralWidget;
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
    d->closeButton->setVisible(model()->closeButtonVisible());
    d->spinner->setVisible(model()->progressIndicatorVisible());

    d->updateWidgetVisibility(d->buttonBox,
                              model()->buttonBoxVisible(),
                              d->contentsLayout->count(),
                              d->contentsLayout);

    d->updateWidgetVisibility(d->titleBar,
                              model()->titleBarVisible(),
                              0,
                              d->dialogBoxLayout);

    d->repopulateButtonBox();

    d->setCentralWidget(model()->centralWidget());
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
        if (member == MDialogModel::CloseButtonVisible) {
            d->closeButton->setVisible(model()->closeButtonVisible());
        } else if (member == MDialogModel::ProgressIndicatorVisible) {
            d->spinner->setVisible(model()->progressIndicatorVisible());
        } else if (member == MDialogModel::Title) {
            d->titleLabel->setText(model()->title());
        } else if (member == MDialogModel::ButtonBoxVisible) {
            d->updateWidgetVisibility(d->buttonBox,
                                      model()->buttonBoxVisible(),
                                      d->contentsLayout->count(),
                                      d->contentsLayout);
        } else if (member == MDialogModel::TitleBarVisible) {
            d->updateWidgetVisibility(d->titleBar,
                                      model()->titleBarVisible(),
                                      0,
                                      d->dialogBoxLayout);

        } else if (member == MDialogModel::CentralWidget) {
            d->setCentralWidget(model()->centralWidget());
        } else if (member == MDialogModel::Buttons) {
            d->updateButtonBox();
        }
    }
}

QPainterPath MDialogView::shape() const
{
    Q_D(const MDialogView);
    QPainterPath path;

    path = d->dialogBox->shape();

    path.translate(d->horizontalWidget->pos());

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

M_REGISTER_VIEW_NEW(MDialogView, MDialog)
