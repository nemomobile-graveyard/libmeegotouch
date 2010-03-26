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

#include "duidialogview.h"
#include "duidialogview_p.h"

#include "duidialog.h"
#include "duidialog_p.h"
#include "duiviewcreator.h"
#include "duipannableviewport.h"
#include "duiscenemanager.h"
#include <DuiLabel>
#include <DuiButton>
#include <DuiProgressIndicator>
#include <DuiStylableWidget>
#include <DuiLayout>
#include "duibuttongrouplayoutpolicy_p.h"

#include <QGraphicsLinearLayout>

DuiDialogViewPrivate::DuiDialogViewPrivate()
    : rootLayout(0),
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

DuiDialogViewPrivate::~DuiDialogViewPrivate()
{
}

void DuiDialogViewPrivate::createWidgetHierarchy()
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
    class DuiTransparentWidget : public QGraphicsWidget {
	QGraphicsWidget *box;
        public:
	DuiTransparentWidget(QGraphicsWidget *widget){
	    box = widget;
	}
	QPainterPath shape() const {
	    QPainterPath path;
	    path = box->shape();
	    path.translate(box->pos());
	    return path;
	}
    };
    horizontalWidget = new DuiTransparentWidget(dialogBox);
    horizontalWidget->setLayout(horizontalLayout);
    rootLayout->addItem(horizontalWidget);
    //rootLayout->addItem(horizontalLayout);
}

QGraphicsWidget *DuiDialogViewPrivate::createSpacer()
{
    QGraphicsWidget *spacer = new QGraphicsWidget();

    spacer->hide();
    spacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    spacer->setMinimumSize(0, 0);
    spacer->setPreferredSize(0, 0);

    return spacer;
}

QGraphicsLinearLayout *DuiDialogViewPrivate::createLayout(Qt::Orientation orientation)
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(orientation);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    return layout;
}

void DuiDialogViewPrivate::createHorizontalLayout()
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

