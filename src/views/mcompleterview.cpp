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

#include "mcompleter.h"
#include "mcompleterview.h"
#include "mcompleterview_p.h"

#include "mtextedit.h"
#include "mtextedit_p.h"
#include "moverlay.h"
#include "mlabel.h"
#include "mbutton.h"
#include "mpopuplist.h"
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mscenemanager.h"
#include "mprogressindicator.h"

#include <MCancelEvent>
#include <MInputMethodState>
#include <MLocale>

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLinearLayout>
#include <QTextDocumentFragment>

namespace
{
    //! default height for completer
    const int DefaultCompleterHeight = 60;
    //!default object names
    const QString CompleterCandidatesLabelObjectName("MCompleterCandidatesLabel");
    const QString CompleterTotalButtonObjectName("MCompleterTotalButton");
    //! default maximum hits is 10
    const int DefaultMaximumHits = 10;
}

MCompleterViewPrivate::MCompleterViewPrivate(MCompleter *controller, MCompleterView *q)
    : controller(controller),
      q_ptr(q),
      completionLabel(0),
      completionsButton(0),
      layout(0),
      popup(0),
      popupSelectedIndex(-1)
{
    completionLabel = new MLabel(controller);
    completionLabel->setObjectName(CompleterCandidatesLabelObjectName);
    completionLabel->setTextElide(true);
    completionLabel->setWordWrap(false);
    completionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    completionsButton = new CompletionCountButton(controller);
    completionsButton->setObjectName(CompleterTotalButtonObjectName);
    completionsButton->hide();

    layout = new QGraphicsLinearLayout(Qt::Horizontal, controller);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addItem(completionLabel);
    layout->setAlignment(completionLabel, Qt::AlignCenter);

    controller->setLayout(layout);

    //click focus, and eat focusout for the textedit widget
    controller->setFocusPolicy(Qt::ClickFocus);
    controller->setManagedManually(true);
}

MCompleterViewPrivate::~MCompleterViewPrivate()
{
    delete completionLabel;
    completionLabel = 0;
    delete completionsButton;
    completionsButton = 0;

    if (popup) {
        // The deletion of popup might cause the emission of its disappeared() signal.
        // That, on its turn, would cause one of our slots to be called.
        // To avoid that we brake the connections between this instance and popup
        // right away instead of waiting for ~QObject() to do that, which will be
        // too late in this case.
        disconnect(popup, 0, this, 0);

        delete popup;
        popup  = 0;
    }
}


void MCompleterViewPrivate::init()
{
    connect(completionsButton, SIGNAL(clicked()), this, SLOT(showPopup()));
    connect(controller, SIGNAL(shown()), this, SLOT(createContents()));
    if (controller->widget()) {
        connect(controller->widget()->sceneManager(),
                SIGNAL(orientationChangeFinished(const M::Orientation &)),
                this, SLOT(updatePosition()));
        connect(controller, SIGNAL(shown()),
                this,       SLOT(handleCompleterShown()));
        connect(controller, SIGNAL(hidden()),
                this,       SLOT(handleCompleterHidden()));
    }
}

