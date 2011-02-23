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

#include "textentryattributeextensionpage.h"

#include <QList>
#include <QPointer>
#include <MTextEdit>
#include <MLabel>
#include <MButton>
#include <MLayout>
#include <MInputMethodState>
#include <MLinearLayoutPolicy>
#include <QDebug>

TextEntryAttributeExtensionPage::TextEntryAttributeExtensionPage()
    : TemplatePage(TemplatePage::UserInput),
      noteLabel(0),
      pushButton1(0),
      pushButton2(0),
      pushButton3(0),
      pushButton4(0),
      pushButton5(0),
      pushButton6(0),
      pushButton7(0),
      pushButton8(0),
      registeredAttributeExtensionId1(-1),
      registeredAttributeExtensionId2(-1)
{}

TextEntryAttributeExtensionPage::~TextEntryAttributeExtensionPage()
{}

QString TextEntryAttributeExtensionPage::timedemoTitle()
{
    return "TextEntryAttributeExtension";
}

void TextEntryAttributeExtensionPage::createContent()
{
    TemplatePage::createContent();

    registeredAttributeExtensionId1 = MInputMethodState::instance()->registerAttributeExtension();
    registeredAttributeExtensionId2 = MInputMethodState::instance()->registerAttributeExtension();

    QGraphicsWidget *panel = centralWidget();

    MLayout *layout1 = new MLayout;
    MLinearLayoutPolicy* hPolicy1 = new MLinearLayoutPolicy(layout1, Qt::Vertical);

    edits << new MTextEdit(MTextEditModel::SingleLine, "", panel);
    //% "Example text edit without extended attributes"
    edits.at(0)->setPrompt(qtTrId("xx_tooltip_editwithoutextendedattribute1"));
    hPolicy1->addItem(edits.at(0));

    noteLabel = new MLabel(panel);
    hPolicy1->addItem(noteLabel);

    edits << new MTextEdit(MTextEditModel::MultiLine, "", panel);
    // create buttons
    MLayout *layout2 = new MLayout;
    MLinearLayoutPolicy* hPolicy2 = new MLinearLayoutPolicy(layout2, Qt::Horizontal);
    // The policy notifies the widgets of their relative position inside the layout,
    // this causes the buttons to be rendered with different backgrounds for each position
    hPolicy2->setNotifyWidgetsOfLayoutPositionEnabled(true);

    QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    pushButton1 = new MButton();
    pushButton1->setCheckable(true);
    pushButton1->setViewType(MButton::toggleType);
    pushButton1->setSizePolicy(sizePolicy);
    connect(pushButton1, SIGNAL(toggled(bool)), this, SLOT(setUseLabel(bool)));

    pushButton2 = new MButton();
    pushButton2->setCheckable(true);
    pushButton2->setViewType(MButton::toggleType);
    pushButton2->setSizePolicy(sizePolicy);
    connect(pushButton2, SIGNAL(toggled(bool)), this, SLOT(setUseIcon(bool)));

    pushButton3 = new MButton();
    pushButton3->setCheckable(true);
    pushButton3->setViewType(MButton::toggleType);
    pushButton3->setSizePolicy(sizePolicy);
    connect(pushButton3, SIGNAL(toggled(bool)), this, SLOT(setKeyHighlighted(bool)));

    pushButton4 = new MButton();
    pushButton4->setCheckable(true);
    pushButton4->setViewType(MButton::toggleType);
    pushButton4->setSizePolicy(sizePolicy);
    connect(pushButton4, SIGNAL(toggled(bool)), this, SLOT(setKeyDisabled(bool)));

    hPolicy2->addItem(pushButton1);
    hPolicy2->addItem(pushButton2);
    hPolicy2->addItem(pushButton3);
    hPolicy2->addItem(pushButton4);

    MLayout *layout3 = new MLayout;
    MLinearLayoutPolicy* hPolicy3 = new MLinearLayoutPolicy(layout3, Qt::Horizontal);
    hPolicy3->setContentsMargins(20, 0, 20, 0);

    edits.at(1)->attachToolbar(registeredAttributeExtensionId1);
    //% "Example text edit 1 has extended attributes"
    edits.at(1)->setPrompt(qtTrId("xx_tooltip_customextendedattribute1"));

    hPolicy3->addItem(edits.at(1));
    layout3->setMinimumHeight(100);

    edits << new MTextEdit(MTextEditModel::MultiLine, "", panel);
    // create buttons
    MLayout *layout4 = new MLayout;
    MLinearLayoutPolicy* hPolicy4 = new MLinearLayoutPolicy(layout4, Qt::Horizontal);
    // The policy notifies the widgets of their relative position inside the layout,
    // this causes the buttons to be rendered with different backgrounds for each position
    hPolicy4->setNotifyWidgetsOfLayoutPositionEnabled(true);

    pushButton5 = new MButton();
    pushButton5->setCheckable(true);
    pushButton5->setViewType(MButton::toggleType);
    pushButton5->setSizePolicy(sizePolicy);
    connect(pushButton5, SIGNAL(toggled(bool)), this, SLOT(setUseLabel(bool)));

    pushButton6 = new MButton();
    pushButton6->setCheckable(true);
    pushButton6->setViewType(MButton::toggleType);
    pushButton6->setSizePolicy(sizePolicy);
    connect(pushButton6, SIGNAL(toggled(bool)), this, SLOT(setUseIcon(bool)));

    pushButton7 = new MButton();
    pushButton7->setCheckable(true);
    pushButton7->setViewType(MButton::toggleType);
    pushButton7->setSizePolicy(sizePolicy);
    connect(pushButton7, SIGNAL(toggled(bool)), this, SLOT(setKeyHighlighted(bool)));

    pushButton8 = new MButton();
    pushButton8->setCheckable(true);
    pushButton8->setViewType(MButton::toggleType);
    pushButton8->setSizePolicy(sizePolicy);
    connect(pushButton8, SIGNAL(toggled(bool)), this, SLOT(setKeyDisabled(bool)));

    hPolicy4->addItem(pushButton5);
    hPolicy4->addItem(pushButton6);
    hPolicy4->addItem(pushButton7);
    hPolicy4->addItem(pushButton8);

    MLayout *layout5 = new MLayout;
    MLinearLayoutPolicy* hPolicy5 = new MLinearLayoutPolicy(layout5, Qt::Horizontal);
    hPolicy5->setContentsMargins(20, 0, 20, 0);

    edits.at(2)->attachToolbar(registeredAttributeExtensionId2);
    //% "Example text edit 2 has extended attributes"
    edits.at(2)->setPrompt(qtTrId("xx_tooltip_customextendedattribute2"));

    hPolicy5->addItem(edits.at(2));
    layout5->setMinimumHeight(100);

    containerPolicy->addItem(layout1);
    containerPolicy->addItem(layout2);
    containerPolicy->addItem(layout3);
    containerPolicy->addItem(layout4);
    containerPolicy->addItem(layout5);

    pushButton1->setFocusPolicy(Qt::ClickFocus);
    pushButton1->setFocusProxy(edits.at(1));
    pushButton2->setFocusPolicy(Qt::ClickFocus);
    pushButton2->setFocusProxy(edits.at(1));
    pushButton3->setFocusPolicy(Qt::ClickFocus);
    pushButton3->setFocusProxy(edits.at(1));
    pushButton4->setFocusPolicy(Qt::ClickFocus);
    pushButton4->setFocusProxy(edits.at(1));
    pushButton5->setFocusPolicy(Qt::ClickFocus);
    pushButton5->setFocusProxy(edits.at(2));
    pushButton6->setFocusPolicy(Qt::ClickFocus);
    pushButton6->setFocusProxy(edits.at(2));
    pushButton7->setFocusPolicy(Qt::ClickFocus);
    pushButton7->setFocusProxy(edits.at(2));
    pushButton8->setFocusPolicy(Qt::ClickFocus);
    pushButton8->setFocusProxy(edits.at(2));

    retranslateUi();
}