void DuiDialogViewPrivate::createDialogBox()
{
    dialogBox = new QGraphicsWidget;
    dialogBox->setObjectName("DuiDialogBox");
    dialogBoxLayout = createLayout(Qt::Vertical);
    dialogBox->setLayout(dialogBoxLayout);

    createTitleBar();
    dialogBoxLayout->addItem(titleBar);

    contentsViewport = new DuiPannableViewport;
    contentsViewport->setObjectName("DuiDialogContentsViewport");
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

void DuiDialogViewPrivate::createButtonBox()
{
    buttonBox = new QGraphicsWidget;
    buttonBoxLayout = new DuiLayout();
    buttonBoxLayoutPolicy = new DuiButtonGroupLayoutPolicy(buttonBoxLayout, Qt::Horizontal);
    buttonBoxLayout->setPolicy(buttonBoxLayoutPolicy);
    buttonBoxLayout->setContentsMargins(0, 0, 0, 0);
    buttonBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    buttonBox->setLayout(buttonBoxLayout);
}

void DuiDialogViewPrivate::createTitleBar()
{
    QGraphicsLinearLayout *layout = createLayout(Qt::Horizontal);
    titleBar = new DuiStylableWidget;
    titleBar->setObjectName("DuiDialogTitleBar");
    titleBar->setLayout(layout);
    titleBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    spinner = new DuiProgressIndicator(titleBar, DuiProgressIndicator::spinnerType);
    spinner->setUnknownDuration(true);
    spinner->setObjectName("DuiDialogProgressIndicator");
    layout->addItem(spinner);
    spinner->setVisible(false); // hidden by default

    titleLabel = new DuiLabel(titleBar);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setTextElide(true);
    titleLabel->setObjectName("DuiDialogTitleLabel");
    layout->addItem(titleLabel);
    layout->setAlignment(titleLabel, Qt::AlignCenter);

    closeButton = new DuiButton(titleBar);
    closeButton->setObjectName("DuiDialogCloseButton");
    closeButton->setViewType("icon");
    closeButton->setIconID("Icon-close");
    layout->addItem(closeButton);
    layout->setAlignment(closeButton, Qt::AlignVCenter | Qt::AlignHCenter);
    QObject::connect(closeButton, SIGNAL(clicked()), controller, SLOT(reject()));
}

DuiDialogView::DuiDialogView(DuiDialog *controller) :
    DuiSceneWindowView(*new DuiDialogViewPrivate, controller)
{
    Q_D(DuiDialogView);
    d->controller = controller;
    d->createWidgetHierarchy();
}

DuiDialogView::DuiDialogView(DuiDialogViewPrivate &dd, DuiDialog *controller)
    : DuiSceneWindowView(dd, controller)
{
    Q_D(DuiDialogView);
    d->controller = controller;
    d->createWidgetHierarchy();
}


DuiDialogView::~DuiDialogView()
{
    Q_D(DuiDialogView);

    // Make sure the model's centralWidget doesn't get deleted along with us.
    d->setCentralWidget(0);
}

void DuiDialogView::applyStyle()
{
    Q_D(DuiDialogView);
    DuiDialogPrivate *dialogPrivate = d->controller->d_func();

    if (dialogPrivate->dumbMode) {
        return;
    }

    DuiSceneWindowView::applyStyle();

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

void DuiDialogViewPrivate::setupDialogVerticalAlignment()
{
    Q_Q(DuiDialogView);

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

void DuiDialogViewPrivate::updateWidgetVisibility(QGraphicsWidget *widget, bool visible, int index,
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
            contentsViewport->setViewType(DuiPannableWidget::defaultType);
        } else {
            contentsViewport->setViewType("titlebarless");
        }
    }
}

void DuiDialogViewPrivate::repopulateButtonBox()
{
    Q_Q(DuiDialogView);
    DuiButton *button;

    while (buttonBoxLayout->count() > 0) {
        button = static_cast<DuiButton *>(buttonBoxLayout->itemAt(0));
        buttonBoxLayout->removeAt(0);
        delete button;
    }

    DuiDialogButtonsList buttonModelsList = q->model()->buttons();

    const int buttonModelsSize = buttonModelsList.size();
    for (int i = 0; i < buttonModelsSize; ++i) {
        addButton(q->model()->button(i));
    }
}

void DuiDialogViewPrivate::updateButtonBox()
{
    removeButtonsNotInDialogModel();
    addButtonsFromDialogModel();
}

void DuiDialogViewPrivate::removeButtonsNotInDialogModel()
{
    Q_Q(DuiDialogView);
    DuiButton *button;
    DuiDialogButtonsList buttonModelsList = q->model()->buttons();
    int i = 0;

    while (i < buttonBoxLayout->count()) {
        button = static_cast<DuiButton *>(buttonBoxLayout->itemAt(i));
        if (!buttonModelsList.contains(button->model())) {
            buttonBoxLayout->removeAt(i);
            delete button;
        } else {
            ++i;
        }
    }
}

void DuiDialogViewPrivate::addButtonsFromDialogModel()
{
    Q_Q(DuiDialogView);
    DuiButtonModel *buttonModel;
    DuiDialogButtonsList buttonModelsList = q->model()->buttons();

    const int buttonModelsSize = buttonModelsList.size();
    for (int i = 0; i < buttonModelsSize; ++i) {
        buttonModel = buttonModelsList[i];

        if (!isButtonInButtonBox(buttonModel)) {
            addButton(buttonModel);
        }
    }
}

bool DuiDialogViewPrivate::isButtonInButtonBox(DuiButtonModel *buttonModel)
{
    int i = 0;
    bool foundButton = false;
    const int buttonBoxSize = buttonBoxLayout->count();

    while (i < buttonBoxSize && !foundButton) {
        foundButton = static_cast<DuiButton *>(buttonBoxLayout->itemAt(i))->model() == buttonModel;
        if (!foundButton) {
            ++i;
        }
    }

    return foundButton;
}

void DuiDialogViewPrivate::addButton(DuiButtonModel *buttonModel)
{
    Q_Q(DuiDialogView);
    Dui::StandardButton currButtonType;
    DuiButton *newButton = new DuiButton(0, buttonModel);
    DuiButton *currButton = 0;
    bool buttonAdded = false;
    int i = 0;
    Dui::StandardButton buttonType = q->model()->standardButton(buttonModel);

    /* The order of buttons should be arranged so, that from left-to-right,
       top-down, the positive/confirming button should be the first one, and
       the negative/canceling action should be the last.

       OBS: buttons should be also mirrored in right-to-left-layouts, e.g. Arabic.
    */
    do {
        if (i < buttonBoxLayout->count()) {
            currButton = static_cast<DuiButton *>(buttonBoxLayout->itemAt(i));

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

int DuiDialogViewPrivate::stdButtonOrder(Dui::StandardButton buttonType)
{
    int order;

    switch (buttonType) {
        // Positive actions
    case Dui::OkButton:
    case Dui::SaveButton:
    case Dui::SaveAllButton:
    case Dui::OpenButton:
    case Dui::YesButton:
    case Dui::YesToAllButton:
    case Dui::RetryButton:
    case Dui::ApplyButton:
    case Dui::DoneButton:
        order = 1;
        break;

        // Negative actions
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
        order = 2;
        break;

    default:
        order = 3;
        break;
    };

    return order;
}

void DuiDialogViewPrivate::setCentralWidget(QGraphicsWidget *newCentralWidget)
{
    if (centralWidget)
        contentsLayout->removeItem(centralWidget);

    // Place the new one
    if (newCentralWidget) {
        contentsLayout->insertItem(0, newCentralWidget);
        centralWidget = newCentralWidget;
    }
}

void DuiDialogView::setupModel()
{
    Q_D(DuiDialogView);
    DuiDialogPrivate *dialogPrivate = d->controller->d_func();

    DuiSceneWindowView::setupModel();

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

void DuiDialogView::updateData(const QList<const char *> &modifications)
{
    Q_D(DuiDialogView);

    DuiSceneWindowView::updateData(modifications);

    DuiDialogPrivate *dialogPrivate = d->controller->d_func();
    const char *member;

    if (dialogPrivate->dumbMode) {
        return;
    }

    foreach(member, modifications) {
        if (member == DuiDialogModel::CloseButtonVisible) {
            d->closeButton->setVisible(model()->closeButtonVisible());
        } else if (member == DuiDialogModel::ProgressIndicatorVisible) {
            d->spinner->setVisible(model()->progressIndicatorVisible());
        } else if (member == DuiDialogModel::Title) {
            d->titleLabel->setText(model()->title());
        } else if (member == DuiDialogModel::ButtonBoxVisible) {
            d->updateWidgetVisibility(d->buttonBox,
                                      model()->buttonBoxVisible(),
                                      d->contentsLayout->count(),
                                      d->contentsLayout);
        } else if (member == DuiDialogModel::TitleBarVisible) {
            d->updateWidgetVisibility(d->titleBar,
                                      model()->titleBarVisible(),
                                      0,
                                      d->dialogBoxLayout);

        } else if (member == DuiDialogModel::CentralWidget) {
            d->setCentralWidget(model()->centralWidget());
        } else if (member == DuiDialogModel::Buttons) {
            d->updateButtonBox();
        }
    }
}

QPainterPath DuiDialogView::shape() const
{
    Q_D(const DuiDialogView);
    QPainterPath path;

    path = d->dialogBox->shape();

    path.translate(d->horizontalWidget->pos());

    path.translate(d->dialogBox->geometry().x(),
                   d->dialogBox->geometry().y());

    return path;
}

QGraphicsLinearLayout *DuiDialogView::contentsLayout()
{
    Q_D(DuiDialogView);
    return d->contentsLayout;
}

DuiPannableViewport *DuiDialogView::contentsViewport()
{
    Q_D(DuiDialogView);
    return d->contentsViewport;
}

DUI_REGISTER_VIEW_NEW(DuiDialogView, DuiDialog)