void MCompleterViewPrivate::updatePosition()
{
    Q_Q(MCompleterView);

    if (!controller->widget() || !controller->isActive())
        return;

    const QRect cursorRect = cursorRectangle();

    QPoint completerPos;

    const int completerHeight = static_cast<int>(controller->preferredHeight());

    // Calculate new visible scene height, with input method height subtracted.
    const int visibleSceneBottom = controller->sceneManager()->visibleSceneSize().height()
                                   - static_cast<int>(mapSceneRectToRoot(MInputMethodState::instance()->inputMethodArea()).height());

    // Visible scene top could take into account status bar and others, when present, but because it is difficult to reliably get
    // the correct coordinate we set it to zero. This is good enough because the top coordinate is only used when setting
    // completer's position _above_ text widget and in practice this only occurs in landscape when status bar is hidden.
    const int visibleSceneTop = 0;

    const int cursorBottom = cursorRect.y() + cursorRect.height();
    const int roomBelow = qMax<int>(0, visibleSceneBottom - cursorBottom);
    const int roomAbove = qMax<int>(0, cursorRect.top() - visibleSceneTop);

    if (roomBelow >= completerHeight) {
        // Completer fits below text widget. Prefer this.

        // Position to widget bottom if possible, otherwise use cursor bottom.

        const QRect widgetRect = widgetRectangle();
        const int yPositionOffset = qMin<int>(q->style()->yPositionOffset()
                                              + ((widgetRect.y() + widgetRect.height()) - cursorBottom),
                                              roomBelow - completerHeight);
        completerPos.setY(cursorRect.y() + cursorRect.height() + yPositionOffset);
    } else if (roomAbove >= completerHeight) {
        // Completer fits above text widget

        // Position above cursor. Could also use widget top but because we don't know
        // the real status bar & navigation bar height it's best to just use cursor.
        // Also completer is closer to keyboard and user's fingers this way.

        const int yPositionOffset = qMin<int>(q->style()->yPositionOffset(),
                                              roomAbove - completerHeight);
        completerPos.setY(cursorRect.top() - completerHeight - yPositionOffset);
    } else {
        // No space to show completer fully. Pick the side which has most space.
        if (roomBelow >= roomAbove) {
            // Prefer below.
            completerPos.setY(cursorRect.y() + cursorRect.height());
        } else {
            completerPos.setY(cursorRect.top() - completerHeight);
        }
    }

    if (completerPos != controller->pos())
        controller->setPos(completerPos);

    // Consider removing updateGeometry call.
    // This is not required to be called for what happens in this method because
    // sizeHint is not changed. It might be needed though because this is called
    // on orientationChangeFinished and sizeHint currently depends on orientation.
    q->updateGeometry();
}

QRect MCompleterViewPrivate::cursorRectangle() const
{
    const MWidget *widget = controller->widget();

    QRect result;

    if (widget->scene()->focusItem() == widget
        && widget->flags() & QGraphicsItem::ItemAcceptsInputMethod) {
        // Input method query on scene will be forwarded to correct item.
        QVariant queryResult = widget->scene()->inputMethodQuery(Qt::ImMicroFocus);

        if (queryResult.isValid()) {
            result = mapSceneRectToRoot(queryResult.toRect()).toRect();
        }
    }

    if (result.isNull()) {
        // Fall back to widget rectangle
        result = widgetRectangle();
    }

    return result;
}

QRect MCompleterViewPrivate::widgetRectangle() const
{
    const MWidget *widget = controller->widget();
    return mapRectToRoot(widget, widget->rect()).toRect();
}

QRectF MCompleterViewPrivate::mapRectToRoot(const QGraphicsItem *item,
                                            const QRectF &itemRect) const
{
    // Calculate item rectangle in our parent coordinates by first bringing it
    // to local coordinates and then translating.
    return controller->mapRectFromItem(item, itemRect).translated(controller->pos());
}

QRectF MCompleterViewPrivate::mapSceneRectToRoot(const QRectF &sceneRect) const
{
    return controller->mapRectFromScene(sceneRect).translated(controller->pos()).toRect();
}

void MCompleterViewPrivate::createContents()
{
    Q_Q(MCompleterView);

    if (controller->widget() && q->model()->matchedModel()) {
        // Add controller to scene when first time to show, it is neccessary for later setFocusProxy.
        // MSceneWindow's should not be added to scene this way normally.
        if (!controller->widget()->scene()->items().contains(controller))
            controller->widget()->scene()->addItem(controller);

        updateCompletionLabelText();
        updateCompletionsButton();

        completionLabel->setFocusProxy(controller->widget());
        controller->setFocusProxy(controller->widget());

        //set the focus back to textwidget
        //even we already set focus proxy for controller, this step is still necessary,
        //otherwise the textwidget can not get focus.
        controller->scene()->setFocusItem(controller->widget());
        updatePosition();
    }
}

void MCompleterViewPrivate::clear()
{
    //clear focus proxy
    completionLabel->setFocusProxy(0);
    completionsButton->setFocusProxy(0);
    controller->setFocusProxy(0);
}


