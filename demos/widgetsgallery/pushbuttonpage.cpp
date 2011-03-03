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

#include "pushbuttonpage.h"

#include <QGraphicsLinearLayout>

#include <MLabel>
#include <MLayout>
#include <MLocale>
#include <MApplicationPage>
#include <MGridLayoutPolicy>
#include <MLinearLayoutPolicy>
#include <MButton>
#include <MDebug>
#include <MApplication>
#include <MSeparator>

PushButtonPage::PushButtonPage():
        TemplatePage(TemplatePage::Buttons),
        button1(0),button2(0),button3(0),
        button4(0),button5(0),button6(0),
        button7(0),button8(0),button9(0),
        button10(0),button11(0),button12(0),
        button13(0),button14(0),button15(0),
        button16(0),button17(0),button18(0),
        button19(0),button20(0),button21(0),
        button22(0),button23(0),button24(0),
        button25(0),text1(0),text2(0),text3(0),
        text4(0),text5(0),text6(0),text7(0),
        text8(0),text9(0),text10(0),text11(0),
        text12(0)
{
}

PushButtonPage::~PushButtonPage()
{
}

QString PushButtonPage::timedemoTitle()
{
    return "PushButton";
}

void PushButtonPage::createContent()
{
    TemplatePage::createContent();

    QGraphicsWidget *panel = centralWidget();
    panel->setContentsMargins(0,0,0,0);

    setContentsMargins(0,0,0,0);

    layout = new MLayout(panel);

    layoutPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    layoutPolicy->setSpacing(0);
    layoutPolicy->setContentsMargins(0,0,0,0);

    layout->setPortraitPolicy(layoutPolicy);
    layout->setLandscapePolicy(layoutPolicy);

    //Creating layouts & buttons & text

    singleButton();
    twoButtons();
    threeButtons();
    twoButtonsRowLayout();
    threeButtonsRowLayout();
    callRejectButtonLayout();
    callRejectRowLayout();
    splitButtonsLayout();
    buttonTransitionsLayout();

    retranslateUi();
}

void PushButtonPage::twoButtonsRowLayout()
{
    /*
     * two buttons in a row
     */

    button7 = new MButton();
    button7->setStyleName("CommonLeftButton");

    button8 = new MButton();
    button8->setStyleName("CommonRightButton");

    // sublayout
    MLayout* layoutB7B8 = new MLayout;
    MLinearLayoutPolicy* layoutB7B8LandscapePolicy = new MLinearLayoutPolicy(layoutB7B8,Qt::Horizontal);
    layoutB7B8->setLandscapePolicy(layoutB7B8LandscapePolicy);
    layoutB7B8LandscapePolicy->setSpacing(0); // distance between both buttons is exactly 2 margins (right,left), see css
    layoutB7B8LandscapePolicy->addItem(button7);
    layoutB7B8LandscapePolicy->addItem(button8);

    MLinearLayoutPolicy* layoutB7B8PortraitPolicy = new MLinearLayoutPolicy(layoutB7B8,Qt::Horizontal);
    layoutB7B8->setPortraitPolicy(layoutB7B8PortraitPolicy);
    layoutB7B8PortraitPolicy->addItem(button7);
    layoutB7B8PortraitPolicy->addItem(button8);

    layoutPolicy->addItem(layoutB7B8);
    layoutPolicy->setAlignment(layoutB7B8,Qt::AlignCenter);
    layoutPolicy->addItem(new MSeparator(0,Qt::Horizontal));
}

