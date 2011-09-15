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

#include "textentrypanningpage.h"

#include <MButton>
#include <MBasicListItem>
#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MPannableViewport>
#include <MPositionIndicator>
#include <MSceneManager>
#include <MTextEdit>

#include <QDebug>
#include <QPanGesture>

TextEntryPanningPage::TextEntryPanningPage()
    : TemplatePage(TemplatePage::UserInput),
      infoLabel(0),
      loseFocusOnPanButton(0),
      nestedViewport(0)
{}

TextEntryPanningPage::~TextEntryPanningPage()
{
}

QString TextEntryPanningPage::timedemoTitle()
{
    return "TextEntryPanning";
}

void TextEntryPanningPage::createContent()
{
    setStyleName(inv("CommonApplicationPage"));
    MLayout *layout = new MLayout();
    MLinearLayoutPolicy *layoutPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    layoutPolicy->setContentsMargins(0, 0, 0, 0);
    layoutPolicy->setSpacing(0);
    layout->setPolicy(layoutPolicy);

    infoLabel = new MLabel;
    infoLabel->setObjectName("InfoLabel");
    infoLabel->setStyleName(inv("CommonBodyText"));
    infoLabel->setWordWrap(true);
    infoLabel->setAlignment(Qt::AlignTop);

    layoutPolicy->addItem(infoLabel);

    {   // Text edit and button in horizontal layout
        MLayout *topLayout = new MLayout();
        MLinearLayoutPolicy *topHorizontalPolicy = new MLinearLayoutPolicy(topLayout, Qt::Horizontal);
        addTextEdit(topHorizontalPolicy, M::NumberContentType);

        loseFocusOnPanButton = new MButton;
        loseFocusOnPanButton->setStyleName(inv("CommonSingleButton"));
        loseFocusOnPanButton->setCheckable(true);
        topHorizontalPolicy->addItem(loseFocusOnPanButton);
        layoutPolicy->addItem(topLayout);
    }

    addTextEdit(layoutPolicy, M::FreeTextContentType);
    addContentItems(layoutPolicy, 2);

    // Add nested viewport.
    nestedViewport = addNestedViewport(layoutPolicy);

    addContentItems(layoutPolicy, 1);
    addTextEdit(layoutPolicy, M::FreeTextContentType);

    pannableViewport()->setObjectName("TopPannable");
    pannableViewport()->setHorizontalPanningPolicy(MPannableWidget::PanningAlwaysOff);
    pannableViewport()->setVerticalPanningPolicy(MPannableWidget::PanningAsNeeded);
    centralWidget()->setLayout(layout);

    MApplicationPage::createContent();
    pannableViewport()->setAcceptGesturesFromAnyDirection(true);

    retranslateUi();

    pannableViewport()->installSceneEventFilter(this);
    nestedViewport->installSceneEventFilter(this);
}

void TextEntryPanningPage::retranslateUi()
{
    setTitle(qtTrId("xx_textpan_page_title"));
    if (!isContentCreated())
        return;

    //% "This page contains text fields inside pannable viewport. "
    //% "You can toggle whether panning removes focus or not.\n\n"
    //% "Text fields can also be inside nested viewports and "
    //% "the framework should make sure focused one is always visible."
    infoLabel->setText(qtTrId("xx_textpan_page_info_label").replace('\n', "<br>"));

    //% "Lose focus on pan"
    loseFocusOnPanButton->setText(qtTrId("xx_textpan_page_lose_focus_button"));

    // TODO: Add translations if needed.
    foreach (QPointer<MTextEdit> edit, textEdits) {
        if (!edit) {
            continue;
        }
        switch (edit->contentType()) {
        case M::NumberContentType:
            edit->setText("1234567890");
            break;

        case M::PhoneNumberContentType:
            edit->setText("+358 555 555");
            break;

        case M::FreeTextContentType:
        default:
            if (edit->lineMode() == MTextEditModel::SingleLine) {
                edit->setText("free text");
            } else {
                edit->setText("Vivamus justo turpis, pharetra vitae porta quis,\n"
                              "mattis et eros. Sed magna magna, volutpat ut adipiscing at,\n"
                              "adipiscing vitae urna. Nunc mauris ligula,\n"
                              "accumsan eget varius varius, pellentesque nec magna.");
            }
            break;
        }
    }

    int runningItemIndex = 1;
    foreach (QPointer<MBasicListItem> item, contentItems) {
        if (!item) {
            continue;
        }
        item->setTitle("Item");
        item->setSubtitle(QString("Dummy content item #%1").arg(runningItemIndex));
        ++runningItemIndex;
    }
}