void TextEntryAttributeExtensionPage::retranslateUi()
{
    //TODO: use proper qtTrId
    noteLabel->setText("Toggle the extended attribute to change key overrides:");
    pushButton1->setText("Label");
    pushButton2->setText("Icon");
    pushButton3->setText("Highlight");
    pushButton4->setText("Disable");
    pushButton5->setText("Label");
    pushButton6->setText("Icon");
    pushButton7->setText("Highlight");
    pushButton8->setText("Disable");
}

void TextEntryAttributeExtensionPage::setUseLabel(bool useLabel)
{
    int id = -1;
    QString label;
    if (QObject::sender() == pushButton1) {
        id = registeredAttributeExtensionId1;
        label = "custom label1";
    } else {
        id = registeredAttributeExtensionId2;
        label = "custom label2";
    }
    if (useLabel)
        MInputMethodState::instance()->setExtendedAttribute(id,
                                                            "/keys",
                                                            "actionKey",
                                                            "label",
                                                            label);
    else
        MInputMethodState::instance()->setExtendedAttribute(id,
                                                            "/keys",
                                                            "actionKey",
                                                            "label",
                                                            "");
}

void TextEntryAttributeExtensionPage::setUseIcon(bool useIcon)
{
    int id = -1;
    if (QObject::sender() == pushButton2)
        id = registeredAttributeExtensionId1;
    else
        id = registeredAttributeExtensionId2;
    if (useIcon)
        MInputMethodState::instance()->setExtendedAttribute(id,
                                                            "/keys",
                                                            "actionKey",
                                                            "icon",
                                                            "/usr/share/widgetsgallery/imtoolbar/icons/icon-m-messaging-smiley-cool.svg");
    else
        MInputMethodState::instance()->setExtendedAttribute(id,
                                                            "/keys",
                                                            "actionKey",
                                                            "icon",
                                                            "");
}

void TextEntryAttributeExtensionPage::setKeyHighlighted(bool highlighted)
{
    int id = -1;
    if (QObject::sender() == pushButton3)
        id = registeredAttributeExtensionId1;
    else
        id = registeredAttributeExtensionId2;
    if (highlighted)
        MInputMethodState::instance()->setExtendedAttribute(id,
                                                            "/keys",
                                                            "actionKey",
                                                            "highlighted",
                                                            QVariant(true));
    else
        MInputMethodState::instance()->setExtendedAttribute(id,
                                                            "/keys",
                                                            "actionKey",
                                                            "highlighted",
                                                            QVariant(false));
}

void TextEntryAttributeExtensionPage::setKeyDisabled(bool disabled)
{
    int id = -1;
    if (QObject::sender() == pushButton4)
        id = registeredAttributeExtensionId1;
    else
        id = registeredAttributeExtensionId2;
    if (!disabled)
        MInputMethodState::instance()->setExtendedAttribute(id,
                                                            "/keys",
                                                            "actionKey",
                                                            "enabled",
                                                            QVariant(true));
    else
        MInputMethodState::instance()->setExtendedAttribute(id,
                                                            "/keys",
                                                            "actionKey",
                                                            "enabled",
                                                            QVariant(false));
}