void PushButtonPage::threeButtonsRowLayout()
{
    /*
     * three buttons in a row
     */

    button9 = new MButton();
    button9->setStyleName("CommonLeftButton");

    button10 = new MButton();
    button10->setStyleName("CommonHorizontalCenterButton");

    button11 = new MButton("abc"); // even if text is shorter, buttons have same width
    button11->setStyleName("CommonRightButton");

    // sublayout
    MLayout* layoutB9B10B11 = new MLayout;
    MLinearLayoutPolicy* layoutB9B10B11LandscapePolicy = new MLinearLayoutPolicy(layoutB9B10B11,Qt::Horizontal);
    layoutB9B10B11->setLandscapePolicy(layoutB9B10B11LandscapePolicy);
    layoutB9B10B11LandscapePolicy->setSpacing(0); // distance between both buttons is exactly 2 margins (right,left), see css
    layoutB9B10B11LandscapePolicy->addItem(button9);
    layoutB9B10B11LandscapePolicy->addItem(button10);
    layoutB9B10B11LandscapePolicy->addItem(button11);

    MLinearLayoutPolicy* layoutB9B10B11PortraitPolicy = new MLinearLayoutPolicy(layoutB9B10B11,Qt::Horizontal);
    layoutB9B10B11->setPortraitPolicy(layoutB9B10B11PortraitPolicy);
    layoutB9B10B11PortraitPolicy->addItem(button9);
    layoutB9B10B11PortraitPolicy->addItem(button10);
    layoutB9B10B11PortraitPolicy->addItem(button11);

    layoutPolicy->addItem(layoutB9B10B11);
    layoutPolicy->setAlignment(layoutB9B10B11,Qt::AlignCenter);
    layoutPolicy->addItem(new MSeparator(0,Qt::Horizontal));
}

void PushButtonPage::callRejectButtonLayout()
{
    /*
     * text call & reject button
     */

    text10 = new MLabel();
    text10->setStyleName("CommonTitle");
    text10->setWordWrap(true);
    text10->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(text10);

    layoutPolicy->addItem(new MSeparator(0,Qt::Horizontal));
}

void PushButtonPage::callRejectRowLayout()
{
    /*
     * two buttons in a row, call and reject buttons
     */

    button12 = new MButton();
    button12->setStyleName("CommonLeftCallButton");
    button12->setIconID("icon-m-telephony-call");

    button13 = new MButton();
    button13->setStyleName("CommonRightRejectButton");
    button13->setIconID("icon-m-telephony-call-end");

    // sublayout
    MLayout* layoutB12B13 = new MLayout;
    MLinearLayoutPolicy* layoutB12B13LandscapePolicy = new MLinearLayoutPolicy(layoutB12B13,Qt::Horizontal);
    layoutB12B13->setLandscapePolicy(layoutB12B13LandscapePolicy);
    layoutB12B13LandscapePolicy->setSpacing(0); // distance between both buttons is exactly 2 margins (right,left), see css
    layoutB12B13LandscapePolicy->addItem(button12);
    layoutB12B13LandscapePolicy->addItem(button13);

    MLinearLayoutPolicy* layoutB12B13PortraitPolicy = new MLinearLayoutPolicy(layoutB12B13,Qt::Horizontal);
    layoutB12B13->setPortraitPolicy(layoutB12B13PortraitPolicy);
    layoutB12B13PortraitPolicy->addItem(button12);
    layoutB12B13PortraitPolicy->addItem(button13);

    layoutPolicy->addItem(layoutB12B13);
    layoutPolicy->setAlignment(layoutB12B13,Qt::AlignCenter);
    layoutPolicy->addItem(new MSeparator(0,Qt::Horizontal));
}

