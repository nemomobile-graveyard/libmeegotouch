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

#include "mcomboboxview.h"
#include "mcomboboxview_p.h"
#include "mcombobox.h"

#include "mtheme.h"
#include "mimagewidget.h"
#include "mlabel.h"
#include "mscalableimage.h"
#include "mviewcreator.h"
#include "mpopuplist.h"
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mscenemanager.h"
#include "mlocale.h"
#include "mprogressindicator.h"
#include "mlayout.h"
#include "mgridlayoutpolicy.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>


MComboBoxButton::MComboBoxButton(QGraphicsItem *parent)
    : MListItem(parent),
      layoutGrid(0),
      _iconSubtitlePolicy(0),
      _basicSubtitlePolicy(0),
      _iconProgressPolicy(0),
      _basicProgressPolicy(0),
      _iconWidget(0),
      _titleWidget(0),
      _subtitleWidget(0),
      _indicatorWidget(0),
      _progressWidget(0),
      progressVisible(false),
      iconVisible(false)
{
    initLayout();
}

void MComboBoxButton::initLayout()
{
    layoutGrid = new MLayout(this);
    layoutGrid->setContentsMargins(0, 0, 0, 0);
    setLayout(layoutGrid);
    updatePolicy();
}

void MComboBoxButton::updatePolicy()
{
    if (!iconVisible && !progressVisible)
        layoutGrid->setPolicy(basicSubtitlePolicy());
    else if (iconVisible && !progressVisible)
        layoutGrid->setPolicy(iconSubtitlePolicy());
    if (!iconVisible && progressVisible)
        layoutGrid->setPolicy(basicProgressPolicy());
    else if (iconVisible && progressVisible)
        layoutGrid->setPolicy(iconProgressPolicy());
}

MGridLayoutPolicy* MComboBoxButton::iconSubtitlePolicy() {
    if (!_iconSubtitlePolicy) {
        _iconSubtitlePolicy = new MGridLayoutPolicy(layoutGrid);
        _iconSubtitlePolicy->addItem(iconWidget(), 0, 0, 3, 1);
        _iconSubtitlePolicy->addItem(titleWidget(), 0, 1);
        _iconSubtitlePolicy->addItem(subtitleWidget(), 1, 1);
        _iconSubtitlePolicy->addItem(indicatorWidget(), 0, 2, 3, 1, Qt::AlignVCenter);
        _iconSubtitlePolicy->addItem(new QGraphicsWidget(this), 2, 1);
    }
    return _iconSubtitlePolicy;
}

MGridLayoutPolicy* MComboBoxButton::basicSubtitlePolicy() {
    if (!_basicSubtitlePolicy) {
        _basicSubtitlePolicy = new MGridLayoutPolicy(layoutGrid);
        _basicSubtitlePolicy->addItem(titleWidget(), 0, 0);
        _basicSubtitlePolicy->addItem(subtitleWidget(), 1, 0);
        _basicSubtitlePolicy->addItem(indicatorWidget(), 0, 1, 3, 1, Qt::AlignVCenter);
        _basicSubtitlePolicy->addItem(new QGraphicsWidget(this), 2, 0);
    }
    return _basicSubtitlePolicy;
}

MGridLayoutPolicy* MComboBoxButton::iconProgressPolicy() {
    if (!_iconProgressPolicy) {
        _iconProgressPolicy = new MGridLayoutPolicy(layoutGrid);
        _iconProgressPolicy->addItem(iconWidget(), 0, 0, 3, 1);
        _iconProgressPolicy->addItem(titleWidget(), 0, 1);
        _iconProgressPolicy->addItem(progressWidget(), 1, 1);
        _iconProgressPolicy->addItem(indicatorWidget(), 0, 2, 3, 1, Qt::AlignVCenter);
        _iconProgressPolicy->addItem(new QGraphicsWidget(this), 2, 1);
    }
    return _iconProgressPolicy;
}

