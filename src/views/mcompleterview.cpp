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
      overLaypreferredSize(QSizeF(0, 0))
{
    completionLabel = new MLabel(controller);
    completionLabel->setObjectName(CompleterCandidatesLabelObjectName);
    completionLabel->setTextElide(true);
    completionLabel->setWordWrap(false);
    completionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    completionsButton = new MButton(controller);
    completionsButton->setObjectName(CompleterTotalButtonObjectName);

    layout = new QGraphicsLinearLayout(Qt::Horizontal, controller);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addItem(completionLabel);
    layout->setAlignment(completionLabel, Qt::AlignCenter);
    layout->addItem(completionsButton);
    layout->setAlignment(completionsButton, Qt::AlignCenter);

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

    delete popup;
    popup  = 0;
}


void MCompleterViewPrivate::init()
{
    connect(completionsButton, SIGNAL(clicked()), this, SLOT(showPopup()));
    connect(controller, SIGNAL(shown()), this, SLOT(createContents()));
    if (controller->widget()) {
        connect(controller->widget()->sceneManager(),
                SIGNAL(orientationChangeFinished(const M::Orientation &)),
                this, SLOT(organizeContents()));
        connect(controller, SIGNAL(shown()),
                this,       SLOT(handleCompleterShown()));
        connect(controller, SIGNAL(hidden()),
                this,       SLOT(handleCompleterHidden()));
    }
}

void MCompleterViewPrivate::organizeContents()
{
    Q_Q(MCompleterView);

    if (!controller->widget() || !controller->isActive())
        return;
    QSize screenSize = MApplication::activeWindow()->visibleSceneSize();
    int textWidgetWidth = 0;

    //mcompleter has by default the same width than the according Text-field that it is attached to.
    //default position is aligning left bottom with text widget
    const QRect textWidgetRect = controller->widget()->mapToScene(
                                     controller->widget()->boundingRect()).boundingRect().toRect();
    textWidgetWidth = textWidgetRect.width();

    int width = 0;
    int height = DefaultCompleterHeight;

    if (q->style()->height() > 0)
        height = q->style()->height();

    const int buttonWidth = q->style()->buttonWidth();
    const int displayBorder = q->style()->displayBorder();

    width = completionLabel->preferredWidth();
    bool buttonVisible = (!completionsButton->text().isEmpty()) && completionsButton->isVisible();
    if (buttonVisible) {
        const int buttonMargin = q->style()->buttonMargin();
        width += buttonWidth + buttonMargin;
    }
    // Input suggestions that do not fit into the available space force the completer to grow.
    // First the completer grows to the right until it reaches the displayBorder.
    // If then continues growing to the left until it reaches the left displayBorder.
    // This means that the maximum size of the completer is the width of the display minus 2*displayBorder
    if (width < textWidgetWidth)
        width = textWidgetWidth;
    if (width > (screenSize.width() - 2 * displayBorder)) {
        width = screenSize.width() - 2 * displayBorder;
    }
    overLaypreferredSize = QSizeF(width, height);

    QPoint completerPos = locatePosition(textWidgetRect);
    if ((completerPos.x() + width) > (screenSize.width() - displayBorder)) {
        completerPos.setX(screenSize.width() - displayBorder - width);
    }
    //add y position offset
    const int  yPositionOffset = q->style()->yPositionOffset();
    if (yPositionOffset > 0) {
        completerPos.setY(completerPos.y() - yPositionOffset);
    }

    //adjust position, to avoid the completer to be beyond the screen edge
    //completer's position is fixed, aligning left bottom with text widget
    //judge completion should stay at text widget below or upper
    if (height > (screenSize.height() - completerPos.y())) {
        completerPos.setY(completerPos.y() - controller->widget()->boundingRect().height()
                          - height + yPositionOffset);
    }
    if (completerPos != controller->pos())
        controller->setPos(completerPos);
    q->updateGeometry();
}

QPoint MCompleterViewPrivate::locatePosition(const QRect &r) const
{
    QSize screenSize = MApplication::activeWindow()->visibleSceneSize();
    QPoint p;
    switch (MApplication::activeWindow()->orientationAngle()) {
    case M:: Angle90:
        p = r.topLeft();
        p = QPoint(p.y(), (screenSize.height() - p.x()));
        break;
    case M:: Angle180:
        p = r.topRight();
        p = QPoint((screenSize.width() - p.x()), (screenSize.height() - p.y()));
        break;
    case M:: Angle270:
        p = r.bottomRight();
        p = QPoint((screenSize.width() - p.y()), p.x());
        break;
    case M:: Angle0:
    default:
        p = r.bottomLeft();
        break;
    }
    return p;
}