void PushButtonPage::splitButtonsLayout()
{
    /*
     * split buttons
     */

    text11 = new MLabel();
    text11->setStyleName("CommonTitle");
    text11->setWordWrap(true);
    text11->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(text11);

    text12 = new MLabel();
    text12->setStyleName("TempWidgetsgalleryBodyText");
    text12->setWordWrap(true);
    text12->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(text12);

    // horiz.

    button14 = new MButton();
    button14->setStyleName("CommonLeftSplitButton");

    button15 = new MButton();
    button15->setStyleName("CommonHorizontalCenterSplitButton");

    button16 = new MButton("abc"); // even if text is shorter, buttons have same width
    button16->setStyleName("CommonRightSplitButton");

    // sublayout
    MLayout* layoutB14B15B16 = new MLayout;
    MLinearLayoutPolicy* layoutB14B15B16LandscapePolicy = new MLinearLayoutPolicy(layoutB14B15B16,Qt::Horizontal);
    layoutB14B15B16->setLandscapePolicy(layoutB14B15B16LandscapePolicy);
    layoutB14B15B16LandscapePolicy->setSpacing(0); // distance between both buttons is exactly 2 margins (right,left), see css
    layoutB14B15B16LandscapePolicy->addItem(button14);
    layoutB14B15B16LandscapePolicy->addItem(button15);
    layoutB14B15B16LandscapePolicy->addItem(button16);

    MLinearLayoutPolicy* layoutB14B15B16PortraitPolicy = new MLinearLayoutPolicy(layoutB14B15B16,Qt::Horizontal);
    layoutB14B15B16->setPortraitPolicy(layoutB14B15B16PortraitPolicy);
    layoutB14B15B16PortraitPolicy->addItem(button14);
    layoutB14B15B16PortraitPolicy->addItem(button15);
    layoutB14B15B16PortraitPolicy->addItem(button16);

    layoutPolicy->addItem(layoutB14B15B16);
    layoutPolicy->setAlignment(layoutB14B15B16,Qt::AlignCenter);
    layoutPolicy->addItem(new MSeparator(0,Qt::Horizontal));

    // vertical
    button17 = new MButton();
    button17->setStyleName("CommonTopSplitButton");

    button18 = new MButton();
    button18->setStyleName("CommonVerticalCenterSplitButton");

    button19 = new MButton("abc"); // even if text is shorter, buttons have same width
    button19->setStyleName("CommonBottomSplitButton");

    // sublayout
    MLayout* layoutB17B18B19 = new MLayout;
    MLinearLayoutPolicy* layoutB17B18B19LandscapePolicy = new MLinearLayoutPolicy(layoutB17B18B19,Qt::Horizontal);
    layoutB17B18B19->setLandscapePolicy(layoutB17B18B19LandscapePolicy);
    layoutB17B18B19LandscapePolicy->setSpacing(0); // distance between both buttons is exactly 2 margins (right,left), see css
    layoutB17B18B19LandscapePolicy->addItem(button17);
    layoutB17B18B19LandscapePolicy->addItem(button18);
    layoutB17B18B19LandscapePolicy->addItem(button19);

    MLinearLayoutPolicy* layoutB17B18B19PortraitPolicy = new MLinearLayoutPolicy(layoutB17B18B19,Qt::Vertical);
    layoutB17B18B19->setPortraitPolicy(layoutB17B18B19PortraitPolicy);
    layoutB17B18B19PortraitPolicy->addItem(button17);
    layoutB17B18B19PortraitPolicy->addItem(button18);
    layoutB17B18B19PortraitPolicy->addItem(button19);

    layoutPolicy->addItem(layoutB17B18B19);
    layoutPolicy->setAlignment(layoutB17B18B19,Qt::AlignCenter);
    layoutPolicy->addItem(new MSeparator(0,Qt::Horizontal));
}

void PushButtonPage::singleButton()
{
    /*
     * single button
     */

    text1 = new MLabel();
    text1->setStyleName("TempWidgetsgalleryBodyText");
    text1->setWordWrap(true);
    text1->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(text1);

    text2 = new MLabel();
    text2->setStyleName("CommonTitle");
    text2->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(text2);

    text3 = new MLabel();
    text3->setStyleName("TempWidgetsgalleryBodyText");
    text3->setWordWrap(true);
    text3->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(text3);


    button1 = new MButton();
    button1->setStyleName("CommonSingleButton");


    layoutPolicy->addItem(new MSeparator(0,Qt::Horizontal));

    layoutPolicy->setAlignment(text1,Qt::AlignLeft);
    layoutPolicy->addItem(button1);
    layoutPolicy->setAlignment(button1,Qt::AlignCenter);

    layoutPolicy->addItem(new MSeparator(0,Qt::Horizontal));
}