MGridLayoutPolicy* MComboBoxButton::basicProgressPolicy() {
    if (!_basicProgressPolicy) {
        _basicProgressPolicy = new MGridLayoutPolicy(layoutGrid);
        _basicProgressPolicy->addItem(titleWidget(), 0, 0);
        _basicProgressPolicy->addItem(progressWidget(), 1, 0);
        _basicProgressPolicy->addItem(indicatorWidget(), 0, 1, 3, 1, Qt::AlignVCenter);
        _basicProgressPolicy->addItem(new QGraphicsWidget(this), 2, 0);
    }
    return _basicProgressPolicy;
}

void MComboBoxButton::setProgressIndicatorVisible(bool visible)
{
    if (visible != progressVisible) {
        progressVisible = visible;
        updatePolicy();
    }
}

void MComboBoxButton::setIconVisible(bool visible)
{
    if (visible != iconVisible) {
        iconVisible = visible;
        updatePolicy();
    }
}

void MComboBoxButton::setIconID(const QString& id)
{
    iconWidget()->setImage(id);
}

void MComboBoxButton::setTitle(const QString &title)
{
    titleWidget()->setText(title);
}

void MComboBoxButton::setSubtitle(const QString &subtitle)
{
    subtitleWidget()->setText(subtitle);
}

MImageWidget * MComboBoxButton::iconWidget()
{
    if (!_iconWidget) {
        _iconWidget = new MImageWidget(this);
        _iconWidget->setObjectName("CommonMainIcon");
    }
    return _iconWidget;
}

MLabel * MComboBoxButton::titleWidget()
{
    if (!_titleWidget) {
        _titleWidget = new MLabel(this);
        _titleWidget->setTextElide(true);
        _titleWidget->setObjectName("CommonTitle");
    }
    return _titleWidget;
}

MLabel * MComboBoxButton::subtitleWidget()
{
    if (!_subtitleWidget) {
        _subtitleWidget = new MLabel(this);
        _subtitleWidget->setTextElide(true);
        _subtitleWidget->setObjectName("CommonSubTitle");
    }
    return _subtitleWidget;
}

MProgressIndicator* MComboBoxButton::progressWidget()
{
    if (!_progressWidget) {
        _progressWidget = new MProgressIndicator(this, MProgressIndicator::barType);
        _progressWidget->setUnknownDuration(true);
        _progressWidget->setObjectName("CommonProgressIndicator");
    }
    return _progressWidget;
}

MImageWidget* MComboBoxButton::indicatorWidget()
{
    if (!_indicatorWidget) {
        _indicatorWidget = new MImageWidget(this);
        _indicatorWidget->setObjectName("ComboBoxIndicator");
    }
    return _indicatorWidget;
}


MComboBoxViewPrivate::MComboBoxViewPrivate()
  : q_ptr(0),
    controller(0),
    button(0),
    popuplist(0),
    isPopupShowing(0)
{
}

MComboBoxViewPrivate::~MComboBoxViewPrivate()
{
    delete popuplist;
    delete button;
}

void MComboBoxViewPrivate::init()
{
    Q_Q(MComboBoxView);

    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    controller->setLayout(layout);

    button = new MComboBoxButton();
    layout->addItem(button);

    QObject::connect(button, SIGNAL(clicked()), controller, SLOT(click()));
    QObject::connect(controller, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_itemModelCurrentIndexChanged(int)));
    QObject::connect(controller, SIGNAL(clicked()), q, SLOT(_q_showPopup()));
    QObject::connect(controller, SIGNAL(dismissed()), q ,SLOT(_q_dismissPopup()));
}

void MComboBoxViewPrivate::updateSubtitle(int currentIndex) {
    if (currentIndex != -1)  {
        button->setSubtitle(controller->itemText(currentIndex));
    } else {
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.7.doc
        //: default value for Popup List button sublabel when nothing has been selected yet
        //% "Tap to Select"
        button->setSubtitle(qtTrId("xx_ComboBoxSubtitle"));
    }
}

void MComboBoxViewPrivate::_q_itemModelCurrentIndexChanged(int currentIndex)
{
    updateSubtitle(currentIndex);
}