void MCompleterViewPrivate::showPopup()
{
    Q_Q(MCompleterView);
    if (!q->model()->matchedModel() || q->model()->matchedModel()->rowCount() <= 0)
        return;

    if (!popup) {
        popup = new MPopupList();
        popup->setItemModel(q->model()->matchedModel());
        popup->setTitle(q->model()->completionTitle());
        connect(popup, SIGNAL(appearing()), this, SLOT(handlePopupAppearing()));
        connect(popup, SIGNAL(disappearing()), this, SLOT(handlePopupDisappearing()));
        connect(popup, SIGNAL(disappeared()), this, SLOT(handlePopupDisappeared()));
    }

    if (popup->currentIndex().row() < 0)
        popup->setCurrentIndex(popup->itemModel()->index(0, 0));

    //if the label of the button is "10+", should query all before showing popup
    if (completionsButton->text() == QString("<qt>%1+").arg(MLocale().formatNumber(DefaultMaximumHits)))
        controller->queryAll();
    q->model()->setPopupActive(true);

    popup->setProgressIndicatorVisible(q->model()->fetchInProgress());
    controller->sceneManager()->appearSceneWindow(popup);
}

void MCompleterViewPrivate::handlePopupAppearing()
{
    //hide completion widget before showing popup
    controller->hideCompleter();
    if (controller->widget()) {
        controller->widget()->clearFocus();

        // Refocusing popup is a workaround for an issue where text edit
        // receives focus automatically when app is minimized and restored
        // during the time popup dialog is appeared.
        connect(controller->widget(), SIGNAL(gainedFocus(Qt::FocusReason)),
                this, SLOT(refocusPopup()), Qt::UniqueConnection);
    }
}

void MCompleterViewPrivate::handlePopupDisappearing()
{
    // Signal disappearing() (or accept()) must be used to confirm selection.
    // On disappeared() is too late since matchedModel might've been updated
    // during disappearance animation and index returned by popup could be incorrect.

    // Calling confirm() here however would lead to a regression when recipient editor opens
    // a contact detail picker while this, higher z-level popup, is still visible.
    // Normally the new dialog would remove focus from text edit (by QEvent::WindowBlocked)
    // but because the popup is still visible the focus remains and shows input method on
    // top of detail picker dialog. See bug NB#203502.

    popupSelectedIndex = popup->result() == MDialog::Accepted
                         ? popup->currentIndex().row() : -1;
}

void MCompleterViewPrivate::handlePopupDisappeared()
{
    Q_Q(MCompleterView);

    if (controller->widget()) {
        disconnect(controller->widget(), SIGNAL(gainedFocus(Qt::FocusReason)),
                   this, SLOT(refocusPopup()));
        if (popup->result() == MDialog::Accepted) {

            // If model changed after selection, popup's currentIndex seem to be -1.
            if (popupSelectedIndex != popup->currentIndex().row()) {
                qWarning("MCompleterView: Model updated during dialog's disappearance animation. "
                         "Selected item may be wrong!");
                // The value of popupSelectedIndex is not validated here before
                // setting it as matchedIndex, it is done in confirm().
            }
            controller->scene()->setFocusItem(controller->widget());
            q->model()->setMatchedIndex(popupSelectedIndex);

            controller->confirm();
        } else {
            controller->scene()->setFocusItem(controller->widget());
        }
    }
    q->model()->setPopupActive(false);
}

void MCompleterViewPrivate::refocusPopup()
{
    Q_Q(MCompleterView);
    // if text widget gains focus again when popup list is still
    // visibile, should transfer the focus to popup list.
    if (popup && q->model()->popupActive()) {
        controller->widget()->clearFocus();
        controller->scene()->setFocusItem(popup);
        controller->setActive(true);
    }
}

void MCompleterViewPrivate::handleCompleterShown()
{
    // connect to MTextEdit private signal scenePositionChanged() to enable
    // the completer to follow the text widget.
    MTextEdit *textWidget = qobject_cast<MTextEdit *>(controller->widget());
    if (!textWidget)
        return;

    MTextEditPrivate *textWidgetPrivate = static_cast<MTextEditPrivate *>(textWidget->d_func());
    connect(&textWidgetPrivate->signalEmitter,
            SIGNAL(scenePositionChanged()),
            this,
            SLOT(updatePosition()),
            Qt::UniqueConnection);

    // Since completer's position is affected by input method area, follow
    // its changes.
    connect(MInputMethodState::instance(),
            SIGNAL(inputMethodAreaChanged(QRect)),
            this,
            SLOT(updatePosition()), Qt::UniqueConnection);
}