void PushButtonPage::twoButtons()
{
    // text

    text4 = new MLabel();
    text4->setStyleName("CommonTitle");
    text4->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(text4);

    text5 = new MLabel();
    text5->setStyleName("TempWidgetsgalleryBodyText");
    text5->setWordWrap(true);
    text5->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(text5);

    text6 = new MLabel();
    text6->setStyleName("CommonTitle");
    text6->setWordWrap(true);
    text6->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(text6);

    text7 = new MLabel();
    text7->setStyleName("TempWidgetsgalleryBodyText");
    text7->setWordWrap(true);
    text7->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(text7);


    layoutPolicy->addItem(new MSeparator(0,Qt::Horizontal));

    /*
    * two buttons
    */

    button2 = new MButton();
    button2->setStyleName("CommonTopButton");

    button3 = new MButton();
    button3->setStyleName("CommonBottomButton");

    // sublayout
    MLayout* layoutB2B3 = new MLayout;
    MLinearLayoutPolicy* layoutB2B3LandscapePolicy = new MLinearLayoutPolicy(layoutB2B3,Qt::Horizontal);
    layoutB2B3->setLandscapePolicy(layoutB2B3LandscapePolicy);
    layoutB2B3LandscapePolicy->setSpacing(0); // distance between both buttons is exactly 2 margins (right,left), see css
    layoutB2B3LandscapePolicy->addItem(button2);
    layoutB2B3LandscapePolicy->addItem(button3);

    MLinearLayoutPolicy* layoutB2B3PortraitPolicy = new MLinearLayoutPolicy(layoutB2B3,Qt::Vertical);
    layoutB2B3->setPortraitPolicy(layoutB2B3PortraitPolicy);
    layoutB2B3PortraitPolicy->addItem(button2);
    layoutB2B3PortraitPolicy->addItem(button3);

    layoutPolicy->addItem(layoutB2B3);
    layoutPolicy->setAlignment(layoutB2B3,Qt::AlignCenter);
    layoutPolicy->addItem(new MSeparator(0,Qt::Horizontal));
}

void PushButtonPage::threeButtons()
{
    /*
    * three buttons
    */

    button4 = new MButton();
    button4->setStyleName("CommonTopButton");

    button5 = new MButton();
    button5->setStyleName("CommonVerticalCenterButton");

    button6 = new MButton("abc"); // even if text is shorter, buttons have same width
    button6->setStyleName("CommonBottomButton");

    // sublayout
    MLayout* layoutB4B5B6 = new MLayout;
    MLinearLayoutPolicy* layoutB4B5B6LandscapePolicy = new MLinearLayoutPolicy(layoutB4B5B6,Qt::Horizontal);
    layoutB4B5B6->setLandscapePolicy(layoutB4B5B6LandscapePolicy);
    layoutB4B5B6LandscapePolicy->setSpacing(0); // distance between both buttons is exactly 2 margins (right,left), see css
    layoutB4B5B6LandscapePolicy->addItem(button4);
    layoutB4B5B6LandscapePolicy->addItem(button5);
    layoutB4B5B6LandscapePolicy->addItem(button6);

    MLinearLayoutPolicy* layoutB4B5B6PortraitPolicy = new MLinearLayoutPolicy(layoutB4B5B6,Qt::Vertical);
    layoutB4B5B6->setPortraitPolicy(layoutB4B5B6PortraitPolicy);
    layoutB4B5B6PortraitPolicy->addItem(button4);
    layoutB4B5B6PortraitPolicy->addItem(button5);
    layoutB4B5B6PortraitPolicy->addItem(button6);

    layoutPolicy->addItem(layoutB4B5B6);
    layoutPolicy->setAlignment(layoutB4B5B6,Qt::AlignCenter);
    layoutPolicy->addItem(new MSeparator(0,Qt::Horizontal));

    /*
    * text
    */

    text8 = new MLabel();
    text8->setStyleName("CommonTitle");
    text8->setWordWrap(true);
    text8->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(text8);

    text9 = new MLabel();
    text9->setStyleName("TempWidgetsgalleryBodyText");
    text9->setWordWrap(true);
    text9->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(text9);

    layoutPolicy->addItem(new MSeparator(0,Qt::Horizontal));
}