void MCompleterViewPrivate::createContents()
{
    Q_Q(MCompleterView);

    if (controller->widget() && q->model()->matchedModel()) {
        //add controller to scence when first time to show, it is neccessary for later setFocusProxy
        if (!controller->widget()->scene()->items().contains(controller))
            controller->widget()->scene()->addItem(controller);

        QString text;
        if (q->model()->matchedModel()->rowCount() > q->model()->matchedIndex()) {
            QVariant var = q->model()->matchedModel()->data(
                               q->model()->matchedModel()->index(q->model()->matchedIndex(), 0));
            if (var.isValid())
                text += var.toString();
        }
        if (text.isEmpty())
            return;

        //highlight the completionPrefix
        QString prefix = q->model()->completionPrefix();
        text = text.replace('<', "&lt;");
        text = text.replace('>', "&gt;");
        prefix = prefix.replace('<', "&lt;");
        prefix = prefix.replace('>', "&gt;");
        QRegExp exp(QString("^%1").arg(prefix), Qt::CaseInsensitive);
        int index = exp.indexIn(text);
        if (index == -1) {
            exp.setPattern(QString("\\W%1").arg(prefix));
            index = exp.indexIn(text);
            if (index != -1)
                ++index;
        }

        // only highlight if there is a match in text.
        if (index != -1) {
            QString highlightColor = q->style()->highlightColor().name();
            QString replacedString = text.mid(index, prefix.length());
            text = text.replace(index, replacedString.length(),
                                QString("<font color=%1>" + replacedString + "</font>").arg(highlightColor));
        }
        completionLabel->setText(text);

        //set button's visibility and label
        int total = q->model()->matchedModel()->rowCount();
        if (total > 1) {
            // Workaround for NB#177781: MButton has different alignments for rich text and normal text in its label.
            // Both completionLabel and completionsButton use rich text label to get same alignment
            if (total <= DefaultMaximumHits)
                completionsButton->setText(QString("<b></b>%1").arg(total));
            else
                completionsButton->setText(QString("<b></b>%1+").arg(DefaultMaximumHits));
            completionsButton->setFocusProxy(controller->widget());
            completionsButton->setVisible(true);
            layout->addItem(completionsButton);
            layout->setAlignment(completionsButton, Qt::AlignCenter);
        } else {
            completionsButton->setText("");
            completionsButton->setFocusProxy(0);
            completionsButton->setVisible(false);
            layout->removeItem(completionsButton);
        }

        completionLabel->setFocusProxy(controller->widget());
        controller->setFocusProxy(controller->widget());

        //set the focus back to textwidget
        //even we already set focus proxy for controller, this step is still necessary,
        //otherwise the textwidget can not get focus.
        controller->scene()->setFocusItem(controller->widget());
        organizeContents();
    }
}

void MCompleterViewPrivate::clear()
{
    overLaypreferredSize = QSize(0, 0);
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
        connect(popup, SIGNAL(disappeared()), this, SLOT(handlePopupDisappeared()));
    }

    if (popup->currentIndex().row() < 0)
        popup->setCurrentIndex(popup->itemModel()->index(0, 0));

    //if the label of the button is "10+", should query all before showing popup
    if (completionsButton->text() == QString("%1+").arg(DefaultMaximumHits))
        controller->queryAll();
    controller->sceneManager()->appearSceneWindow(popup);
}

void MCompleterViewPrivate::handlePopupAppearing()
{
    Q_Q(MCompleterView);
    q->model()->setPopupActive(true);
    //hide completion widget before showing popup
    controller->hideCompleter();
    controller->widget()->clearFocus();
    connect(controller->widget(), SIGNAL(gainedFocus(Qt::FocusReason)),
            this, SLOT(refocusPopup()), Qt::UniqueConnection);
}

void MCompleterViewPrivate::handlePopupDisappeared()
{
    Q_Q(MCompleterView);
    disconnect(controller->widget(), SIGNAL(gainedFocus(Qt::FocusReason)),
               this, SLOT(refocusPopup()));
    if (popup->result() == MDialog::Accepted) {
        //only confirm when accept
        controller->scene()->setFocusItem(controller->widget());
        q->model()->setMatchedIndex(popup->currentIndex().row());
        controller->confirm();
    } else {
        controller->scene()->setFocusItem(controller->widget());
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
            SLOT(organizeContents()),
            Qt::UniqueConnection);
}

void MCompleterViewPrivate::handleCompleterHidden()
{
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
    Q_D(const MCompleterView);

    // return the actual height of mcompleter to avoid twice relocation.
    if (!d->overLaypreferredSize.isEmpty())
        return d->overLaypreferredSize;
    else {
        int height = (style()->height() > 0) ? style()->height() : DefaultCompleterHeight;
        return QSize(MApplication::activeWindow()->visibleSceneSize().width(), height);
    }
}

void MCompleterView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    style()->pressFeedback().play();
    style().setModePressed();
    applyStyle();
    update();
    event->accept();
}

void MCompleterView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MCompleterView);
    style()->releaseFeedback().play();
    style().setModeDefault();
    applyStyle();
    update();
    if (d->controller && d->controller->isVisible())
        d->controller->confirm();
    event->accept();
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
    }

    MSceneWindowView::updateData(modifications);
}

M_REGISTER_VIEW_NEW(MCompleterView, MCompleter)