void MCompleterViewPrivate::handleCompleterHidden()
{
    Q_Q(MCompleterView);
    q->style().setModeDefault();
    q->applyStyle();
    // connect to MTextEdit private signal scenePositionChanged() to enable
    // the completer to follow the text widget.
    MTextEdit *textWidget = qobject_cast<MTextEdit *>(controller->widget());
    if (!textWidget)
        return;

    MTextEditPrivate *textWidgetPrivate = static_cast<MTextEditPrivate *>(textWidget->d_func());
    disconnect(&textWidgetPrivate->signalEmitter,
               SIGNAL(scenePositionChanged()),
               this,
               0);

    disconnect(MInputMethodState::instance(),
               SIGNAL(inputMethodAreaChanged(QRect)),
               this, 0);
}

void MCompleterViewPrivate::showCompletionsButton()
{
    if (completionsButton->isVisible()) {
        return;
    }

    completionsButton->setFocusProxy(controller->widget());
    // Prevent text eliding by using Fixed width.
    completionsButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    layout->addItem(completionsButton);
    layout->setAlignment(completionsButton, Qt::AlignCenter);
    completionsButton->show();
}

void MCompleterViewPrivate::hideCompletionsButton()
{
    if (!completionsButton->isVisible()) {
        return;
    }

    // When hiding, the button is removed from layout in order to give
    // more space for completionLabel item.

    // First clear focus proxy so hiding won't cause focus out there.
    completionsButton->setFocusProxy(0);

    completionsButton->hide();
    completionsButton->setText("");
    layout->removeItem(completionsButton);
}

void MCompleterViewPrivate::updateCompletionLabelText()
{
    Q_Q(MCompleterView);

    QString text;
    if (q->model()->matchedModel()
        && q->model()->matchedIndex() >= 0
        && q->model()->matchedModel()->rowCount() > q->model()->matchedIndex()) {
        QVariant var = q->model()->matchedModel()->data(
                           q->model()->matchedModel()->index(q->model()->matchedIndex(), 0));
        if (var.isValid())
            text = var.toString();
    }

    if (!text.isEmpty()) {
        //highlight the completionPrefix
        QString prefix = q->model()->completionPrefix().trimmed();
        text = text.replace('<', "&lt;");
        text = text.replace('>', "&gt;");
        prefix = prefix.replace('<', "&lt;");
        prefix = prefix.replace('>', "&gt;");
        int index = -1;
        if (!text.isEmpty() && !prefix.isEmpty()) {
            index = text.indexOf(prefix, 0, Qt::CaseInsensitive);
        }

        // only highlight if there is a match in text.
        if (index != -1) {
            const QString highlightColor = q->style()->highlightColor().name();
            const QString replacedString = text.mid(index, prefix.length());
            const QString colorfulText = QString("<font color=%1>" + replacedString + "</font>").arg(highlightColor);
            text = text.replace(index, replacedString.length(),
                                q->style()->highlightDecoration().arg(colorfulText));
        }
    } else if (q->model()->fetchInProgress()) {
        text = q->model()->fetchInProgressLabel();
    }

    completionLabel->setText(text);
}

void MCompleterViewPrivate::updateCompletionsButton()
{
    Q_Q(MCompleterView);

    //set button's visibility and label
    int total = q->model()->matchedModel()->rowCount();
    bool showButton = false;

    // There must be more than one match to show completions button with total count.
    // If there are matches there must also be a valid matchedIndex. If not, it means we have no matches and matchedModel is outdated.
    if (total > 1 && q->model()->matchedIndex() >= 0) {
        // Workaround for NB#177781: MButton has different alignments for rich text and normal text in its label.
        // Both completionLabel and completionsButton use rich text label to get same alignment
        if (total <= DefaultMaximumHits)
            completionsButton->setText(QString("<qt>%1").arg(MLocale().formatNumber(total)));
        else
            completionsButton->setText(QString("<qt>%1+").arg(MLocale().formatNumber(DefaultMaximumHits)));
        completionsButton->setAllowMouseInteraction(true);
        showButton = true;
    } else {
        // Only one or zero matches -> clear button contents and disallow user input.
        // Without spinner inside the button we would just hide here.
        completionsButton->setAllowMouseInteraction(false);
        completionsButton->setText(QString());
    }

    if (q->model()->fetchInProgress()) {
        showButton = true;
        completionsButton->setProgressIndicatorVisible(true);
    } else {
        completionsButton->setProgressIndicatorVisible(false);
    }

    if (showButton) {
        showCompletionsButton();

        // For some reason the widget does not activate properly. Do it now.
        // The use case for this is when button label is changing between "١٠+" and "٢", for example.
        layout->activate();
    } else {
        hideCompletionsButton();
    }
}