void PushButtonPage::buttonTransitionsLayout()
{
    button20 = new MButton();
    button20->setStyleName("ExpandingBackgroundButton");

    button21 = new MButton();
    button21->setStyleName("ExpandingBackgroundButton");

    button22 = new MButton();
    button22->setStyleName("ExpandingBackgroundButton");

    text13 = new MLabel();
    text13->setStyleName("CommonTitle");
    text13->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(text13);

    text14 = new MLabel();
    text14->setStyleName("TempWidgetsgalleryBodyText");
    text14->setWordWrap(true);
    text14->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(text14);

    layoutPolicy->addItem(new MSeparator(0,Qt::Horizontal));

    layoutPolicy->setAlignment(text14,Qt::AlignLeft);

    // sublayout
    MLayout* layoutB20B21B22 = new MLayout;
    MLinearLayoutPolicy* layoutB20B21B22LandscapePolicy = new MLinearLayoutPolicy(layoutB20B21B22,Qt::Horizontal);
    layoutB20B21B22->setLandscapePolicy(layoutB20B21B22LandscapePolicy);
    layoutB20B21B22LandscapePolicy->setSpacing(0); // distance between both buttons is exactly 2 margins (right,left), see css
    layoutB20B21B22LandscapePolicy->addItem(button20);
    layoutB20B21B22LandscapePolicy->addItem(button21);
    layoutB20B21B22LandscapePolicy->addItem(button22);

    MLinearLayoutPolicy* layoutB20B21B22PortraitPolicy = new MLinearLayoutPolicy(layoutB20B21B22,Qt::Horizontal);
    layoutB20B21B22->setPortraitPolicy(layoutB20B21B22PortraitPolicy);
    layoutB20B21B22PortraitPolicy->addItem(button20);
    layoutB20B21B22PortraitPolicy->addItem(button21);
    layoutB20B21B22PortraitPolicy->addItem(button22);

    layoutPolicy->addItem(layoutB20B21B22);
    layoutPolicy->setAlignment(layoutB20B21B22,Qt::AlignCenter);

    layoutPolicy->addItem(new MSeparator(0,Qt::Horizontal));

    button23 = new MButton("A");
    button23->setStyleName("ImplodingBackgroundButton");

    button24 = new MButton("B");
    button24->setStyleName("ImplodingBackgroundButton");

    button25 = new MButton("C");
    button25->setStyleName("ImplodingBackgroundButton");

    MLayout* layoutB23B24B25 = new MLayout;
    MLinearLayoutPolicy* layoutB23B24B25LandscapePolicy = new MLinearLayoutPolicy(layoutB23B24B25,Qt::Horizontal);
    layoutB23B24B25->setLandscapePolicy(layoutB23B24B25LandscapePolicy);
    layoutB23B24B25LandscapePolicy->setSpacing(0); // distance between both buttons is exactly 2 margins (right,left), see css
    layoutB23B24B25LandscapePolicy->addItem(button23);
    layoutB23B24B25LandscapePolicy->addItem(button24);
    layoutB23B24B25LandscapePolicy->addItem(button25);

    MLinearLayoutPolicy* layoutB23B24B25PortraitPolicy = new MLinearLayoutPolicy(layoutB23B24B25,Qt::Horizontal);
    layoutB23B24B25->setPortraitPolicy(layoutB23B24B25PortraitPolicy);
    layoutB23B24B25PortraitPolicy->addItem(button23);
    layoutB23B24B25PortraitPolicy->addItem(button24);
    layoutB23B24B25PortraitPolicy->addItem(button25);

    layoutPolicy->addItem(layoutB23B24B25);
    layoutPolicy->setAlignment(layoutB23B24B25, Qt::AlignCenter);

    layoutPolicy->addItem(new MSeparator(0,Qt::Horizontal));
}