bool TextEntryPanningPage::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    if (event->type() == QEvent::Gesture
        && loseFocusOnPanButton
        && loseFocusOnPanButton->isChecked()) {

        QGestureEvent *gestureEvent = static_cast<QGestureEvent *>(event);
        QPanGesture *panGesture = static_cast<QPanGesture *>(gestureEvent->gesture(Qt::PanGesture));

        if (panGesture
            && panGesture->state() == Qt::GestureStarted
            && (watched == pannableViewport()
                || watched == nestedViewport)
            && watched->focusItem()) {

            // Pan gesture coordinates are in screen coordinates. Need to switch components.
            QPointF offset;
            if (sceneManager()->orientation() == M::Landscape) {
                offset = panGesture->offset();
            } else {
                offset = QPointF(panGesture->offset().y(),
                                 panGesture->offset().x());
            }

            Qt::Orientation direction = qAbs<qreal>(offset.y()) > qAbs<qreal>(offset.x())
                                        ? Qt::Vertical : Qt::Horizontal;

            MPannableViewport *viewport = static_cast<MPannableViewport *>(watched);
            if (viewport->panDirection() & direction) {
                viewport->focusItem()->clearFocus();
            }
        }
    }
    return TemplatePage::sceneEventFilter(watched, event);
}

void TextEntryPanningPage::addTextEdit(MLinearLayoutPolicy *layoutPolicy,
                                       M::TextContentType contentType,
                                       MTextEditModel::LineMode lineMode)
{
    MTextEdit *edit = new MTextEdit(lineMode);
    edit->setContentType(contentType);

    switch (contentType) {
    case M::NumberContentType:
        edit->setObjectName("number field");
        break;

    case M::PhoneNumberContentType:
        edit->setObjectName("phone number field");
        break;

    case M::FreeTextContentType:
    default:
        if (lineMode == MTextEditModel::SingleLine) {
            edit->setObjectName("free text field");
        } else {
            edit->setObjectName("multi line free text field");
        }
        break;
    }

    edit->setStyleName(inv("CommonSingleInputField"));

    textEdits << edit;
    layoutPolicy->addItem(edit);
}

void TextEntryPanningPage::addContentItems(MLinearLayoutPolicy *layoutPolicy, int count)
{
    for (int i = 0; i < count; i++) {

        MBasicListItem* contentItem = new MBasicListItem(MBasicListItem::TitleWithSubtitle);
        contentItem->setObjectName("PannableItem");
        contentItem->setStyleName(inv("CommonBasicListItem"));

        contentItems << contentItem;
        layoutPolicy->addItem(contentItem);
    }
}

MPannableViewport *TextEntryPanningPage::addNestedViewport(MLinearLayoutPolicy *parentLayoutPolicy)
{
    MLayout* layout = new MLayout();
    MLinearLayoutPolicy* linearLayoutPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    linearLayoutPolicy->setContentsMargins(0, 0, 0, 0);
    linearLayoutPolicy->setSpacing(0);
    layout->setPolicy(linearLayoutPolicy);

    addTextEdit(linearLayoutPolicy, M::FreeTextContentType);
    addTextEdit(linearLayoutPolicy, M::PhoneNumberContentType);
    addContentItems(linearLayoutPolicy, 1);
    addTextEdit(linearLayoutPolicy, M::FreeTextContentType, MTextEditModel::MultiLine);
    addContentItems(linearLayoutPolicy, 2);
    addTextEdit(linearLayoutPolicy, M::FreeTextContentType);

    MPannableViewport* viewport = new MPannableViewport();
    viewport->setObjectName("NestedPannable");
    viewport->positionIndicator()->setStyleName(inv("CommonPositionIndicator"));
    viewport->setAutoRange(true);
    viewport->setWidget(new MWidget());
    viewport->widget()->setLayout(layout);
    viewport->setPreferredHeight(200);
    viewport->setHorizontalPanningPolicy(MPannableWidget::PanningAlwaysOff);
    viewport->setVerticalPanningPolicy(MPannableWidget::PanningAsNeeded);

    parentLayoutPolicy->addItem(viewport);

    return viewport;
}