MCompleterView::MCompleterView(MCompleter *controller)
    : MSceneWindowView(controller),
      d_ptr(new MCompleterViewPrivate(controller, this))
{
    Q_D(MCompleterView);
    d->init();
}


MCompleterView::~MCompleterView()
{
    Q_D(MCompleterView);
    delete d;
}

QSizeF MCompleterView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);

    return QSize(MApplication::activeWindow()->visibleSceneSize().width(),
                 DefaultCompleterHeight);
}

void MCompleterView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    style()->pressFeedback().play();
    style().setModePressed();
    applyStyle();
    event->accept();
}

void MCompleterView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MCompleterView);
    style()->releaseFeedback().play();
    style().setModeDefault();
    applyStyle();
    if (d->controller && d->controller->isVisible())
        d->controller->confirm();
    event->accept();
}

void MCompleterView::cancelEvent(MCancelEvent *event)
{
    style().setModeDefault();
    applyStyle();
    event->accept();
}

void MCompleterView::setupModel()
{
    Q_D(MCompleterView);

    MSceneWindowView::setupModel();

    // Set initial state for spinner.
    d->updateCompletionsButton();
}

void MCompleterView::updateData(const QList<const char *>& modifications)
{
    Q_D(MCompleterView);

    if (modifications.contains(MCompleterModel::Active)) {
        if (!model()->active()) {
            // clear focus proxy when control become inactive (before really hidden
            // to avoid focus out)
            d->clear();
        }
    } else if (modifications.contains(MCompleterModel::CompletionTitle) && d->popup) {
        d->popup->setTitle(model()->completionTitle());
    } else if (modifications.contains(MCompleterModel::FetchInProgress)) {
        d->updateCompletionsButton();
        // Switch between FetchInProgressLabel and actual completion.
        d->updateCompletionLabelText();

        // If popup is active, update its spinner visibility.
        if (model()->popupActive()) {
            d->popup->setProgressIndicatorVisible(model()->fetchInProgress());
        }
    } else if (modifications.contains(MCompleterModel::FetchInProgressLabel)) {
        d->updateCompletionLabelText();
    } else if (modifications.contains(MCompleterModel::MatchedIndex)) {
        d->updateCompletionLabelText();

        // Whenever MatchedIndex changes between -1 and >= 0 we need to update button.
        d->updateCompletionsButton();
    }

    MSceneWindowView::updateData(modifications);
}

CompletionCountButton::CompletionCountButton(QGraphicsItem *parent)
    : MButton(parent),
      spinner(this, MProgressIndicator::spinnerType),
      allowMouseInteraction(true)
{
    // Spinner position is adjusted by margins to match
    // visual button position.
    spinner.setPos(QPointF());
    spinner.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    spinner.setUnknownDuration(true);
    spinner.setStyleName("CompletionSpinner");
    setProgressIndicatorVisible(false);
}

CompletionCountButton::~CompletionCountButton()
{
}

void CompletionCountButton::setProgressIndicatorVisible(bool visible)
{
    if (spinner.isVisible() != visible) {
        spinner.setVisible(visible);
        update(); // button background drawing depends of this
    }
}

bool CompletionCountButton::progressIndicatorVisible() const
{
    return spinner.isVisible();
}

void CompletionCountButton::setAllowMouseInteraction(bool allow)
{
    allowMouseInteraction = allow;
}

void CompletionCountButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (allowMouseInteraction) {
        MButton::mousePressEvent(event);
    }
    // Otherwise do nothing and stop mouse event from propagating.
}

void CompletionCountButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Draw button background only when spinner is not shown or when button is down.
    // The exception with isDown is to give user feedback.
    if (!spinner.isVisible() || isDown()) {
        MButton::paint(painter, option, widget);
    }
}

M_REGISTER_VIEW_NEW(MCompleterView, MCompleter)