void PushButtonPage::retranslateUi()
{
    //% "Push Button"
    setTitle(qtTrId("xx_push_button_page_title"));
    if (!isContentCreated())
        return;

    //% "Push Buttons can have an icon and a label. "
    //% "Push Button can be used in any container but usually "
    //% "it is used with Dialogs, Notifications and Main windows "
    //% "to invoke actions.\n\n"
    //% "A Push Button generally looks like a traditional button, "
    //% "with button-style edges."

    infoLabel->setText("<a></a>" + qtTrId("xx_push_button_page_info_label").replace('\n', "<br>"));

    /*% "Predefined common styles are available for Push Buttons in cases where they have no custom requirement whatsoever."
    "These buttons can have icons next to their label."
    "<b>Common Styles</b> are applied using the according Object Names."
    */
    text1->setText(qtTrId("xx_push_button_text1"));

    /*% "1. Single button" */
    text2->setText(qtTrId("xx_push_button_text2"));

    /*% "When using a button that literally has no other button next to it (neither in rows or columns) set the Object name to <i>#CommonSingleButton.</i>" */
    text3->setText(qtTrId("xx_push_button_text3"));

    /*% "Multiple buttons" */
    text4->setText(qtTrId("xx_push_button_text4"));

    /*% "When more than one button is needed, the naming of each button is depending on their arrangement in <b>portrait</b> orientation. In landscape buttons are always arranged in rows. " */
    text5->setText(qtTrId("xx_push_button_text5"));

    /*% "2. Multiple buttons that are collected as a column in portrait and rearrange to rows when switching to landscape" */
    text6->setText(qtTrId("xx_push_button_text6"));

    /*% "For these buttons apply the names <i>#CommonTopButton, #CommonVerticalCenterButton and #CommonBottomButton.</i>" */
    text7->setText(qtTrId("xx_push_button_text7"));

    /*% "3. Multiple buttons that are always collected in rows (in both orientations)" */
    text8->setText(qtTrId("xx_push_button_text8"));
    /*% "For these buttons apply the names <i>#CommonLeftButton, #CommonHorizontalCenterButton and #CommonRightButton.</i>" */
    text9->setText(qtTrId("xx_push_button_text9"));

    /*% "4. Call and Reject buttons have also equivalent sets of names. This example contains also the icon." */
    text10->setText(qtTrId("xx_push_button_text10"));

    /*% "5. Split Buttons" */
    text11->setText(qtTrId("xx_push_button_text11"));

    /*% "In cases where standard buttons have been used and need to be changed to split buttons just change the Object Names for example from <i>#CommonTopButton to #CommonTopSplitButton</i>. But first check whether you could use the <b>MButtonGroup</b> for your purpose. "*/
    text12->setText(qtTrId("xx_push_button_text12"));

    /*% "6. Button transitions" */
    text13->setText(qtTrId("xx_push_button_text13"));

    /*% "Currently available transitions are \"default\", \"expanding-background\" and \"imploding-background\"." */
    text14->setText(qtTrId("xx_push_button_text14"));

    /*% "only button" */
    button1->setText(qtTrId("xx_push_button_button1"));

    /*% "first button" */
    button2->setText(qtTrId("xx_push_button_button2"));

    /*% "second button" */
    button3->setText(qtTrId("xx_push_button_button3"));

    /*% "first button" */
    button4->setText(qtTrId("xx_push_button_button4"));

    /*% "second button" */
    button5->setText(qtTrId("xx_push_button_button5"));

    /*% "first button" */
    button7->setText(qtTrId("xx_push_button_button7"));

    /*% "second button" */
    button8->setText(qtTrId("xx_push_button_button8"));

    /*% "first button" */
    button9->setText(qtTrId("xx_push_button_button9"));

    /*% "second button" */
    button10->setText(qtTrId("xx_push_button_button10"));

    /*% "call" */
    button12->setText(qtTrId("xx_push_button_button12"));

    /*% "reject" */
    button13->setText(qtTrId("xx_push_button_button13"));

    /*% "first button" */
    button14->setText(qtTrId("xx_push_button_button14"));

    /*% "second button" */
    button15->setText(qtTrId("xx_push_button_button15"));

    /*% "first button" */
    button17->setText(qtTrId("xx_push_button_button17"));

    /*% "second button" */
    button18->setText(qtTrId("xx_push_button_button18"));
}