void MComboBoxViewPrivate::_q_showPopup()
{
    Q_Q(MComboBoxView);

    if (controller->count() == 0) return;

    if (!popuplist) {
        popuplist = new MPopupList();
        popuplist->setItemModel(controller->itemModel());
        popuplist->setSelectionModel(controller->selectionModel());
        popuplist->setTitle(controller->title());
        QObject::connect(popuplist, SIGNAL(clicked(QModelIndex)), controller, SLOT(_q_itemClicked(QModelIndex)));
        QObject::connect(popuplist, SIGNAL(appeared()), q, SLOT(_q_popuplistAppeared()));
        QObject::connect(popuplist, SIGNAL(disappeared()), q, SLOT(_q_popuplistDisappeared()));

        QObject::connect(popuplist, SIGNAL(rejected()), controller, SIGNAL(dismissed()));
    }

    popuplist->scrollTo(q->model()->selectionModel()->currentIndex());

    if (controller->sceneManager())
        controller->sceneManager()->appearSceneWindow(popuplist);
}

void MComboBoxViewPrivate::_q_dismissPopup()
{
    if (!popuplist)
        return;

    if (!isPopupShowing)
        return;

    QObject::disconnect(popuplist, SIGNAL(rejected()), controller, SIGNAL(dismissed()));
    popuplist->reject();
    QObject::connect(popuplist, SIGNAL(rejected()), controller, SIGNAL(dismissed()));
}

void MComboBoxViewPrivate::_q_popuplistAppeared()
{
    isPopupShowing = true;
}

void MComboBoxViewPrivate::_q_popuplistDisappeared()
{
    isPopupShowing = false;
}

MComboBoxView::MComboBoxView(MComboBox *controller) :
    MWidgetView(controller),
    d_ptr(new MComboBoxViewPrivate)
{
    Q_D(MComboBoxView);
    d->q_ptr = this;
    d->controller = controller;
    d->init();
}

MComboBoxView::MComboBoxView(MComboBoxViewPrivate &dd, MComboBox *controller) :
    MWidgetView(controller),
    d_ptr(&dd)
{
    Q_D(MComboBoxView);
    d->q_ptr = this;
    d->controller = controller;
    d->init();
}

MComboBoxView::~MComboBoxView()
{
    delete d_ptr;
}

void MComboBoxView::applyStyle()
{
    Q_D(MComboBoxView);
    MWidgetView::applyStyle();

    d->button->setObjectName(style()->contentItemObjectName());
    d->button->indicatorWidget()->setImage(style()->indicatorImage());
}

void MComboBoxView::updateData(const QList<const char *>& modifications)
{
    Q_D(MComboBoxView);

    MWidgetView::updateData(modifications);

    foreach(const char* member, modifications) {
        if (member == MComboBoxModel::IconID) {
            d->button->setIconID(model()->iconID());
            d->button->setIconVisible(model()->iconVisible() && !model()->iconID().isEmpty());
        } else if (member == MComboBoxModel::IconVisible) {
            d->button->setIconVisible(model()->iconVisible() && !model()->iconID().isEmpty());
        } else if (member == MComboBoxModel::ProgressIndicatorVisible) {
            d->button->setProgressIndicatorVisible(model()->progressIndicatorVisible());
        } else if (member == MComboBoxModel::Title) {
            d->button->setTitle(model()->title());
            if (d->popuplist)
                d->popuplist->setTitle(model()->title());
        } else if (member == MComboBoxModel::ItemModel) {
            if (d->popuplist)
                d->popuplist->setItemModel(model()->itemModel());
        } else if (member == MComboBoxModel::SelectionModel) {
            if (d->popuplist)
                d->popuplist->setSelectionModel(model()->selectionModel());
        } else if (member == MWidgetModel::LayoutPosition) {
            d->button->setLayoutPosition(model()->layoutPosition());
        }
    }
}

void MComboBoxView::setupModel()
{
    Q_D(MComboBoxView);
    MWidgetView::setupModel();

    d->button->setTitle(model()->title());
    d->button->setIconID(model()->iconID());
    d->button->setIconVisible(model()->iconVisible() && !model()->iconID().isEmpty());
    d->button->setProgressIndicatorVisible(model()->progressIndicatorVisible());
    d->updateSubtitle(d->controller->currentIndex());
}

M_REGISTER_VIEW_NEW(MComboBoxView, MComboBox)

#include "moc_mcomboboxview.cpp"
